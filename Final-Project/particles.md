Parallelizing a Particle Simulation
===================================

Overview


-------------------------------------

This project is a toy particle simulation (similar simulations are used in mechanics, [biology](http://www.ks.uiuc.edu/Research/namd/), and [astronomy](http://www.mpa-garching.mpg.de/gadget/clusters/index.html)).  In this problem, particles interact by repelling one another. A run of the simulation is shown here:

![](https://lh3.googleusercontent.com/sitesv/APaQ0SSgAXL0Ah-AggGeS_fArZwbEVA5TYjZbhxLiCt42zZNE452IGhzC6V5FqA-_bQPD_5mPU5KTakzj5YfqFCuNUdmdErEGCNydBQ_4RRrsQ8Sv7uLqUM-uu3UNVmewYmDYuGWz7b3wn0R0i5imemX2YGZUx8oY98XcLTLTmBNFllikkpB2VCefnTV15g=w1280)

The particles repel one another, but only when closer than a cutoff distance highlighted around one particle in grey.

Asymptotic Complexity


--------------------------------------------------

### Serial Solution Time Complexity

If we were to naively compute the forces on the particles by iterating through every pair of particles, then we would expect the asymptotic complexity of our simulation to be O(n^2). However, if a density of particles is sufficiently low, so that with n particles, we expect only O(n) interactions.  An efficient implementation can reach this time complexity.  The first part of this problem will be to implement a linear time solution in a serial code, given a naive O(n^2) implementation.

### Parallel Speedup

Suppose we have a code that runs in time T = O(n) on a single processor. Then we'd hope to run close to time T/p when using p processors.  After implementing an efficient serial O(n) solution, you will attempt to reach this speedup using OpenMP.

----------------------------------------------------------------------------

Instructions


=========================================

Getting Set Up


-------------------------------------------

The starter code is available on Github at [https://github.com/Berkeley-CS267/hw2-1](https://github.com/Berkeley-CS267/hw2-1) and should work out of the box.  To get started, we recommend you log in to Bridges-2 and clone the git repository, using 
`
git clone https://github.com/Berkeley-CS267/hw2-1
`

There are seven files in the base repository. Their purposes are as follows:

`CMakeLists.txt`

The build system that manages compiling your code.

`main.cpp`

A driver program that runs your code. 

`common.h`

A header file with shared declarations

`job-openmp`

A sample job script to run the OpenMP executable

`job-serial`

A sample job script to run the serial executable

`serial.cpp` - - - You may modify this file.

A simple O(n^2) particle simulation algorithm. It is your job to write an O(n) serial algorithm within the simulate\_one\_step function.

`openmp.cpp` - - - You may modify this file.

A skeleton file where you will implement your openmp simulation algorithm. It is your job to write a parallel algorithm within the simulate\_one\_step function.


=========================================


Building our Code


----------------------------------------------

First, we need to make sure that the CMake module is loaded.

`module load cmake`

You should put these commands in your `~/.bash\_profile` file to avoid typing them every time you log in.

Next, let's build the code. CMake prefers out of tree builds, so we start by creating a build directory.

`mkdir build`

`cd build`

Next, we have to configure our build. We can either build our code in Debug mode or Release mode. In debug mode, optimizations are disabled and debug symbols are embedded in the binary for easier debugging with GDB. In release mode, optimizations are enabled, and debug symbols are omitted. For example:

`cmake -DCMAKE\_BUILD\_TYPE=Release ..`

\-- The C compiler identification is GNU 12.3.0

...

\-- Configuring done

\-- Generating done

\-- Build files have been written to ...

Once our build is configured, we may actually execute the build:

`make`

`ls`

CMakeCache.txt  CMakeFiles  cmake\_install.cmake  job-openmp  job-serial  Makefile  openmp  serial

We now have two binaries (openmp and serial) and two job scripts (job-openmp and job-serial).


--------------------------------------------------------------------------------------------------


Running the Program


------------------------------------------------

Both executables have the same command line interface. Without losing generality, we discuss how to operate the serial program here. Run: 

`./serial

Simulation Time = 1.17782 seconds for 1000 particles.
`

You can also run the program using the job scripts. 

By default, the program runs with 1000 particles. The number of particles can be changed with the "-n" command line parameter:

`
./serial -n 10000

Simulation Time = 115.992 seconds for 10000 particles.
`

If we rerun the program, the initial positions and velocities of the particles will be randomized because the particle seed is unspecified. By default, the particle seed will be unspecified; this can be changed with the "-s" command line parameter:

`
./serial -s 150

Simulation Time = 1.18632 seconds for 1000 particles.
`

This will set the particle seed to 150 which initializes the particles in a reproducible way. We will test the correctness of your code by randomly selecting several particle counts and seeds and ensuring the particle positions are correct when printed with the "-o" command line parameter. You can print the particle positions to a file specified with the "-o" parameter:

`
./serial -o serial.parts.out

Simulation Time = 1.46991 seconds for 1000 particles.
`

This will create a serial.parts.out file with the particle positions after each step listed. You can use the rendering tool below to convert this into a .gif file of your particles. See the below section on Rendering Output for more information.

You can use the "-h" command line parameter to print the help menu summarizing the parameter options:

`./serial -h

Options:

\-h: see this help

\-n <int>: set number of particles

\-o <filename>: set the output file name

\-s <int>: set particle initialization seed
`

### Important notes for Performance:

There will be two types of scaling that are tested for your parallel codes:

*   In strong scaling we keep the problem size constant but increase the number of processors
    
*   In weak scaling we increase the problem size proportionally to the number of processors so the work/processor stays the same (Note that for the purposes of this assignment we will assume a linear scaling between work and processors)
    

While the scripts we are providing have small numbers of particles 1000 to allow for the O(n2) algorithm to finish execution, the final codes should be tested with values much larger (500000-1000000) to better see their performance.

=========================================


Grading


------------------------------------

We will grade your assignment by reviewing your assignment write-up, measuring the scaling of both the openmp and serial implementations, and benchmarking your code's raw performance. To benchmark your code, we will compile it with the exact process detailed above, with the GNU compiler. 

There are usually some groups every year who come up with faster methods to compute the particle repulsion force function (i.e. rearranging the arithmetic, changing the formula, or using some fancy instructions). This is great, but small differences in the floating point position values begin to add up until the simulation output diverges from our ground truth (even though your method of computation might be more accurate than ours). Since (a) the point of the assignment is to explore OpenMP parallelism, and (b) we can't anticipate every possible way to compute this force function, here is the rule: if it doesn't pass the correctness check we provide you reliably, then it's not allowed. 

### Submission Details

Create an appropriate submission archive:

*   Ensure that your write-up is located in your source directory, next to serial.cpp. 
    
*   From your build directory, run:
    
`
cmake -DGROUP\_NAME=04 ..

make package
`

=========================================


Write-up Details

*   Your write-up should contain:
    
    *   The names of the people in your group and each member's contribution.
        
    *   A plot in log-log scale that shows that your serial and parallel codes run in O(n) time and a description of the data structures that you used to achieve it.
        
    *   A description of the synchronization you used in the shared memory implementation.
        
    *   A description of the design choices that you tried and how did they affect the performance.
        
    *   Speedup plots that show how closely your OpenMP code approaches the idealized p-times speedup and a discussion on whether it is possible to do better.
        
    *   Where does the time go? Consider breaking down the runtime into computation time, synchronization time and/or communication time. How do they scale with p?
        

Notes:

*   Your grade will mostly depend on three factors:
    
    *   Scaling sustained by your codes on the perlmutter supercomputer (varying n).
        
    *   Performance sustained by your codes on the perlmutter supercomputer.
        
    *   Explanations of your methodologies and the performance features you observed (including what didn't work).
        
*   You must use the GNU C Compiler for this assignment. If your code does not compile and run with GCC, it will not be graded.
    
*   If your code produces incorrect results, it will not be graded.
  
    
=========================================


Rendering Output


---------------------------------------------

The output files that are produced from running the program with the "-o" command line parameter can be fed into the rendering tool made available to convert them into .gif files. These animations will be a useful tool in debugging. To get started clone the rendering repo and load the python module:

'
git clone [https://github.com/Berkeley-CS267/hw2-rendering](https://github.com/Berkeley-CS267/hw2-rendering)

module load python
`

We can then convert the output files to gifs with the following command:

`render.py serial.parts.out particles.gif 0.01`

Here `serial.parts.out` is an output file from the "-o" command line parameter. You should find a `particles.gif` file in your directory. The number 0.01 is the cutoff distance (will be drawn around each particle).

=========================================


Output Correctness


-----------------------------------------------

The output files that are produced from running the program with the "-o" command line parameter can be fed into the correctness tool made available to perform a correctness check. This is the same correctness check we will be performing when grading this, however, we will randomly select the particle seeds. To get started clone the correctness repo and load the python module:

`git clone [https://github.com/Berkeley-CS267/hw2-correctness](https://github.com/Berkeley-CS267/hw2-correctness)`

module load python
`

We can then test the output files for correctness with the following command: 

`correctness-check.py serial.parts.out correct.parts.out`

If the program prints an error, then your output is incorrect. Here serial.parts.out is an output file from the "-o" command line parameter from your code. This can be substituted for any output you wish to test the correctness for. The correct.parts.out can be generated from the provided O(n^2) serial implementation. Remember to specify a particle seed with "-s" to ensure the same problem is solved between the two output files. The correctness repo provides the "verf.out" file which is the correct output with particle seed set to 1 "-s 1". 

=========================================


