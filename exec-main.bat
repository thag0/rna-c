echo off
cls

rem -- compilar --
set COMPILADOR=gcc
set ARQ=main.c
set SAIDA=main
set INCLUDE=-I./lib/rna

%COMPILADOR% %ARQ% -o %SAIDA% %INCLUDE% 

rem -- executar --
"%SAIDA%.exe"