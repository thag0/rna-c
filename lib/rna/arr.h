#ifndef INC_ARR
   #define INC_ARR

   #include "assert.h"
   #include "stdlib.h"

   /**
    * Array dinâmico genérico.
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

   //calcula o tamanho de um array
   #define arr_tam(arr) sizeof(arr)/sizeof(arr[0])

   /**
    * Preenche todo o conteúdo do array com o valor fornecido.
    * @param arr array desejado.
    * @param val valor de preenchimento.
   */
   void arr_preencher(Array* arr, double val){
      for(int i = 0; i < arr->_tam; i++){
         arr->elementos[i] = val;
      }
   }

   /**
    * Aloca um array dinamicamente.
    * @param tam tamanho do array.
    * @return array alocado.
   */
   Array* arr_alocar(int tam){
      assert(tam > 0 && "Tamanho do array deve ser maior que zero.");

      Array* arr = (Array*) malloc(sizeof(Array));
      assert(arr != NULL && "arr_alocar, arr");

      arr->_tam = tam;
      arr->elementos = (double*) malloc(sizeof(double) * arr->_tam);
      assert(arr != NULL && "arr_alocar, arr->elementos");

      arr_preencher(arr, 0);

      return arr;
   }

   /**
    * Desaloca os elementos dinâmicos do array.
    * @param arr array desejado.
   */
   void arr_desalocar(Array* arr){
      free(arr->elementos);
      free(arr);
   }

   /**
    * Exibe o conteúdo do array.
    * @param arr array desejado.
   */
   void arr_print(Array* arr){
      char* pad = "   ";
      printf("Array = [\n%s", pad);
      for(int i = 0; i < arr->_tam; i++){
         printf("%f", arr->elementos[i]);
      }
      printf("]\n");
   }

   /**
    * Atribui os valores do array ao destino.
    * @param dest estrutura de array para destino.
    * @param arr array desejado para atribuição.
   */
   void arr_atribuir_array(Array* dest, double arr[], int tam_arr){
      assert(dest->_tam == tam_arr);
      
      for(int i = 0; i < dest->_tam; i++){
         dest->elementos[i] = arr[i];
      }
   }

   /**
    * Recupera o valor contido no array.
    * @param arr array desejado.
    * @param id índice do elemento.
    * @return valor contido no array.
   */
   double arr_elemento(Array* arr, int id){
      assert((id >= 0 && id < arr->_tam) && "Indice invalido.");
      return arr->elementos[id];
   }

   /**
    * Edita o valor dentro do array.
    * @param arr array desejado,
    * @param id índice dentro do array.
    * @param val novo valor para o elemento.
   */
   void arr_editar(Array* arr, int id, double val){
      assert((id >= 0 && id < arr->_tam) && "Indice invalido.");
      arr->elementos[id] = val;
   }

   /**
    * Clona o conteúd do array numa estrutura separada.
    * @param arr array desejado.
    * @return clone do array.
   */
   Array* arr_clonar(Array* arr){
      Array* clone = arr_alocar(arr->_tam);

      for(int i = 0; i < clone->_tam; i++){
         clone->elementos[i] = arr->elementos[i];
      }

      return clone;
   }

#endif