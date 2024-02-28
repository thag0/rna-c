#ifndef PERDA_IMPL
#include "mat.h"
#include "rna.h"

double perda_mse(Densa camada, Mat entrada, Mat saida){
   assert(entrada.lin == saida.lin && "Entrada e saida possuem tamanhos diferentes");
   assert(camada.saida.col == saida.col && "Saida da camada e matriz de saida possuem tamanhos diferentes");

   double perda = 0;
   int amostras = entrada.lin;
   for(int i = 0; i < amostras; i++){
      rna_densa_forward(camada, mat_obter_linha(entrada, i));
      
      for(int j = 0; j < camada.saida.col; j++){
         double prev = mat_elemento(camada.saida, 0, j);
         double real = mat_elemento(saida, i, j);
         double dif = prev - real;
         perda += dif*dif;
      }
   }
   
   return perda / amostras;
}

void perda_mse_derivada(Mat dest, Mat prev, Mat real){
   assert(mat_comparar_colunas(prev, real) && "Previsto e Real possuem colunas diferentes");
   assert(mat_comparar_colunas(dest, prev) && "Destino e Previsto possuem colunas diferentes");

   double perda = 0;
   int n = prev.lin;

   double grad[n];
   for(int i = 0; i < n; i++){
      mat_editar(dest, 0, i, (
         (2.0 / n) * (mat_elemento(prev, 0, i) - mat_elemento(real, 0, i))
      ));
   }
}

#endif