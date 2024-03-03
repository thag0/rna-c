#ifndef INC_PERDA
   #define INC_PERDA
   
   #include "mat.h"
   #include "arr.h"

   /**
    * Função de perda genérica.
   */
   typedef struct{
      //Calcula o valor de perda.
      double (*calcular) (Array* prev, Array* real);

      //Calcula a derivada da função de perda.
      void (*derivada) (Array* dest, Array* prev, Array* real);
   }Perda;

   /**
    * Calcula o erro médio quadrado entre os dados previstos e os dados reais.
    * @param prev valores previstos.
    * @param real valores reais.
    * @return valor de perda
   */
   double perda_mse(Array* prev, Array* real){
      if(prev->_tam != real->_tam){
         printf(
            "Os tamanhos dos arrays previsto (%d) e real (%d) devem ser iguais.",
            prev->_tam,
            real->_tam
         );
         assert(0);
      }

      double perda = 0;
      int n = prev->_tam;
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
   void perda_mse_derivada(Array* dest, Array* prev, Array* real){
      if((dest->_tam != prev->_tam) || (dest->_tam != real->_tam)){
         printf(
            "Os tamanhos dos arrays de destino (%d), previsto (%d) e real (%d) devem ser iguais.",
            dest->_tam,
            prev->_tam,
            real->_tam
         );
         assert(0);
      }

      int n = prev->_tam;
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
   double perda_entropia_cruzada(Array* prev, Array* real){
      if(prev->_tam != real->_tam){
         printf(
            "Os tamanhos dos arrays previsto (%d) e real (%d) devem ser iguais.",
            prev->_tam,
            real->_tam
         );
         assert(0);
      }

      double perda = 0;
      int n = prev->_tam;
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
   void perda_entropia_cruzada_derivada(Array* dest, Array* prev, Array* real){
      if((dest->_tam != prev->_tam) || (dest->_tam != real->_tam)){
         printf(
            "Os tamanhos dos arrays de destino (%d), previsto (%d) e real (%d) devem ser iguais.",
            dest->_tam,
            prev->_tam,
            real->_tam
         );
         assert(0);
      }

      int n = prev->_tam;
      for(int i = 0; i < n; i++){
         arr_editar(dest, i, (arr_elemento(prev, i) - arr_elemento(real, i)));
      }
   }

   /**
    * Aloca uma nova função de perda.
    * @param nome nome da função desejada.
    * @return função de perda.
   */
   Perda* perda_alocar(char* nome){
      Perda* perda = (Perda*) malloc(sizeof(Perda));
      assert(perda != NULL && "perda_alocar, perda");

      if(strcmp(nome, "mse") == 0){
         perda->calcular = perda_mse;
         perda->derivada = perda_mse_derivada;
         return perda;

      }else if(strcmp(nome, "ec") == 0){
         perda->calcular = perda_entropia_cruzada;
         perda->derivada = perda_entropia_cruzada_derivada;
         return perda;

      }else{
         printf("Perda \"%s\" nao suportada.", nome);
         assert(0);
      }
   }

   /**
    * Desaloca os dados dinâmicos da função de perda.
    * @param perda função de perda.
   */
   void perda_desalocar(Perda* perda){
      free(perda);
   }

#endif