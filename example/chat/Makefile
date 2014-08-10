.PHONY:clean
CC=g++
CFLAGS=-Wall -g
BIN=server
OBJS=main.o ChatServer.o
OTHER=-std=c++0x
LINKS=-lecho
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LINKS)
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(OTHER)
clean:
	rm -f *.o $(BIN)
