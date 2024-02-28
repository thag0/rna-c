#include <stdio.h>

#include "rna.h"
#define RNA_IMPL

double amostras[] = {
   0, 0, 0,
   0, 1, 1,
   1, 0, 1,
   1, 1, 0,
};

int main(void){
   Densa densa = rna_densa_alocar(2, 3);
   rna_densa_print(densa);

   Mat entrada = mat_alocar(1, 2);
   mat_editar(entrada, 0, 0, 1);
   mat_editar(entrada, 0, 1, 2);
   rna_densa_forward(densa, entrada);

   Mat g = mat_alocar(1, 3);
   mat_editar(g, 0, 0, 0.2);
   mat_editar(g, 0, 1, 0.4);
   mat_editar(g, 0, 2, 0.6);
   rna_densa_backward(densa, g);
   mat_print(densa._grad_pesos);
   mat_print(densa._grad_bias);

   rna_densa_destruir(densa);

   return 0;
}