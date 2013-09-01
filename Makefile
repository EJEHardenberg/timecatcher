tcatch: tc.o
	mv tc.o  tcatch 

tc.o: tc-init.c
	cc tc-init.c -o tc.o -ansi -pedantic -Wall -Wextra -Werror -g

clean:
	rm  tcatch
