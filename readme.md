Time Catcher
=======================================================================

Time Catcher is a simple program that allows one to keep track of time
and to generate simple tables of information for processing. The usage
is simple. 

To begin a task quickly, simply run:

    tcatch start <task title>

To add information about the task you're currently working on, it's
simple. 

    tcatch add-info <information text>

This information will be appended to the current description of the
task. In this way, you can keep a running log of work done on the task
for future reference.

To pause the current task:

    tcatch pause

To end a task

    tcatch finish <task title> 

To view a previous task

    tcatch view <task title>

Or to view all tasks that have been done or are in progress:

    tcatch view --all

How To Install
-----------------------------------------------------------------------
From github:
    
    $git clone https://github.com/EJEHardenberg/timecatcher.git
    $cd timecatcher
    $make
    $cp completion.sh /etc/bash_completion.d/
    $chmod +x tcatch
    $cp tcatch /usr/bin/
    $source completion.sh

You can download a zip as well and then run the same commands besides
clone. 

What you're doing, if your distribution is different from mine (mint)
then place the completion.sh script into your bash completion directory
and put the tcatch binary into your path somehow. You could do this:
    
    $source completion.sh
    $PATH=$PATH:~/your/path/to/timetatcher/

And you'd have it available in your shell for the duration of your 
session.

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

You can edit the information files as much as you'd like so long as you
don't remove the task name as the first line of the file. If you want to
be able to enjoy the view --all command then don't mess with the first
line of the .info files. If you don't like the .tc directory being in 
your home directory, then you'll have to modify the source.



Compiling and verifying the program
-----------------------------------------------------------------------

Running make should create the program. To verify 0 memory leaks:

    valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch
 

How to Contribute (If you're inclined)
-----------------------------------------------------------------------

1. Fork the repository
2. Clone it to your local machine
3. Do the following commands:
    
    `git checkout -b develop`  
    `git remote add upstream https://github.com/EJEHardenberg/timecatcher.git`  
    `git pull upstream develop`  
    `gitk`  

4. Read the logfile and enjoy my humor
5. Use git flow or whatever you like to play around
6. Run valgrind or the validate script against your changes
7. Send pull request
8. ???
9. Profit?

Improvements I want to see / plan on
-----------------------------------------------------------------------

1. Some type of timeline that makes use of the index file
2. Integration with a git hook
