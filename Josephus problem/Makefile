CFLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
GCC = gcc $(CFLAGS)

# list all .c files
SRCS = main.c pa12.c

# object files are created by converting .c to .o
OBJS = $(SRCS:%.c=%.o)

pa12: $(OBJS) 
	$(GCC) $(OBJS) -o pa12


testall:test1 test2 test3 test4 test5 test6

test1: pa12
	./pa12 -J 10 3 1 > result1.txt
	diff result1.txt case1.txt
test2: pa12
	./pa12 -J 10 4 2 > result2.txt
	diff result2.txt case2.txt
test3: pa12
	./pa12 -J 8 5 3 > result3.txt
	diff result3.txt case3.txt
test4: pa12
	./pa12 -J 8 9 1 > result4.txt
	diff result4.txt case4.txt
test5: pa12
	./pa12 -J 41 3 1 > result5.txt
	diff result5.txt case5.txt
test6: pa12
	./pa12 -J 100 3 20 > result6.txt
	diff result6.txt case6.txt

memleak: pa12
	valgrind --track-origins=yes --leak-check=full ./pa12 -J 10 3 1
	valgrind --track-origins=yes --leak-check=full ./pa12 -J 10 4 2
	valgrind --track-origins=yes --leak-check=full ./pa12 -J 8 5 3
	valgrind --track-origins=yes --leak-check=full ./pa12 -J 8 9 1
	valgrind --track-origins=yes --leak-check=full ./pa12 -J 41 3 1
	valgrind --track-origins=yes --leak-check=full ./pa12 -J 100 3 20

clean: # remove all machine generated files
	rm -f pa12 *.o *.out *gcda *gcno *gcov




