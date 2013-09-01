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
