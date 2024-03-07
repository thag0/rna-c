#ifndef INC_RNA
   #define INC_RNA

   #include "arr.h"
   #include "mat.h"
   #include "ativacoes.h"
   #include "perdas.h"
   #include "otm/otimizador.h"
   #include "densa.h"

   /**
    * Modelo de rede neural Multilayer Perceptron
   */
   typedef struct{
      /**
       * Conjunto de camadas
      */
      Densa** _camadas;

      /**
       * Quantidade de camadas da rede.
      */
      int _tam;

      Otimizador* otm;

      //temp, talvez util futuramente
      Array* arq;

   }Mlp;

   /**
    * Aloca um modelo Multilayer Perceptron.
    * @param arq arquitetura do modelo, cada elementos da arquitetura deve
    * conter a quantidade de neurônios de cada camada do modelo.
   */
   Mlp* rna_alocar(Array* arq){
      if(arq->_tam < 2){
         printf("A arquitetura da rede deve conter ao menos dois elementos, recebido: %d", arq->_tam);
         assert(0);
      }

      Mlp* mlp = (Mlp*) malloc(sizeof(Mlp));
      assert(mlp != NULL && "rna_alocar, mlp");

      mlp->arq = arr_clonar(arq);
      mlp->_tam = arq->_tam-1;//remover "camada" de entrada
      mlp->_camadas = (Densa**) malloc(sizeof(Densa*) * mlp->_tam);
      assert(mlp->_camadas != NULL && "rna_alocar, densa->camadas");

      mlp->_camadas[0] = densa_alocar(arq->elementos[0], arq->elementos[1]);
      for(int i = 1; i < mlp->_tam; i++){
         mlp->_camadas[i] = densa_alocar(arq->elementos[i], arq->elementos[i+1]);
      }

      mlp->otm = otm_alocar("gd");// otimizador padrão

      return mlp;
   }

   /**
    * Desaloca os elementos dinâmicos do modelo.
    * @param mlp modelo multilayer perceptron.
   */
   void rna_desalocar(Mlp* mlp){
      for(int i = 0; i < mlp->_tam; i++){
         free(mlp->_camadas[i]);
      }
      free(mlp->_camadas);

      arr_desalocar(mlp->arq);
      otm_desalocar(mlp->otm);
      free(mlp);
   }

   /**
    * Exibe as informações da rede.
    * @param mlp modelo multilayer perceptron.
   */
   void rna_print(Mlp* mlp){
      char* pad = "   ";
      int parametros = 0;

      printf("MLP = [\n");
      
         for(int i = 0; i < mlp->_tam; i++){
            parametros += densa_num_parametros(mlp->_camadas[i]);
            printf("%s%d - entrada (%d) - saida (%d) - Atv: %s\n", pad, i,
               mlp->_camadas[i]->entrada->col,
               mlp->_camadas[i]->saida->col,
               mlp->_camadas[i]->atv->nome
            );
         }
         printf("\n%sParametros treinaveis: %d\n", pad, parametros);

      printf("]\n");
   }

   /**
    * Retorna a entrada do modelo.
    * @param mlp modelo multilayer perceptron.
    * @return entrada do modelo.
   */
   Mat* rna_entrada(Mlp* mlp){
      return densa_entrada(mlp->_camadas[0]);
   }

   /**
    * Retorna a saída do modelo.
    * @param mlp modelo multilayer perceptron.
    * @return saída do modelo.
   */
   Mat* rna_saida(Mlp* mlp){
      return densa_saida(mlp->_camadas[mlp->_tam-1]);
   }

   /**
    * Retorna o tamanho de entrada do modelo.
    * @param mlp modelo multilayer perceptron.
    * @return quantidade de elementos de entrada.
   */
   int mlp_tam_entrada(Mlp* mlp){
      return rna_entrada(mlp)->_tam;
   }

   /**
    * Retorna o tamanho de saída do modelo.
    * @param mlp modelo multilayer perceptron.
    * @return quantidade de elementos de saída.
   */
   int mlp_tam_saida(Mlp* mlp){
      return rna_saida(mlp)->_tam;
   }

   /**
    * Propaga os dados de entrada através do modelo.
    * @param mlp modelo multilayer perceptron.
    * @param entrada dados de entrada para o modelo.
   */
   void rna_forward(Mlp* mlp, Array* entrada){
      if(mlp_tam_entrada(mlp) != entrada->_tam){
         printf(
            "Tamanho do array de entrada (%d) diferente da capacidade da camada (%d)", 
            entrada->_tam, 
            mlp_tam_entrada(mlp)
         );
         assert(0);
      }

      densa_forward(mlp->_camadas[0], entrada);
      for(int i = 1; i < mlp->_tam; i++){
         densa_forward(
            mlp->_camadas[i], 
            mat_linha_para_array(mlp->_camadas[i-1]->saida, 0)
         );
      }
   }

   /**
    * Retropaga os gradientes através do modelo.
    * @param mlp modelo multilayer perceptron.
    * @param entrada gradientes em relação as saídas do modelo.
   */
   void rna_backward(Mlp* mlp, Array* grad){
      if(mlp_tam_saida(mlp) != grad->_tam){
         printf(
            "Tamanho do array de gradientes (%d) diferente da capacidade da camada (%d)", 
            grad->_tam, 
            mlp_tam_saida(mlp)
         );
         assert(0);
      }

      int ultimo_id = mlp->_tam-1;
      densa_backward(mlp->_camadas[ultimo_id], grad);
      for(int i = ultimo_id-1; i >= 0; i--){
         densa_backward(
            mlp->_camadas[i], 
            mat_linha_para_array(mlp->_camadas[i+1]->grad_entrada, 0)
         );
      }
   }

   /**
    * Zera todos os gradientes acumulados do modelo.
    * @param mlp modelo multilayer perceptron.
   */
   void rna_zerar_gradientes(Mlp* mlp){
      for(int i = 0; i < mlp->_tam; i++){
         densa_zerar_gradientes(mlp->_camadas[i]);
      }
   }

   /**
    * Aplica o gradient descente em todo as camadas do modelo.
    * @param mlp modelo multilayer perceptron.
    * @param t_a taxa de aprendizado do otimizador.
   */
   void rna_otimizar(Mlp* mlp, double t_a){
      for(int i = 0; i < mlp->_tam; i++){
         mlp->otm->calcular(
            mlp->otm,
            mlp->_camadas[i]->_pesos, mlp->_camadas[i]->_grad_pesos, 
            mlp->_camadas[i]->_bias, mlp->_camadas[i]->_grad_bias, 
            t_a
         );
      }
   }

   /**
    * Calcula o valor de perda do modelo.
    * @param mlp modelo multilayer perceptron.
    * @param entrada dados de entrada para o modelo.
    * @param saida dados de saída em relação a entrada.
    * @param perda função de perda que calculará o erro do modelo.
   */
   double rna_avaliar(Mlp* mlp, Mat* entrada, Mat* saida, Perda* perda){
      //informar erros personalizados.
      if(mlp_tam_entrada(mlp) != entrada->col){
         assert(0);
      }
      if(mlp_tam_saida(mlp) != saida->col){
         assert(0);
      }
      if(entrada->lin != saida->lin){
         assert(0);
      }

      int amostras = entrada->lin;
      double p = 0;
      for(int i = 0; i < amostras; i++){
         rna_forward(mlp, mat_linha_para_array(entrada, i));
         p += perda->calcular(
            mat_linha_para_array(rna_saida(mlp), 0), 
            mat_linha_para_array(saida, i)
         );
      }
      
      return p / amostras;
   }

   /**
    * Aplica o algoritmo de treino para o modelo.
    * @param mlp modelo multilayer perceptron.
    * @param entrada dados de entrada para o modelo.
    * @param saida dados de saída em relação a entrada.
    * @param perda função de perda que será usada para calcular os gradientes do modelo.
    * @param t_a taxa de aprendizado para o otimizador.
    * @param epochs épocas de treinamento.
   */
   void rna_treinar(Mlp* mlp, Mat* entrada, Mat* saida, Perda* perda, double t_a, int epochs){
      if(mlp_tam_entrada(mlp) != entrada->col){
         printf(
            "A quantiade de amostras de entrada (%d) é diferente da capacidade do modelo (%d).\n", 
            entrada->col, 
            mlp_tam_entrada(mlp)
         );
         assert(0);
      }
      if(mlp_tam_saida(mlp) != saida->col){
         printf(
            "A quantiade de amostras de saida (%d) é diferente da capacidade do modelo (%d).\n", 
            saida->col, 
            mlp_tam_saida(mlp)
         );
         assert(0);
      }
      if(entrada->lin != saida->lin){
         printf("Os dados de entrada e saída devem conter a mesma quantidade de amostras\n.");
         assert(0);
      }

      Array* grad = arr_alocar(mlp->_camadas[mlp->_tam-1]->grad_saida->col);
      int amostras = entrada->lin;
      for(int e = 0; e < epochs; e++){
         for(int i = 0; i < amostras; i++){
            rna_zerar_gradientes(mlp);
            rna_forward(mlp, mat_linha_para_array(entrada, i));
            
            perda->derivada(
               grad, 
               mat_linha_para_array(rna_saida(mlp), 0), 
               mat_linha_para_array(saida, i)
            );

            rna_backward(mlp, grad);
            rna_otimizar(mlp, t_a);
         }
      }

      arr_desalocar(grad);
   }

#endif