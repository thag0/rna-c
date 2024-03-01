#ifndef INC_ARR
   #define INC_ARR

   #include "assert.h"
   #include "stdlib.h"

   /**
    * Array genérico.
   */
   typedef struct{
      /**
       * Quantidade de elementos do array.
      */
      int _tam;

      /**
       * Conjunto de elementos do array.
      */
      double* elementos;
   }Array;

   /**
    * Preenche todo o conteúdo do array com o valor fornecido.
    * @param arr array desejado.
    * @param val valor de preenchimento.
   */
   void arr_preencher(Array arr, double val){
      for(int i = 0; i < arr._tam; i++){
         arr.elementos[i] = val;
      }
   }

   /**
    * Aloca um array dinamicamente.
    * @param tam tamanho do array.
    * @return array alocado.
   */
   Array arr_alocar(int tam){
      assert(tam > 0 && "Tamanho do array deve ser maior que zero.");

      Array arr;
      arr._tam = tam;
      arr.elementos = (double*) malloc(sizeof(double) * arr._tam);

      arr_preencher(arr, 0);

      return arr;
   }

   /**
    * Recupera o valor contido no array.
    * @param arr array desejado.
    * @param id índice do elemento.
    * @return valor contido no array.
   */
   double arr_elemento(Array arr, int id){
      assert((id >= 0 && id < arr._tam) && "Indice invalido.");
      return arr.elementos[id];
   }

   /**
    * Edita o valor dentro do array.
    * @param arr array desejado,
    * @param id índice dentro do array.
    * @param val novo valor para o elemento.
   */
   void arr_editar(Array arr, int id, double val){
      assert((id >= 0 && id < arr._tam) && "Indice invalido.");
      arr.elementos[id] = val;
   }

#endif