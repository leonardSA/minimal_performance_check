OUT_DIR=output
SRC_DIR=src
TEST_DIR=test
EXEC=min_perf_check

.PHONY: output test

all: compile

test:
	make test -C $(TEST_DIR)

compile: output
	make -C $(SRC_DIR)

output:
	if [ ! -d $(OUT_DIR) ]; then mkdir $(OUT_DIR); fi

clean:
	make clean -C $(SRC_DIR)
	make clean -C $(TEST_DIR)
	rm -rf $(OUT_DIR)
