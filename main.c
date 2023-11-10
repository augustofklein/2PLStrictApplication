#include<stdio.h>
typedef struct historia
{
    char operacao[10];
    int transacao;
} tHistoria;


int main(){

    tHistoria historia[] = {{'ls[x]',1},
                            {'r[x]',1}, 
                            {'ls[x]',2},
                            {'r[x]',2},
                            {'lx[y]',1},
                            {'w[y]',1},
                            {'c',1},
                            {'ux[y]',1},
                            {'us[x]',1},
                            {'lx[y]',2},
                            {'w[y]',2},
                            {'lx[x]',2},
                            {'w[x]',2},
                            {'c',2},
                            {'ux[x]',2},
                            {'ux[y]',2}};
    
    return 0;

}