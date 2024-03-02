#ifndef INC_DENSA
   #define INC_DENSA

   #include "mat.h"
   #include "ativacoes.h"

   /**
    * Camada densa de neurônios.
   */
   typedef struct{
      /**
       * Entrada da camada, com formato (1, tam_entrada)
      */
      Mat entrada;

      /**
       * Resultado parcial do processamento da camada.
      */
      Mat somatorio;

      /**
       * Resultado da ativação da camada.
      */
      Mat saida;

      /**
       * Gradiente da saída em relação a entrada (pra retropropagar).
      */
      Mat grad_entrada;

      /**
       * Gradiente em relação a saída da camada.
      */
      Mat grad_saida;

      /**
       * Kernel da camada.
      */
      Mat _pesos;

      /**
       * Gradiente para os kernels da camada.
      */
      Mat _grad_pesos;

      /**
       * Bias da camada.
      */
      Mat _bias;

      /**
       * Gradiente para os bias da camada.
      */
      Mat _grad_bias;

      /**
       * Resultado da derivada da função de ativação.
      */
      Mat _derivada;

      /**
       * Função de ativação da camada.
      */
      Ativacao atv;
   }Densa;

   /**
    * Configura a função de ativação que será usada pela camada densa.
    * @param densa camada densa.
    * @param nome nove da função de ativação desejada.
   */
   void densa_config_ativacao(Densa densa, char* nome){
      atv_config(&densa.atv, nome);
   }

   /**
    * Aloca uma nova camada densa. Os valores dos padrâmetros treináveis
    * dela serão inicializados aleatoriamente.
    * @param entrada tamanho da entrada da camada.
    * @param neuronios quantidade de neurônios da camada.
    * @return camada alocada.
   */
   Densa densa_alocar(int entrada, int neuronios){
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

      d.atv = atv_alocar();
      atv_config(&d.atv, "sigmoid");//ativação padrão

      return d;
   }

   /**
    * Desaloca os dados dinâmicos da camada densa.
    * @param densa camda alocada.
   */
   void densa_destruir(Densa densa){
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

      atv_destruir(densa.atv);
   }

   /**
    * Retorna a quantidade de parâmetros treináveis da camada.
    * @param densa camada densa.
    * @return parâmetros treináveis da camada.
   */
   int densa_num_parametros(Densa densa){
      return densa._pesos._tam + densa._bias._tam;
   }

   /**
    * Exibe as principais caratverísticas da camada densa.
    * @param densa camada densa.
   */
   void densa_print(Densa densa){
      char* pad = "   ";

      printf("Densa = [\n");
         _mat_print(densa._pesos, "kernel", pad);
         _mat_print(densa._bias, "bias", pad);
         printf("%sAtv: %s\n", pad, densa.atv.nome);
      printf("]\n");
   }

   /**
    * Zera os valores de gradientes da camada.
    * @param densa camada densa.
   */
   void densa_zerar_gradientes(Densa densa){
      mat_preencher(densa._grad_pesos, 0);
      mat_preencher(densa._grad_bias, 0);
   }

   /**
    * Propaga os dados de entrada através da camada densa.
    * @param densa camada densa.
    * @param entrada matriz com dados de entrada, será considerada
    * apenas a primeira linha da matriz.
   */
   void densa_forward(Densa densa, Array entrada){
      mat_copiar_array(densa.entrada, entrada, 0);

      mat_mult(densa.somatorio, densa.entrada, densa._pesos);
      mat_add(densa.somatorio, densa.somatorio, densa._bias);

      atv_ativacao(densa.saida, densa.somatorio, densa.atv);
   }

   /**
    * Retropropaga o gradiente através da camada densa.
    * @param densa camada densa.
    * @param grad gradientes da camada seguinte.
   */
   void densa_backward(Densa densa, Array grad){
      if(densa.grad_saida.col != grad._tam){
         printf(
            "Tamanho do gradiente de saida da camada densa (%d) e gradiente recebido (%d) possuem tamanhos diferentes.",
            densa.grad_saida.col,
            grad._tam
         );
         assert(0);
      }

      mat_copiar_array(densa.grad_saida, grad, 0);
      atv_derivada(densa._derivada, densa.somatorio, densa.atv);
      mat_had(densa._derivada, densa.grad_saida, densa._derivada);

      //usar de acumulador
      Mat temp_grad = mat_alocar(densa._pesos.lin, densa._pesos.col);

      //grad pesos
      mat_mult(temp_grad, mat_transpor(densa.entrada), densa._derivada);
      mat_add(densa._grad_pesos, densa._grad_pesos, temp_grad);

      mat_add(densa._grad_bias, densa._grad_bias, densa._derivada);//grad bias
      mat_mult(densa.grad_entrada, densa._derivada, mat_transpor(densa._pesos));//grad entrada
   
      mat_desalocar(temp_grad);
   }

#endif