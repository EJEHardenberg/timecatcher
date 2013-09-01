tcatch: tc.o
	cc tc.o tc-dir.o -o tcatch

tc.o: tc-init.c tc-dir.o
	cc -c tc-init.c -o tc.o -ansi -pedantic -Wall -Wextra -Werror -g

tc-dir.o: tc-directory.c tc-directory.h
	cc -c tc-directory.c -o tc-dir.o -ansi -pedantic -Wall -Wextra -Werror -g

clean:
	rm  tcatch tc-dir.o
