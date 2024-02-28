#include <stdio.h>

#include "rna.h"
#define RNA_IMPL

int main(void){
   Densa densa = rna_densa_alocar(2, 2);

   Mat entrada = mat_alocar(1, 2);
   mat_editar(entrada, 0, 0, 1);
   mat_editar(entrada, 0, 1, 2);

   rna_forward_densa(densa, entrada);

   rna_densa_print(densa);
   mat_print(densa.saida);

   return 0;
}