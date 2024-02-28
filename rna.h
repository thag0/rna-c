#include "mat.h"
#define MAT_IMPL

//Camada densa
typedef struct{
   Mat entrada;
   Mat saida;
   Mat _pesos;
   Mat _bias;
}Densa;

#ifndef RNA_IMPL

Densa rna_densa_alocar(int tamEntrada, int neuronios){
   Densa d;

   d.entrada = mat_alocar(1, tamEntrada);
   d.saida = mat_alocar(1, neuronios);

   d._pesos = mat_alocar(tamEntrada, neuronios);
   d._bias = mat_alocar(1, neuronios);

   mat_randomizar(d._pesos, -1, 1);
   mat_randomizar(d._bias, -1, 1);

   return d;
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

void rna_forward_densa(Densa densa, Mat entrada){
   mat_copiar_linha(densa.entrada, entrada, 0, 0);
   mat_mult(densa.saida, densa.entrada, densa._pesos);
   mat_add(densa.saida, densa.saida, densa._bias);
}

#endif