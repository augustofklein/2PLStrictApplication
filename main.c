#include<stdio.h>

int TRANSACAO1 = 1;
int TRANSACAO2 = 2;

int COMMIT                      = 1;  // c
int LEITURA_X                   = 2;  // r[x]
int LEITURA_Y                   = 3;  // r[y]
int ESCRITA_X                   = 4;  // w[x]
int ESCRITA_Y                   = 5;  // w[y]
int BLOQUEIO_COMPARTILHADO_X    = 6;  // ls[x] 
int BLOQUEIO_COMPARTILHADO_Y    = 7;  // ls[y]
int BLOQUEIO_EXCLUSIVO_X        = 8;  // lx[x]
int BLOQUEIO_EXCLUSIVO_Y        = 9;  // lx[y]
int DESBLOQUEIO_EXCLUSIVO_X     = 10; // ux[x]
int DESBLOQUEIO_EXCLUSIVO_Y     = 11; // ux[y]
int DESBLOQUEIO_COMPARTILHADO_X = 12; // us[x]
int DESBLOQUEIO_COMPARTILHADO_Y = 13; // us[y]

typedef struct historia
{
    int operacao;
    int transacao;
} tHistoria;


int main(){

    // HI = ls[x] r[x] ls[x] r[x] lx[y] w[y] c ux[y] us[x] lx[y] w[y] lx[x] w[x] c ux[x] ux[y]
    tHistoria historia[] = {{BLOQUEIO_COMPARTILHADO_X,    TRANSACAO1},
                            {LEITURA_X,                   TRANSACAO1}, 
                            {BLOQUEIO_COMPARTILHADO_X,    TRANSACAO2},
                            {LEITURA_X,                   TRANSACAO2},
                            {BLOQUEIO_EXCLUSIVO_Y,        TRANSACAO1},
                            {ESCRITA_Y,                   TRANSACAO1},
                            {COMMIT,                      TRANSACAO1},
                            {DESBLOQUEIO_EXCLUSIVO_Y,     TRANSACAO1},
                            {DESBLOQUEIO_COMPARTILHADO_X, TRANSACAO1},
                            {BLOQUEIO_EXCLUSIVO_Y,        TRANSACAO2},
                            {ESCRITA_Y,                   TRANSACAO2},
                            {BLOQUEIO_EXCLUSIVO_X,        TRANSACAO2},
                            {ESCRITA_X,                   TRANSACAO2},
                            {COMMIT,                      TRANSACAO2},
                            {DESBLOQUEIO_EXCLUSIVO_X,     TRANSACAO2},
                            {DESBLOQUEIO_EXCLUSIVO_Y,     TRANSACAO2}};
    
    return 0;

}