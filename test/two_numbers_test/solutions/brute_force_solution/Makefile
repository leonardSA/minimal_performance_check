OUT_DIR=output
CC=gcc

SRC_IMPL=$(wildcard *.c)
OBJ_IMPL=$(patsubst %.c, %.o, $(SRC_IMPL))
LFLAGS=
CFLAGS=
ENTRIES=../../../data_folder/entries.txt

EXEC=twosum
TEST_EXEC=min_perf_check 

.PHONY: output

test: output copy $(EXEC) move 
	cd $(OUT_DIR) ;\
	./$(TEST_EXEC) -e $(EXEC) -v $(ENTRIES) -x 1

$(EXEC): $(OBJ_IMPL)  
	$(CC) $(OBJ_IMPL) $(LFLAGS) -o $(EXEC)
	
%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

copy:
	cp ../../../../$(OUT_DIR)/$(TEST_EXEC) $(OUT_DIR)

clean:
	rm -f *.o
	rm -rf $(OUT_DIR)

move:
	mv $(EXEC) ./$(OUT_DIR)/

output:
	if [ ! -d $(OUT_DIR) ]; then mkdir $(OUT_DIR); fi
