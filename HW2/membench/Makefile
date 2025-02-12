# Use the OpenMP clock
CC = gcc
CFLAGS = -O2 -fopenmp
#LIBS = 

.PHONY: default clean tgz

default: membench

membench.pdf: membench.gp membench.out
	gnuplot membench.gp
	ps2pdf membench.eps

run_login: membench
	./membench | tee membench.out

run_node: membench
	sbatch -o membench.out submit.job

membench: membench.c
	$(CC) $(CFLAGS) -o membench membench.c $(LIBS)

clean:
	rm -f membench membench.eps membench.pdf membench.out

tgz: clean
	(cd ..; tar -czf membench.tgz membench)
