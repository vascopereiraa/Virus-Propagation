
/* 
 * File:   main.c
 * Author: Vasco Daniel Matos Pereira
 * Nr: 2019134744
 * 
 * Created on 31 de março de 2020, 21:18
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

int main(int argc, char** argv) {

    local *espaco = NULL;
    pessoas_l *head = NULL;
    pundo lista = NULL;
    int nEsp = 0, nPes = 0, i, dif = 0;
    char fichEsp[25], fichPessoas[25], fichTerm[25];

    // Inicialização do gerador de numeros    
    initRandom();

    // 1a fase do programa -> Carregar ficheiros
    puts("******************************** INCIALIZACAO DA SIMULACAO ********************************");
    printf("\n");

    do {
        printf("Indique o ficheiro binario pretende utilizar para carregar os espacos: ");
        fscanf(stdin, "%s", fichEsp);
    } while(!(strstr(fichEsp, ".bin") || strstr(fichEsp, ".dat")));
    
    do {
        printf("Indique o ficheiro de texto pretende utilizar para carregar as pessoas: ");
        fscanf(stdin, "%s", fichPessoas);
    } while(!strstr(fichPessoas, ".txt"));
    printf("\n");
        
    /* Carrega e verifica o espaco */
    espaco = carregaEspaco(espaco, &nEsp, fichEsp);
    printf("Numero de espacos carregados: %d\n", nEsp);
    int capacidade[nEsp]; // Array com as capacidades dos locais
    carregaCapacidade(capacidade, nEsp, espaco); // Carrega o array de cap

    if (espaco == NULL)
        return 0;

    if (verificaId(espaco, nEsp) == 0)
        return 0;
    if (verificaLigacao(espaco, nEsp) == 0)
        return 0;

    puts("SUCESSO - Espaco carregado corretamente!");
    imprimeEspacos(espaco, nEsp);

    /* Carrega e verifica as pessoas */
    head = carregaPessoas(head, fichPessoas, &nPes);
    if (verificaIdade(head) == 0)
        return 0;
    if (verificaNomeInit(head, nPes) == 0)
        return 0;
    if (verificaEstado(head) == 0)
        return 0;

    distribuiPessoas(espaco, head, nEsp, capacidade);
    for (i = 0; i < nPes; i++)
        head = apagaPessoaLocal(head, -1, &dif);
    nPes -= dif;
    printf("Numero de pessoas carregadas: %d\n", nPes);
    puts("SUCESSO - Pessoas carregadas corretamente");
    imprimePessoas(head);

    // 2a fase do programa -> Simulacao
    printf("\n");
    pausa();
    head = simulacao(espaco, head, lista, &nEsp, &nPes, capacidade);
    
    // 3a fase do programa -> Guardar dados
    if(gravaRelatorio(head, espaco, &nEsp, &nPes, capacidade))
        puts("SUCESSO - Relatorio Final gravado corretamente!");
    
    printf("\n");
    do {
        printf("Indique o ficheiro de texto pretende utilizar para gravar as pessoas: ");
        fscanf(stdin, "%s", fichTerm);
    } while(!strstr(fichTerm, ".txt"));
    
    if(gravaPessoas(head, fichTerm))
        puts("SUCESSO - Ficheiro de pessoas gravado corretamente!");
    
    printf("\n - FIM DA SIMULACAO - \n");
    printf("\n------------------------------------------------------------------\n");
    printf("Trabalho elaborado por: Vasco Daniel Matos Pereira  .:  2019134744");
    printf("\n -> Simulacao da propagacao de Virus\n\n");
    
    /* Libertar espaco dos arrays dinamicos */
    free(espaco);
    apagaLista(head);
    apagaListaInterna(lista);
    apagaListaRecuperacao(lista);

    return (EXIT_SUCCESS);
}
