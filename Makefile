tcatch: tc.o tc-task.o tc-start.o tc-view.o tc-info.o tc-finish.o
	cc tc.o tc-init.o tc-dir.o tc-task.o tc-view.o tc-start.o tc-info.o tc-finish.o  tc-pause.o -o tcatch -lcrypto
	rm *.o

tc.o: src/tcatch.c tc-init.o tc-dir.o tc-view.o tc-info.o tc-finish.o tc-pause.o
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

tc-info.o: src/tc-info.c headers/tc-info.h tc-task.o tc-dir.o
	cc -c src/tc-info.c -o tc-info.o -ansi -pedantic -Wall -Wextra -Werror -g -I ./headers

tc-finish.o: src/tc-finish.c headers/tc-finish.h tc-task.o tc-dir.o
	cc -c src/tc-finish.c -o tc-finish.o -ansi -pedantic -Wall -Wextra -Werror -g -I ./headers	

tc-pause.o: src/tc-pause.c headers/tc-pause.h tc-init.o tc-task.o tc-dir.o
	cc -c src/tc-pause.c -o tc-pause.o -ansi -pedantic -Wall -Wextra -Werror -g -I ./headers	

clean:
	rm  tcatch
