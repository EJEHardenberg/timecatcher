#!/bin/sh
#verification script:

#Store the tc folder temporarilly
mv ~/.tc ~/.tctmp

#try to look at a current that isn't there at all.
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch view

#verbosely look for a current that isn't there
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch view -v

#start a task
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch start task1

#view information on the current task:
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch view

#verbosely look at the current task
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch view -v

#try to start a new task while there is a current one without a switch statement
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch start task2

#switch to a new task correctly
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch start -s task2

#can we switch back to an old task?
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch start -s task2


#put the old tc back
rm -Rf ~/.tc
mv ~/.tctmp ~/.tc
