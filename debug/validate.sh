#!/bin/sh
#verification script:

echo "Store the tc folder temporarilly"
mv ~/.tc ~/.tctmp

echo "try to look at a current that isn't there at all."
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch view

echo "verbosely look for a current that isn't there"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch view -v

echo "start a task"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch start task1

echo "view information on the current task:"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch view

echo "verbosely look at the current task"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch view -v

echo "try to start a new task while there is a current one without a switch statement"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch start task2

echo "switch to a new task correctly"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch start -s task2

echo "can we switch back to an old task?"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch start -s task1

echo "swiching to the same task should not be allowed"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch start -s task1

echo "finish a task"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch finish task2

echo "try to finish the task again (will say no)"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch finish task2

echo "There will still be a current task (task1) becuase we finished task2 not task1"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch view

echo "Trying to start a new task will fail becuase we're working on one still"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch start task3

echo "finishing task 1 will remove the current file"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch finish task1

echo "See? No current task:"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch view

echo "Viewing a finished task will show completed"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch view task2

echo "Task 3 does not exist because we never started it"
valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./tcatch view task3

#put the old tc back
rm -Rf ~/.tc
mv ~/.tctmp ~/.tc
