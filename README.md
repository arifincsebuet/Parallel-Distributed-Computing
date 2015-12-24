#1. Problem Definition:
The collapse of a set of integers is defined to be the sum of the integers in the set. Similarly, the collapse of a single integer is defined to be the integer that is the sum of its digits. The process of collapsing an integer to a single digit will be referred to herein as the ultimate collapse of an integer. This can clearly be carried out recursively, until a single digit results. For example, the collapse of the integer 134957 is 29. The collapse of 29 is 11, and its collapse followed by the result being collapsed recursively until only a single digit remains. In this example the ultimate collapse is 2. Our task is to find the collapse sum of a set of integers using master/worker parallel programming paradigm. Read a stream of integers and find the sum of their digits using the master and workers in parallel.
Details here:
<a href="http://sydney.edu.au/engineering/it/~comp5426/sem12015/Assgn/Ass1/assignment1.html" target="_blank">Problem Definition</a>

#2.	Requirements:
•	MPI </br>
•	Language: C++ </br>
•	Microsoft Visual Studio 2010 </br>
•	Windows Command Prompt </br>
•	Windows 7 environment
Set up in Visual Studio 2010
<a href="http://nick-goodman.blogspot.com/2012/02/using-mpich-from-ms-visual-studio-2010.html" target="_blank">Using MPI on Windows 7 with MS Visual Studio 2010</a>

#3.	Parallel algorithm design:
We’ve used the master/worker parallel programming paradigm. In our design there is a master process which controls the whole program. Master process is responsible to check if any input exists and then starts taking input from a file. It takes input block by block. Block size is pre-defined and adjustable. So if we instruct to take block input e.g. 10 integers at once, master process will read 10 integers and in next cycle another 10 integers and so on. Then master process will send these inputs to available parallel processes. If system is one processor trivial system master process itself will calculate total collapse result. Otherwise it’ll send block of numbers to worker processes and worker process will calculate the sum of those blocks and send them to the master process. Master process will collect all the results from workers and check for the input end. Then master process will calculate the final collapse result.

#4.	Implementation and Testing:
a.	Trivial single CPU case: </br>
    • No parallelism. </br>
    • Read the input from file and calculate the final collapse result </br>
b.	Master process and workers process: </br>
    • Read the input from file  and send block of numbers to worker processes </br>
    • Worker process will calculate the sum of those blocks and send them to the master process. </br>
    • Master process will collect all the results from workers and check for the input end. </br>
    • Then master process will calculate the final collapse result.

#5.	Manual (how to run the program, input and output):
	•	Put the .exe file and input file (in input.txt format) in same folder.
	•	Run to cmd.exe.
	•	Go to MPI project directory in command prompt. (i. e. C:\Program Files\MPI\Debug).
	•	Execute the exe file from command "mpiexec -n 4 Assignment1MPI.exe" *here 4 is number of processor.
	•	Get output in the command prompt.
