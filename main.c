#include <stdio.h>

#include "rna.h"
#define RNA_IMPL

#include "perdas.h"
#define PERDA_IMPL

double dados_entrada[] = {
   0, 0,
   0, 1,
   1, 0,
   1, 1,
};

double dados_saida[] = {
   0,
   1,
   1,
   1,
};

void treinar_densa(Densa camada, Mat entrada, Mat saida, double ta, int epochs){
   assert(camada.saida.col == saida.col && "Saida da cama matriz de saida possuem colunas difernetes.");

   int amostras = entrada.lin;
   Mat grad = mat_alocar(1, camada.saida.col);

   for(int e = 0; e < epochs; e++){
      for(int j = 0; j < amostras; j++){
         rna_densa_forward(camada, mat_obter_linha(entrada, j));
         perda_mse_derivada(grad, camada.saida, saida);
         rna_densa_backward(camada, grad);

         mat_mult_escalar(camada._grad_pesos, camada._grad_pesos, ta);
         mat_mult_escalar(camada._grad_bias, camada._grad_bias, ta);

         mat_add(camada._pesos, camada._pesos, camada._grad_pesos);
         mat_add(camada._bias, camada._bias, camada._grad_bias);
      }
   }

   mat_desalocar(grad);
}

void testar_dados(Densa densa, Mat entrada, Mat saida){
   int amostras = entrada.lin;

   for(int i = 0; i < amostras; i++){
      rna_densa_forward(densa, mat_obter_linha(entrada, i));
      double prev = mat_elemento(densa.saida, 0, 0);
      double real = mat_elemento(saida, i, 0);
      printf("%.1f  %.1f -> prev: %.1f real %.1f\n", mat_elemento(entrada, i, 0), mat_elemento(entrada, i, 1), prev, real);
   }
}

int main(void){
   Densa densa = rna_densa_alocar(2, 1);
   // rna_densa_print(densa);

   Mat entrada = mat_alocar(4, 2);
   Mat saida = mat_alocar(4, 1);
   mat_atribuir_array(entrada, dados_entrada, tam_arr(dados_entrada));
   mat_atribuir_array(saida, dados_saida, tam_arr(dados_saida));

   double ta = 0.1;
   double perda; 
   perda = perda_mse(densa, entrada, saida);
   printf("perda: %f\n", perda);

   treinar_densa(densa, entrada, saida, ta, 20*1000);   
   
   perda = perda_mse(densa, entrada, saida);
   printf("perda: %f\n", perda);
   
   testar_dados(densa, entrada, saida);

   return 0;
}