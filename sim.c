
/* 
 * File:   sim.c
 * Author: Vasco Daniel Matos Pereira
 * Nr: 2019134744
 * 
 * Created on 19 de maio de 2020, 12:04
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

/* FICHEIRO DA SIMULACAO DA PROPAGACAO DE VIRUS */

ppessoas preenchePessoa(ppessoas new, ppessoas head) {

    // Obtem dados
    do {
        printf("\nIdentificador: ");
        scanf("%50s", new->id);
    } while (verificaNomeNovo(head, new->id) == 0 && strlen(new->id) > 50);

    do {
        printf("Idade: ");
        scanf("%d", &new->idade);
    } while (new->idade <= 0);

    new->estado = 'D';

    do {
        printf("Dias: ");
        scanf("%d", &new->tempo);
    } while (new->tempo <= 0);

    return new;
}

int verificaNomeNovo(ppessoas head, char* nome) {

    while (head != NULL) {
        if (!strcmp(head->id, nome)) {
            puts("Pessoa com ID repetido! Introduza um novo nome");
            return 0;
        }
        head = head->next;
    }

    return 1;
}

ppessoas adicionaPessoa(ppessoas head, int* nPes, int* capacidade, plocal espaco, const int* nEsp) {

    ppessoas new = NULL, aux;
    int flag, count, i, pos;

    // Verifica se ha capacidade no espaco
    count = 0;
    for (i = 0; i < *nEsp; i++)
        if (capacidade[i] == 0)
            count++;

    if (count == *nEsp) {
        puts("Espaco cheio! Impossivel acrescentar novo doente");
        return head;
    }

    // Alocar espaco para a nova pessoa
    new = malloc(sizeof (pessoas_l));
    if (new == NULL)
        return head;

    // Preencher new pessoa, já com verificacoes
    new = preenchePessoa(new, head);
    new->next = NULL;
    (*nPes)++;

    printf("\nLocais com espacos disponiveis: ");
    for (i = 0; i < *nEsp; i++)
        if (capacidade[i] > 0)
            printf("%d ", espaco[i].id);
    printf("\n");

    // Pedir e atribuir o local
    do {
        printf("Introduza o local que pretende adicionar o doente: ");
        scanf("%d", &new->local);

        flag = 0;
        for (i = 0; i < *nEsp; i++)
            if (espaco[i].id == new->local)
                if (capacidade[i] > 0) {
                    flag = 1;
                    pos = i;
                }

    } while (flag == 0);
    capacidade[pos]--;

    // Introduzir pessoa na lista ligada
    if (head == NULL) // Caso lista vazia
        head = new;

    else { // Caso contrario - Insere no final
        aux = head;
        while (aux->next != NULL)
            aux = aux->next;
        aux->next = new;
    }

    // Teste
    /*
    printf("%s %d %c %d %d\n", new->id, new->idade, new->estado, new->tempo, new->local);
    for(int i = 0; i<*nEsp; i++)
        printf("%d\t", capacidade[i]);
     */

    puts("Doente adicionado com sucesso!");
    return head;
}

void imprimeEstatistica(plocal espaco, ppessoas head, const int* nEsp, const int* nPes, int* capacidade, FILE* fname) {

    int j, aux, flag = 0, ocp = 0;
    int i = 0, s = 0, d = 0;
    int i1 = 0, s1 = 0, d1 = 0;
    float ti = 0, ts = 0, td = 0;
    float ti1 = 0, ts1 = 0, td1 = 0;
    ppessoas ptr = NULL;

    // Atualizar a lista de ocupacao e mostrar distribuicao por sala
    for (j = 0; j < *nEsp; j++) {

        s1 = 0, d1 = 0, i1 = 0;
        ts1 = 0, td1 = 0, ti1 = 0;
        ocp = espaco[j].capacidade - capacidade[j];
        ptr = head;
        aux = espaco[j].id;
        fprintf(fname, "\nID: %d\tCapacidade: %d\tOcupacao: %d", espaco[j].id, espaco[j].capacidade, ocp);
        if(capacidade[j] == 0)
            fprintf(fname, "  -> SALA CHEIA!");
        fprintf(fname, "\nPessoas na sala: \n");

        while (ptr != NULL) {

            if (ptr->local == aux) {
                fprintf(fname, "Nome: %s\tIdade: %d\tEstado: %c\t", ptr->id, ptr->idade, ptr->estado);
                if(ptr->estado == 'D')
                    fprintf(fname, "Tempo: %d", ptr->tempo);
                fprintf(fname, "\n");
                switch (ptr->estado) {
                    case 'S':
                        s1++;
                        break;
                    case 'D':
                        d1++;
                        break;
                    case 'I':
                        i1++;
                        break;
                }
            }

            if (flag == 0)
                switch (ptr->estado) {
                    case 'S':
                        s++;
                        break;
                    case 'D':
                        d++;
                        break;
                    case 'I':
                        i++;
                        break;
                }
            ptr = ptr->next;
        }

        flag = 1;
        // TAXAS INDIVIDUAIS DE CADA ESPACO
        ti1 = (i1 / (float) ocp) * 100;
        ts1 = (s1 / (float) ocp) * 100;
        td1 = (d1 / (float) ocp) * 100;

        fprintf(fname, "\nNumero de IMUNES: %d\tTaxa de IMUNES: %.2f%%\n", i1, ti1);
        fprintf(fname, "Numero de SAUDAVEIS: %d\tTaxa de SAUDAVEIS: %.2f%%\n", s1, ts1);
        fprintf(fname, "Numero de DOENTES: %d\tTaxa de DOENTES: %.2f%%\n\n", d1, td1);
    }

    // TAXAS GLOBAIS
    ti = (i / (float) (*nPes)) * 100;
    ts = (s / (float) (*nPes)) * 100;
    td = (d / (float) (*nPes)) * 100;

    fprintf(fname, "\n");
    fprintf(fname, " - TAXA GLOBAL DO ESPACO - ");
    fprintf(fname, "\nNumero de pessoas no espaco: %d\n", *nPes);
    fprintf(fname, "Numero de IMUNES: %d\tTaxa de IMUNES: %.2f%%\n", i, ti);
    fprintf(fname, "Numero de SAUDAVEIS: %d\tTaxa de SAUDAVEIS: %.2f%%\n", s, ts);
    fprintf(fname, "Numero de DOENTES: %d\tTaxa de DOENTES: %.2f%%\n\n", d, td);
}

ppessoas transferirPessoas(ppessoas head, plocal espaco, const int* nPes, const int* nEsp, int* capacidade) {

    int or, dest, num, n;
    int flag = 0, posO = 0, posD = 0, max;
    int i, j, k;
    int random, c;
    ppessoas aux;

    // Verificacao dos parametros de entrada
    printf("Ids de origem possiveis: ");
    for (j = 0; j < *nEsp; j++)
        printf("%d\t", espaco[j].id);
    printf("//\t0 - CANCELAR\n");
    
    do {
        printf("Introduza o local de origem: ");
        scanf("%d", &or);

        flag = 0;
        for (i = 0; i < *nEsp; i++)
            if (espaco[i].id == or) {
                flag = 1;
                posO = i;
            }
        
        if(or == 0) {
            printf("\n");
            return head;
        }

    } while (flag == 0);

    printf("\nIds de destino possiveis: ");
    for (j = 0; j < 3; j++)
        if (espaco[posO].liga[j] != -1)
            printf("%d\t", espaco[posO].liga[j]);
    printf("//\t0 - CANCELAR\n");

    do {
        printf("Introduza o local de destino: ");
        scanf("%d", &dest);

        flag = 0;
        for (k = 0; k < 3; k++)
            if (espaco[posO].liga[k] == dest) 
                flag = 1;
        
        for(i = 0; i < *nEsp; i++)
            if(espaco[i].id == dest)
                posD = i;
        
        if(dest == 0) {
            printf("\n");
            return head;
        }
        
    } while (flag == 0);

    // TESTE
/*
    for(int a = 0; a < *nEsp; a++)
        printf("%d\t", capacidade[a]);
    printf("\n\n");
    printf("NP - OR: %d\n", espaco[posO].capacidade - capacidade[posO]);
    printf("NP - DEST: %d\n\n", espaco[posD].capacidade - capacidade[posD]);
    
*/
    
    // Limite de pessoas possiveis de mover
    if (capacidade[posD] >= espaco[posO].capacidade - capacidade[posO])
        max = espaco[posO].capacidade - capacidade[posO];
    else
        max = capacidade[posD];

    printf("\nE possivel mover ate %d pessoas\t//\t0 - CANCELAR\n", max);
    do {
        printf("Introduza o numero de pessoas a mover: ");
        scanf("%d", &num);
    } while (num > max || num < 0);

    if(num == 0) {
        printf("\n");
        return head;
    }
    
    n = espaco[posO].capacidade - capacidade[posO];
    
    // Atualizar capacidades
    capacidade[posO] += num;
    capacidade[posD] -= num;
    
    printf("Total de pessoas movidas: %d\n", num);
    while (num != 0) {
        random = intUniformRnd(1, n);
        aux = head;
        c = 0;
        while (aux != NULL) {
            if (aux->local == or)
                c++;
            if (c == random)
                break;
            aux = aux->next;
        }

        printf("Pessoa: %s\n", aux->id);
        printf("%i ->", aux->local);
        aux->local = dest;
        printf(" %i\n", aux->local);
        n--;
        num--;
    }

    // Teste
    //    printf("%d -> %d // %d", or, dest, num);

    return head;
}

float taxaDisseminacao(int num) {

    return num * 0.05;
}

float probabilidadeRecuperacao(int idade) {

    return 1 / (float) idade;
}

int duracaoInfecao(int idade) {

    return 5 + (idade / 10);
}

ppessoas avancaSimulacao(ppessoas head, plocal espaco, int* nPes, int* nEsp, int* capacidade) {

    ppessoas aux = head;
    int td = 0;
    int count = 0, i, random, c;
    int ocupacao[*nEsp], ocp, flag, spread = 0;

    // Preencher array de ocupacao
    for (i = 0; i < *nEsp; i++)
        ocupacao[i] = espaco[i].capacidade - capacidade[i];

    // Avancar 1 dia
    while (aux != NULL) {
        if (aux->estado == 'D')
            aux->tempo++;
        aux = aux->next;
    }

    while (count != *nEsp) {

        // Taxa de disseminacao
        td = (int) floorf(taxaDisseminacao(ocupacao[count]));
        ocp = ocupacao[count];
        aux = head;

        while (td != 0) {
            c = 0;
            random = intUniformRnd(1, ocp);
            aux = head;
            while (aux != NULL) {
                if (aux->local == espaco[count].id) {
                    c++;
                    if (c == random && aux->estado == 'S') {
                        aux->estado = 'D';
                        aux->tempo = 1;
                        spread++;
                    }
                }
                aux = aux->next;
            }
            ocp--;
            td--;
        }

        count++;
    }

    // Recuperacao
    aux = head;
    while (aux != NULL) {

        if (aux->estado == 'D') {

            flag = 0;

            // Probabilidade de Recuperacao
            if (probEvento(probabilidadeRecuperacao(aux->idade))) {
                aux->estado = 'S';
                aux->tempo = 0;
                flag = 1;
            }

            // Duracao maxima da infecao
            if (duracaoInfecao(aux->idade) <= aux->tempo) {
                aux->estado = 'S';
                aux->tempo = 0;
                flag = 1;
            }
            
            // Probabilidade de Imunidade
            if (flag == 1)
                if (probEvento(0.2))
                    aux->estado = 'I';

        }

        aux = aux->next;
    }

    return head;
}

int menu() {

    int menu;

    puts(" 1 - Avancar 1 iteracao na simulacao");
    puts(" 2 - Apresentar estatistica");
    puts(" 3 - Adicionar doente");
    puts(" 4 - Transferir pessoas");
    puts(" 5 - Voltar a tras X iteracoes");
    printf("\n");
    puts(" 9 - Mostra dados");
    puts(" 0 - Terminar Simulacao");

    printf("\n Opcao: ");
    fscanf(stdin, "%d", &menu);
    return menu;
}

// FUNCAO "MAIN" DA SIMULACAO

ppessoas simulacao(plocal espaco, ppessoas head, pundo lista, int* nEsp, int* nPes, int* capacidade) {

    int flag;

    while (1) {

        //system("cls");
        printf("\n");
        puts("**************************** SIMULACAO DA PROPAGACAO DE VIRUS *****************************");
        printf("\n");

        switch (menu()) {
            case 1:
                printf("\n");
                puts(" -> Avanco na simulacao: ");
                puts("Foi avancado 1 dia na simulacao\n\n");
                lista = carregaIteracao(lista, head, nPes);
                lista = controlo(lista);
                head = avancaSimulacao(head, espaco, nPes, nEsp, capacidade);
                pausa();
                break;
            case 2:
                printf("\n");
                puts(" -> Estatistica da Simulacao: ");
                imprimeEstatistica(espaco, head, nEsp, nPes, capacidade, stdout);
                pausa();
                break;
            case 3:
                printf("\n");
                puts(" -> Adiciona Doente: ");
                head = adicionaPessoa(head, nPes, capacidade, espaco, nEsp);
                printf("\n");
                imprimePessoas(head);
                printf("\n");
                pausa();
                break;
            case 4:
                printf("\n");
                puts(" -> Transfere pessoas: ");
                head = transferirPessoas(head, espaco, nPes, nEsp, capacidade);
                pausa();
                break;
            case 5:
                printf("\n");
                puts(" -> Recupera iteracoes: \n");
                imprimeListaRecuperacao(lista);
                head = recuperaLista(lista, head, nPes, &flag);
                capacidade = recuperaCapacidade(head, espaco, nEsp, capacidade);
                if (flag == 1) {
                    apagaListaInterna(lista);
                    apagaListaRecuperacao(lista);
                    lista = NULL;
                }
                printf("\n");
                pausa();
                break;
            case 0:
                printf("\n");
                puts(" -> Simulacao terminada!\nLista de recuperacao libertada");
                apagaListaInterna(lista);
                apagaListaRecuperacao(lista);
                return head;
                break;
            case 9:
                printf("\n");
                puts(" -> Dados carregados: ");
                printf("\nEspacos carregados: %d\n", *nEsp);
                imprimeEspacos(espaco, *nEsp);
                printf("Pessoas carregadas: %d\n", *nPes);
                imprimePessoas(head);
                printf("\n");
                pausa();
                break;
            default:
                puts("Introduza uma opcao valida!");
                pausa();
                break;
        }
    }
    
    return head;
}


