echo off
cls

set COMPILADOR=gcc
set INCLUDE=-I../lib/rna
set SAIDA=saida/

%COMPILADOR% matriz.c -o "%SAIDA%matriz" %INCLUDE% 
%COMPILADOR% densa.c -o "%SAIDA%densa" %INCLUDE%