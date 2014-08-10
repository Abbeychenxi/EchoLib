.PHONY:clean
CC=g++
CFLAGS=-Wall -g
BIN=server
OBJS=main.o QueryServer.o TextQuery.o
OTHER=-std=c++0x
LINKS=-lecho -lpthread
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LINKS)
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(OTHER)
clean:
	rm -f *.o $(BIN)
