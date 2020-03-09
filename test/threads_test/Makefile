OUT_DIR=output
CC=gcc

SRC_IMPL=$(wildcard *.c)
OBJ_IMPL=$(patsubst %.c, %.o, $(SRC_IMPL))
LFLAGS=
CFLAGS=
ENTRIES=entries.txt

EXEC=bad_pow
TEST_EXEC=min_perf_check 
TEST_SCRIPT=test.sh
NB_THREADS=4

.PHONY: output

test: output copy data $(EXEC) move 
	./$(TEST_SCRIPT) $(NB_THREADS)

$(EXEC): $(OBJ_IMPL)  
	$(CC) $(OBJ_IMPL) $(LFLAGS) -o $(EXEC)
	
%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

data:
	for i in `seq -100000 10 100000`; do echo 2,$$i >> $(OUT_DIR)/$(ENTRIES); done

copy:
	cp ../../$(OUT_DIR)/$(TEST_EXEC) $(OUT_DIR)

clean:
	rm -f *.o
	rm -rf $(OUT_DIR)

move:
	mv $(EXEC) ./$(OUT_DIR)/

output:
	if [ ! -d $(OUT_DIR) ]; then mkdir $(OUT_DIR); fi
