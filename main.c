#include<stdio.h>

int QTD_HISTORIA = 16;

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

typedef struct historia
{
    int operacao;
    char variavel;
    int transacao;
} tHistoria;

tHistoria HF[16];

void enviaOperacaoEscalonador(tHistoria historia){

}

void processaEscalonamentoDados(tHistoria HI[QTD_HISTORIA]){

    int i;

    for(i=0;i<QTD_HISTORIA;i++){
        enviaOperacaoEscalonador(HI[i]);
    }

}

int main(){

    // HI = ls[x] r[x] ls[x] r[x] lx[y] w[y] c ux[y] us[x] lx[y] w[y] lx[x] w[x] c ux[x] ux[y]
    tHistoria HI[] = {{BLOQUEIO_COMPARTILHADO,    VARIAVEL_X, TRANSACAO1},
                      {LEITURA,                   VARIAVEL_X, TRANSACAO1}, 
                      {BLOQUEIO_COMPARTILHADO,    VARIAVEL_X, TRANSACAO2},
                      {LEITURA,                   VARIAVEL_X, TRANSACAO2},
                      {BLOQUEIO_EXCLUSIVO,        VARIAVEL_Y, TRANSACAO1},
                      {ESCRITA,                   VARIAVEL_Y, TRANSACAO1},
                      {COMMIT,                    'c',        TRANSACAO1},
                      {DESBLOQUEIO_EXCLUSIVO,     VARIAVEL_Y, TRANSACAO1},
                      {DESBLOQUEIO_COMPARTILHADO, VARIAVEL_X, TRANSACAO1},
                      {BLOQUEIO_EXCLUSIVO,        VARIAVEL_Y, TRANSACAO2},
                      {ESCRITA,                   VARIAVEL_Y, TRANSACAO2},
                      {BLOQUEIO_EXCLUSIVO,        VARIAVEL_X, TRANSACAO2},
                      {ESCRITA,                   VARIAVEL_X, TRANSACAO2},
                      {COMMIT,                    'c',        TRANSACAO2},
                      {DESBLOQUEIO_EXCLUSIVO,     VARIAVEL_X, TRANSACAO2},
                      {DESBLOQUEIO_EXCLUSIVO,     VARIAVEL_Y, TRANSACAO2}};

    processaEscalonamentoDados(HI);
    
    return 0;

}