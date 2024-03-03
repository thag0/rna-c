echo off
cls

rem -- compilar --
set COMPILADOR=gcc
set ARQ=matriz.c
set SAIDA=saida/matriz
set INCLUDE=-I../lib/rna

%COMPILADOR% %ARQ% -o %SAIDA% %INCLUDE% 

rem -- executar --
"%SAIDA%.exe"