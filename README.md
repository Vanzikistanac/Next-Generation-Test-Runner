# Next-Generation-Test-Runner
Communication between 2 processes from standard IO

We have 2 C++ files, ProgramA.cpp and ProgramB.cpp.

Compile and run ProgramA from one console, and in the second console compile and run ProgramB.
Make sure to first run ProgramA and than to run ProgramB.
(Commands for compiling C++ files, I'm using gcc version 13.1.0 compiler)

g++ ProgramA.cpp -o ProgramA

g++ ProgramB.cpp -o ProgramB

#ProgramA works as a controller. In ProgramA we can input one of 3 options:

-Shutdown -> both processes (ProgramA and ProgramBa) will shutdown.

-GetRandom -> ProgramB prints 100 random numbers (less than 1000), pass them to ProgramA which sorts, prints median and prints average value.

-Hi -> ProgramB will output "Hi" to standard output


How does this code work?
First, what are pipes?
-In Windows, pipes are a method of inter-process communication (IPC) that allows data to be transferred between processes.

How to create pipe in C++?
First we need Windows library.

CreateNamedPipeA function, returns HANDLE to the pipe
Documentation:
https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-createnamedpipea

Connect ProgramB with the pipe
Documentation:
https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea

Read and Write data
Documentation
https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile


