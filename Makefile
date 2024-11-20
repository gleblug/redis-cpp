СС=g++
CFLAGS=-Wall -Wextra -O2
BUILD=bin
SOURCE=src

all: server client

dirs:
	mkdir -p $(BUILD)

server: dirs
	$(CC) $(CFLAGS) $(SOURCE)/server.cpp -o $(BUILD)/server	

client: dirs
	$(CC) $(CFLAGS) $(SOURCE)/client.cpp -o $(BUILD)/client

clean:
	rm -r $(BUILD)