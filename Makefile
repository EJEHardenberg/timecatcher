tcatch: tc.o
	cc tc.o tc-dir.o -o tcatch
	rm *.o

tc.o: src/tc-init.c tc-dir.o
	cc -c src/tc-init.c -o tc.o -ansi -pedantic -Wall -Wextra -Werror -g -I ./headers

tc-dir.o: src/tc-directory.c headers/tc-directory.h
	cc -c src/tc-directory.c -o tc-dir.o -ansi -pedantic -Wall -Wextra -Werror -g -I ./headers

clean:
	rm  tcatch *.o
