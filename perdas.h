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
    * @param dest destino dos cálculos de derivadas..
    * @param prev valores previstos.
    * @param real valores reais.
   */
   void perda_mse_derivada(Array dest, Array prev, Array real){
      assert(prev._tam == real._tam && "Previsto e Real possuem tamanhos diferentes");
      assert(dest._tam == prev._tam && "Destino e Previsto possuem tamanhos diferentes");

      int n = prev._tam;
      for(int i = 0; i < n; i++){
         arr_editar(dest, i, (
            (2.0 / n) * (arr_elemento(prev, i) - arr_elemento(real, i))
         ));
      }
   }

#endif