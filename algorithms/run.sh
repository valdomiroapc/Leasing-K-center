#!/bin/bash
i=0;
make approximation;
while [ $i -lt 1000 ]; do
    ./approximation;
    let i=i+1;
done 
