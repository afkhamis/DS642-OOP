#Uncomment your choice of compiler
#CC    =   icc
CC    =   gcc

#Add your choice of flags
FLAGS   =   -O1

all:	hello

clean:
	rm  -rf    \
	hello     \
        *.o

hello: hello.c
	${CC} ${FLAGS} hello.c -o hello
