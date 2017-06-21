/*

Francisco Klismark Barbosa
2º Semestre SI

Projeto II - Paciência Spider

CHEATS{
0   - Chama o monte
99  - Reinicia Partida
789 - Vira todas as cartas
98  - Volta uma Jogada
-6  - Ganha o jogo
}

*/
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#define tam_deck 10

void carregando();
void quadro();
void telaNivel();
void telaInicial();
void desenhaLinha(int X, int Y,int tamanho,int dorme,char caractere );
void desenhaColuna(int X, int Y,int tamanho,int dorme,char caractere );
void iniciaJogo();
void pontuacao();
void apagaQuadro(int X,int Y,int altura,int largura);

struct cartas *baralho = NULL;
struct cartas *monte = NULL;
struct cartas *tabuleiro[tam_deck];

struct cartas *temporario = NULL;
struct cartas *tempo2 = NULL;

int i,j,k,l,-6o,c1,c2,qnt;
int numero_carta;
char dific,confir;
int pontos = 100000;
int desenho = 15;
int carregado = 0;

struct cartas{
   int num;
   int pos;
   int virada;// Variável Booleana virada 1 desvirada 0
   struct cartas *ant;
   struct cartas *prx;
   int naipe;
}typedef cartas;

void imprimeLista(cartas *temp,cartas *t){
     if(t != NULL){
         while(temp != t){
             printf("[%2d] -->%2d", temp->num, temp->pos);
             printf("\n");
             temp = temp->prx;
         }
     }
}

void gotoxy(int x, int y) {
  COORD c;
  c.X = x - 1;
  c.Y = y - 1;
  SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}
/*Criação e enbaralhamento das cartas*/
int geraAleatorio(int fim){
   return rand() % fim;
}

cartas * criaCarta(){
   cartas *nova_carta = (cartas*) malloc(sizeof(cartas));
   nova_carta->prx = NULL;
   nova_carta->ant = NULL;
   return nova_carta;
}

cartas * insereValor(int numero,int posicao,int vira,int naipe){
       cartas *nova_carta = criaCarta();
       nova_carta->num = numero;
       nova_carta->pos = posicao;
       nova_carta->virada = vira;
       nova_carta->naipe = naipe;
       return nova_carta;
}

void insereNoBaralho(cartas *temp){
     if(baralho == NULL){
        baralho = criaCarta();
        baralho->ant = baralho;
        baralho->prx = baralho;
     }
        temp->prx = baralho->prx;
        temp->ant = baralho->prx->ant;
        baralho->prx->ant = temp;
        baralho->prx = temp;

}

void insereNoMonte(cartas *temp){
     if(monte == NULL){
        monte = criaCarta();
        monte->ant = monte;
        monte->prx = monte;
     }
        temp->prx = monte->prx;
        temp->ant = monte->prx->ant;
        monte->prx->ant = temp;
        monte->prx = temp;

}

void insereNoDeck(cartas *temp,int ind){
     if(tabuleiro[ind] == NULL){
        tabuleiro[ind] = criaCarta();
        tabuleiro[ind]->ant = tabuleiro[ind];
        tabuleiro[ind]->prx = tabuleiro[ind];
     }
        temp->prx = tabuleiro[ind]->prx;
        temp->ant = tabuleiro[ind]->prx->ant;
        tabuleiro[ind]->prx->ant = temp;
        tabuleiro[ind]->prx = temp;
}
void insereNoFinalDeck(cartas *t,cartas *temp,int ind){
     if(tabuleiro[ind] == NULL){
        tabuleiro[ind] = criaCarta();
        tabuleiro[ind]->ant = tabuleiro[ind];
        tabuleiro[ind]->prx = tabuleiro[ind];
     }
        temp->ant->ant = temp->ant;
        temp->ant->prx = temp->ant;
        temp->prx = t->prx;
        temp->ant = t->prx->ant;
        t->prx->ant = temp;
        t->prx = temp;
}

void insereNaLista(cartas *temp){
     if(temporario == NULL){
        temporario = criaCarta();
        temporario->ant = temporario;
        temporario->prx = temporario;
     }
        temp->prx = temporario->prx;
        temp->ant = temporario->prx->ant;
        temporario->prx->ant = temp;
        temporario->prx = temp;

}
int pegaValorTrocado(cartas *temp,int numero){
    int retorna;
     if(baralho != NULL){
         while(temp != baralho){
             if(temp->pos == numero){
                retorna = temp->num;
             break;
             }
             temp = temp->prx;
         }

     }
    return retorna;
}
int pegaNaipeTrocado(cartas *temp,int numero){
    int retorna;
     if(baralho != NULL){
         while(temp != baralho){
             if(temp->pos == numero){
                retorna = temp->naipe;
             break;
             }
             temp = temp->prx;
         }

     }
    return retorna;
}
void insereValorTrocado(cartas *temp, int numero, int posi,int naipe){
     if(baralho != NULL){
         while(temp != baralho){
             if(temp->pos==posi){
                temp->num = numero;
                temp->naipe = naipe;
                break;
             }
             temp = temp->prx;
         }
     }
}

void embaralha(cartas *temp,int fim){
     if(baralho != NULL){
         while(temp != baralho){

             int carta1 = temp->num; //Valor Carta 1
             int naipe1 = temp->naipe;//Naipe Carta 1

             int Idcarta2 = (1+geraAleatorio(fim-1)); //Id carta 2

             temp->num = pegaValorTrocado(baralho->prx,Idcarta2);
             temp->naipe = pegaNaipeTrocado(baralho->prx,Idcarta2);

             insereValorTrocado(baralho->prx,carta1,Idcarta2,naipe1);
             temp = temp->prx;
         }
     }
}

void enumeraCartas(int numNaipe,int numDeCartas){
   numero_carta = 1;
   int naipe = (geraAleatorio(numNaipe)+3);
   for(i=1;i<=numDeCartas;i++){

      if(numero_carta == 14){
         numero_carta = 1;
         naipe = (geraAleatorio(numNaipe)+3);
      }
      cartas *nova_carta = insereValor(numero_carta,i,0,naipe);
      insereNoBaralho(nova_carta);
      numero_carta++;
   }

}

void separaMonte(cartas *temp,int nivel){
     int num_no_monte = nivel;

     if(baralho != NULL){
         while(temp != baralho){
             if((temp->pos)<= num_no_monte){
                cartas *nova_carta = insereValor(temp->num,temp->pos,temp->virada,temp->naipe);
                insereNoMonte(nova_carta);
             }
             temp = temp->prx;
         }
     }
}

void separaTabuleiro(cartas *temp,int minimo,int maximo){
     i = 0;
     if(baralho != NULL){
         while(temp != baralho){
             if( ((temp->pos)>=minimo) && ((temp->pos)<=maximo) ){
                i == tam_deck ? i=0 : i;
                cartas *new_carta = insereValor(temp->num,temp->pos,temp->virada,temp->naipe);
                insereNoDeck(new_carta,i);
                i++;
             }
             temp = temp->prx;
         }
     }
}

void viraCartas(cartas *temp,cartas *t){
     int n;
     if(t != NULL){
         while(temp != t){
             n = (1+geraAleatorio(4));
             if(n!=4){
                temp->virada = 1;
             }else{
                temp->virada = 1;
                temp->ant->virada = 1;
             }
             break;
             temp = temp->ant;
         }
     }
}
int contaCartas(cartas *temp,cartas *t){
    int count = 0;
     if(t != NULL){
         while(temp != t){
            count++;
            temp = temp->prx;
         }
     }
   return count;
}

int contaCartasVirada(cartas *temp,cartas *t){
    int count = 0;
     if(t != NULL){
         while(temp != t){
            if(temp->virada==1){
               count++;
            }
            temp = temp->prx;
         }
     }
   return count;
}
void separaCartas(int nivel){
   if(nivel==1){               //Nível Fácil Numero de cartas igual a 39 e 1 Naipe
      enumeraCartas(1,39);
      embaralha(baralho->prx,39);
      separaMonte(baralho->prx,tam_deck);// Numero no monte
      separaTabuleiro(baralho->prx,11,39);
      for(i=0;i<tam_deck;i++){
         viraCartas(tabuleiro[i]->ant,tabuleiro[i]);
      }
   }else if(nivel==2){         //Nível Médio Numero de cartas igual a 65 e 2 Naipe
      enumeraCartas(2,65);
      embaralha(baralho->prx,65);
      separaMonte(baralho->prx,30);// Numero no monte
      separaTabuleiro(baralho->prx,31,65);
      for(i=0;i<tam_deck;i++){
         viraCartas(tabuleiro[i]->ant,tabuleiro[i]);
      }
   }else if(nivel==3){                      //Nível Difícil Numero de cartas igual a 78 e 3 Naipe
      enumeraCartas(3,78);
      embaralha(baralho->prx,78);
      separaMonte(baralho->prx,40);// Numero no monte
      separaTabuleiro(baralho->prx,41,78);
      for(i=0;i<tam_deck;i++){
         viraCartas(tabuleiro[i]->ant,tabuleiro[i]);
      }
   }else{
      telaNivel();
   }
}


/*Criação e enbaralhamento das cartas*/

/* JOGADAS */
void viraUltimaCarta(cartas *temp){
   temp->ant->virada = 1;
}
void desenhaCartaDesvirada(int X,int Y){
  gotoxy(X,Y);    printf("%c%c%c%c%c",218,196,196,196,191);
  gotoxy(X,(Y+1));printf("%c   %c",179,179);
  gotoxy(X,(Y+2));printf("%c %c %c",179,desenho,179);
  gotoxy(X,(Y+3));printf("%c   %c",179,179);
  gotoxy(X,(Y+4));printf("%c%c%c%c%c",192,196,196,196,217);
}
void desenhaCartaVirada(int X,int Y,int num,int naipe){
  gotoxy(X,Y);printf("%c%c%c%c%c",201,205,205,205,187);
  gotoxy(X,(Y+1));printf("%c  %c%c",186,naipe,186);
  gotoxy(X,(Y+2));printf("%c   %c",186,186);
  gotoxy(X,(Y+3));printf("%c   %c",186,186);
  gotoxy(X,(Y+4));printf("%c%c%c%c%c",200,205,205,205,188);
   gotoxy((X+1),(Y+1));
   if(num==1){
      printf("A");
   }else if(num==11){
      printf("J");
   }else if(num==12){
      printf("Q");
   }else if(num==13){
      printf("K");
   }else{
      printf("%d",num);
   }

}

void apagaDesenhoDaCarta(int X,int Y){
  gotoxy(X,Y);printf("      ");
  gotoxy(X,(Y+1));printf("      ");
  gotoxy(X,(Y+2));printf("      ");
  gotoxy(X,(Y+3));printf("      ");
  gotoxy(X,(Y+4));printf("      ");
  gotoxy((X+1),(Y+1));
}
void apagaTabuleiro(cartas *temp,cartas *t,int X, int Y){
     j=0;l=0;
     int h;
     while(j<15){
        apagaDesenhoDaCarta((X+j),(Y+l));
        l+=2;j++;
     }
}
void desenhaCartas(cartas *temp,cartas *t,int X, int Y){
     j=0;l=0;
     apagaTabuleiro(temp,t,X,Y);
     j=0;l=0;
     if(t != NULL){
         while(temp != t){
            if(temp->virada==0){
               desenhaCartaDesvirada((X+j),(Y+l));
               j++;l+=2;
               sleep(2);
            }else{
               desenhaCartaVirada((X+j),(Y+l),temp->num,temp->naipe);
               j++;l+=2;
               sleep(2);
            }
            temp = temp->prx;
         }
     }

}

void desenhaMonte(int X, int Y){
   gotoxy(X,Y);
   apagaQuadro(X,Y,10,25);
   int num_mont = (int)(contaCartas(monte->prx,monte)/10);
   for(j=0;j<num_mont;j++){
      desenhaCartaDesvirada((X+j),(Y+j));
   }
}

void removeCarta(cartas *temp){
   if((temp->prx==temp) && (temp->ant==temp)){
      temp->ant->prx = temp->ant;
      temp->ant->ant = temp->ant;
      free(temp);
   }else{
      temp->ant->prx = temp->prx;
      temp->prx->ant = temp->ant;
      free(temp);
   }
}

void apagaLista(cartas *temp,cartas *t){
     if(t != NULL){
         while(temp != t){
            removeCarta(temp);
            temp = temp->prx;
         }
     }
}

void retiraCartas(cartas *temp,cartas *t){
   cartas *new_carta = insereValor(temp->num,temp->pos,temp->virada,temp->naipe);
   insereNaLista(new_carta);
}

void insereEmTempo2(cartas *temp){
     if(tempo2 == NULL){
        tempo2 = criaCarta();
        tempo2->ant = tempo2;
        tempo2->prx = tempo2;
     }
        temp->prx = tempo2->prx;
        temp->ant = tempo2->prx->ant;
        tempo2->prx->ant = temp;
        tempo2->prx = temp;

}

void criaTempo2(cartas* temp,cartas *t,int quant){
     int x = 1;
     if(t != NULL){
         while(temp != t){
            if(x<=quant){
               cartas *new_carta = insereValor(temp->num,temp->pos,temp->virada,temp->naipe);
               insereEmTempo2(new_carta);
            }
            x++;
            temp = temp->ant;
         }

     }

}

int retornaValor(cartas* temp,cartas *t,int x){
     int y = 1;
     int retorno = 0;
     if(t != NULL){
         while(temp != t){
            if(y==x){
               retorno = temp->num;
               break;
            }
            y++;
            temp = temp->prx;
         }
     }
     return retorno;
}
int retornaNaipe(cartas* temp,cartas *t,int x){
     int y = 1;
     int retorno = 0;
     if(t != NULL){
         while(temp != t){
            if(y==x){
               retorno = temp->naipe;
               break;
            }
            y++;
            temp = temp->prx;
         }
     }
     return retorno;
}

int confereJogada(int nume,int quant,int col,int naipe){
   int x = 1;
   int retorno = 0;
   criaTempo2(tabuleiro[col]->ant,tabuleiro[col],quant);
   if(nume==0){
      nume=tempo2->prx->num+1;
      naipe=tempo2->prx->naipe;
   }
   if(tempo2 != NULL){
      while(tempo2->ant != tempo2){
         if((nume-x)==retornaValor(tempo2->prx,tempo2,x) && (naipe==retornaNaipe(tempo2->prx,tempo2,x))){
            retorno = 1;
         }else{
            retorno = 0;
            break;
         }
         x++;

         tempo2->ant = tempo2->ant->ant;
      }
   }

   return retorno;
}

int verificaColuna(cartas* temp,cartas *t){
     int x=13;
     int retorno = 0;
     int naipe = 0;
     if(t != NULL){
         while(temp != t){
            if((x==temp->num) && (temp->virada==1)){
               if(naipe==0){
                  naipe = temp->naipe;
               }
               if((temp->naipe==naipe)) {
                  retorno = 1;
                  x--;
               }else{
                  retorno = 0;
                  break;
               }
            }else if(temp->virada==0){
               retorno = 0;
            }else{
               retorno = 0;
               break;
            }
            if((x==0) && (retorno==1)){
               retorno = 1;
            }else{
               retorno = 0;
            }
         temp = temp->prx;
         }
     }

    return retorno;
}

void movimentaCartas(int quant,int col1,int col2){
   if( ((col1>=1)&&(col1<=10)) && ((col2>=1)&&(col2<=10))){
      if(( quant<=contaCartasVirada(tabuleiro[(col1-1)]->prx,tabuleiro[(col1-1)]) )){
         c1 = col1-1;
         c2 = col2-1;
         qnt = quant;

         int p;
         cartas *temp2 = tabuleiro[(col2-1)]->ant;
         int regra = 0;

         if(temp2->prx==temp2){//Se a coluna2 estiver vazia
            regra = confereJogada(0,quant,(col1-1),0);
         }else{
            regra = confereJogada(temp2->num,quant,(col1-1),temp2->naipe);
         }
         apagaLista(tempo2->prx,tempo2);
         if(temporario!=NULL){
            apagaLista(temporario->prx,temporario);
         }
         if(regra==1){
             for(p=0;p<quant;p++){
                retiraCartas(tabuleiro[(col1-1)]->ant,tabuleiro[(col1-1)]);
                removeCarta(tabuleiro[(col1-1)]->ant);
                insereNoFinalDeck(temp2,temporario->prx,(col2-1));
                pontos -= 2197;
                pontuacao();
             }
             if(verificaColuna(tabuleiro[(col2-1)]->prx,tabuleiro[(col2-1)])==1){
                for(p=0;p<13;p++){
                   if(tabuleiro[(col2-1)]->ant->virada == 1){
                      removeCarta(tabuleiro[(col2-1)]->ant);
                   }
                }
                pontos -= 1349;
                pontuacao();
             }
             apagaQuadro(20,54,1,100);
         }else{
            //Jogada proibida
            gotoxy(20,54);
            apagaQuadro(20,54,1,100);
            printf("Jogada proibida->  A Ordem deve ser: 'K-Q-J-10-9-8-7-6-5-4-3-2-A' com naipes iguais");
            pontos -= 3286;
            pontuacao();
         }

         iniciaJogo();
      }else{
         //Numero de cartas viradas insuficiente
         apagaQuadro(20,54,1,100);
         gotoxy(20,54);
         printf("Jogada proibida-> Numero de cartas solicitada n%co existe ou n%co est%co viradas",134,134,134);
         iniciaJogo();
      }
   }else{
     //Não existe a coluna solicitada
         apagaQuadro(20,54,1,100);
         gotoxy(20,54);
         printf("Jogada proibida-> Coluna solicitada n%co existe",134,134,134);
         iniciaJogo();
   }
}
void insereMonte(cartas *temp,cartas *t,int ind){
     if(tabuleiro[ind] == NULL){
        tabuleiro[ind] = criaCarta();
        tabuleiro[ind]->ant = tabuleiro[ind];
        tabuleiro[ind]->prx = tabuleiro[ind];
     }
     t->ant = temp;
     t->prx = temp->prx;
     temp->prx->ant = t;
     temp->prx = t;
}

void chamaMonte(cartas* temp,cartas *t){
     int x = 0;
     cartas *tmp = NULL;
     for(x=0;x<tam_deck;x++){
        tmp = tabuleiro[x]->ant;
        cartas *new_carta = insereValor(monte->ant->num,monte->ant->pos,1,monte->ant->naipe);
        insereMonte(tmp,new_carta,x);
        removeCarta(monte->ant);
     }
}

void cheatViraCartas(){
 for(i=0;i<tam_deck;i++){
   cartas *t = tabuleiro[i]->prx;
   if(tabuleiro[i] != NULL){
      while(t != tabuleiro[i]){
         t->virada = 1;
         t = t->prx;
      }
   }
 }
 iniciaJogo();
}

void reiniciaPartida(){
   carregado = 0;
   apagaLista(baralho->prx,baralho);
   apagaLista(monte->prx,monte);
   for(i=0;i<tam_deck;i++){
      apagaLista(tabuleiro[i]->prx,tabuleiro[i]);
   }
   telaNivel();
}

void voltaJogada(){

if(temporario != NULL){
      cartas *temp2 = tabuleiro[(c1)]->ant;
      int p;
      for(p=0;p<qnt;p++){
         retiraCartas(tabuleiro[(c2)]->ant,tabuleiro[(c2)]);
         removeCarta(tabuleiro[(c2)]->ant);
         insereNoFinalDeck(temp2,temporario->prx,(c1));
         pontos += 2197;
         pontuacao();
      }
      apagaLista(temporario->prx,temporario);
   }
   iniciaJogo();
}

void desenhaNumeros(char num,int X, int Y){
 gotoxy(X,Y);
 int caracter = 178;
 int numero = atoi(&num);
 if(numero==0){
       desenhaLinha(X,Y,3,0,caracter);
       desenhaColuna(X,Y,5,0,caracter);
       desenhaColuna((X+2),Y,5,0,caracter);
       desenhaLinha(X,(Y+4),3,0,caracter);
 }else if(numero==1){
       desenhaColuna((X+1),Y,5,0,caracter);
       desenhaColuna((X),(Y+1),1,0,caracter);
       desenhaLinha(X,(Y+4),3,0,caracter);
 }else if(numero==2){
       desenhaColuna((X+2),Y,3,0,caracter);
       desenhaColuna((X),(Y+2),3,0,caracter);
       desenhaLinha(X,(Y),3,0,caracter);
       desenhaLinha(X,(Y+2),3,0,caracter);
       desenhaLinha(X,(Y+4),3,0,caracter);
 }else if(numero==3){
       desenhaColuna((X+2),Y,5,0,caracter);
       desenhaLinha(X,(Y),3,0,caracter);
       desenhaLinha(X,(Y+2),3,0,caracter);
       desenhaLinha(X,(Y+4),3,0,caracter);
 }else if(numero==4){
       desenhaColuna(X,Y,3,0,caracter);
       desenhaColuna((X+2),Y,5,0,caracter);
       desenhaLinha(X,(Y+2),3,0,caracter);
 }else if(numero==5){
       desenhaColuna((X+2),(Y+2),3,0,caracter);
       desenhaColuna((X),(Y),3,0,caracter);
       desenhaLinha(X,(Y),3,0,caracter);
       desenhaLinha(X,(Y+2),3,0,caracter);
       desenhaLinha(X,(Y+4),3,0,caracter);
 }else if(numero==6){
       desenhaColuna(X,Y,5,0,caracter);
       desenhaColuna((X+2),(Y+2),3,0,caracter);
       desenhaLinha(X,(Y),3,0,caracter);
       desenhaLinha(X,(Y+2),3,0,caracter);
       desenhaLinha(X,(Y+4),3,0,caracter);
 }else if(numero==7){
       desenhaLinha(X,Y,3,0,caracter);
       desenhaColuna((X+2),Y,5,0,caracter);
 }else if(numero==8){
       desenhaColuna(X,Y,5,0,caracter);
       desenhaColuna((X+2),Y,5,0,caracter);
       desenhaLinha(X,(Y),3,0,caracter);
       desenhaLinha(X,(Y+2),3,0,caracter);
       desenhaLinha(X,(Y+4),3,0,caracter);
 }else if(numero==9){
       desenhaColuna(X,Y,3,0,caracter);
       desenhaColuna((X+2),Y,5,0,caracter);
       desenhaLinha(X,(Y),3,0,caracter);
       desenhaLinha(X,(Y+2),3,0,caracter);
       desenhaLinha(X,(Y+4),3,0,caracter);
 }else{

 }
}
void pontuacao(){
     int x = 93;
     int y = 1;
     gotoxy(x,y);
     desenhaColuna(x,y,8,0,177);
     desenhaColuna((x+30),y,8,0,177);
     desenhaLinha(x,(y+8),31,0,177);
     gotoxy(103,1);
     int i;
     char pt[6];
     itoa(pontos,pt,10);
     printf("PONTUA%c%cO",128,142);
     apagaQuadro(x+2,3,6,27);
     for(i=0;i<6;i++){
        desenhaNumeros(pt[i],(x+(i*4)+5),3);
     }

}
void apresentacao(int X, int Y){
     j=0;l=0;
     int h;
     while(j<14){
        if((X+j)>120){
           break;
        }
        desenhaCartaDesvirada((X+j),(Y+l));
        l+=2;j++;
        sleep(35);
     }
     while(j<26){
        if((X+j)>120){
           break;
        }
        desenhaCartaDesvirada((X+j),(Y+l));
        l-=2;j++;
        sleep(30);
     }
     while(j<38){
        if((X+j)>120){
           break;
        }
        desenhaCartaDesvirada((X+j),(Y+l));
        l+=2;j++;
        sleep(25);
     }
     while(j<45){
        if((X+j)>120){
           break;
        }
        desenhaCartaDesvirada((X+j),(Y+l));
        l-=2;j++;
        sleep(20);
     }
     while(j<52){
        if((X+j)>120){
           break;
        }
        desenhaCartaDesvirada((X+j),(Y+l));
        l+=2;j++;
        sleep(15);
     }
     while(j<58){
        if((X+j)>120){
           break;
        }
        desenhaCartaDesvirada((X+j),(Y+l));
        l-=2;j++;
        sleep(10);
     }
     while(j<63){
        if((X+j)>120){
           break;
        }
        desenhaCartaDesvirada((X+j),(Y+l));
        l+=2;j++;
        sleep(5);
     }
}
void vitoria(){

   for(k=0;k<=11;k++){
      apresentacao(((k*10)+7),21);
   }
   apagaQuadro(40,30,15,56);
   gotoxy(45,35);
   printf("Deseja jogar novamente? (1)-SIM /(0)-NAO [   ]");
   gotoxy(55,38);
   printf("Sua PONTUACAO foi: %d",pontos);
   gotoxy(88,35);
   int n;
   scanf("%d",&n);
   if(n==1){
      reiniciaPartida();
   }else if(n==0){
      exit(0);
   }else{
      reiniciaPartida();
   }
   getch();
   telaInicial();

}

void analisaVitoria(){
     int x,y;
     for(x=0;x<tam_deck;x++){
        if(tabuleiro[x]->prx==tabuleiro[x]){
           y = 1;
        }else{
           y = 0;
           break;
        }
     }
     if(y==1){
        system("cls");
        quadro();
        vitoria();
     }
}

void analisaEntrada(int entrada){
     if(entrada==0 ){
        if(monte->ant!=monte){
           chamaMonte(monte->ant,monte);
        }
        iniciaJogo();
     }else if(entrada == -6){//Cheat Ganha o Jogo
         int i;
         for(i=0;i<tam_deck;i++){
            apagaLista(tabuleiro[i]->prx,tabuleiro[i]);
         }
         iniciaJogo();
     }else if(entrada == 99){//Reinicia a partida
        reiniciaPartida();
     }else if(entrada == 789){//Vira Cartas
        cheatViraCartas();
     }else if(entrada == 98){//volta a jogada
        voltaJogada();
     }
}


void controles(int X,int Y){
   gotoxy(X,Y);
   desenhaColuna(X,Y,3,0,177);
   desenhaLinha(X,(Y+3),42,0,177);
   desenhaColuna((X+42),Y,4,0,177);
   gotoxy((X+1),(Y+2));
   int quant_cartas,col_retira,col_coloca;
   printf("Quantas cartas deseja mover:------>[   ]");
   gotoxy((X+38),(Y+2));
   scanf("%d",&quant_cartas);
   analisaEntrada(quant_cartas);

   gotoxy((X+1),(Y+2));
   printf("Retirar carta(s) de qual coluna?:->[   ]");
   gotoxy((X+38),(Y+2));
   scanf("%d",&col_retira);
   analisaEntrada(col_retira);

   gotoxy((X+1),(Y+2));
   printf("Colocar carta(s) em qual coluna?:->[   ]");
   gotoxy((X+38),(Y+2));
   scanf("%d",&col_coloca);
   analisaEntrada(col_coloca);

   movimentaCartas(quant_cartas,col_retira,col_coloca);
}

void apagaQuadro(int X,int Y,int altura,int largura){
   int a,b;
   gotoxy(X,Y);
   for(a=0;a<altura;a++){
      for(b=0;b<largura;b++){
         printf(" ");
      }
      gotoxy(X,(Y+a));
   }

}
void iniciaJogo(){


   if(carregado==0){
      system("cls");
      carregando();
      system("cls");
      carregado = 1;
   }
   quadro();
   for(k=0;k<10;k++){
      gotoxy(((k*10)+7),18);
      printf("COLUNA");
      gotoxy(((k*10)+7),19);
      printf("%c %02d %c",176,(k+1),176);
      gotoxy(((k*10)+7),20);
      printf("%c%c%c%c%c%c",176,176,176,176,176,176);
      viraUltimaCarta(tabuleiro[k]);
      desenhaCartas(tabuleiro[k]->prx,tabuleiro[k],((k*10)+7),21);
   }
   desenhaMonte(5,5);
   pontuacao();
   analisaVitoria();
   controles(50,1);


}

/* JOGADAS */

/* TELA */

void desenhaLinha(int X, int Y,int tamanho,int dorme,char caractere ){
   gotoxy(X,Y);
   for(i=0;i<tamanho;i++){
      if(dorme!=0){sleep(dorme);}
      gotoxy((X+i),Y);
      printf("%c",caractere);
   }
}
void desenhaColuna(int X, int Y,int tamanho,int dorme,char caractere){
   gotoxy(X,Y);
   for(i=0;i<tamanho;i++){
      if(dorme!=0){sleep(dorme);}
      gotoxy(X,(Y+i));
      printf("%c",caractere);
   }
}
void desenhaLogo(int X,int Y){
   //LETRA P
   desenhaLinha(X,Y,3,5,177);
   desenhaLinha(X,(Y+2),3,5,177);
   desenhaColuna(X,Y,5,5,177);
   desenhaColuna((X+2),Y,3,5,177);
   //LETRA A
   desenhaColuna((X+4),(Y+1),4,5,177);
   desenhaColuna((X+6),(Y+1),4,5,177);
   desenhaLinha((X+5),(Y),1,5,177);
   desenhaLinha((X+5),(Y+3),1,5,177);
   //LETRA C
   desenhaColuna((X+8),(Y+1),3,5,177);
   desenhaLinha((X+9),(Y),2,5,177);
   desenhaLinha((X+9),(Y+4),2,5,177);
   //LETRA I
   desenhaColuna((X+12),(Y),5,5,177);
   //LETRA E
   desenhaColuna((X+14),(Y),5,5,177);
   desenhaLinha((X+15),(Y),2,5,177);
   desenhaLinha((X+15),(Y+2),2,5,177);
   desenhaLinha((X+15),(Y+4),2,5,177);
   //LETRA N
   desenhaColuna((X+18),(Y),5,5,177);
   desenhaColuna((X+19),(Y),2,5,177);
   desenhaColuna((X+20),(Y+1),2,5,177);
   desenhaColuna((X+21),(Y+2),2,5,177);
   desenhaColuna((X+22),(Y),5,5,177);
   //LETRA C
   desenhaColuna((X+24),(Y+1),3,5,177);
   desenhaLinha((X+25),(Y),2,5,177);
   desenhaLinha((X+25),(Y+4),2,5,177);
   //LETRA I
   desenhaColuna((X+28),(Y),5,5,177);
   //LETRA A
   desenhaColuna((X+30),(Y+1),4,5,177);
   desenhaColuna((X+32),(Y+1),4,5,177);
   desenhaLinha((X+31),(Y),1,5,177);
   desenhaLinha((X+31),(Y+3),1,5,177);

   //LETRA S
   desenhaLinha((X+24),(Y+6),3,5,177);
   desenhaLinha((X+24),(Y+8),3,5,177);
   desenhaLinha((X+24),(Y+10),3,5,177);
   desenhaColuna((X+24),(Y+6),3,5,177);
   desenhaColuna((X+26),(Y+8),3,5,177);
   //LETRA P
   desenhaLinha((X+28),(Y+6),3,5,177);
   desenhaLinha((X+28),(Y+8),3,5,177);
   desenhaColuna((X+28),(Y+6),5,5,177);
   desenhaColuna((X+30),(Y+6),3,5,177);
   //LETRA I
   desenhaColuna((X+32),(Y+6),5,5,177);
   //LETRA D
   desenhaColuna((X+34),(Y+6),5,5,177);
   desenhaColuna((X+36),(Y+7),3,5,177);
   desenhaLinha((X+35),(Y+6),1,5,177);
   desenhaLinha((X+35),(Y+10),1,5,177);
   //LETRA E
   desenhaColuna((X+38),(Y+6),5,5,177);
   desenhaLinha((X+39),(Y+6),2,5,177);
   desenhaLinha((X+39),(Y+8),2,5,177);
   desenhaLinha((X+39),(Y+10),2,5,177);
   //LETRA R
   desenhaColuna((X+42),(Y+6),5,5,177);
   desenhaColuna((X+44),(Y+6),3,5,177);
   desenhaLinha((X+42),(Y+6),3,5,177);
   desenhaLinha((X+43),(Y+8),1,5,177);
   desenhaLinha((X+43),(Y+9),1,5,177);
   desenhaLinha((X+44),(Y+10),1,5,177);
}
void carregando(){
   gotoxy(30,24);
   printf("CARREGANDO");
   for(i=0;i<50;i++){
      sleep(100);
      gotoxy(30+i,25);
      printf("_");
      gotoxy(76,24);
      printf("%d%%",((i*2)+2));
   }
}
void quadro(){
   desenhaLinha(1,1,128,0,177);
   desenhaColuna(128,1,55,0,177);
   desenhaColuna(1,1,55,0,177);
   desenhaLinha(1,55,128,0,177);
}
void menuNivel(int X,int Y){
   desenhaColuna(X,(Y+1),30,0,178);
   desenhaColuna((X+60),(Y+1),30,0,178);
   desenhaLinha(X,Y,61,0,178);
   desenhaLinha(X,(Y+30),61,0,178);
   desenhaLinha(X,(Y+6),61,0,178);
   gotoxy((X+11),(Y+3));
   printf("ESCOLHA O NIVEL DE DIFICULDADE DO JOGO");
   gotoxy((X+17),(Y+4));
   printf("APERTE 'K' PARA CONFIRMAR");
   gotoxy((X+25),(Y+10));printf("FACIL");
   gotoxy((X+25),(Y+17));printf("MEDIO");
   gotoxy((X+25),(Y+25));printf("DIFICIL");
}
void pegaopt(){
   confir = dific;
   dific = getch();
}

void mostraopt(){
   pegaopt();
   if(dific=='1'){
      desenhaLinha(34,41,59,0,' ');
      desenhaLinha(34,43,59,0,' ');
      desenhaLinha(34,33,59,0,' ');
      desenhaLinha(34,35,59,0,' ');
      desenhaLinha(34,26,59,0,176);
      desenhaLinha(34,28,59,0,176);
      mostraopt();
   }else if(dific=='2'){
      desenhaLinha(34,26,59,0,' ');
      desenhaLinha(34,28,59,0,' ');
      desenhaLinha(34,41,59,0,' ');
      desenhaLinha(34,43,59,0,' ');

      desenhaLinha(34,33,59,0,176);
      desenhaLinha(34,35,59,0,176);
      mostraopt();
   }else if(dific=='3'){
      desenhaLinha(34,26,59,0,' ');
      desenhaLinha(34,28,59,0,' ');
      desenhaLinha(34,33,59,0,' ');
      desenhaLinha(34,35,59,0,' ');

      desenhaLinha(34,41,59,0,176);
      desenhaLinha(34,43,59,0,176);
      mostraopt();
   }else if(dific=='k'){
      dific=confir;
      int num = atoi(&confir);
      separaCartas(num);
      iniciaJogo();
   }else{
      mostraopt();
   }
}

void ouro(int X,int Y){
    gotoxy(X,Y);
    printf("              %c%c              ",4,4);
    gotoxy(X,Y+1);
    printf("             %c%c%c%c             ",4,4,4,4);
    gotoxy(X,Y+2);
    printf("            %c%c%c%c%c%c            ",4,4,4,4,4,4);
    gotoxy(X,Y+3);
    printf("           %c%c%c%c%c%c%c%c           ",4,4,4,4,4,4,4,4);
    gotoxy(X,Y+4);
    printf("          %c%c%c%c%c%c%c%c%c%c          ",4,4,4,4,4,4,4,4,4,4);
    gotoxy(X,Y+5);
    printf("         %c%c%c%c%c%c%c%c%c%c%c%c         ",4,4,4,4,4,4,4,4,4,4,4,4,4,4);
    gotoxy(X,Y+6);
    printf("       %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c       ",4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4);
    gotoxy(X,Y+7);
    printf("      %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c      ",4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4);
    gotoxy(X,Y+8);
    printf("     %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c     ",4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4);
    gotoxy(X,Y+9);
    printf("    %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c    ",4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4);
    gotoxy(X,Y+10);
    printf("   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c   ",4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4);
    gotoxy(X,Y+11);
    printf("    %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c    ",4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4);
    gotoxy(X,Y+12);
    printf("     %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c     ",4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4);
    gotoxy(X,Y+13);
    printf("      %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c      ",4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4);
    gotoxy(X,Y+14);
    printf("       %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c       ",4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4);
    gotoxy(X,Y+15);
    printf("         %c%c%c%c%c%c%c%c%c%c%c%c         ",4,4,4,4,4,4,4,4,4,4,4,4);
    gotoxy(X,Y+16);
    printf("          %c%c%c%c%c%c%c%c%c%c          ",4,4,4,4,4,4,4,4,4,4);
    gotoxy(X,Y+17);
    printf("            %c%c%c%c%c%c            ",4,4,4,4,4,4);
    gotoxy(X,Y+18);
    printf("             %c%c%c%c             ",4,4,4,4);
    gotoxy(X,Y+19);
    printf("              %c%c              ",4,4);
}

void paus(int X,int Y){
    gotoxy(X,Y);
    printf("           %c%c%c%c%c%c             ",5,5,5,5,5,5);
    gotoxy(X,Y+1);
    printf("         %c%c%c%c%c%c%c%c%c%c           ",5,5,5,5,5,5,5,5,5,5);
    gotoxy(X,Y+2);
    printf("        %c%c%c%c%c%c%c%c%c%c%c%c          ",5,5,5,5,5,5,5,5,5,5,5,5);
    gotoxy(X,Y+3);
    printf("       %c%c%c%c%c%c%c%c%c%c%c%c%c%c         ",5,5,5,5,5,5,5,5,5,5,5,5,5,5);
    gotoxy(X,Y+4);
    printf("       %c%c%c%c%c%c%c%c%c%c%c%c%c%c         ",5,5,5,5,5,5,5,5,5,5,5,5,5,5);
    gotoxy(X,Y+5);
    printf("        %c%c%c%c%c%c%c%c%c%c%c%c          ",5,5,5,5,5,5,5,5,5,5,5,5);
    gotoxy(X,Y+6);
    printf("     %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c       ",5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5);
    gotoxy(X,Y+7);
    printf("   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c     ",5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5);
    gotoxy(X,Y+8);
    printf("  %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c    ",5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5);
    gotoxy(X,Y+9);
    printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c   ",5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5);
    gotoxy(X,Y+10);
    printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c   ",5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5);
    gotoxy(X,Y+11);
    printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c   ",5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5);
    gotoxy(X,Y+12);
    printf("  %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c    ",5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5);
    gotoxy(X,Y+13);
    printf("    %c%c%c%c%c%c%c%c %c%c %c%c%c%c%c%c%c%c      ",5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5);
    gotoxy(X,Y+14);
    printf("             %c%c               ",5,5);
    gotoxy(X,Y+15);
    printf("             %c%c               ",5,5);
    gotoxy(X,Y+16);
    printf("            %c%c%c%c              ",5,5,5,5);
}

void espada(int X,int Y){
    gotoxy(X,Y);
    printf("              %c%c              ",6,6);
    gotoxy(X,Y+1);
    printf("             %c%c%c%c%c            ",6,6,6,6,6);
    gotoxy(X,Y+2);
    printf("           %c%c%c%c%c%c%c%c           ",6,6,6,6,6,6,6,6);
    gotoxy(X,Y+3);
    printf("          %c%c%c%c%c%c%c%c%c%c%c         ",6,6,6,6,6,6,6,6,6,6,6);
    gotoxy(X,Y+4);
    printf("        %c%c%c%c%c%c%c%c%c%c%c%c%c%c        ",6,6,6,6,6,6,6,6,6,6,6,6,6,6);
    gotoxy(X,Y+5);
    printf("       %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c       ",6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6);
    gotoxy(X,Y+6);
    printf("      %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c      ",6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6);
    gotoxy(X,Y+7);
    printf("     %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c     ",6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6);
    gotoxy(X,Y+8);
    printf("     %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c     ",6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6);
    gotoxy(X,Y+9);
    printf("    %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c    ",6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6);
    gotoxy(X,Y+10);
    printf("    %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c    ",6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6);
    gotoxy(X,Y+11);
    printf("     %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c    ",6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6);
    gotoxy(X,Y+12);
    printf("      %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c     ",6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6);
    gotoxy(X,Y+13);
    printf("        %c%c%c   %c%c    %c%c%c       ",6,6,6,6,6,6,6,6,6,6);
    gotoxy(X,Y+14);
    printf("              %c%c              ",6,6);
    gotoxy(X,Y+15);
    printf("             %c%c%c%c             ",6,6,6,6);
}

void copas(int X,int Y){
    gotoxy(X,Y);
    printf("      %c%c%c           %c%c%c       ",3,3,3,3,3,3);
    gotoxy(X,Y+1);
    printf("   %c%c%c%c%c%c%c%c%c    %c%c%c%c%c%c%c%c%c     ",3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+2);
    printf("  %c%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c   ",3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+3);
    printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c  ",3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+4);
    printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c  ",3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+5);
    printf(" %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c  ",3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+6);
    printf("  %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c  ",3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+7);
    printf("  %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c   ",3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+8);
    printf("   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c   ",3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+9);
    printf("   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c    ",3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+10);
    printf("    %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c     ",3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+11);
    printf("     %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c      ",3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+12);
    printf("      %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c       ",3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+13);
    printf("       %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c        ",3,3,3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+14);
    printf("        %c%c%c%c%c%c%c%c%c%c%c%c%c         ",3,3,3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+15);
    printf("         %c%c%c%c%c%c%c%c%c%c%c          ",3,3,3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+16);
    printf("          %c%c%c%c%c%c%c%c%c           ",3,3,3,3,3,3,3,3,3);
    gotoxy(X,Y+17);
    printf("           %c%c%c%c%c%c%c            ",3,3,3,3,3,3,3);
    gotoxy(X,Y+18);
    printf("            %c%c%c%c%c             ",3,3,3,3,3);
    gotoxy(X,Y+19);
    printf("             %c%c%c              ",3,3,3);
    gotoxy(X,Y+20);
    printf("              ?               ",3);
}
void telaNivel(){
   system("cls");
   quadro();
   desenhaLogo(40,5);
   ouro(95,5);
   espada(2,5);
   copas(2,29);
   paus(95,33);

   gotoxy(70,5);
   menuNivel(33,17);
   mostraopt();
}
/* TELA */
void telaInicial(){
   system("cls");
   quadro();
   desenhaLogo(40,5);
}

int main(){
    srand(time(NULL));
    telaNivel();
    getch(0);
}
