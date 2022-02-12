#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 50

typedef struct{
  char id[50];
  int x, y;
} Bot;

typedef struct{
  int x, y, quantidade, valor;
} Peixe;

typedef struct{
  int x, y;
} Porto;

typedef struct{
  int x, y;
  char acao;
}Alvo;

Bot   *bots;
Peixe *peixes;
Porto *portos;
int   qtdBots = 0, qtdPeixes = 0, qtdPortos = 0, peso = 0, altura, largura;

/* ADAPTAR EM FUNÇÃO DE COMO OS DADOS SERÃO ARMAZENADOS NO SEU BOT */
void readData(int h, int w){
  char id[MAX_STR];
  int v, n, x, y;
  //v: Valor da casa
  //n: Quantidade de bots
  //x: Localização linha
  //y: Localização coluna

  int contPeixe = 0;
  int contPorto = 0;

  // lê os dados da área de pesca
  int f;
  for (f = 0; f < h; f++){
    int f2;
    for (f2 = 0; f2 < w; f2++){
      scanf("%i", &v);
      
      if (v == 1){                                                                                                          //Adiciona um porto aos dados
          portos = realloc(portos, sizeof(Porto) * (contPorto + 1));
          portos[contPorto].x = f;
          portos[contPorto].y = f2;
          contPorto += 1;
      }else if(v > 9 && v < 40){                                                                                            //Adiciona peixe aos dados
        int tipo = (int)(v/10);
        int qtdPeixes = v - tipo*10;

        if(qtdPeixes > 0){                                                                                                  //Só adiciona o ponto de pesca se tiver ao menos um peixe
          peixes = realloc(peixes, sizeof(Peixe) * (contPeixe + 1));                                                        //Realoca a memória para adicionar espaço para mais um peixe
          
          int valorPeixe = 0;
          valorPeixe = (tipo == 1)? 100: valorPeixe; //Peixe do tipo Tainhas
          valorPeixe = (tipo == 2)? 150: valorPeixe; //Peixe do tipo Cioba
          valorPeixe = (tipo == 3)? 200: valorPeixe; //Peixe do tipo Robalo
          
          peixes[contPeixe].x = f;
          peixes[contPeixe].y = f2;
          peixes[contPeixe].quantidade = qtdPeixes;
          peixes[contPeixe].valor = valorPeixe;

          contPeixe += 1;
        }
      }
    }
  }

  qtdPeixes = contPeixe;
  qtdPortos = contPorto;

  
  scanf(" BOTS %i", &n);                                                                                                    //Lê os dados dos bots. O " " antes de BOTS é necessário para ler o '\n' da linha anterior
  qtdBots = n;
  
  bots = malloc(sizeof(Bot) * n);
  for(f = 0; f < n; f++){
    scanf("%s %i %i", bots[f].id, &bots[f].x, &bots[f].y);                                                                  //Adiciona os bots aos dados
  }
}

/*
char conferirConflito(int x, int y, char movA[10], char movB[10], int primeiro){

    int f;
    char resultado[10];
    if(strcmp(movA, "UP")){
      for(f = 0; f < qtdBots; f++){
        if(y-1 == bots[f].y && x == bots[f].x){//Há um barco acima
          if(primeiro == 1){//Conferir a segunda possibilidade
            if(strcmp(movB, "LEFT")){
              strcpy(resultado, conferirConflito(x, y, "LEFT", NULL, 0));
            }else if(strcmp(movB, "RIGHT")){
              strcpy(resultado, conferirConflito(x, y, "RIGHT", NULL, 0));
            }

          }else{//Essa é a segunda possibilidade
            strcpy(resultado, "");
          }
          if(strcmp(resultado, NULL) == 0){
            return movB;
          }
        }
      }
      if(strcmp(resultado, NULL) == 0){
        return movA;
      }
    }else if(strcmp(movA, "LEFT")){
      for(f = 0; f < qtdBots; f++){
        if(x-1 == bots[f].x && y == bots[f].y){//Há um barco a esquerda
          if(primeiro == 1){//Conferir a segunda possibilidade
            if(strcmp(movB, "UP")){
              strcpy(resultado, conferirConflito(x, y, "UP", NULL, 0));
            }else if(strcmp(movB, "DOWN")){
              strcpy(resultado, conferirConflito(x, y, "DOWN", NULL, 0));
            }
            
            if(strcmp(resultado, NULL) == 0){
              strcpy(resultado, conferirConflito(x, y, "RIGHT", 0));
            }
          }else{//Segunda possibilidade já conferida
            strcpy(resultado, "");
          }
        }
      }
    }else if(strcmp(movA, "DOWN")){
      for(f = 0; f < qtdBots; f++){
        if(y+1 == bots[f].y && x == bots[f].x){//Há um barco abaixo
          if(primeiro == 1){
            strcpy(resultado, conferirConflito(x, y, 0, 0));
          }else{
            strcpy(resultado, "");
          }
        }
      }
    }else if(strcmp(movA, "RIGHT")){
      for(f = 0; f < qtdBots; f++){
        if(x+1 == bots[f].x && y == bots[f].y){//Há um barco a direita
          if(primeiro == 1){
            strcpy(resultado, conferirConflito(x, y, 1, 0));
          }else{
            strcpy(resultado, "");
          }
        }
      }
    }

    return resultado;
    
  }
//*/

int definirAcao(int *x, int *y, int alvo[3]){                                                                               fprintf(stderr, "definirAcao(%i, %i, alvo)\n", *x, *y);
  /*
    x: Linha atual,  localização vertical
    y: Coluna atual, localização horizontal
    alvo[2]: {
      x,
      y,
      tipo: 1 = Tainha
            2 = Cioba
            3 = Robalo
            4 = porto
    } 
  */

  char movHor[10], movVer[10];
  int f, f2, distancia = 50;


  if(alvo[2] == 4){                                                                                                         fprintf(stderr, "PROCURANDO UM PORTO\n");//Ir para um porto
    for(f = 0; f < qtdPortos; f++){                                                                                         //Conferir qual o porto mais próximo

      int distanciaX = *x-portos[f].x;
      distanciaX = (distanciaX < 0)? distanciaX*-1:distanciaX;

      int distanciaY = *y-portos[f].y;
      distanciaY = (distanciaY < 0)? distanciaY*-1:distanciaY;

      if(distanciaX+distanciaY < distancia){                                                                                fprintf(stderr, "L189 %i+%i < %i\n", distanciaX, distanciaY, distancia);
        distancia = distanciaX+distanciaY;

        alvo[0] = portos[f].x;
        alvo[1] = portos[f].y;

        if(*x < alvo[0]){
          strcpy(movVer, "DOWN");
        }else if(*x > alvo[0]){
          strcpy(movVer, "UP");
        }else{
          strcpy(movVer, "");
        }

        if(*y < alvo[1]){
          strcpy(movHor, "RIGHT");
        }else if(*y > alvo[1]){
          strcpy(movHor, "LEFT");
        }else{
          strcpy(movHor, "");
        }
      }

      /*
      if(*x > portos[f].x && *x-portos[f].x < distanciaX){                                                                  //Mover para esquerda
        strcpy(movHor, "LEFT");
        distanciaX = *x-portos[f].x;
        alvo[0] = portos[f].x;
      }else if(portos[f].x-*x < distanciaX){                                                                                //Mover para direita
        strcpy(movHor, "RIGHT");
        distanciaX = portos[f].x-*x;
        alvo[0] = portos[f].x;
      }

      if(*y > portos[f].y && *y-portos[f].y < distanciaY){                                                                  //Mover para cima
        strcpy(movVer, "UP");
        distanciaY = *y-portos[f].y;
        alvo[1] = portos[f].y;
      }else if(portos[f].y-*y < distanciaY){                                                                                //Mover para baixo
        strcpy(movVer, "DOWN");
        distanciaY = portos[f].y-*y;
        alvo[1] = portos[f].y;
      }
      */
      
    }
                                                                                                                            fprintf(stderr, "L236 distancia = %i\n", distancia);
    if(distancia == 0){
      return 5;
    }else{
      int distanciaX = *x-alvo[0];
      distanciaX = (distanciaX<0)? distanciaX*-1:distanciaX;
      int distanciaY = *y-alvo[1];
      distanciaY = (distanciaY<0)? distanciaY*-1:distanciaY;
                                                                                                                          fprintf(stderr, "L243 %i < %i\n", distanciaX, distanciaY);
      if(distanciaX < distanciaY){                                                                                        fprintf(stderr, "movHor = %s\n", movHor);
        if(strcmp(movHor, "LEFT") == 0){
          return 1;
        }else if(strcmp(movHor, "RIGHT") == 0){
          return 3;
        }
      }else{                                                                                                              fprintf(stderr, "movVer = %s\n", movVer);
        if(strcmp(movVer, "UP") == 0){
          return 0;
        }else if(strcmp(movVer, "DOWN") == 0){
          return 3;
        }
      }
    }
  }else{                                                                                                                    //Procurar por peixes
    switch(alvo[2]){
      case 1: alvo[2] = 100; break;
      case 2: alvo[2] = 150; break;
      case 3: alvo[2] = 200; break;
    }

    for(f = 0; f < qtdPeixes; f++){
      if(peixes[f].valor == alvo[2] && peixes[f].quantidade > 1){                                                           //Esse local tem o peixe procurado
        //Conferir qual local do peixe alvo mais próximo

        int distanciaX = *x-peixes[f].x;                                                                                    
        distanciaX = (distanciaX < 0)? distanciaX*-1: distanciaX;

        int distanciaY = *y-peixes[f].y;                                                                                    
        distanciaY = (distanciaY < 0)? distanciaY*-1: distanciaY;

        
        if((distanciaX+distanciaY) < distancia){
          distancia = distanciaX+distanciaY;                                                                                

          alvo[0] = peixes[f].x;
          alvo[1] = peixes[f].y;

          if(*x < alvo[0]){
            strcpy(movVer, "DOWN");
          }else if(*x > alvo[0]){
            strcpy(movVer, "UP");
          }else{                                                                                                            fprintf(stderr, "L287 AAAAAAAAAAAAAAAAAAAAAAA %i == %i\n", *x, peixes[f].x);
            strcpy(movVer, "");
          }

          if(*y < alvo[1]){
            strcpy(movHor, "RIGHT");
          }else if(*y > alvo[1]){
            strcpy(movHor, "LEFT");
          }else{                                                                                                            fprintf(stderr, "L290 AAAAAAAAAAAAAAAAAAAAAAA %i == %i\n", *y, peixes[f].y);
            strcpy(movHor, "");
          }
        }

        
        /*if(*x > peixes[f].x && *x-peixes[f].x < distanciaX){                                                                  //Mover para esquerda
            if(*y > peixes[f].y && *y-peixes[f].y < distanciaY){                                                                  //Mover para cima
              fprintf(stderr, "L220 || ");
              
              strcpy(movHor, "RIGHT");
              distanciaX = peixes[f].x-*x;
              alvo[0] = peixes[f].x;

              strcpy(movVer, "UP");
              distanciaY = *y-peixes[f].y;
              alvo[1] = peixes[f].y;
            }else if(*y <= peixes[f].y && peixes[f].y-*y < distanciaY){                                                           //Mover para baixo
              fprintf(stderr, "L231 || ");

              strcpy(movHor, "RIGHT");
              distanciaX = peixes[f].x-*x;
              alvo[0] = peixes[f].x;

              strcpy(movVer, "DOWN");
              distanciaY = peixes[f].y-*y;
              alvo[1] = peixes[f].y;
            }
            
          }else if(*x <= peixes[f].x && peixes[f].x-*x < distanciaX){                                                           //Mover para direita
            if(*y > peixes[f].y && *y-peixes[f].y < distanciaY){                                                                  //Mover para cima
              fprintf(stderr, "L245 || ");
              
              strcpy(movHor, "RIGHT");
              distanciaX = peixes[f].x-*x;
              alvo[0] = peixes[f].x;

              strcpy(movVer, "UP");
              distanciaY = *y-peixes[f].y;
              alvo[1] = peixes[f].y;
            }else if(*y <= peixes[f].y && peixes[f].y-*y < distanciaY){                                                           //Mover para baixo
              fprintf(stderr, "L256 || ");

              strcpy(movHor, "RIGHT");
              distanciaX = peixes[f].x-*x;
              alvo[0] = peixes[f].x;

              strcpy(movVer, "DOWN");
              distanciaY = peixes[f].y-*y;
              alvo[1] = peixes[f].y;
            }
          }
        */
      
      }
    }
                                                                                                                            fprintf(stderr, "L351 distancia = %i || alvo = {%i, %i, %i}\n", distancia, alvo[0], alvo[1], alvo[2]);
    if(distancia == 0){
      return 4;
    }else{
      int distanciaX = *x-alvo[0];
      distanciaX = (distanciaX < 0)? distanciaX*-1: distanciaX;
      int distanciaY = *y-alvo[1];
      distanciaY = (distanciaY < 0)? distanciaY*-1: distanciaY;
                                                                                                                            fprintf(stderr, "L359 %i < %i\n", distanciaX, distanciaY);
      if(distanciaX < distanciaY){                                                                                          fprintf(stderr, "L360 movVer = %s\n", movVer);
        if(strcmp(movHor, "LEFT") == 0){
          return 1;
        }else if(strcmp(movHor, "RIGHT") == 0){
          return 3;
        }
      }else{                                                                                                                fprintf(stderr, "L366 movHor = %s\n", movHor);
        if(strcmp(movVer, "UP") == 0){
          return 0;
        }else if(strcmp(movVer, "DOWN") == 0){
          return 2;
        }
      }
    }
  }

  
  /*Refatoramento para recursão
    char resultado;
    if(distanciaY > distanciaX){                                                                                              //Mover-se verticalmente
      if(strcmp(movVer, "UP") == 0){
        if(strcmp(movHor, "LEFT") == 0){
          strcpy(resultado, conferirConflito(x, y, "UP", "LEFT", 1));
        }else if(strcmp(movHor, "RIGHT") == 0){
          strcpy(resultado, conferirConflito(x, y, "UP", "RIGHT", 1));
        }
        if(strcmp(resultado, NULL)){
          return movVer;
        }
      }else if(strcmp(movVer, "DOWN") == 0){
        strcpy(resultado, conferirConflito(x, y, "DOWN", 1));

        if(strcmp(resultado, NULL)){
          return movVer;
        }
      }
    }else{                                                                                                                    //Mover-se horizontalmente
      if(strcmp(movHor, "LEFT") == 0){
        strcpy(resultado, conferirConflito(x, y, "LEFT", 1));
        
        if(strcmp(resultado, NULL)){
          return movHor;
        }
      }else if(strcmp(movHor, "RIGHT") == 0){
        strcpy(resultado, conferirConflito(x, y, "RIGHT", 1));
        
        if(strcmp(resultado, NULL)){
          return movHor;
        }
      }
    }
    return resultado;
  */
  

  /*
  if(distanciaY > distanciaX){                                                                                                //Mover-se verticalmente
    if(strcmp(movVer, "UP") == 0){
      for(f = 0; f < qtdBots; f++){                                                                                           //Conferir conflitos com outros barcos
        if(*y-1 == bots[f].y && *x == bots[f].x){//Há um barco acima
          if(strcmp(movHor, "LEFT") == 0){
            for(f2 = 0; f2 < qtdBots; f2++){
              if(*x-1 == bots[f2].x && *y == bots[f2].y){//Há um barco a esquerda
                return -3;
              }
            }
            return 1;
          }else if(strcmp(movHor, "RIGHT") == 0){
            for(f2 = 0; f2 < qtdBots; f2++){
              if(*x+1 == bots[f2].x && *y == bots[f2].y){//Há um barco a direita
                return -3;
              }
            }
            return 3;
          }
        }
      }
      return 0;
    }else if(strcmp(movVer, "DOWN") == 0){
      for(f = 0; f < qtdBots; f++){
        if(*y+1 == bots[f].y && *x == bots[f].x){//Há um barco abaixo
          if(strcmp(movHor, "LEFT") == 0){
            for(f2 = 0; f2 < qtdBots; f2++){
              if(*x-1 == bots[f2].x && *y == bots[f2].y){//Há um barco esquerda
                return -3;
              }
            }
            return 1;
          }else if(strcmp(movHor, "RIGHT") == 0){
            for(f2 = 0; f2 < qtdBots; f2++){
              if(*x+1 == bots[f2].x && *y == bots[f2].y){//Há um barco a direita
                return -3;
              }
            }
            return 3;
          }
        }
      }
      return 2;
    }
  }else{                                                                                                                    //Mover-se horizontalmente
    if(strcmp(movHor, "LEFT") == 0){
      for(f = 0; f < qtdBots; f++){
        if(*x-1 == bots[f].x && *y == bots[f].y){//Há um barco a esquerda
          if(strcmp(movVer, "UP") == 0){
            for(f2 = 0; f2 < qtdBots; f2++){
              if(*y-1 == bots[f2].y && *x == bots[f2].x){//Há um barco acima
                return -3;
              }
            }
            return 0;
          }else if(strcmp(movVer, "DOWN") == 0){
            for(f2 = 0; f2 < qtdBots; f2++){
              if(*y+1 == bots[f2].y && *x == bots[f2].x){//Há um barco abaixo
                return -3;
              }
            }
            
            return 2;
          }
        }
      }
      return 1;
    }else if(strcmp(movHor, "RIGHT") == 0){
      for(f = 0; f < qtdBots; f++){
        if(*x+1 == bots[f].x && *y == bots[f].y){//Há um barco a direita 
          if(strcmp(movVer, "UP") == 0){
            for(f2 = 0; f2 < qtdBots; f2++){
              if(*y-1 == bots[f2].y && *x == bots[f2].x){//Há um barco acima
                return -3;
              }
            }
            return 0;
          }else if(strcmp(movVer, "DOWN") == 0){
            for(f2 = 0; f2 < qtdBots; f2++){
              if(*y+1 == bots[f2].y && *x == bots[f2].x){//Há um barco abaixo
                return -3;
              }
            }
            return 2;
          }
        }
      }
      return 3;
    }
  }
  //*/
}

int main(){
  char line[MAX_STR];                                                                                                       //Dados temporários
  char myId[MAX_STR];                                                                                                       //Identificador do bot em questão

  setbuf(stdin, NULL); setbuf(stdout, NULL); setbuf(stderr, NULL);                                                          //Stdin, stdout e stderr não terão buffers. Assim, nada é "guardado temporariamente"

  //===INÍCIO DA PARTIDA===
  int h, w;                                                                                                                 //h = height //w = width

  scanf("AREA %i %i", &h, &w);                                                                                              //Lê a dimensão da área de pesca: altura (h) x largura (w)
  altura  = h;
  largura = w;
  scanf(" ID %s", myId);                                                                                                    //...e o id do bot. O " " antes de ID é necessário para ler o '\n' da linha anterior


  //Para "debugar", é possível enviar dados para a saída de erro padrão (stderr), eis um exemplo:       fprintf(stderr, "Meu id = %s\n", myId);

  //===PARTIDA===
  int alvo[3];                                                                                                              //{x, y, tipo}
  alvo[2] = 3;                                                                                                              //Procurar por Robalos
  while (1){                                                                                                                // O bot entra num laço infinito, mas não se preocupe porque o simulador irá encerrar o processo quando o jogo terminar.
    readData(h, w);                                                                                                         //LÊ OS DADOS DO JOGO E ATUALIZA OS DADOS DO BOT

    // INSIRA UMA LÓGICA PARA ESCOLHER UMA AÇÃO A SER EXECUTADA

    int f;
    //for(f = 0; f < qtdPortos; f++){
    //  fprintf(stderr, "[X:%d, Y:%d]\n", portos[f].x, portos[f].y);
    //}
    char acao[10];
    for(f = 0; f < qtdBots; f++){
      if(strcmp(myId, bots[f].id) == 0){                                                                                    //Encontrar esse bot
        if(peso == 10){
          alvo[2] = 4;
        }else{
          alvo[2] = 3;
        }
        int res;
        res = definirAcao(&bots[f].x, &bots[f].y, alvo);                                                                    
        switch(res){
          case  0: strcpy(acao, "UP");    break;
          case  1: strcpy(acao, "LEFT");  break;
          case  2: strcpy(acao, "DOWN");  break;
          case  3: strcpy(acao, "RIGHT"); break;
          case  4: strcpy(acao, "FISH");  break;
          case  5: strcpy(acao, "SELL");  break;
          default: strcpy(acao, "");      break;
        }
                                                                                                                            fprintf(stderr, "res = %s\n", acao);
        break;
      }
    }

                                                                                                                            fprintf(stderr, "L559 peso = %i", peso);
    printf(" %s\n", acao);                                                                                                  //Envia a ação escolhida
    if(strcmp(acao, "FISH") == 0){
      peso = peso+1;
    }
    if(strcmp(acao, "SELL") == 0){
      peso = 0;
    }

    scanf("%s", line);                                                                                                      //Lê qual foi o resultado da ação (e eventualmente atualiza os dados do bot).
  }

  return 0;
}



/*Comando para rodar:
  gcc bot_A.c -o bot_A; gcc bot_B.c -o bot_B; node ocma bot_A bot_B
//*/