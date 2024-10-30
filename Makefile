main: main.c plist.c
	gcc -Werror -Wall -Wextra -Wno-unused-function -Wno-unused-but-set-variable -O2 $^ -o $@
