
/* 
 * File:   structs.h
 * Author: Vasco Daniel Matos Pereira
 * Nr: 2019134744
 * 
 * Created on 6 de abril de 2020, 17:28
 */

#ifndef ESPACOS_H
#define ESPACOS_H

// Estrutura para os Espaços
typedef struct sala local, *plocal;
struct sala {
    int id;             // id numerico do local
    int capacidade;     // capacidade maxima
    int liga[3];         // id das ligações (-1 nos casos não usados)
};

// Declaracao das funcoes relativas aos espacos em init.c
void imprimeEspacos (local *espaco, int nEsp);
local* carregaEspaco (local espaco[], int* nEsp, char* fname);
int verificaId (local espaco[], int nEsp);
int verificaLigacao (local espaco[], int nEsp);
void carregaCapacidade(int* capacidade, int nEsp, plocal espaco);

#endif /* ESPACOS_H */

