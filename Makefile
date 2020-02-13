OUT_DIR=output
SRC_DIR=src
EXEC=min_perf_check

.PHONY: output

all: compile

compile: output
	make -C $(SRC_DIR)

output:
	if [ ! -d $(OUT_DIR) ]; then mkdir $(OUT_DIR); fi

clean:
	make clean -C $(SRC_DIR)
	rm -rf $(OUT_DIR)
