#include<stdio.h>

int QTD_REGISTROS = 50;

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

int VALOR_VARIAVEL_X = 10;
int VALOR_VARIAVEL_Y = 5;

typedef struct historia
{
    int operacao;
    char variavel;
    int transacao;
} tHistoria;

tHistoria HF[16];
tHistoria controle_bloqueios[50];

void enviaOperacaoEscalonador(tHistoria historia){

    int i, possui_bloqueio;
    int posicao_HF, posicao_bloqueio;

    possui_bloqueio = 0;

    for(i=0;i<QTD_REGISTROS;i++){

        if(historia.transacao == controle_bloqueios[i].transacao &&
           historia.variavel == controle_bloqueios[i].variavel){

            if(historia.operacao == LEITURA && controle_bloqueios[i].operacao == BLOQUEIO_COMPARTILHADO){
                possui_bloqueio = 1;
                break;
            }

            if (historia.operacao == ESCRITA && controle_bloqueios[i].operacao == BLOQUEIO_EXCLUSIVO)
            {
                possui_bloqueio = 1;
                break;
            }
            
        } else {
            possui_bloqueio = 0;
        }
    } 

    if (possui_bloqueio == 1)
    {
        //só escreve na HF a operação
    } else {
        posicao_bloqueio = retornaProximaPosicaoLivre(controle_bloqueios);
        controle_bloqueios[posicao_bloqueio].variavel = historia.variavel;
        controle_bloqueios[posicao_bloqueio].transacao = historia.transacao;

        if(historia.operacao == LEITURA){
            controle_bloqueios[posicao_bloqueio].operacao = BLOQUEIO_COMPARTILHADO;
        } else {
            controle_bloqueios[posicao_bloqueio].operacao = BLOQUEIO_EXCLUSIVO;
        }

        posicao_HF = retornaProximaPosicaoLivre(HF);
        HF[posicao_HF].operacao = controle_bloqueios[posicao_bloqueio].operacao;
        HF[posicao_HF].transacao = controle_bloqueios[posicao_bloqueio].transacao;
        HF[posicao_HF].variavel = controle_bloqueios[posicao_bloqueio].variavel;

        posicao_HF = retornaProximaPosicaoLivre(HF);
        HF[posicao_HF].operacao = historia.operacao;
        HF[posicao_HF].transacao = historia.transacao;
        HF[posicao_HF].variavel = historia.variavel;

    }
    
}

void mostraHistoriaFinal(){

    int i;

    printf("HF = ");

    for(i=0;i<QTD_REGISTROS;i++){

        if(i != 0 && HF[i].operacao != NULL){
            printf(" - ");
        }

        switch (HF[i].operacao)
        {
        case 1:
            printf("c%d", HF[i].transacao);
            break;
        case 2:
            printf("r%d[%c]", HF[i].transacao, HF[i].variavel);
            break;
        case 3:
            printf("w%d[%c]", HF[i].transacao, HF[i].variavel);
            break;
        case 4:
            printf("ls%d[%c]", HF[i].transacao, HF[i].variavel);
            break;
        case 5:
            printf("lx%d[%c]", HF[i].transacao, HF[i].variavel);
            break;
        case 6:
            printf("ux%d[%c]", HF[i].transacao, HF[i].variavel);
            break;
        case 7:
            printf("us%d[%c]", HF[i].transacao, HF[i].variavel);
            break;
        
        default:
            break;
        }


    }
}

void processaEscalonamentoDados(tHistoria HI[]){

    int i;

    for(i=0;i<QTD_REGISTROS;i++){
        enviaOperacaoEscalonador(HI[i]);
    }

    mostraHistoriaFinal();

}

int retornaProximaPosicaoLivre(tHistoria lista[]){

    int i;

    for(i=0;i<QTD_REGISTROS;i++){

        if (lista[i].operacao == NULL &&
            lista[i].transacao == NULL &&
            lista[i].variavel == ' ')
        {
            return i;
        }
        
    }
}

void inicializaBloqueios(){
    
    int i;

    for (i=0;i<QTD_REGISTROS;i++)
    {
        controle_bloqueios[i].operacao = NULL;
        controle_bloqueios[i].transacao = NULL;
        controle_bloqueios[i].variavel = ' ';
    }
    
}

void inicializaHistoriaFinal(){

    int i;

    for (i=0;i<QTD_REGISTROS;i++)
    {
        HF[i].operacao = NULL;
        HF[i].transacao = NULL;
        HF[i].variavel = ' ';
    }
}

int main(){

    // HF =  ls1[x] - r1[x] - ls2[x] - r2[x] - lx1[y] - w1[y] - c1 - ux1[y] - us1[x] - lx2[y] - w2[y] - lx2[x] - w2[x] - c2 - ux2[x] - ux2[y]
    tHistoria HI[] = {{LEITURA,                   VARIAVEL_X, TRANSACAO1}, 
                      {LEITURA,                   VARIAVEL_X, TRANSACAO2},
                      {ESCRITA,                   VARIAVEL_Y, TRANSACAO1},
                      {COMMIT,                    'c',        TRANSACAO1},
                      {ESCRITA,                   VARIAVEL_Y, TRANSACAO2},
                      {ESCRITA,                   VARIAVEL_X, TRANSACAO2},
                      {COMMIT,                    'c',        TRANSACAO2}};

    inicializaBloqueios();
    inicializaHistoriaFinal();
    processaEscalonamentoDados(HI);
    
    return 0;

}