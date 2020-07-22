
/* 
 * File:   undo.h
 * Author: Vasco Daniel Matos Pereira
 * Nr: 2019134744
 * 
 * Created on 21 de maio de 2020, 20:25
 */

#ifndef UNDO_H
#define UNDO_H

// Estrutura para a recuperacao de iteracoes
typedef struct rec  undo, *pundo;
struct rec {
    int nPes;
    ppessoas lista;
    pundo next;
};

pundo carregaIteracao(pundo lista, ppessoas head, const int* nPes);
void imprimeListaRecuperacao(pundo lista);
ppessoas gravaDados(ppessoas head);
pundo controlo(pundo lista);
ppessoas recuperaLista(pundo lista, ppessoas head, int* nPes, int* flag);
int* recuperaCapacidade(ppessoas head, plocal espaco, const int* nEsp, int *capacidade);
void apagaListaInterna(pundo lista);
void apagaListaRecuperacao(pundo lista);

#endif /* UNDO_H */

