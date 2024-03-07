#ifndef INC_OTM
   #define INC_OTM

   #include <string.h>
   #include "../mat.h"

   typedef struct Otimizador Otimizador;
   struct Otimizador{
      void (*calcular)(Otimizador* otm, Mat* kernel, Mat* grad_k, Mat* bias, Mat* grad_b, double t_a);
   };

   /**
    * Atualiza os parâmetros do modelo usando a técnica do gradiente descendente.
    * @param kernel kernel de uma camada.
    * @param grad_k gradientes da camada em relação ao kernel.
    * @param bias bias da camada.
    * @param grad_b graidentes da camada em relação ao bias.
    * @param t_a taxa de aprendizado do otimizador.
   */
   void _gradient_descent(Otimizador* otm, Mat* kernel, Mat* grad_k, Mat* bias, Mat* grad_b, double t_a){
      // var -= grad * t_a
      mat_mult_escalar(grad_k, grad_k, t_a);
      mat_mult_escalar(grad_b, grad_b, t_a);
      mat_sub(kernel, kernel, grad_k);
      mat_sub(bias, bias, grad_b);
   }

   /**
    * Aloca um novo otimizador
    * @param nome nome do otimizador desejado.
    * @return otimizador alocado.
   */
   Otimizador* otm_alocar(const char* nome){
      Otimizador* otm = (Otimizador*) malloc(sizeof(Otimizador));

      if(strcmp(nome, "gd") == 0){
         otm->calcular = _gradient_descent;
      
      }else{
         printf("Otimizador \"%s\" invalida.\n", nome);
         assert(0);
      }
   }

   /**
    * Desaloca o otimizador e seus dados.
    * @param otm otimizador alocado.
   */
   void otm_desalocar(Otimizador* otm){
      free(otm);
   }

#endif