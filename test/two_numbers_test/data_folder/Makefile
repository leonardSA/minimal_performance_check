CC=gcc
SRC_IMPL=$(wildcard *.c)
OBJ_IMPL=$(patsubst %.c, %.o, $(SRC_IMPL))
LFLAGS=
CFLAGS=
DATA_FILE_EXT=dat
DATA_DIR=data_folder
ENTRIES=entries.txt
DEPTH=../../../
TARGET=9
EXEC=gen_random_ints

test: $(EXEC) execute entries

$(EXEC): $(OBJ_IMPL)  
	$(CC) $(OBJ_IMPL) $(LFLAGS) -o $(EXEC)
	
%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

execute:
	./$(EXEC) 100000 100 -1000 1000

entries:
	for f in `ls *$(DATA_FILE_EXT)`; do \
		nb_lines=`wc -l $$f | cut -d' ' -f1` ;\
		echo "$$nb_lines,$(DEPTH)$(DATA_DIR)/$$f,$(TARGET)" >> $(ENTRIES)_tmp ;\
	done ;\
	cat $(ENTRIES)_tmp | sort -n > $(ENTRIES) ;\
	rm $(ENTRIES)_tmp

clean:
	rm -f *.o
	rm -f $(EXEC)
	rm -f *$(DATA_FILE_EXT)
	rm -f $(ENTRIES)
