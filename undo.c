
/* 
 * File:   undo.c
 * Author: Vasco Daniel Matos Pereira
 * Nr: 2019134744
 * 
 * Created on 21 de maio de 2020, 20:28
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "utils.h"
#include "espacos.h"
#include "pessoas.h"
#include "sim.h"
#include "undo.h"
#include "term.h"

pundo carregaIteracao(pundo lista, ppessoas head, const int* nPes) {
    
    pundo new = NULL;
    pundo aux = NULL;

    // Criacao do novo node
    new = malloc(sizeof(undo));
    if(new == NULL) {
        printf("Erro ao gravar dados na lista de recuperacao");
        return lista;
    }

    // Carregar dados na lista de recuperacao
    new->nPes = *nPes;
    new->lista = gravaDados(head);
    new->next = NULL;

    // Inserir node new na lista ligada
    if(lista == NULL)
        return new;
    else {
        aux = lista;
        while(aux->next != NULL)
            aux = aux->next;
        aux->next = new;
    }
    
    return lista;  
}

// Imprime a lista de listas para recuperar
void imprimeListaRecuperacao(pundo lista) {
    
    int c = 1;
    while(lista != NULL) {
        printf("%da iteracao na recuperacao\n", c++);
        printf("Numero de pessoas guardadas: %d\n", lista->nPes);
        imprimePessoas(lista->lista);
        lista = lista->next;         
        printf("\n"); 
    } 
}

ppessoas gravaDados(ppessoas head) {
    
    ppessoas new = NULL, init = NULL, aux = NULL;
    
    while(head != NULL) {
        
        // Alocar espaco para o node de pessoas a recuperar
        new = malloc(sizeof(pessoas_l));
        if(new == NULL)
            return init;
        
        // Guardar dados
        strcpy(new->id, head->id);
        new->idade = head->idade;
        new->estado = head->estado;
        new->tempo = head->tempo;
        new->local = head->local;
        new->next = NULL;
        
        // Adicionar pessoa a lista de recuperacao
        if(init == NULL)
            init = new;
        else {
            aux = init;
            while(aux->next != NULL)
                aux = aux->next;
            aux->next = new;
        }
        
        head = head->next;
        
    }
    return init;
}

pundo controlo(pundo lista) {

    pundo init = lista;
    int c = 0;

    // Verificar se temos < 3 nodes
    while (c <= 3) {
        if (init == NULL)
            return lista;
        init = init->next;
        c++;
    }

    // Se houver 4 nodes -> Liberta F.I. - ultimo node
    init = lista->next;
    apagaLista(lista->lista);
    free(lista);
    
    return init;
}

ppessoas recuperaLista(pundo lista, ppessoas head, int* nPes, int* flag) {
    
    pundo aux = lista;
    ppessoas ptr = NULL;
    int n, count = 0;
    
    *flag = 0;
    
    // Verificar quantas iteracoes estao guardadas
    while(aux != NULL) {
        count++;
        aux = aux->next;
    } 
    
    if(count == 0) {
        printf("\nNão existem iteracoes para recuperar!\n");
        return head;
    }
    
    // Perguntar que iteracao pretende recuperar
    printf("\nPode recuperar %d iteracoes\n", count);
    printf("0 - CANCELAR\n");
    do {
        printf("Que iteracao pretende recuperar: ");
        scanf("%d", &n);
    } while(n > count || n < 0);

    if(n == 0) {
        printf("\n - Nao foi carregada nenhuma iteracao! - \n");
        return head;
    }
    
    // Carrega os dados da iteracao anterior
    aux = lista;
    count = 1;
    while(count != n) {
        count++;
        aux = aux->next;
    }
    
    ptr = aux->lista;
    nPes = &aux->nPes;
    printf("\nNumero de pessoas recuperadas: %d\n", *nPes);
    imprimePessoas(ptr);
    
    apagaLista(head);
    head = gravaDados(ptr);
    
    *flag = 1;
    return head;
}

int* recuperaCapacidade(ppessoas head, plocal espaco, const int* nEsp, int *capacidade) {
    
    ppessoas aux = head;
    int i;
    
    carregaCapacidade(capacidade, *nEsp, espaco);
    
    for(i = 0; i < *nEsp; i++) {
        aux = head;
        while(aux != NULL) {
            if(aux->local == espaco[i].id)
                capacidade[i]--;
            aux = aux->next;
        }
    }
    
    /* teste */
/*
    for(int k = 0; k < *nEsp; k++)
        printf("\t%d", capacidade[k]);
    printf("\n\n");
*/
    
    return capacidade;
    
}

void apagaListaInterna(pundo lista) {
    
    while(lista != NULL) {
        apagaLista(lista->lista);
        lista = lista->next;
    }
    
}

void apagaListaRecuperacao(pundo lista) {
    
    pundo aux = NULL;
    
    while(lista != NULL) {
        aux = lista;
        lista = lista->next;
        free(aux);
    }
    
}