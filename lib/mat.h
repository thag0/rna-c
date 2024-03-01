#ifndef INC_MAT
   #define INC_MAT

   #include <stdio.h>
   #include <stdlib.h>
   #include <assert.h>
   #include <stdbool.h>
   #include <time.h>
   #include "arr.h"

   //Matriz genérica
   typedef struct{
      //quantidade de linhas
      int lin;

      //quantidade de colunas
      int col;

      //conjunto de elementos
      double* elementos;

      //tamanho do aray de elementos
      int _tam;
   }Mat;

   //print padrão de uma matriz
   #define mat_print(m) _mat_print(m, #m, "")

   #define _calcular_indice(mat, lin, col) (lin)*(m).col + (col)

   /**
    * Preenche todo o conteúdo da matriz.
    * @param m matriz alvo.
    * @param val valor desejado de preenchimento.
   */
   void mat_preencher(Mat m, double val){
      for(int i = 0; i < m._tam; i++){
         m.elementos[i] = val;
      }
   }

   /**
    * Aloca dinamicamente uma matriz vazia de 
    * acordo com a quantidade desejada de elementos.
    * @param linhas quantidade de linhas da matriz.
    * @param colunas quantidade de colunas da matriz.
    * @return matriz alocada.
   */
   Mat mat_alocar(int linhas, int colunas){
      Mat m;
      
      m.lin = linhas;
      m.col = colunas;
      m._tam = m.lin * m.col;
      m.elementos = (double*) malloc(sizeof(double) * m._tam);

      mat_preencher(m, 0);

      return m;
   }

   /**
    * Desaloca os elementos dinâmicos da matriz
    * @param m matriz.
   */
   void mat_desalocar(Mat m){
      free(m.elementos);
   }

   /**
    * Aleatoriza os valores da matriz dentro do intervalo.
    * @param m matriz alvo.
    * @param min valor mínimo.
    * @param max valor máximo.
   */
   void mat_randomizar(Mat m, double min, double max){
      for(int i = 0; i < m._tam; i++){
         double val = (double)rand() / (double)RAND_MAX;
         m.elementos[i] = min + val * (max-min);
      }
   }

   /**
    * Retorna o valor contido na matriz.
    * @param m matriz desejada.
    * @param lin índice da linha desejada.
    * @param col índice da coluna desejada.
    * @return valor contido de acordo com os índices.
   */
   double mat_elemento(Mat m, int lin, int col){
      return m.elementos[_calcular_indice(m, lin, col)];
   }

   /**
    * Edita o valor no conteúdo na matriz.
    * @param m matriz desejada.
    * @param lin índice da linha desejada.
    * @param col índice da coluna desejada.
    * @param val valor que será inserido.
   */
   void mat_editar(Mat m, int lin, int col, double val){
      m.elementos[_calcular_indice(m, lin, col)] = val;
   }

   /**
    * Preenche todo o conteúdo da matriz.
    * @param m matriz alvo.
    * @param crescente cresce ou n
   */
   void mat_preencher_contador(Mat m, bool crescente){
      if(crescente){
         for(int i = 0; i < m._tam; i++){
            m.elementos[i] = i + 1;
         }
      }else{
         int n = m._tam;
         for(int i = 0; i < m._tam; i++){
            m.elementos[i] = n - i;
         }
      }
   }

   /**
    * Exibe o conteúdo da matriz.
    * @param m matriz
   */
   void _mat_print(Mat m, char* nome, char* padIn){
      char* pad = "   ";

      printf("%s%s = [\n", padIn, nome);
      for(int i = 0; i < m.lin; i++){
         printf("%s%s", padIn, pad);
         for(int j = 0; j < m.col; j++){
            printf("%f  ", mat_elemento(m, i, j));
         }
         printf("\n");
      }
      printf("%s]\n", padIn);
   }

   /**
    * Transforma o conteúdo da matriz em identidade, onde todos os
    * elementos são 0 e os elementos da diagonal principal são 1.
    * @param m matriz.
   */
   void mat_identidade(Mat m){
      for(int i = 0; i < m.lin; i++){
         for(int j = 0; j < m.col; j++){
            mat_editar(m, i, j, (i == j ? 1 : 0));
         }
      }   
   }

   /**
    * Verifica se as linhas de A e B são iguais.
    * @param a primeira matriz.
    * @param b segunda matriz.
    * @return resultado da verificação
   */
   bool mat_comparar_linhas(Mat a, Mat b){
      return a.lin == b.lin;
   }

   /**
    * Verifica se as colunas de A e B são iguais.
    * @param a primeira matriz.
    * @param b segunda matriz.
    * @return resultado da verificação
   */
   bool mat_comparar_colunas(Mat a, Mat b){
      return a.col == b.col;
   }

   /**
    * Verifica se as colunas de A e B são iguais.
    * @param a primeira matriz.
    * @param b segunda matriz.
    * @return resultado da verificação
   */
   bool mat_comparar_linhas_colunas(Mat a, Mat b){
      return (a.lin == b.lin)&&(a.col == b.col);
   }

   /**
    * Realiza a operação de adição entre as matrizes, salvando o
    * resultado no destino.
    * @param dest matriz de destino para A + B.
    * @param a primeira matriz.
    * @param b segunda matriz.
   */
   void mat_add(Mat dest, Mat a, Mat b){
      mat_comparar_linhas_colunas(a, b);
      mat_comparar_linhas_colunas(a, dest);

      int n = dest._tam;
      for(int i = 0; i < n; i++){
         dest.elementos[i] = a.elementos[i] + b.elementos[i];
      }
   }

   /**
    * Realiza a operação de subtração entre as matrizes, salvando o
    * resultado no destino.
    * @param dest matriz de destino para A - B.
    * @param a primeira matriz.
    * @param b segunda matriz.
   */
   void mat_sub(Mat dest, Mat a, Mat b){
      mat_comparar_linhas_colunas(a, b);
      mat_comparar_linhas_colunas(a, dest);

      int n = dest._tam;
      for(int i = 0; i < n; i++){
         dest.elementos[i] = a.elementos[i] - b.elementos[i];
      }
   }

   /**
    * Realiza a operação de multiplicação matricial entre A e B, 
    * salvando o resultado no destino.
    * @param dest matriz de destino para A * B.
    * @param a primeira matriz.
    * @param b segunda matriz.
   */
   void mat_mult(Mat dest, Mat a, Mat b){
      assert(a.col == b.lin);
      assert(dest.lin == a.lin);
      assert(dest.col == b.col);

      int n = a.col;
      for(int i = 0; i < dest.lin; i++){
         for(int j = 0; j < dest.col; j++){
            double r = 0;
            for(int k = 0; k < n; k++){
               r += mat_elemento(a, i, k) * mat_elemento(b, k, j);
            }
            mat_editar(dest, i, j, r);
         }
      }
   }

   /**
    * Realiza a operação de multiplicação ponto a ponto entre as matrizes, 
    * salvando o resultado no destino.
    * @param dest matriz de destino para A ⊙ B.
    * @param a primeira matriz.
    * @param b segunda matriz.
   */
   void mat_had(Mat dest, Mat a, Mat b){
      mat_comparar_linhas_colunas(a, b);
      mat_comparar_linhas_colunas(a, dest);

      int n = dest._tam;
      for(int i = 0; i < n; i++){
         dest.elementos[i] = a.elementos[i] * b.elementos[i];
      }
   }

   /**
    * Multiplica todo o conteúdo da matriz por um valor constante.
    * @param dest matriz de destino.
    * @param m matriz alvo.
    * @param esc valor constante.
   */
   void mat_mult_escalar(Mat dest, Mat m, double esc){
      assert(mat_comparar_linhas_colunas(dest, m) && "Matriz e destino fornecidas devem possuir o mesmo formato.");

      for(int i = 0; i < dest._tam; i++){
         dest.elementos[i] = m.elementos[i] * esc;
      }
   }

   /**
    * Transpôe o conteúdo da matriz.
    * @param m matriz alvo.
    * @return matriz transposta.
   */
   Mat mat_transpor(Mat m){
      Mat t = mat_alocar(m.col, m.lin);

      for(int i = 0; i < m.lin; i++){
         for(int j = 0; j < m.col; j++){
            mat_editar(t, j, i, mat_elemento(m, i, j));
         }
      }

      return t;
   }

   /**
    * Copia o conteúdo da matriz para o destino.
    * @param dest matriz de destino.
    * @param m matriz com os dados.
   */
   void mat_copiar(Mat dest, Mat m){
      assert(mat_comparar_linhas_colunas(dest, m) && "Matriz e distino possuem formatos diferentes.");

      for(int i = 0; i < dest._tam; i++){
         dest.elementos[i] = m.elementos[i];
      }
   }

   /**
    * Copia o conteúdo da matriz para o destino.
    * @param dest matriz de destino.
    * @param m matriz alvo com os dados.
    * @param id_dest índice da linha na matriz de destino.
    * @param id_m índice da linha na matriz alvo.
   */
   void mat_copiar_linha(Mat dest, Mat m, int id_dest, int id_m){
      assert(0 < id_dest < dest.col);
      assert(0 < id_m < m.col);

      for(int i = 0; i < dest.col; i++){
         mat_editar(dest, id_dest, i, mat_elemento(m, id_m, i));
      }
   }

   /**
    * Copia o conteúdo do array para a matriz de destino.
    * @param dest matriz de destino.
    * @param arr array com os dados para cópia.
    * @param id_dest índice da linha na matriz de destino.
   */
   void mat_copiar_array(Mat dest, Array arr, int id_dest){
      assert(0 < id_dest < dest.col);
      assert(dest.col == arr._tam);

      for(int i = 0; i < dest.col; i++){
         mat_editar(dest, id_dest, i, arr_elemento(arr, i));
      }
   }

   /**
    * Copia o conteúdo do array para a matriz.
    * @param dest matriz de destino.
    * @param arr array contendo os dados.
    * @param tam_arr tamanho do array.
   */
   void mat_atribuir_array(Mat dest, double arr[], int tam_arr){
      assert(dest._tam == tam_arr);

      for(int i = 0; i < dest._tam; i++){
         dest.elementos[i] = arr[i];
      }
   }

   /**
    * Retorna um sub matriz contendo apenas os valores da
    * linha da matriz original.
    * @param m matriz base.
    * @param lin índice da linha desejada.
    * @return submatriz.
   */
   Mat mat_obter_linha(Mat m, int lin){
      assert(0 <= lin < m.lin);
      
      Mat linha = mat_alocar(1, m.col);
      for(int i = 0; i < m.col; i++){
         mat_editar(linha, 0, i, mat_elemento(m, lin, i));
      }

      return linha;
   }

   /**
    * Retorna um Array contendo apenas os valores da
    * linha da matriz original.
    * @param m matriz base.
    * @param lin índice da linha desejada.
    * @return submatriz.
   */
   Array mat_linha_para_array(Mat m, int lin){
      assert((lin >= 0 && lin < m.lin) && "Indice invalido");
      
      Array arr = arr_alocar(m.col);
      for(int i = 0; i < m.col; i++){
         arr_editar(arr, i, mat_elemento(m, lin, i));
      }

      return arr;
   }

#endif