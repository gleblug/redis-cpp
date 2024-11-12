all:
	gcc -Wall -Wextra -O2 -g server.c -o build/server
	gcc -Wall -Wextra -O2 -g client.c -o build/client