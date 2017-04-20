.PHONY:clean
CC=g++
CPPFLAGS=-Wall -g
BIN=minishell
OBJS=main.o parse.o init.o execute.o builtin.o
$(BIN):$(OBJS)
	$(CC) $(CPPFLAGS) $^ -o $@
%.o:%.c
	$(CC) -c $(CPPFLAGS) $< -o $@
clean:
	rm -f *.o $(BIN)
