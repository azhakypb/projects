#include "ne.h"

struct route_entry routingTable[MAX_ROUTERS];
int NumRoutes;


int addNewRoute(struct pkt_RT_UPDATE *recv, int costToNbr, struct route_entry *recvdRouter);
int doSplitHorizonAndForcedUpd(struct route_entry *curRouter, struct route_entry *recvdRouter, int costToNbr, int myID, int sender_id);


void InitRoutingTbl (struct pkt_INIT_RESPONSE *InitResponse, int myID){
	int i;
	for(i = 0; i < InitResponse->no_nbr; i++){
		routingTable[i].dest_id = InitResponse->nbrcost[i].nbr;
		routingTable[i].next_hop = InitResponse->nbrcost[i].nbr;
		routingTable[i].cost = InitResponse->nbrcost[i].cost;
	}
	routingTable[i].dest_id = myID;
	routingTable[i].next_hop = myID;
	routingTable[i].cost = 0;
	NumRoutes = i + 1;
}

int UpdateRoutes(struct pkt_RT_UPDATE *RecvdUpdatePacket, int costToNbr, int myID){
	struct route_entry *curRouter;
	struct route_entry recvdRouter;
	int i;
	int j;
	int changeOccured = 0;

	for(i = 0; i < RecvdUpdatePacket->no_routes; i++){
		recvdRouter = RecvdUpdatePacket->route[i];
		for(j = 0; j < NumRoutes; j++){
			curRouter = &routingTable[j];
			if(curRouter->dest_id == recvdRouter.dest_id){
				if(doSplitHorizonAndForcedUpd(curRouter, &recvdRouter, costToNbr, myID, RecvdUpdatePacket->sender_id)){
					curRouter->next_hop = RecvdUpdatePacket->sender_id;
					//if current path is dead and suggested path is also dead, do nothing
					if(curRouter->cost == INFINITY && costToNbr + recvdRouter.cost >= INFINITY){
						curRouter->cost = INFINITY;
						changeOccured = 0;
					}
					//if received info tells us that the router is dead, change to dead
					else if(costToNbr + recvdRouter.cost >= INFINITY){
						curRouter->cost = INFINITY;
						changeOccured = 1;
					}
					//otherwise replace the cost with the new one
					else{
						curRouter->cost = costToNbr + recvdRouter.cost;
						changeOccured = 1;
					}

				}
				break;
			}

		}
		//if there is more received routes than existing, add the new ones
		if(j == NumRoutes){
			changeOccured = addNewRoute(RecvdUpdatePacket, costToNbr, &recvdRouter);

		}
	}
	return changeOccured;
}

void ConvertTabletoPkt(struct pkt_RT_UPDATE *UpdatePacketToSend, int myID){
	int i;
	UpdatePacketToSend->sender_id = myID;
	UpdatePacketToSend->no_routes = NumRoutes;
	for(i = 0; i < NumRoutes; i++){
		UpdatePacketToSend->route[i].dest_id = routingTable[i].dest_id;
		UpdatePacketToSend->route[i].next_hop = routingTable[i].next_hop;
		UpdatePacketToSend->route[i].cost = routingTable[i].cost;
	}
}
void PrintRoutes (FILE* ptr, int myID){
	fprintf(ptr, "\nRouting Table:\n");
	int i;
	int j;

	for (j = 0; j < MAX_ROUTERS; j++) {
		for (i = 0; i < NumRoutes; i++) {
			if (routingTable[i].dest_id == j) {
				fprintf(ptr, "%d -> %d: %d, %d\n", myID, routingTable[i].dest_id, routingTable[i].next_hop, routingTable[i].cost);
			}
		}
	}
}
void UninstallRoutesOnNbrDeath(int DeadNbr){
	int i;
	for (i = 0; i < NumRoutes; i++ ) {
		if (routingTable[i].next_hop == DeadNbr ) {
			routingTable[i].cost = INFINITY;
		}
	}
}



// UTILITY FUNCTIONS //
int addNewRoute(struct pkt_RT_UPDATE *RecvdUpdatePacket, int costToNbr, struct route_entry *recvdRouter){
		routingTable[NumRoutes].dest_id = recvdRouter->dest_id;
		routingTable[NumRoutes].next_hop = RecvdUpdatePacket->sender_id;
		routingTable[NumRoutes].cost = costToNbr + recvdRouter->cost;
		NumRoutes++;
		return 1;
}
int doSplitHorizonAndForcedUpd(struct route_entry *curRouter, struct route_entry *recvdRouter, int costToNbr, int myID, int recvdID){
		if(curRouter->next_hop == recvdID){
			if(curRouter->cost == costToNbr + recvdRouter->cost){
				return 0;
			}
			return 1;
		}
		else if(recvdRouter->cost + costToNbr < curRouter->cost && recvdRouter->next_hop != myID){
			return 1;
		}
		else{
			return 0;
		}
}