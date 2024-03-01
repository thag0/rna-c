#include <stdio.h>

#include "rna.h"
#define RNA_INC

double dados_entrada[] = {
   0, 0,
   0, 1,
   1, 0,
   1, 1,
};

double dados_saida[] = {
   0,
   0,
   0,
   1,
};

void testar_dados(Densa densa, Mat entrada, Mat saida){
   int amostras = entrada.lin;
   char* pad = "   ";
   double perda = 0;

   printf("teste = [\n");
   for(int i = 0; i < amostras; i++){
      rna_densa_forward(densa, mat_linha_para_array(entrada, i));
      double prev = mat_elemento(densa.saida, 0, 0);
      double real = mat_elemento(saida, i, 0);
      printf("%s%.1f  %.1f -> %.1f, prev: %f\n",
         pad, 
         mat_elemento(entrada, i, 0), 
         mat_elemento(entrada, i, 1), 
         real,
         prev 
      );

      perda += perda_mse(
         mat_linha_para_array(densa.saida, 0) , mat_linha_para_array(saida, i));
   }
   perda /= 4;
   printf("%sperda: %f\n", pad, perda);
   printf("]\n");
}

int main(void){
   srand(time(NULL));

   Mat entrada = mat_alocar(4, 2);
   Mat saida = mat_alocar(4, 1);
   mat_atribuir_array(entrada, dados_entrada, arr_tam_arr(dados_entrada));
   mat_atribuir_array(saida, dados_saida, arr_tam_arr(dados_saida));
   
   Densa densa = rna_densa_alocar(entrada.col, saida.col);
   rna_densa_config_ativacao(densa, "tanh");

   double ta = 0.01;
   rna_treinar_densa(densa, entrada, saida, ta, 20*1000); 
   testar_dados(densa, entrada, saida);
   rna_densa_print(densa);

   return 0;
}