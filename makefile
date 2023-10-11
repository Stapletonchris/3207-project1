
output: MYps.o 
	gcc MYps.o -o output

cpuinfo.o: MYps.c
	gcc -c MYps.c

clean: 
	rm *.o output

