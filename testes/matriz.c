#include <stdio.h>
#include "mat.h"

int main(void){
   Mat* a = mat_alocar(2, 2);
   Mat* b = mat_alocar(2, 2);
   Mat* c = mat_alocar(2, 2);

   mat_preencher_contador(a, true);
   mat_identidade(b);
   mat_mult(c, a, b);
   mat_print(c);

   return 0;
}