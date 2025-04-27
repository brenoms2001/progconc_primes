#!/bin/bash

gcc -o geraNumeros geraNumeros.c
for ((i=10; i<10000000; i*=10))
do
    echo "Gerando lista com $i números"
    ./geraNumeros $i
done