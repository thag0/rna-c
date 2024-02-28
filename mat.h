#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

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
} Mat;

//print padrão de uma matriz
#define mat_print(m) _mat_print(m, #m, "")

//calcula o tamanho de um array
#define tam_arr(arr) sizeof(arr)/sizeof(arr[0])

#ifndef MAT_IMPL
#define MAT_IMPL

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
   m.elementos = (double*) malloc(sizeof(double) * linhas * colunas);
   m._tam = m.lin * m.col;

   for(int i = 0; i < m._tam; i++){
      m.elementos[i] = 0;
   }

   return m;
}

/**
 * Desaloca os elementos dinâmicos da matriz
 * @param m matriz.
*/
void mat_desalocar(Mat m){
   free(m.elementos);
}

void mat_randomizar(Mat m, double min, double max){
   srand(time(0));

   for(int i = 0; i < m._tam; i++){
      m.elementos[i] = (rand() / (double)RAND_MAX) * ((max - min) + min);
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
   return m.elementos[lin*m.lin + col];
}

/**
 * Edita o valor no conteúdo na matriz.
 * @param m matriz desejada.
 * @param lin índice da linha desejada.
 * @param col índice da coluna desejada.
 * @param val valor que será inserido.
*/
void mat_editar(Mat m, int lin, int col, double val){
   m.elementos[lin*m.lin + col] = val;
}

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
 * Realiza a operação de adição entre as matrizes, salvando o
 * resultado no destino.
 * @param dest matriz de destino para A + B.
 * @param a primeira matriz.
 * @param b segunda matriz.
*/
void mat_add(Mat dest, Mat a, Mat b){
   assert(a.lin == b.lin);
   assert(a.col == b.col);
   assert(a.lin == dest.lin);
   assert(a.col == dest.col);

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
   assert(a.lin == b.lin == dest.lin);
   assert(a.col == b.col == dest.col);

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
void mad_had(Mat dest, Mat a, Mat b){
   assert(a.lin == b.lin == dest.lin);
   assert(a.col == b.col == dest.col);

   int n = dest._tam;
   for(int i = 0; i < n; i++){
      dest.elementos[i] = a.elementos[i] * b.elementos[i];
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

void mat_copiar_array(Mat m, double arr[], int tam_arr, int lin){
   assert(tam_arr == m.col);
   assert(0 <= lin < tam_arr);

   for(int i = 0; i < m.col; i++){
      mat_editar(m, lin, i, arr[i]);
   }
}

void mat_copiar_linha(Mat dest, Mat m, int id_dest, int id_m){
   assert(dest.col == m.col);

   for(int i = 0; i < dest.col; i++){
      mat_editar(dest, id_dest, i, mat_elemento(m, id_m, i));
   }
}

#endif