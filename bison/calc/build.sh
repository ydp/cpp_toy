#!/bin/bash


make clean
bison -d -v calc.y
flex calc.lex
cp calc.tab.h tok.h
g++ -o calc calc.tab.c lex.yy.c main.cc /usr/local/opt/flex/lib/libfl.a
