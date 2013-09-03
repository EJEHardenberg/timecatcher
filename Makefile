tcatch: tc.o tc-task.o tc-start.o tc-view.o
	cc tc.o tc-init.o tc-dir.o tc-task.o tc-view.o tc-start.o -o tcatch -lcrypto
	rm *.o

tc.o: src/tcatch.c tc-init.o tc-dir.o tc-view.o
	cc -c src/tcatch.c -o tc.o -ansi -pedantic -Wall -Wextra -Werror -g -I ./headers

tc-init.o: src/tc-init.c headers/tc-init.h tc-dir.o
	cc -c src/tc-init.c -o tc-init.o -ansi -pedantic -Wall -Wextra -Werror -g -I ./headers

tc-dir.o: src/tc-directory.c headers/tc-directory.h
	cc -c src/tc-directory.c -o tc-dir.o -ansi -pedantic -Wall -Wextra -Werror -g -I ./headers

tc-task.o: src/tc-task.c headers/tc-task.h
	cc -c src/tc-task.c -o tc-task.o -ansi -pedantic -Wall -Wextra -Werror -g -I ./headers

tc-view.o: src/tc-view.c headers/tc-view.h
	cc -c src/tc-view.c -o tc-view.o -ansi -pedantic -Wall -Wextra -Werror -g -I ./headers

tc-start.o: src/tc-start.c headers/tc-start.h tc-dir.o
	cc -c src/tc-start.c -o tc-start.o -ansi -pedantic -Wall -Wextra -Werror -g -I ./headers

clean:
	rm  tcatch
