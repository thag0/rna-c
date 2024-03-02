#include <stdio.h>
#include "lib/rna.h"

/**
 * .
 * @param .
 * @param .
 * @param .
*/
void testar_dados(Mlp rede, Mat entrada, Mat saida){
   int amostras = entrada.lin;
   char* pad = "    ";
   double perda = 0;

   printf("Resultados = [\n");
      for(int i = 0; i < amostras; i++){
         rna_mlp_forward(rede, mat_linha_para_array(entrada, i));
         double prev = mat_elemento(rna_mlp_saida(rede), 0, 0);
         double real = mat_elemento(saida, i, 0);

         printf("%s%.1f  %.1f -> %.1f, prev: %.8f\n",
            pad, 
            mat_elemento(entrada, i, 0), 
            mat_elemento(entrada, i, 1), 
            real,
            prev 
         );

         perda += perda_mse(
            mat_linha_para_array(rna_mlp_saida(rede), 0), 
            mat_linha_para_array(saida, i)
         );
      }
      printf("\n%sperda: %.8f\n", pad, (perda / amostras));
   printf("]\n");
}

/**
 * Testes com um modelo para aprender a se comportar como
 * uma porta lógica XOR.
*/
void modelo_xor(){
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

   Mat entrada = mat_alocar(4, 2);
   Mat saida = mat_alocar(4, 1);
   mat_atribuir_array(entrada, dados_entrada, arr_tam_arr(dados_entrada));
   mat_atribuir_array(saida, dados_saida, arr_tam_arr(dados_saida));

   double arr[] = {2, 3, 1};
   Array arq = arr_alocar(arr_tam_arr(arr));
   arr_atribuir_array(arq, arr, arr_tam_arr(arr));

   Mlp rede = rna_alocar_mlp(arq);
   rna_print(rede);

   rna_mlp_treino(rede, entrada, saida, 0.1, 10*1000);
   testar_dados(rede, entrada, saida);
}

int main(void){
   srand(time(NULL));

   modelo_xor();

   return 0;
}