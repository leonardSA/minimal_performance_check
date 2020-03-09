#!/usr/bin/env bash

OUT_DIR=output
ENTRIES=entries.txt
EXEC=bad_pow
TEST_EXEC=min_perf_check 

DATA_FILE=data.dat
GRAPH_FILE=graph.svg

TIME_DATA_FILE=time.dat

cd $OUT_DIR

for i in `seq 1 $1`; do
    result_time=`(time ./$TEST_EXEC -e $EXEC -v $ENTRIES -x 2 -t "Bad pow (--parallel $i)" -n "pow(2, x)" -p $i) |& grep "real"`
    mv $DATA_FILE p$i\_$DATA_FILE
    mv $GRAPH_FILE p$i\_$GRAPH_FILE
    result_time=`echo $result_time | egrep -o "[0-9]+m[0-9]+\.[0-9]+s"`
    echo "Time taken for --parallel $i: $result_time"
done
