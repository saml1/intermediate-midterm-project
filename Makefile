CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g


project: ppm_io.o imageManip.o
	$(CC) project.c ppm_io.o imageManip.o -o project -lm 	

demo: demo_ppm.o ppm_io.o imageManip.o
	$(CC) demo_ppm.o ppm_io.o imageManip.o -o demo -lm

demo_ppm.o: demo_ppm.c ppm_io.h
	$(CC) $(CFLAGS) -c demo_ppm.c

ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

imageManip.o: imageManip.c imageManip.h
	$(CC) $(CFLAGS) -c imageManip.c
clean:
	rm -f *.o demo
