Time Catcher
=======================================================================

Time Catcher is a simple program that allows one to keep track of time
and to generate simple tables of information for processing. The usage
is simple. 

To begin a task quickly, simply run:

    ./tcatch start <task title>

To add information about the task you're currently working on, it's
simple. 

    ./tcatch add-info <information text>

This information will be appended to the current description of the
task. In this way, you can keep a running log of work done on the task
for future reference.

To end a task and commit a summary message

    ./tcatch finish <task title> 
Then a text editor will pop up and allow you to create a simple summary
message to go along with your task.

To view a previous task

    ./tcatch view <task title>

Or to view all tasks that have been done or are in progress:

    ./tcatch view --all


How it works
-----------------------------------------------------------------------

Time Catcher creates a .tc directory within the home folder. This is
where all information about the tasks worked on is stored. Whenever you 
create a task, 2 new files are created in the tasks directory. A
sequence file (.seq) and an informational file (.info). The .info file
stores your plain text and human readable notes about the project. The
.seq file stores a record of starts and stops for the task. This
information is later used to calculate a tasks length.


Within the .tc directory is a file called current which contains
information about the current task being worked on. The file contains
the location of the starting as well as the ending file. The difference
between these two files (and any pause files)  is used to calculate any
information about the task at all.



Compiling and verifying the program
-----------------------------------------------------------------------

Running make should create the program. To verify 0 memory leaks:

    valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch
 
