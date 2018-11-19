#include "ne.h"
#include "router.h"
#include <sys/timerfd.h>
#define MAX(x,y) ((x) > (y) ? x:y)
int findNbr(int sender_id, struct pkt_INIT_RESPONSE *resp);
void timerInit(struct itimerspec *temp, time_t tvsec, long tvnsec, time_t x, long y);

int main(int argc, char *argv[]){
	if(argc != 5){
		printf("Not enough inputs or too many inputs\n");
		return EXIT_FAILURE;
	}

	struct pkt_INIT_REQUEST req;
	struct pkt_INIT_RESPONSE resp;

	unsigned int id;
	char* hostname;
	int neUDPPort;
	int routerUDPPort;

	int sendfd;
	int maxfd = 0;

	struct sockaddr_in neAddr;
	struct sockaddr_in routerAddr;

	id = atoi(argv[1]);
	hostname = argv[2];
	neUDPPort = atoi(argv[3]);
	routerUDPPort = atoi(argv[4]);

	if(id > MAX_ROUTERS-1 || id < 0 ){
		printf("Router's id is invalid\n");
		return EXIT_FAILURE;
	}

	req.router_id = htonl(id);

	if((sendfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		printf("Failed to open socket, line 36\n");
		return EXIT_FAILURE;
	}
	bzero((char*)&routerAddr, sizeof(routerAddr));
	routerAddr.sin_family = AF_INET;
	routerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	routerAddr.sin_port = htons((unsigned short)routerUDPPort);

	bzero((char*)&neAddr, sizeof(neAddr));
	neAddr.sin_family = AF_INET;
	inet_aton(hostname, &neAddr.sin_addr);
	neAddr.sin_port = htons((unsigned short)neUDPPort);

	if(bind(sendfd, (struct sockaddr*)&routerAddr, sizeof(routerAddr)) < 0){
		close(sendfd);
		printf("Failed to bind socket, line 50\n");
		return EXIT_FAILURE;
	}

	if(sendto(sendfd, (struct pkt_INIT_REQUEST*)&req, sizeof(req), 0, (struct sockaddr*)&neAddr, sizeof(neAddr)) < 0){
		printf("errrrrrrrrrrrrrror1\n");
		return EXIT_FAILURE;
	}

	if(recvfrom(sendfd, (struct pkt_INIT_RESPONSE*)&resp, sizeof(resp), 0, NULL, NULL) < 0){
		printf("errrrrrrrrrrrrrror2\n");
		return EXIT_FAILURE;
	}

	ntoh_pkt_INIT_RESPONSE(&resp);
	InitRoutingTbl(&resp, id);

	// HERE ENDS INITIALIZATION //

	char outputName[20];
	strcpy(outputName, "router");
	strcat(outputName, argv[1]);
	strcat(outputName, ".log");
	FILE * ptr = fopen(outputName, "w");
	PrintRoutes(ptr, id);

	// HERE STARTS UPDATES AND MAIN FUNCTIONALITY //

	//default initialization of the things we will need//
	struct pkt_RT_UPDATE toSend;
	struct pkt_RT_UPDATE toRecv;

	fd_set rfds;

	//fd's timings//
	int updateTimer;
	int convergeTimer;
	int nbrDeathTimer[resp.no_nbr];
	int totalTimer;

	//struct for fd's timings//
	struct itimerspec updTimer;
	struct itimerspec cnvTimer;
	struct itimerspec dthTimer[resp.no_nbr];
	struct itimerspec ttlTimer;

	//initialization of the timers//
	timerInit(&updTimer, UPDATE_INTERVAL, 0,0,0);
	timerInit(&cnvTimer, CONVERGE_TIMEOUT, 0,0,0);
	timerInit(&ttlTimer, 0, 250000000, 0,0);

	int i;
	int rvl;
	int changeOccured;
	int curNbr;
	int cnved = 0;
	int nbrDeathStatus[resp.no_nbr];
	int ttlsec;
	int quartersec;

	updateTimer = timerfd_create(CLOCK_REALTIME, 0);
	timerfd_settime(updateTimer, 0, &updTimer, NULL);

	convergeTimer = timerfd_create(CLOCK_REALTIME, 0);
	timerfd_settime(convergeTimer, 0, &cnvTimer, NULL);

	totalTimer = timerfd_create(CLOCK_REALTIME, 0);
	timerfd_settime(totalTimer, 0, &ttlTimer, NULL);

	maxfd = MAX(updateTimer, convergeTimer);
	maxfd = MAX(maxfd, totalTimer);

	for(i = 0; i < resp.no_nbr; i++){
		timerInit(&dthTimer[i], FAILURE_DETECTION, 0,0,0);
		nbrDeathTimer[i] = timerfd_create(CLOCK_REALTIME, 0);
		timerfd_settime(nbrDeathTimer[i], 0, &dthTimer[i], NULL);
		maxfd = MAX(maxfd, nbrDeathTimer[i]);
		nbrDeathStatus[i] = 0;
	}


	while(1){
		FD_ZERO(&rfds);
		FD_SET(sendfd, &rfds);
		FD_SET(updateTimer, &rfds);
		FD_SET(convergeTimer, &rfds);
		FD_SET(totalTimer, &rfds);
		for(i = 0; i < resp.no_nbr; i++) {
			FD_SET(nbrDeathTimer[i], &rfds);
		}

		rvl = select(maxfd + 1, &rfds, NULL, NULL, NULL);

		//if router receives an updated message//
		if(FD_ISSET(sendfd, &rfds)){
			if(recvfrom(sendfd, (struct pkt_RT_UPDATE *)&toRecv, sizeof(toRecv), 0, NULL, NULL) < 0){
				printf("errrrrrrrrrrrrrror3\n");
				return EXIT_FAILURE;
			}

			ntoh_pkt_RT_UPDATE(&toRecv);
			curNbr = findNbr(toRecv.sender_id, &resp);
			changeOccured = UpdateRoutes(&toRecv, resp.nbrcost[curNbr].cost, id);
			if(changeOccured){
				PrintRoutes(ptr, id);
				cnvTimer.it_value.tv_sec = CONVERGE_TIMEOUT;
				cnvTimer.it_value.tv_nsec = 0;
				timerfd_settime(convergeTimer, 0, &cnvTimer, NULL);
				cnved = 0;
			}
			dthTimer[curNbr].it_value.tv_sec = FAILURE_DETECTION;
			dthTimer[curNbr].it_value.tv_nsec = 0;
			timerfd_settime(nbrDeathTimer[curNbr], 0, &dthTimer[curNbr], NULL);
			nbrDeathStatus[curNbr] = 0;
			fflush(ptr);
		}

		//Send updated table to neighbours after update interval
		if(FD_ISSET(updateTimer, &rfds)){
			bzero((char *)&toSend, sizeof(toSend));
			ConvertTabletoPkt(&toSend, id);

			for(i = 0; i < resp.no_nbr; i++) {
				
				toSend.dest_id = resp.nbrcost[i].nbr;
				hton_pkt_RT_UPDATE(&toSend);
				if (sendto(sendfd, (struct pkt_RT_UPDATE *)&toSend, sizeof(toSend), 0, (struct sockaddr *)&neAddr, sizeof(neAddr)) < 0) {
					printf("errrrrrrrrrrrrrror4\n");
					return EXIT_FAILURE;
    			}
    			ntoh_pkt_RT_UPDATE(&toSend);
    		}
    		updTimer.it_value.tv_sec = UPDATE_INTERVAL;
			updTimer.it_value.tv_nsec = 0;
			timerfd_settime(updateTimer, 0, &updTimer, NULL);
			fflush(ptr);
		}

		//if converged
		if(FD_ISSET(convergeTimer, &rfds)){
			if(cnved == 0){
				fprintf(ptr, "%d:Converged\n", ttlsec);
			}
			cnvTimer.it_value.tv_sec = CONVERGE_TIMEOUT;
			cnvTimer.it_value.tv_nsec = 0;
			timerfd_settime(convergeTimer, 0, &cnvTimer, NULL);
			cnved = 1;
			fflush(ptr);
		}
		if(FD_ISSET(totalTimer, &rfds)) {
			quartersec = (quartersec + 1) % 4;
			if (quartersec == 3) {
				ttlsec += 1;
			}

			ttlTimer.it_value.tv_sec = 0;
			ttlTimer.it_value.tv_nsec = 250000000;
			timerfd_settime(totalTimer, 0, &ttlTimer, NULL);
		}
		//check for neighbour death
		for (i = 0; i < resp.no_nbr; i++) {
			if(FD_ISSET(nbrDeathTimer[i], &rfds)) {
				if (nbrDeathStatus[i] == 0) {
					UninstallRoutesOnNbrDeath(resp.nbrcost[i].nbr);
					PrintRoutes(ptr, id);
					cnvTimer.it_value.tv_sec = CONVERGE_TIMEOUT;
					cnvTimer.it_value.tv_nsec = 0;
					timerfd_settime(convergeTimer, 0, &cnvTimer, NULL);
					cnved = 0;
					
					fflush(ptr);
				}
				nbrDeathStatus[i] = 1;
			}
		}
	}

	fclose(ptr);
	return EXIT_SUCCESS;
}

//UTILITY
int findNbr(int sender_id, struct pkt_INIT_RESPONSE *resp) {
	int i;
	for (i = 0; i < resp->no_nbr; i++) {
		if(resp->nbrcost[i].nbr == sender_id){
			return i;
		}
	}
	return EXIT_FAILURE;
}
void timerInit(struct itimerspec *temp, time_t tvsec, long tvnsec, time_t x, long y){
	(*temp).it_value.tv_sec = tvsec;
	(*temp).it_value.tv_nsec = tvnsec;
	(*temp).it_interval.tv_sec = x;
	(*temp).it_interval.tv_nsec = y;
}
