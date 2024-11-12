all: dirs program

dirs:
	mkdir -p build

program:
	gcc -Wall -Wextra -O2 -g server.c -o build/server
	gcc -Wall -Wextra -O2 -g client.c -o build/client

clean:
	rm -r build