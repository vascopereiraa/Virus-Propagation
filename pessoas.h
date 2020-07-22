
/* 
 * File:   pessoas.h
 * Author: Vasco Daniel Matos Pereira
 * Nr: 2019134744
 * 
 * Created on 10 de maio de 2020, 21:05
 */

#ifndef PESSOAS_H
#define PESSOAS_H

// Estrutura para a Lista Ligada de Pessoas
typedef struct pessoas pessoas_l, *ppessoas;
struct pessoas {
    char id[51];
    int idade;
    char estado;
    int tempo;
    int local;
    pessoas_l *next;
};

void imprimePessoas(pessoas_l* head);
int verificaIdade(pessoas_l* head);
int verificaNomeInit(pessoas_l* head, int nPes);
int verificaEstado(pessoas_l* head);
pessoas_l* carregaPessoas(pessoas_l* head, char fname[], int *nPes);
void apagaLista(ppessoas ptr);
ppessoas apagaPessoaLocal(ppessoas ptr, int a, int* dif);
int distribuiPessoas(plocal espaco, ppessoas head, int nEsp, int* capacidade);
void imprimeDistribuicao(ppessoas head);


#endif /* PESSOAS_H */
