#ifndef ATV_IMPL

#include "mat.h"
#include <math.h>

/**
 * Aplica a função de ativação relu na matriz M.
 * @param dest matriz de destino.
 * @param m matriz base para aplicação da função.
*/
void act_relu(Mat dest, Mat m){
   assert(mat_comparar_linhas_colunas(dest, m));

   for(int i = 0; i < dest._tam; i++){
      dest.elementos[i] = (m.elementos[i] > 0) ? m.elementos[i] : 0;
   }
}

/**
 * Aplica a função de ativação relu na matriz M.
 * @param dest matriz de destino.
 * @param m matriz base para aplicação da função.
*/
void act_relu_derivada(Mat dest, Mat m){
   assert(mat_comparar_linhas_colunas(dest, m));

   for(int i = 0; i < dest._tam; i++){
      dest.elementos[i] = (m.elementos[i] > 0) ? 1 : 0;
   }
}

double _sigmoid(double x){
   return 1 / (1 + exp(-x));
}

double _sigmoid_dx(double x){
   double s = _sigmoid(x);
   return s * (1 - s);
}

/**
 * Aplica a função de ativação sigmoid na matriz M.
 * @param dest matriz de destino.
 * @param m matriz base para aplicação da função.
*/
void act_sigmoid(Mat dest, Mat m){
   assert(mat_comparar_linhas_colunas(dest, m));

   for(int i = 0; i < dest._tam; i++){
      dest.elementos[i] = _sigmoid(m.elementos[i]);
   }
}

/**
 * Aplica a função de ativação sigmoid na matriz M.
 * @param dest matriz de destino.
 * @param m matriz base para aplicação da função.
*/
void act_sigmoid_derivada(Mat dest, Mat m){
   assert(mat_comparar_linhas_colunas(dest, m));

   for(int i = 0; i < dest._tam; i++){
      double s = _sigmoid(m.elementos[i]);
      dest.elementos[i] = s * (1 - s);
   }
}

#endif