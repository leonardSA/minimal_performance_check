OUT_DIR=output
CC=gcc

SRC_IMPL=$(wildcard *.c)
OBJ_IMPL=$(patsubst %.c, %.o, $(SRC_IMPL))

EXEC=min_perf_check

all: $(EXEC) move 

$(EXEC): $(OBJ_IMPL)  
	$(CC) $(OBJ_IMPL) $(LFLAGS) -o $(EXEC)
	
%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o

move:
	mv $(EXEC) ../$(OUT_DIR)/

