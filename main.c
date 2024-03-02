#include <stdio.h>
#include "rna.h"

/**
 * Testa todo o conjunto de dados no modelo.
 * @param mlp modelo multilayer perceptron.
 * @param entrada matriz com os dados de entrada.
 * @param saida matriz com os dados de saída.
*/
void testar_dados(Mlp mlp, Mat entrada, Mat saida){
   int amostras = entrada.lin;
   char* pad = "    ";

   printf("Resultados = [\n");
      for(int i = 0; i < amostras; i++){
         rna_forward(mlp, mat_linha_para_array(entrada, i));
         double prev = mat_elemento(rna_saida(mlp), 0, 0);
         double real = mat_elemento(saida, i, 0);

         printf("%s%.1f  %.1f -> %.1f, prev: %.8f\n",
            pad, 
            mat_elemento(entrada, i, 0), 
            mat_elemento(entrada, i, 1), 
            real,
            prev 
         );

      }
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

   double arr[] = {2, 4, 1};
   Array arq = arr_alocar(arr_tam_arr(arr));
   arr_atribuir_array(arq, arr, arr_tam_arr(arr));

   Mlp rede = rna_alocar(arq);
   rna_print(rede);

   Perda perda = perda_alocar("mse");
   rna_treinar(rede, entrada, saida, perda, 0.1, 10*1000);
   testar_dados(rede, entrada, saida);
   printf("Perda: %.9f", rna_avaliar(rede, entrada, saida, perda));
}

int main(void){
   srand(time(NULL));

   modelo_xor();

   return 0;
}