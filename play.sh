#!/bin/bash

gcc -o primos primos.c -lm

threads=(1 2 4)

for i in ${threads[@]}
do
    echo "Executando com $i thread"
    for ((j=0; j<10; j++))
    do
        for ((k=10; k<10000000; k*=10))
        do 
            ./primos lista$k $i
        done
    done
done

echo "Todas as execuções foram salvas no arquivo tempos.csv"

gcc -o gerarMedias gerarMedias.c

./gerarMedias

echo "Medias calculadas e salvas no arquivo medias.csv"