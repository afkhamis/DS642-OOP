# Module 1: Assignment-Matrix Multiply 
---
This assignment is about logging into Bridges-2 machine, compiling the matrix multiplication code, and running the code on the compute node by submitting a job to the cluster queue.

See https://www.psc.edu/resources/bridges-2/user-guide/ for Bridges-2 User Guide.

## Login

Using your ssh client, connect to hostname: bridges2.psc.edu

On your local machine, this looks like (most shell environments by default put your user name and the host name before the $):

```
name@localhost $ ssh your_bridges-2_username@bridges2.psc.edu
```

Enter your Bridges-2 password when prompted.

# File transfer

Download the C files that are provided and transfer them to your Bridges-2 directory. Use any file transfer method to copy the C files to your bridges-2 directory. I recommend using "scp". To use scp for a file transfer you must specify a source and destination for your transfer. The format for either source or destination is

```
$ username@machine-name:path/filename
```

The machine-name should be given as data.bridges2.psc.edu. This is the name for a high-speed data connector at PSC. File transfers using scp must specify full paths for Bridges-2 file systems. You can also use other file transfer methods: rsync, sftp or Globus to copy files to and from Bridges-2.

# Compile and Run

Use the GNU C Compiler (gcc), the standard C compiler, to manually compile the codes provide; for example:

```
$ gcc -o hello hello.c
```

The compilation creates a new file named hello (the name is defined above with the compiler flag -o). You  can run "hello" as follows:

```
$ ./hello
```

Now use the Intel C compiler instead:

```
$ module load intel

$ icc -o hello hello.c
```

The first command loads the Intel C compiler icc into your system path via the module system. You can again run it as before:

```
$ ./hello
```

Use automatic compilation with Make. Make is used to compile source code into executable programs or libraries. Make uses the makefile for, among other things, building and compiling code. Download the provided Makefile and transfer it to your directory on Bridges-2. Run the following commands:

```
$ make clean

$ make
```

# Running on Bridges-2 compute nodes:

We now want to run the job on the Bridges-2 compute nodes using batch file. From now on, all production computing must be done on Bridges-2's compute nodes, NOT on Bridges-2's login nodes, except very light prototyping and scripting codes, or when debugging. Here we use the batch mode - where you first create a batch (or job) script which contains the commands to be run, then submit the job to be run as soon as resources are available. The SLURM scheduler (Simple Linux Utility for Resource Management) manages and allocates all of Bridges-2's compute nodes. An example of a batch file is provided, hello.job. To run an executable on a compute node, you will need to submit the batch file for a compute job. The submission scripts can include sbatch command, to specify the number of nodes/cores/GPU's requested, or the type of the requested nodes to run the job on, walltime requested, and so on. For example, to run the executable "hello", built by compiling hello.c, the batch script hello.job must be submitted as follows:

```
$ sbatch hello.job
```

You can check the progress of your compute job by using the following command

```
$ squeue -u username
```

You can check the output file when the job is done. By default, the standard output and error from a job are saved in a file with the name slurm-jobid.out, in the directory that the job was submitted from.

# HW submission:
Download the code for serial matrix multiplication, ser_mm.c, and transfer it to your Bridges-2 directory, and submit the followings:

1- Makefile for compiling ser_mm.c.

2- A job batch script, named ser_mm.job, for running ser_mm.c on compute node. 

3- A file containing the output of running ser_mm.c on compute node, named ser_mm.out.  
