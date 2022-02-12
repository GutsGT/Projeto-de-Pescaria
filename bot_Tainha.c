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


int definirAcao(int *x, int *y, int alvo[3]){
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


  if(alvo[2] == 4){
    for(f = 0; f < qtdPortos; f++){                                                                                         //Conferir qual o porto mais próximo

      int distanciaX = *x-portos[f].x;
      distanciaX = (distanciaX < 0)? distanciaX*-1:distanciaX;

      int distanciaY = *y-portos[f].y;
      distanciaY = (distanciaY < 0)? distanciaY*-1:distanciaY;

      if(distanciaX+distanciaY < distancia){
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

      
    }
    if(distancia == 0){
      return 5;
    }else{
      int distanciaX = *x-alvo[0];
      distanciaX = (distanciaX<0)? distanciaX*-1:distanciaX;
      int distanciaY = *y-alvo[1];
      distanciaY = (distanciaY<0)? distanciaY*-1:distanciaY;
      if(distanciaX < distanciaY){
        if(strcmp(movHor, "LEFT") == 0){
          return 1;
        }else if(strcmp(movHor, "RIGHT") == 0){
          return 3;
        }
      }else{
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
      
      }
    }
    if(distancia == 0){
      return 4;
    }else{
      int distanciaX = *x-alvo[0];
      distanciaX = (distanciaX < 0)? distanciaX*-1: distanciaX;
      int distanciaY = *y-alvo[1];
      distanciaY = (distanciaY < 0)? distanciaY*-1: distanciaY;
     
      if(distanciaX < distanciaY){
        if(strcmp(movHor, "LEFT") == 0){
          return 1;
        }else if(strcmp(movHor, "RIGHT") == 0){
          return 3;
        }
      }else{       
        if(strcmp(movVer, "UP") == 0){
          return 0;
        }else if(strcmp(movVer, "DOWN") == 0){
          return 2;
        }
      }
    }
  }
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
  alvo[2] = 1;                                                                                                              //Procurar por Tainhas
  while (1){                                                                                                                // O bot entra num laço infinito, mas não se preocupe porque o simulador irá encerrar o processo quando o jogo terminar.
    readData(h, w);                                                                                                         //LÊ OS DADOS DO JOGO E ATUALIZA OS DADOS DO BOT

    // INSIRA UMA LÓGICA PARA ESCOLHER UMA AÇÃO A SER EXECUTADA

    int f;
    char acao[10];
    for(f = 0; f < qtdBots; f++){
      if(strcmp(myId, bots[f].id) == 0){                                                                                    //Encontrar esse bot
        if(peso == 10){
          alvo[2] = 4;
        }else{
          alvo[2] = 1;
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
        break;
      }
    }

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
  gcc bot_Tainha.c -o bot_Tainha; node ocma bot_Tainha
//*/