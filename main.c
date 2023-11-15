#include<stdio.h>
#include <stdlib.h>

int TRANSACAO1 = 1;
int TRANSACAO2 = 2;

int COMMIT                    = 1; // c
int LEITURA                   = 2; // r
int ESCRITA                   = 3; // w
int BLOQUEIO_COMPARTILHADO    = 4; // ls
int BLOQUEIO_EXCLUSIVO        = 5; // lx
int DESBLOQUEIO_EXCLUSIVO     = 6; // ux
int DESBLOQUEIO_COMPARTILHADO = 7; // us

char VARIAVEL_X = 'x';
char VARIAVEL_Y = 'y';
char VARIAVEL_COMMIT = 'c';

int VALOR_VARIAVEL_X = 10;
int VALOR_VARIAVEL_Y = 5;

struct nodo
{
    int operacao;
    char variavel;
    int transacao;
    struct nodo *prox;
};

typedef struct nodo tHistoria;

tHistoria * inicio_HF;
tHistoria * HI;
tHistoria * inicio_bloqueios;

void adicionaPosicaoHF(int operacao, char variavel, int transacao){

    tHistoria *lista_HF;

    lista_HF = inicio_HF;
    tHistoria *nodo;

    nodo = (struct nodo*)malloc(sizeof(tHistoria));
    nodo->operacao = operacao;
    nodo->variavel = variavel;
    nodo->transacao = transacao;
    nodo->prox = NULL;

    if(inicio_HF == NULL){
        inicio_HF = nodo;
    }else{
        while(lista_HF->prox != NULL){
            lista_HF = lista_HF->prox;
        }

        lista_HF->prox = nodo; 
    }
}

void processaRetiradaControleBloqueio(char variavel, int transacao, int operacao){

    tHistoria *anterior, *lista_bloqueios;

    lista_bloqueios = inicio_bloqueios;
    anterior = inicio_bloqueios;

    while (lista_bloqueios != NULL)
    {
        if (lista_bloqueios->operacao == operacao &&
            lista_bloqueios->transacao == transacao &&
            lista_bloqueios->variavel == variavel)
        {
            //Último nodo da lista
            if (lista_bloqueios->prox == NULL){
                anterior->prox = NULL;
                break;
            }
            
            //Primeiro nodo da lista
            if (lista_bloqueios == inicio_bloqueios){
                inicio_bloqueios = inicio_bloqueios->prox;
                lista_bloqueios = inicio_bloqueios;
                break;
            }

            //Nodo no meio da lista
            anterior->prox = lista_bloqueios->prox;
        } else {
            anterior = lista_bloqueios;
            lista_bloqueios = lista_bloqueios->prox;
        }
        
    }
    
}

void processaLiberacaoBloqueio(tHistoria historia){

    int i;
    int posicao_HF;

    tHistoria *lista_bloqueios;
    
    lista_bloqueios = inicio_bloqueios;

    while(lista_bloqueios != NULL){
        if(lista_bloqueios->operacao == BLOQUEIO_EXCLUSIVO &&
           lista_bloqueios->transacao == historia.transacao){
            adicionaPosicaoHF(DESBLOQUEIO_EXCLUSIVO, lista_bloqueios->variavel, lista_bloqueios->transacao);
            processaRetiradaControleBloqueio(lista_bloqueios->variavel, lista_bloqueios->transacao, lista_bloqueios->operacao);
        }

        if(lista_bloqueios->operacao == BLOQUEIO_COMPARTILHADO &&
           lista_bloqueios->transacao == historia.transacao){
            adicionaPosicaoHF(DESBLOQUEIO_COMPARTILHADO, lista_bloqueios->variavel, lista_bloqueios->transacao);
            processaRetiradaControleBloqueio(lista_bloqueios->variavel, lista_bloqueios->transacao, lista_bloqueios->operacao);
        }

        lista_bloqueios = lista_bloqueios->prox;
    }

}

void adicionaPosicaoBloqueio(char variavel, int transacao, int operacao){

    tHistoria *lista_bloqueios;
    tHistoria *anterior;
    tHistoria * nodo;

    nodo = (struct nodo*)malloc(sizeof(tHistoria));
    nodo->operacao = operacao;
    nodo->variavel = variavel;
    nodo->transacao = transacao;
    nodo->prox = NULL;

    lista_bloqueios = inicio_bloqueios;

    if(inicio_bloqueios == NULL){
        inicio_bloqueios = nodo;
    }else{
        while(lista_bloqueios->prox != NULL){
            lista_bloqueios = lista_bloqueios->prox;
        }

        lista_bloqueios->prox = nodo;
    }

}

void enviaOperacaoEscalonador(tHistoria historia){

    int possui_bloqueio;

    tHistoria *lista_bloqueios;
    tHistoria registro_bloqueio;

    lista_bloqueios = inicio_bloqueios;

    possui_bloqueio = 0;    

    while(lista_bloqueios != NULL){

        if(historia.operacao == COMMIT){
            possui_bloqueio = 1;
            break;
        }

        if(historia.transacao == lista_bloqueios->transacao &&
           historia.variavel == lista_bloqueios->variavel){

            if(historia.operacao == LEITURA && lista_bloqueios->operacao == BLOQUEIO_COMPARTILHADO){
                possui_bloqueio = 1;
                break;
            }

            if (historia.operacao == ESCRITA && lista_bloqueios->operacao == BLOQUEIO_EXCLUSIVO)
            {
                possui_bloqueio = 1;
                break;
            }

            lista_bloqueios = lista_bloqueios->prox;
            
        } else {
            possui_bloqueio = 0;
            lista_bloqueios = lista_bloqueios->prox;
        }
    } 

    if (possui_bloqueio == 1){
        adicionaPosicaoHF(historia.operacao, historia.variavel, historia.transacao);

        if(historia.operacao == COMMIT){
            processaLiberacaoBloqueio(historia);
        }
    } else {
        registro_bloqueio.transacao = historia.transacao;
        registro_bloqueio.variavel = historia.variavel;

        if(historia.operacao == LEITURA){
            registro_bloqueio.operacao = BLOQUEIO_COMPARTILHADO;
            adicionaPosicaoBloqueio(historia.variavel, historia.transacao, BLOQUEIO_COMPARTILHADO);
        } else {
            registro_bloqueio.operacao = BLOQUEIO_EXCLUSIVO;
            adicionaPosicaoBloqueio(historia.variavel, historia.transacao, BLOQUEIO_EXCLUSIVO);
        }

        adicionaPosicaoHF(registro_bloqueio.operacao,
                          registro_bloqueio.variavel,
                          registro_bloqueio.transacao);

        adicionaPosicaoHF(historia.operacao, historia.variavel, historia.transacao);

    }
    
}

void mostraHistoriaFinal(){

    tHistoria *lista_HF;

    lista_HF = inicio_HF;

    printf("HF = ");

    while(lista_HF != NULL){

        if(lista_HF != inicio_HF){
            printf(" - ");
        }

        switch (lista_HF->operacao)
        {
        case 1:
            printf("c%d", lista_HF->transacao);
            break;
        case 2:
            printf("r%d[%c]", lista_HF->transacao, lista_HF->variavel);
            break;
        case 3:
            printf("w%d[%c]", lista_HF->transacao, lista_HF->variavel);
            break;
        case 4:
            printf("ls%d[%c]", lista_HF->transacao, lista_HF->variavel);
            break;
        case 5:
            printf("lx%d[%c]", lista_HF->transacao, lista_HF->variavel);
            break;
        case 6:
            printf("ux%d[%c]", lista_HF->transacao, lista_HF->variavel);
            break;
        case 7:
            printf("us%d[%c]", lista_HF->transacao, lista_HF->variavel);
            break;
        
        default:
            break;
        }

        lista_HF = lista_HF->prox;

    }
}

void processaEscalonamentoDados(){

    int i;
    tHistoria *lista_HI = HI;;
    tHistoria historia;

    while(lista_HI != NULL){
        historia.operacao = lista_HI->operacao;
        historia.variavel = lista_HI->variavel;
        historia.transacao = lista_HI->transacao;

        enviaOperacaoEscalonador(historia);

        lista_HI = lista_HI->prox;
    }

    mostraHistoriaFinal();

}

void processaColocacaoNodoListaHI(tHistoria *nodo){

    tHistoria * lista_HI;

    lista_HI = HI;

    if(HI == NULL){
        HI = nodo;
    }else{
        while(lista_HI->prox != NULL){
            lista_HI = lista_HI->prox;
        }
        lista_HI->prox = nodo;
    }

}

void processaCriacaoNodoEnvioAdicaoLista(int operacao, char variavel, int transacao){

    tHistoria * nodo;

    nodo = (struct nodo*)malloc(sizeof(tHistoria));
    nodo->operacao = operacao;
    nodo->variavel = variavel;
    nodo->transacao = transacao;
    nodo->prox = NULL;

    processaColocacaoNodoListaHI(nodo);

}

void processaPopulaDadosTransacoes(){

    // HF =  ls1[x] - r1[x] - ls2[x] - r2[x] - lx1[y] - w1[y] - c1 -
    // ux1[y] - us1[x] - lx2[y] - w2[y] - lx2[x] - w2[x] - c2 - us2[x] - ux2[x] - ux2[y]

    processaCriacaoNodoEnvioAdicaoLista(LEITURA, VARIAVEL_X,      TRANSACAO1);
    processaCriacaoNodoEnvioAdicaoLista(LEITURA, VARIAVEL_X,      TRANSACAO2);
    processaCriacaoNodoEnvioAdicaoLista(ESCRITA, VARIAVEL_Y,      TRANSACAO1);
    processaCriacaoNodoEnvioAdicaoLista(COMMIT,  VARIAVEL_COMMIT, TRANSACAO1);
    processaCriacaoNodoEnvioAdicaoLista(ESCRITA, VARIAVEL_Y,      TRANSACAO2);
    processaCriacaoNodoEnvioAdicaoLista(ESCRITA, VARIAVEL_X,      TRANSACAO2);
    processaCriacaoNodoEnvioAdicaoLista(COMMIT,  VARIAVEL_COMMIT, TRANSACAO2);

}

void inicializaBloqueios(){
    
    inicio_bloqueios = NULL;
    
}

void inicializaHistoriaFinal(){

    inicio_HF = NULL;

}

void inicializaHistoriaInicial(){

    HI = NULL;

}

int main(){

    inicializaBloqueios();
    inicializaHistoriaFinal();
    inicializaHistoriaInicial();
    processaPopulaDadosTransacoes();
    processaEscalonamentoDados();
    
    return 0;

}