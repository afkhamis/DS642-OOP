# membench

This is an experimental study of memory (membench), a simple memory benchmark. See e.g.:

https://people.eecs.berkeley.edu/~demmel/cs267_Spr99/

The `membench` C code measures the time to read and write the elements of an array while it varies the length of the array and the stride through the array:

    for array A of length L from 4KB to 8MB by 2x
      for stride s from 4 Bytes (1 Word) to L/2 by 2x
        time the following loop (repeat many times and average)
          for i = 0 to L by s
            load A[i] from memory (4 Bytes)

The test will illustrate the details of the memory hierarchy of the machine you run the benchmark on. The raw measured times can be plotted using the gnuplot script. Please note that you must submit the job to compute nodes. For comparison, you can see that how different the performace characteristics are if you run your test on the login node. Also, you can try this on your own machine. Report your findings, particulalrly what can you report regarding the cache architecture of the Bridges-2 compute nodes. 

The folder includes:

    Makefile
    The memory benchmark program: membench.c
    A gnuplot script: membench.gp 

To complile and build the executable, type `make`. To submit to the compute node, type `make run_node`. To create the graph, type `make membench.pdf`. See the Makefile for further information.  
    
  
