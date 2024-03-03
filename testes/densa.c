#include <stdio.h>
#include <time.h>

#include "rna.h"
#include "densa.h"
#include "perdas.h"
#include "otimizadores.h"

double entrada[] = {
   0, 0,
   0, 1,
   1, 0,
   1, 1,
};

double saida[] = {
   0,
   1,
   1,
   1,
};

double pesos_2_1[] = {
   5.227311,
   5.228070
};

double bias_1_1[] = {
   -2.356084
};

int main(void){
   srand(time(NULL));
   Densa* densa = densa_alocar(2, 1);

   Mat* treino_x = mat_alocar(4, 2);
   mat_atribuir_array(treino_x, entrada, arr_tam_arr(entrada));

   Mat* treino_y = mat_alocar(4, 1);
   mat_atribuir_array(treino_y, saida, arr_tam_arr(saida));

   int amostras = treino_x->lin;
   int epocas = 10*1000;
   int t_a = 0.1;

   Array* grad = arr_alocar(densa->saida->_tam);
   for(int e = 0; e < epocas; e++){
      for(int i = 0; i < amostras; i++){
         densa_zerar_gradientes(densa);
         densa_forward(densa, mat_linha_para_array(treino_x, i));
         perda_mse_derivada(
            grad, 
            mat_linha_para_array(densa->saida, 0),
            mat_linha_para_array(treino_y, i)
         );
         densa_backward(densa, grad);
         otm_gradient_descent(
            densa->_pesos, densa->_grad_pesos,
            densa->_bias, densa->_grad_bias,
            t_a
         );
      }
   }
   arr_desalocar(grad);

   densa_print(densa);

   printf("Resultados\n");
   for(int i = 0; i < amostras; i++){
      densa_forward(densa, mat_linha_para_array(treino_x, i));
      double x1 = mat_elemento(treino_x, i, 0);
      double x2 = mat_elemento(treino_x, i, 1);
      double y = mat_elemento(treino_y, i, 0);
      printf("%.1f  %.1f  -> %.1f   prev: %f\n", x1, x2, y, mat_elemento(densa->saida, 0, 0));
   }

   mat_desalocar(treino_x);
   mat_desalocar(treino_y);
   densa_desalocar(densa);

   return 0;
}