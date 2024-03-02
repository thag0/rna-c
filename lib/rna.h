#ifndef INC_RNA
   #define INC_RNA

   #include "arr.h"
   #include "mat.h"
   #include "ativacoes.h"
   #include "perdas.h"
   #include "densa.h"

   /**
    * Modelo de rede neural Multilayer Perceptron
   */
   typedef struct{
      /**
       * Conjunto de camadas
      */
      Densa* _camadas;

      /**
       * Quantidade de camadas da rede.
      */
      int _tam;

      //temp, talvez util futuramente
      Array arq;

   }Mlp;

   #define mlp_tam_entrada(mlp) mlp._camadas[0].entrada.col
   #define mlp_tam_saida(mlp) mlp._camadas[mlp._tam-1].saida.col

   /**
    * Aloca um modelo Multilayer Perceptron.
    * @param arq arquitetura do modelo, cada elementos da arquitetura deve
    * conter a quantidade de neurônios de cada camada do modelo.
   */
   Mlp rna_alocar_mlp(Array arq){
      if(arq._tam < 2){
         printf("A arquitetura da rede deve conter ao menos dois elementos, recebido: %d", arq._tam);
         assert(0);
      }

      Mlp mlp;
      mlp.arq = arq;
      mlp._tam = arq._tam-1;

      mlp._camadas = (Densa*) malloc(sizeof(Densa) * mlp._tam);
      assert(mlp._camadas != NULL);

      mlp._camadas[0] = densa_alocar(arq.elementos[0], arq.elementos[1]);
      for(int i = 1; i < mlp._tam; i++){
         mlp._camadas[i] = densa_alocar(arq.elementos[i], arq.elementos[i+1]);
      }

      return mlp;
   }

   /**
    * Desaloca os elementos dinâmicos do modelo.
    * @param mlp modelo multilayer perceptron.
   */
   void rna_mlp_destruir(Mlp mlp){
      free(mlp._camadas);
      arr_desalocar(mlp.arq);
   }

   /**
    * Exibe as informações da rede.
    * @param mlp modelo multilayer perceptron.
   */
   void rna_print(Mlp mlp){
      char* pad = "   ";
      int parametros = 0;

      printf("MLP = [\n");
      
         for(int i = 0; i < mlp._tam; i++){
            parametros += densa_num_parametros(mlp._camadas[i]);
            printf("%s%d - entrada (%d) - saida (%d) - Atv: %s\n", pad, i,
               mlp._camadas[i].entrada.col,
               mlp._camadas[i].saida.col,
               mlp._camadas[i].atv.nome
            );
         }
         printf("\n%sParametros treinaveis: %d\n", pad, parametros);

      printf("]\n");
   }

   /**
    * Propaga os dados de entrada através do modelo.
    * @param mlp modelo multilayer perceptron.
    * @param entrada dados de entrada para o modelo.
   */
   void rna_mlp_forward(Mlp mlp, Array entrada){
      if(mlp_tam_entrada(mlp) != entrada._tam){
         printf(
            "Tamanho do array de entrada (%d) diferente da capacidade da camada (%)", 
            entrada._tam, 
            mlp_tam_entrada(mlp)
         );
         assert(0);
      }

      densa_forward(mlp._camadas[0], entrada);
      for(int i = 1; i < mlp._tam; i++){
         densa_forward(
            mlp._camadas[i], 
            mat_linha_para_array(mlp._camadas[i-1].saida, 0)
         );
      }
   }

   /**
    * Retropaga os gradientes através do modelo.
    * @param mlp modelo multilayer perceptron.
    * @param entrada gradientes em relação as saídas do modelo.
   */
   void rna_mlp_backward(Mlp mlp, Array grad){
      if(mlp_tam_saida(mlp) != grad._tam){
         printf(
            "Tamanho do array de gradientes (%d) diferente da capacidade da camada (%)", 
            grad._tam, 
            mlp_tam_saida(mlp)
         );
         assert(0);
      }

      int ultimo_id = mlp._tam-1;
      densa_backward(mlp._camadas[ultimo_id], grad);
      for(int i = ultimo_id-1; i >=0; i--){
         densa_backward(
            mlp._camadas[i], 
            mat_linha_para_array(mlp._camadas[i+1].grad_entrada, 0)
         );
      }
   }

   /**
    * Zera todos os gradientes acumulados do modelo.
    * @param mlp modelo multilayer perceptron.
   */
   void rna_mlp_zerar_gradientes(Mlp mlp){
      for(int i = 0; i < mlp._tam; i++){
         densa_zerar_gradientes(mlp._camadas[i]);
      }
   }

   /**
    * Atualiza os parâmetros do modelo usando a técnica do gradiente descendente.
    * @param kernel kernel de uma camada.
    * @param grad_k gradientes da camada em relação ao kernel.
    * @param bias bias da camada.
    * @param grad_b graidentes da camada em relação ao bias.
    * @param t_a taxa de aprendizado do otimizador.
   */
   void rna_gradient_descent(Mat kernel, Mat grad_k, Mat bias, Mat grad_b, double t_a){
      // var -= grad * t_a
      mat_mult_escalar(grad_k, grad_k, t_a);
      mat_mult_escalar(grad_b, grad_b, t_a);
      mat_sub(kernel, kernel, grad_k);
      mat_sub(bias, bias, grad_b);
   }

   /**
    * Aplica o gradient descente em todo as camadas do modelo.
    * @param mlp modelo multilayer perceptron.
    * @param t_a taxa de aprendizado do otimizador.
   */
   void rna_mlp_otimizar(Mlp mlp, double t_a){
      for(int i = 0; i < mlp._tam; i++){
         rna_gradient_descent(
            mlp._camadas[i]._pesos, 
            mlp._camadas[i]._grad_pesos, 
            mlp._camadas[i]._bias, 
            mlp._camadas[i]._grad_bias, 
            t_a
         );
      }
   }

   /**
    * Retorna a saída do modelo.
    * @param mlp modelo multilayer perceptron.
    * @return saída do modelo.
   */
   Mat rna_mlp_saida(Mlp mlp){
      return mlp._camadas[mlp._tam-1].saida;
   }

   /**
    * Aplica o algoritmo de treino para o modelo.
    * @param mlp modelo multilayer perceptron.
    * @param entrada dados de entrada para o modelo.
    * @param saida dados de saída em relação a entrada.
    * @param t_a taxa de aprendizado para o otimizador.
    * @param epochs épocas de treinamento.
   */
   void rna_mlp_treino(Mlp mlp, Mat entrada, Mat saida, double t_a, int epochs){
      if(mlp_tam_entrada(mlp) != entrada.col){
         printf(
            "A quantiade de amostras de entrada (%d) é diferente da capacidade do modelo (%d).\n", 
            entrada.col, 
            mlp_tam_entrada(mlp)
         );
         assert(0);
      }
      if(mlp_tam_saida(mlp) != saida.col){
         printf(
            "A quantiade de amostras de saida (%d) é diferente da capacidade do modelo (%d).\n", 
            saida.col, 
            mlp_tam_saida(mlp)
         );
         assert(0);
      }
      if(entrada.lin != saida.lin){
         printf("Os dados de entrada e saída devem conter a mesma quantidade de amostras\n.");
         assert(0);
      }

      Array grad = arr_alocar(mlp._camadas[mlp._tam-1].grad_saida.col);
      int amostras = entrada.lin;
      for(int e = 0; e < epochs; e++){
         for(int i = 0; i < amostras; i++){
            rna_mlp_zerar_gradientes(mlp);
            rna_mlp_forward(mlp, mat_linha_para_array(entrada, i));
            
            perda_mse_derivada(
               grad, 
               mat_linha_para_array(rna_mlp_saida(mlp), 0), 
               mat_linha_para_array(saida, i)
            );

            rna_mlp_backward(mlp, grad);
            rna_mlp_otimizar(mlp, t_a);
         }
      }

      arr_desalocar(grad);
   }

#endif