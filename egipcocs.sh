#!/bin/sh
# @(#)egipcocs.sh	1.1 95/08/17 jsloan@ncar.ucar.edu

H=echo.ucar.edu
P="-p -s 5999"
HR=/home/alpine0/jsloan/src/libipc
RR=/home/aztec0/jsloan/src/libipc

I=0
for W in 10 100 1000; do
 for K in 1 10 100 1000 10000; do
  for O in "" "-b 100 -l 6"; do
   for N in "" "-n"; do
    echo "# $I $W $K $O $N"
    echo "rsh $H $HR/egipcocs $P -k $K $O $N >>& SERVER &"
    echo "sleep 5"
    echo "$RR/egipcocs $P -c -h $H -w $W -k $K $O $N >>& CLIENT"
    echo "wait"
    I=`expr $I + 1`
   done
  done
 done
done
