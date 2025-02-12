#Your choice of compiler
CC    =   icc
#CC    =   gcc

#Add your choice of flags
CFLAGS   =   -qopenmp
#CFLAGS   =   -fopenmp

all: centroid

centroid: centroid.c centroid.h centroid_timer.c
	${CC} -o centroid centroid.c centroid_timer.c ${CFLAGS}

clean:
	rm -f centroid
	rm *.o*
