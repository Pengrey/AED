#!/bin/bash

##################
#
# file processor script
#
#
#   $1 for /path/to/files
#   other argumentes will be ignored
# tb quero uma lista com os profits em k é uma por linha e por ordem :/
# o meu portugues tb n é muito bom
# em caso de duvida dizeu preciso dos P's :/
##pk tb quero as tasks k n têm P's em baixo
#uma por linha
#como está no file
#sei k estou a ser chato mas foi a melhor maneira k arranjei
#de por a funcionar :/
##################
rm files/*
dir=$( printf "%s/%s" "$(pwd)" "$1")
files=( $(grep "End" $dir*.txt  | awk -F "/|:" '{print $9}') )
echo ${#files[@]}
for f in ${files[@]};do
  number_of_tasks=$(awk -F "_" '{printf $1 "\n"}' <<< $f | sed "s/^0//g")
  number_of_programmers=$(awk -F "_" '{printf $2 "\n"}' <<< $f | sed "s/^0//g")
  profit_is_considered=$(awk -F "_" '{printf $3}' <<< $f )
  profit_filename=$(printf "files/profits_%02d_%02d_%s" $number_of_tasks $number_of_programmers "$profit_is_considered")
  task_filename=$(printf "files/tasks_%02d_%02d_%s" $number_of_tasks $number_of_programmers "$profit_is_considered")
  #general_filename=$(printf "files/general_%d_%d_%s" $number_of_tasks $number_of_programmers "$profit_is_considered")
  count=0
  for p in $(seq -1 $(( $number_of_programmers - 1)) );do
    [[ $p != -1 ]] && printf "P$p " >> $task_filename
    head -n $number_of_tasks 097827/$f | awk -v p="P"$p -F "\t" '$0~p{if(p == "P-1") {printf "P-1 " $2 "\n" } else {printf $2 " "} }' >> $task_filename
    [[ $p != -1 ]] && printf "\n" >> $task_filename
  done;
  count=$(grep "P-1" $task_filename | wc -l)
  head -$count $task_filename | sed "s/^P-1 //g" >> $task_filename
  sed -i 1,"$count"d $task_filename
  grep "^  " 097827/$f | awk '{ printf $3"\n"}'  > $profit_filename
done
