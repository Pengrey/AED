#!/bin/bash
mkdir -p $(printf "%06d" $1)
for t in {1..64};do
  for p in {1..10};do
    if [[ !(-n  $( cat $(printf "%06d/%02d_%02d_%d.txt" $1 $t $p $3)) && -n $(grep "End" $(printf "%06d/%02d_%02d_%d.txt" $1 $t $p $3)) ) ]];then
      echo "timeout $2 ./job_selection $1 $t $p 0"
      time timeout $2 ./job_selection $1 $t $p 0 > /dev/null
      [[ $? -gt 0 ]] && continue
    fi
  done
done
