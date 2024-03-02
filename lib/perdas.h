#ifndef INC_PERDA
   #define INC_PERDA
   
   #include "mat.h"
   #include "arr.h"

   /**
    * Calcula o erro médio quadrado entre os dados previstos e os dados reais.
    * @param prev valores previstos.
    * @param real valores reais.
    * @return valor de perda
   */
   double perda_mse(Array prev, Array real){
      if(prev._tam != real._tam){
         printf(
            "Os tamanhos dos arrays previsto (%d) e real (%d) devem ser iguais.",
            prev._tam,
            real._tam
         );
         assert(0);
      }

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
      if((dest._tam != prev._tam) || (dest._tam != real._tam)){
         printf(
            "Os tamanhos dos arrays de destino (%d), previsto (%d) e real (%d) devem ser iguais.",
            dest._tam,
            prev._tam,
            real._tam
         );
         assert(0);
      }

      int n = prev._tam;
      for(int i = 0; i < n; i++){
         arr_editar(dest, i, (
            (2.0 / n) * (arr_elemento(prev, i) - arr_elemento(real, i))
         ));
      }
   }

   /**
    * Calcula o valor de entropia cruzada categórica entre as probabilidades previstas
    * e os rótulos reais.
    * @param prev probabilidades previstas.
    * @param real rótulos reais.
   */
   double perda_entropia_cruzada(Array prev, Array real){
      if(prev._tam != real._tam){
         printf(
            "Os tamanhos dos arrays de probabilidade prevista (%d) e rótulo real (%d) devem ser iguais.",
            prev._tam,
            real._tam
         );
         assert(0);
      }

      double perda = 0;
      int n = prev._tam;
      for(int i = 0; i < n; i++){
         perda += -arr_elemento(real, i) * log(arr_elemento(prev, i));
      }
      
      return perda / n;
   }

   /**
    * Calcula a derivada da entropia cruzada categórica entre as probabilidades previstas
    * e os rótulos reais.
    * @param dest array de destino.
    * @param prev probabilidades previstas.
    * @param real rótulos reais.
   */
   void perda_entropia_cruzada_derivada(Array dest, Array prev, Array real){
      if((dest._tam != prev._tam) || (dest._tam != real._tam)){
         printf(
            "Os tamanhos dos arrays de destino (%d), probabilidade prevista (%d) e rótulo real (%d) devem ser iguais.",
            dest._tam,
            prev._tam,
            real._tam
         );
         assert(0);
      }

      int n = prev._tam;
      for(int i = 0; i < n; i++){
         arr_editar(dest, i, (arr_elemento(prev, i) - arr_elemento(real, i)));
      }
   }

#endif