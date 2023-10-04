
output: cpuinfo.o 
	gcc cpuinfo.o -o output

cpuinfo.o: cpuinfo.c
	gcc -c cpuinfo.c

clean: 
	rm *.o output

