TARGET = pa16
GCC = gcc
CFLAGS = -g -std=c99 -Wall -Wshadow -Wvla -Werror -pedantic
CC = $(GCC) $(CFLAGS)
SRCS = pa16.c bmpimage.c bmpfunc.c
OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --verbose --log-file

all: $(TARGET) testall

testall: test1 test2 test3 valtest

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

test1: $(TARGET)
	./pa16 exampleImages/RV.bmp result1.bmp 5 2
	diff result1.bmp expectedImages/expectedRV.bmp
test2: $(TARGET)
	./pa16 exampleImages/RV2.bmp result2.bmp 3 2
	diff result2.bmp expectedImages/expectedRV2.bmp
test3: $(TARGET)
	./pa16 exampleImages/CondProb.bmp result3.bmp 1 2
	diff result3.bmp expectedImages/expectedCondProb.bmp

valtest: $(TARGET)
	valgrind --track-origins=yes --leak-check=full ./pa16 exampleImages/RV2.bmp result2.bmp 3 2
differ: 
.c.o:
	$(GCC) $(CFLAGS) -c $*.c

clean:
	rm -f $(OBJS) $(TARGET)
