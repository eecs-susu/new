#!/bin/bash
for dir in */
do
    cd "${dir}" && make && make clean && cd ..
done
