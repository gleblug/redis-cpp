СС=g++
CFLAGS=-Wall -Wextra -O2 -g

all: dirs program

dirs:
	mkdir -p build

program:
	$(CC) $(CFLAGS) server.cpp -o build/server
	$(CC) $(CFLAGS) client.cpp -o build/client

clean:
	rm -r build