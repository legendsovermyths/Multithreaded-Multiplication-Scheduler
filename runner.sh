#!/bin/bash
x='x'
for(( k=5 ; k<50 ; k++));do
    out=`python3 generate.py $k$x$k $k$x$k`
    
    out2=`./final.out $k`
    echo "$out2"
done