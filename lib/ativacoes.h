#ifndef INC_ATV
   #define INC_ATV

   #include <math.h>
   #include <assert.h>
   #include <string.h>

   #include "mat.h"
   #define MAT_INC

   typedef struct{
      double (*forward)(double);  // Função de ativação
      double (*backward)(double);  // Derivada da função de ativação
      char* nome;
   }Ativacao;

   /**
    * Calcula o valor da função de ativação para cada elementos da matriz.
    * @param dest matriz de destino.
    * @param m matriz de entrada para a função.
    * @param atv função de ativação que será aplicada.
   */
   void act_ativacao(Mat dest, Mat m, Ativacao atv){
      for(int i = 0; i < dest._tam; i++){
         dest.elementos[i] = atv.forward(m.elementos[i]);
      }
   }

   /**
    * Calcula o valor da derivada da função de ativação para cada elementos 
    * da matriz.
    * @param dest matriz de destino.
    * @param m matriz de entrada para a função.
    * @param atv função de ativação que será aplicada.
   */
   void act_derivada(Mat dest, Mat m, Ativacao atv){
      for(int i = 0; i < dest._tam; i++){
         dest.elementos[i] = atv.backward(m.elementos[i]);
      }
   }

   /**
    * Calcula o resultado da ativação relu.
    * @param x valor de entrada para função.
    * @return resultado da ativação.
   */
   double act_relu(double x){
      return x > 0 ? x : 0;
   }

   /**
    * Calcula o resultado da derivada da ativação relu.
    * @param x valor de entrada para função.
    * @return resultado da derivada da função.
   */
   double act_relu_dx(double x){
      return x > 0 ? 1 : 0;
   }

   /**
    * Calcula o resultado da ativação sigmoid.
    * @param x valor de entrada para função.
    * @return resultado da ativação.
   */
   double act_sigmoid(double x){
      return 1.0 / (1.0 + exp(-x));
   }

   /**
    * Calcula o resultado da derivada da ativação sigmoid.
    * @param x valor de entrada para função.
    * @return resultado da derivada da função.
   */
   double act_sigmoid_dx(double x){
      double s = act_sigmoid(x);
      return s * (1 - s);
   }

   /**
    * Calcula o resultado da ativação tanh (tangente hiperbólica).
    * @param x valor de entrada para função.
    * @return resultado da ativação.
   */
   double act_tanh(double x){
      return (2 / (1 + exp(-2*x))) - 1;
   }

   /**
    * Calcula o resultado da derivada da ativação tanh (tangente hiperbólica).
    * @param x valor de entrada para função.
    * @return resultado da derivada da função.
   */
   double act_tanh_dx(double x){
      double t = act_tanh(x);
      return 1 - (t * t);
   }

   /**
    * Aloca uma função de ativação dinamicamente.
    * @return ativação alocada.
   */
   Ativacao act_alocar(){
      Ativacao at;
      int tam_padrao = 30;//só por garantia
      at.nome = (char*) malloc(sizeof(char) * (strlen("a") * tam_padrao));
      
      return at;
   }

   /**
    * Desaloca os dados dinâmicos da ativação.
    * @param atv função de ativação.
   */
   void act_destruir(Ativacao atv){
      free(atv.nome);
   }

   /**
    * Configura as funções para a ativação desejada.
    * @param atv função de ativação.
    * @param nome nome da função desejada.
   */
   void act_config(Ativacao *atv, const char* nome){
      if(strcmp(nome, "relu") == 0){
         atv->forward = act_relu;
         atv->backward = act_relu_dx;
      
      }else if(strcmp(nome, "sigmoid") == 0){
         atv->forward = act_sigmoid;
         atv->backward = act_sigmoid_dx;
      
      }else if(strcmp(nome, "tanh") == 0){
         atv->forward = act_tanh;
         atv->backward = act_tanh_dx;

      }else{
         printf("Atv \"%s\" invalida.\n", nome);
         assert(0);
      }
      
      strcpy(atv->nome, nome);
   }

#endif