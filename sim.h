
/* 
 * File:   sim.h
 * Author: Vasco Daniel Matos Pereira
 * Nr: 2019134744
 * 
 * Created on 19 de maio de 2020, 12:04
 */

#ifndef SIM_H
#define SIM_H

#include "undo.h"

ppessoas preenchePessoa(ppessoas new, ppessoas head);
int verificaNomeNovo(ppessoas head, char* nome);
ppessoas adicionaPessoa(ppessoas head, int* nPes, int* capacidade, plocal espaco, const int* nEsp);
void imprimeEstatistica(plocal espaco, ppessoas head, const int* nEsp, const int* nPes, int* capacidade, FILE* fname);
ppessoas transferirPessoas(ppessoas head, plocal espaco, const int* nPes, const int* nEsp, int* capacidade);
float taxaDisseminacao(int num);
float probabilidadeRecuperacao(int idade);
int duracaoInfecao(int idade);
ppessoas avancaSimulacao(ppessoas head, plocal espaco, int* nPes, int* nEsp, int* capacidade);
int menu();
ppessoas simulacao(plocal espaco, ppessoas head, pundo lista, int* nEsp, int* nPes, int* capacidade);

#endif /* SIM_H */
