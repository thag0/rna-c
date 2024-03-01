#ifndef RNA_INC
   #define RNA_INC

   #include "arr.h"
   #include "mat.h"
   #include "ativacoes.h"
   #include "perdas.h"

   //Camada densa
   typedef struct{
      Mat entrada;
      Mat grad_saida;

      Mat somatorio;
      Mat saida;
      Mat grad_entrada;

      Mat _pesos;
      Mat _grad_pesos;
      Mat _bias;
      Mat _grad_bias;
      Mat _derivada;
   }Densa;

   //implementações

   /**
    * Aloca uma nova camada densa. Os valores dos padrâmetros treináveis
    * dela serão inicializados aleatoriamente.
    * @param entrada tamanho da entrada da camada.
    * @param neuronios quantidade de neurônios da camada.
    * @return camada alocada.
   */
   Densa rna_densa_alocar(int entrada, int neuronios){
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

      return d;
   }

   /**
    * Desaloca os dados dinâmicos da camada densa.
    * @param densa camda alocada.
   */
   void rna_densa_destruir(Densa densa){
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

   /**
    * Propaga os dados de entrada através da camada densa.
    * @param densa camada densa.
    * @param entrada matriz com dados de entrada, será considerada
    * apenas a primeira linha da matriz.
   */
   void rna_densa_forward(Densa densa, Array entrada){
      mat_copiar_array(densa.entrada, entrada, 0);
      
      mat_mult(densa.somatorio, densa.entrada, densa._pesos);
      mat_add(densa.somatorio, densa.somatorio, densa._bias);

      act_sigmoid(densa.saida, densa.somatorio);
   }

   /**
    * Retropropaga o gradiente através da camada densa.
    * @param densa camada densa.
    * @param grad gradientes da camada seguinte.
   */
   void rna_densa_backward(Densa densa, Array grad){
      assert(densa.grad_saida.col == grad._tam && "Graidente de saida da camada densa e grad possuem tamanhos diferentes.");
      mat_copiar_array(densa.grad_saida, grad, 0);

      act_sigmoid_derivada(densa._derivada, densa.somatorio);
      mat_had(densa._derivada, densa.grad_saida, densa._derivada);

      //calculo dos gradientes da camada
      mat_mult(densa._grad_pesos, mat_transpor(densa.entrada), densa._derivada);
      mat_copiar(densa._grad_bias, densa._derivada);

      //corrigir
      // mat_mult(densa.grad_entrada, densa._derivada, mat_transpor(densa._pesos));
   }

   /**
    * Atualiza os parâmetros do modelo usando a técnica do gradiente descendente.
    * @param kernel kernel de uma camada.
    * @param grad_k gradientes da camada em relação ao kernel.
    * @param bias bias da camada.
    * @param grad_b graidentes da camada em relação ao bias.
    * @param ta taxa de aprendizado.
   */
   void rna_gradient_descent(Mat kernel, Mat grad_k, Mat bias, Mat grad_b, double ta){
      mat_mult_escalar(grad_k, grad_k, ta);
      mat_mult_escalar(grad_b, grad_b, ta);
      mat_sub(kernel, kernel, grad_k);
      mat_sub(bias, bias, grad_b);
   }

   /**
    * Aplica os passos de treino na camada densa.
    * @param camada camada densa.
    * @param entrada dados de entrada para a camada.
    * @param saida dados de saída correspondentes à entrada.
    * @param ta taxa de aprendizagem.
    * @param epochs quantidade de épocas de treino.
   */
   void rna_treinar_densa(Densa camada, Mat entrada, Mat saida, double ta, int epochs){
      assert(camada.saida.col == saida.col && "Saida da camada e matriz de saida possuem colunas diferentes.");

      int amostras = entrada.lin;
      Array grad = arr_alocar(camada.saida.col);

      for(int e = 0; e < epochs; e++){
         for(int i = 0; i < amostras; i++){
            rna_densa_forward(camada, mat_linha_para_array(entrada, i));
            perda_mse_derivada(
               grad, 
               mat_linha_para_array(camada.saida, 0), 
               mat_linha_para_array(saida, i)
            );

            rna_densa_backward(camada, grad);
            rna_gradient_descent(camada._pesos, camada._grad_pesos, camada._bias, camada._grad_bias, ta);
         }
      }

      arr_desalocar(grad);
   }

#endif