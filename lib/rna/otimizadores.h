#ifndef INC_OTM
   #define INC_OTM

   #include "mat.h"

   /**
    * Atualiza os parâmetros do modelo usando a técnica do gradiente descendente.
    * @param kernel kernel de uma camada.
    * @param grad_k gradientes da camada em relação ao kernel.
    * @param bias bias da camada.
    * @param grad_b graidentes da camada em relação ao bias.
    * @param t_a taxa de aprendizado do otimizador.
   */
   void otm_gradient_descent(Mat* kernel, Mat* grad_k, Mat* bias, Mat* grad_b, double t_a){
      // var -= grad * t_a
      mat_mult_escalar(grad_k, grad_k, t_a);
      mat_mult_escalar(grad_b, grad_b, t_a);
      mat_sub(kernel, kernel, grad_k);
      mat_sub(bias, bias, grad_b);
   }

#endif