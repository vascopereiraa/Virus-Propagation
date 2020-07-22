
/* 
 * File:   init.c
 * Author: Vasco Daniel Matos Pereira
 * Nr: 2019134744
 * 
 * Created on 6 de abril de 2020, 22:51
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

/*
 *  FOLHA DE CODIGO PARA A INCIALIZAÇÃO DO PROGRAMA
 */

void imprimeEspacos(local *espaco, int nEsp) {
    int i;

    for (i = 0; i < nEsp; i++) {
        printf("ID: %d\t", espaco[i].id);
        printf("Capacidade: %d\t", espaco[i].capacidade);
        printf("Ligacoes: ");
        printf("%d", espaco[i].liga[0]);
        printf(" %d", espaco[i].liga[1]);
        printf(" %d\n", espaco[i].liga[2]);
    }
    printf("\n");
}

local* carregaEspaco(local espaco[], int* nEsp, char* fname) {

    FILE* fpEspaco = NULL;
    local aux, *pArray = NULL;

    // Abrir o ficheiro binário
    fpEspaco = fopen(fname, "rb");
    if (fpEspaco == NULL) {
        printf("Erro ao abrir o ficheiro\n");
        return espaco;
    }

    // Ler os dados do ficheiro
    fread(&aux, sizeof (local), 1, fpEspaco);
    while (!feof(fpEspaco)) {

        // Realocar espaco para o array dos Espacos
        pArray = realloc(espaco, sizeof (local) * ((*nEsp) + 1));
        if (pArray == NULL) {
            printf("Erro na realocacao de espaco para o array dos Espacos\n");
            fclose(fpEspaco);
            return espaco;
        }

        espaco = pArray; // Passar a nova pos de mem. para o array de espacos

        // Guardar os valores obtidos no array
        espaco[(*nEsp)++] = aux;
        fread(&aux, sizeof (local), 1, fpEspaco);
    }

    // Fechar o ficheiro
    fclose(fpEspaco);

    return espaco;
}

int verificaId(local espaco[], int nEsp) {

    int i, j;

    /* Verificador de ids -> Tem de ser positivos e unicos */
    for (i = 0; i < nEsp; i++) {

        if (espaco[i].id <= 0) {
            printf("O id e negativo ou zero! TERMINAR EXECUCAO\n");
            return 0;
        }

        for (j = 0; j < nEsp; j++) {
            if (j != i) {
                if (espaco[j].id == espaco[i].id) {
                    printf("O id nao e unico! TERMINAR EXECUCAO\n");
                    return 0;
                }
            }
        }
    }

    //    puts("ESPACOS - IDS CORRETOS!");
    return 1;
}

int verificaLigacao(local espaco[], int nEsp) {

    int i, j, k, l;
    int id_teste, aux, id_origem, pos_teste;

    // Verificador de ligacoes -> Tem de ser reciprocas
    for (i = 0; i < nEsp; i++) {
        for (j = 0; j < 3; j++) {
            if (espaco[i].liga[j] != -1) {
                id_teste = espaco[i].liga[j];
                id_origem = espaco[i].id;

                for (l = 0; l < nEsp; l++) {
                    if (espaco[l].id == id_teste) {
                        pos_teste = l;
                        aux = 0;
                        for (k = 0; k < 3; k++) {
                            if (espaco[pos_teste].liga[k] == id_origem) {
                                aux++;
                            }
                            if (k == 2 && aux != 1) {
                                printf("Erro na ligacao! TERMINAR EXECUCAO\n");
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }

    //    puts("ESPACOS - LIGACOES CORRETAS");
    return 1;
}

void carregaCapacidade(int* capacidade, int nEsp, plocal espaco) {

    int i;

    for (i = 0; i < nEsp; i++) {
        capacidade[i] = espaco[i].capacidade;
    }
}

/* Codigo referente ao tratamento da Lista Ligada de Pessoas */

void imprimePessoas(pessoas_l* head) {

    pessoas_l* aux = head;

    if (aux == NULL)
        printf("Lista vazia");
    else
        while (aux != NULL) {
            printf("Id: %s\t    Idade: %d\tEstado: %c\t"
                    "Local: %d\t", aux->id, aux->idade, aux->estado, aux->local);
            if (aux->estado == 'D')
                printf("Tempo: %d\n", aux->tempo);
            else
                printf("\n");
            aux = aux->next;
        }
}

int verificaNomeInit(pessoas_l* head, int nPes) {
    pessoas_l* aux = head;
    pessoas_l* ptr = NULL;
    int i, j;
    char nome[51];

    for (i = 0; i < nPes - 1; i++) {
        strcpy(nome, aux->id);
        ptr = aux->next;
        for (j = i + 1; j < nPes; j++) {
            if (strcmp(ptr->id, nome) == 0) {
                puts("Pessoa com ID repetido! TERMINAR EXECUCAO");
                return 0;
            }
            ptr = ptr->next;
        }
        aux = aux->next;
    }

//    puts("PESSOAS - IDS CORRETOS");
    return 1;
}

int verificaIdade(pessoas_l* head) {
    pessoas_l* aux = head;

    do {
        if (aux->idade <= 0) {
            puts("Pessoa com erro na idade! TERMINAR EXECUCAO");
            return 0;
        }
        aux = aux->next;
    } while (aux != NULL);

    //    puts("PESSOAS - IDADES CORRETAS");
    return 1;
}

int verificaEstado(pessoas_l* head) {
    pessoas_l* aux = head;

    do {
        if (aux->estado != 'D' && aux->estado != 'I' && aux->estado != 'S') {
            puts("Pessoa com estado invalido! TERMINAR EXECUCAO");
            return 0;
        }
        if (aux->estado == 'D')
            if (aux->tempo <= 0) {
                puts("Pessoa com tempo de infetado invalido! TERMINAR EXECUCAO");
                return 0;
            }
        aux = aux->next;
    } while (aux != NULL);

    //    puts("PESSOAS - ESTADOS CORRETOS");
    return 1;
}

pessoas_l* carregaPessoas(pessoas_l* head, char fname[], int *nPes) {

    FILE* fp = NULL;
    pessoas_l* novo = NULL, *aux = NULL;
    char nome[51];

    // Abrir ficheiro de texto para leitura
    fp = fopen(fname, "r");
    if (fp == NULL)
        return head;

    // Ver se existem pessoas por ler
    while (fscanf(fp, "%s", nome) == 1) {

        //Alocar espaço para um node
        novo = malloc(sizeof (pessoas_l));
        if (novo == NULL) {
            printf("\nErro ao alocar espaco para um novo node\n");
            fclose(fp);
            return head;
        }
            

        // Guardar o id da pessoa e acabar de ler os dados
        strcpy(novo->id, nome);
        fscanf(fp, " %d %c", &novo->idade, &novo->estado);
        if (novo->estado == 'D')
            fscanf(fp, " %d", &novo->tempo);
        else
            novo->tempo = 0;
        novo->local = -1;
        novo->next = NULL; // Coloca o seguinte node a null

        // Guardar o node na lista ligada
        if (head == NULL)
            head = novo;
        else {
            aux = head;
            while (aux->next != NULL)
                aux = aux->next;
            aux->next = novo;
        }

        (*nPes)++; // Aumenta o contador do numero de pessoas
    }

    fclose(fp);
    return head;
}

void apagaLista(ppessoas ptr) {
    ppessoas aux = NULL;

    while (ptr != NULL) {
        aux = ptr;
        ptr = ptr->next;
        free(aux);
    }
}

ppessoas apagaPessoaLocal(ppessoas ptr, int a, int* dif) {

    ppessoas atual = ptr, ant = NULL;
    
    while(atual != NULL && atual->local != a){
        ant = atual;
        atual = atual->next;
    }
    // Pessoa nao existe
    if(atual == NULL)               
        return ptr;
    
    // Primeiroa pessoa da lista
    if(atual == ptr)                  
        ptr = ptr->next;
    else                        // Restantes pessoas - Caso Geral                        
        ant->next = atual->next;
    
    free(atual);
    (*dif)++;
    
    return ptr;
}

int distribuiPessoas(plocal espaco, ppessoas head, int nEsp, int* capacidade) {

    int i, count, pos;
    ppessoas aux = head;

    while (aux != NULL) {

        // Verificar se há capacidade
        count = 0;
        for (i = 0; i < nEsp; i++)
            if (capacidade[i] == 0)
                count++;

        // Se houver cap continua
        if (count != nEsp) {

            // Gerar aleat uma pos para a pessoa
            do {
                pos = intUniformRnd(0, nEsp - 1);
            } while (capacidade[pos] == 0);

            aux->local = espaco[pos].id;
            capacidade[pos]--;
        }

        aux = aux->next;
    }

    return 1;
}

void imprimeDistribuicao(ppessoas head) {

    printf("\n");
    while (head != NULL) {
        printf("%s\t%d\n", head->id, head->local);
        head = head->next;
    }
}