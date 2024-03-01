#include <stdio.h>
#include "rna.h"

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
   0,
};

/**
 * .
 * @param .
 * @param .
 * @param .
*/
void testar_dados(MLP rede, Mat entrada, Mat saida){
   int amostras = entrada.lin;
   char* pad = "   ";
   double perda = 0;

   printf("teste = [\n");
   for(int i = 0; i < amostras; i++){
      rna_mlp_forward(rede, mat_linha_para_array(entrada, i));
      double prev = mat_elemento(rna_mlp_saida(rede), 0, 0);
      double real = mat_elemento(saida, i, 0);
      printf("%s%.1f  %.1f -> %.1f, prev: %f\n",
         pad, 
         mat_elemento(entrada, i, 0), 
         mat_elemento(entrada, i, 1), 
         real,
         prev 
      );

      perda += perda_mse(
         mat_linha_para_array(rna_mlp_saida(rede), 0) , mat_linha_para_array(saida, i));
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

   Array arq = arr_alocar(3);
   double arr[] = {2, 3, 1};
   arr_atribuir_array(arq, arr, 3);

   MLP rede = rna_alocar_mlp(arq);
   rna_print(rede);

   rna_mlp_treino(rede, entrada, saida, 0.1, 10*1000);
   testar_dados(rede, entrada, saida);

   return 0;
}