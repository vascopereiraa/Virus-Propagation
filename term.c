
/* 
 * File:   term.c
 * Author: Vasco Daniel Matos Pereira
 * Nr: 2019134744
 * 
 * Created on 22 de maio de 2020, 01:48
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

int gravaRelatorio(ppessoas head, plocal espaco, int *nEsp, int *nPes, int* capacidade) {
    
    FILE* fp = NULL;
    
    // Abrir o ficheiro de txt para escrita
    fp = fopen("report.txt", "wt");
    if(fp == NULL) {
        printf("Erro ao abrir o ficheiro de texto para gravar o relatorio");
        return 0;
    }
    
    // Impressao do relatorio final
    fprintf(fp, "----------------------- Relatorio Final -----------------------\n");
    fprintf(fp, "\n -> Simulacao da propagacao de Virus\n\n");
    fprintf(fp, "Numero de espacos: %d\n", *nEsp);
    fprintf(fp, "Numero de pessoas: %d\n", *nPes);
    fprintf(fp, "\n---------------------------------------------------------------");
    imprimeEstatistica(espaco, head, nEsp, nPes, capacidade, fp);
    fprintf(fp, "---------------------------------------------------------------\n");
    fprintf(fp, "\nTrabalho elaborado por: Vasco Daniel Matos Pereira  .:  2019134744");
    fprintf(fp, "\n -> Simulacao da propagacao de Virus\n\n");
    
    fclose(fp);
    return 1;
}

int gravaPessoas(ppessoas head, char* fichTerm) {
    
    FILE *fp = NULL;
    ppessoas aux = head;

    fp = fopen(fichTerm, "wt");
    if(fp == NULL) {
        printf("\nOcorreu um erro ao abrir o ficheiro de texto!");
        return 0;
    }
    
    if (aux == NULL)
        return 0;
    else 
        while (aux != NULL) {
            fprintf(fp, "%s  %d  %c", aux->id, aux->idade, aux->estado);
            if (aux->estado == 'D')
                fprintf(fp, "  %d\n", aux->tempo);
            else
                fprintf(fp, "\n");
            aux = aux->next;
        }
    
    fclose(fp);
    return 1;
}