#include <stdio.h>
#include "mat.h"
#include "arr.h"

double cont_a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
double cont_b[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void){
   Mat a = mat_alocar(3, 3);
   Mat b = mat_alocar(3, 3);
   Mat r = mat_alocar(3, 3);

   mat_atribuir_array(a, cont_a, arr_tam_arr(cont_a));
   mat_atribuir_array(b, cont_b, arr_tam_arr(cont_b));

   mat_mult(r, a, b);
   mat_print(r);

   return 0;
}