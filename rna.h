#ifndef RNA_IMPL

#include "ativacoes.h"
#include "mat.h"
#define MAT_IMPL

//Camada densa
typedef struct{
   Mat entrada;
   Mat grad_saida;

   Mat somatorio;
   Mat saida;
   Mat grad_entrada;

   Mat _pesos;
   Mat _grad_pesos;
   Mat _bias;
   Mat _grad_bias;
   Mat _derivada;
}Densa;

/**
 * Aloca uma nova camada densa. Os valores dos padrâmetros treináveis
 * dela serão inicializados aleatoriamente.
 * @param entrada tamanho da entrada da camada.
 * @param neuronios quantidade de neurônios da camada.
 * @return camada alocada.
*/
Densa rna_densa_alocar(int entrada, int neuronios){
   Densa d;

   d.entrada = mat_alocar(1, entrada);
   d.somatorio = mat_alocar(1, neuronios);
   d.saida = mat_alocar(1, neuronios);

   d._pesos = mat_alocar(entrada, neuronios);
   d._bias = mat_alocar(1, neuronios);

   d.grad_entrada = mat_alocar(1, entrada);
   d.grad_saida = mat_alocar(1, neuronios);
   d._grad_pesos = mat_alocar(entrada, neuronios);
   d._grad_bias = mat_alocar(1, neuronios);
   d._derivada = mat_alocar(1, neuronios);

   mat_randomizar(d._pesos, -1, 1);
   mat_randomizar(d._bias, -1, 1);

   return d;
}

void rna_densa_destruir(Densa densa){
   mat_desalocar(densa.entrada);
   mat_desalocar(densa.grad_saida);
   mat_desalocar(densa.somatorio);
   mat_desalocar(densa.saida);
   mat_desalocar(densa.grad_entrada);
   mat_desalocar(densa._pesos);
   mat_desalocar(densa._grad_pesos);
   mat_desalocar(densa._bias);
   mat_desalocar(densa._grad_bias);
   mat_desalocar(densa._derivada);
}

/**
 * Exibe as principais características da camada densa que são
 * seus pesos e bias.
 * @param densa camada densa.
*/
void rna_densa_print(Densa densa){
   char* pad = "   ";
   printf("Densa = [\n");
   _mat_print(densa._pesos, "kernel", pad);
   _mat_print(densa._bias, "bias", pad);
   printf("]\n");
}

/**
 * Propaga os dados de entrada através da camada densa.
 * @param densa camada densa.
 * @param entrada matriz com dados de entrada, será considerada
 * apenas a primeira linha da matriz.
*/
void rna_densa_forward(Densa densa, Mat entrada){
   // mat_copiar_linha(densa.entrada, entrada, 0, 0);
   
   mat_mult(densa.somatorio, densa.entrada, densa._pesos);
   mat_add(densa.somatorio, densa.somatorio, densa._bias);

   act_sigmoid(densa.saida, densa.somatorio);
}

/**
 * Retropropaga o gradiente através da camada densa.
 * @param densa camada densa.
 * @param grad gradientes da camada seguinte.
*/
void rna_densa_backward(Densa densa, Mat grad){
   assert(mat_comparar_colunas(densa.grad_saida, grad) && "Graidente de saida da camada densa e grad possuem colunas diferentes.");
   mat_copiar(densa.grad_saida, grad);

   //TODO implementar uma forma de calcular a derivada
   for(int i = 0; i < densa._derivada.col; i++){
      double g = densa.grad_saida.elementos[i];
      double e = densa.somatorio.elementos[i];
      double dx = _sigmoid_dx(e);
      mat_editar(densa._derivada, 0, i, (double)(dx*g));  
   }

   //calculo dos gradientes da camada
   mat_mult(densa._grad_pesos, mat_transpor(densa.entrada), densa._derivada);
   mat_copiar(densa._grad_bias, densa._derivada);

   //corrigir
   // mat_mult(densa.grad_entrada, densa._derivada, mat_transpor(densa._pesos));
}

#endif