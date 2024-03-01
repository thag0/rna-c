#ifndef PERDAS_INC
   #define PERDAS_INC
   
   #include "mat.h"
   #define MAT_INC

   #include "arr.h"

   //implementações

   /**
    * Calcula o erro médio quadrado entre os dados previstos e os dados reais.
    * @param prev valores previstos.
    * @param real valores reais.
    * @return valor de perda
   */
   double perda_mse(Array prev, Array real){
      assert(prev._tam == real._tam && "Os arrays devem possuir tamanhos iguais.");

      double perda = 0;
      int n = prev._tam;
      for(int i = 0; i < n; i++){
         double dif = arr_elemento(prev, i) - arr_elemento(real, i);
         perda += dif*dif;
      }
      
      return perda / n;
   }

   /**
    * Calcula a derivada do erro médio quadrado entre os dados previstos e os dados reais.
    * @param dest matriz de destino dos cálculos de derivadas..
    * @param prev valores previstos.
    * @param real valores reais.
   */
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