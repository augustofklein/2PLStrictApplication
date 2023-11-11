#include<stdio.h>

int TRANSACAO1 = 1;
int TRANSACAO2 = 2;

typedef struct historia
{
    char operacao[10];
    int transacao;
} tHistoria;


int main(){

    tHistoria historia[] = {{'ls[x]',TRANSACAO1},
                            {'r[x]',TRANSACAO1}, 
                            {'ls[x]',TRANSACAO2},
                            {'r[x]',TRANSACAO2},
                            {'lx[y]',TRANSACAO1},
                            {'w[y]',TRANSACAO1},
                            {'c',TRANSACAO1},
                            {'ux[y]',TRANSACAO1},
                            {'us[x]',TRANSACAO1},
                            {'lx[y]',TRANSACAO2},
                            {'w[y]',TRANSACAO2},
                            {'lx[x]',TRANSACAO2},
                            {'w[x]',TRANSACAO2},
                            {'c',TRANSACAO2},
                            {'ux[x]',TRANSACAO2},
                            {'ux[y]',TRANSACAO2}};
    
    return 0;

}