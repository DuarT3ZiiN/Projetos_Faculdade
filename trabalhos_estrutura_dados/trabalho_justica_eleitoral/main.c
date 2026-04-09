#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

static void mostrarMenu(void) {
    printf("\n\n\t---------------------------------------------------------------\n");
    printf("\t|===== SISTEMA DE MANIPULACAO DE CSV - JUSTICA ELEITORAL =====|\n");
    printf("\t---------------------------------------------------------------\n");
    printf("\t|1 - Concatenar todos os arquivos CSV da pasta|\n");
    printf("\t|2 - Gerar resumo por sigla_tribunal          |\n");
    printf("\t|3 - Gerar arquivo TXT por municipio          |\n");
    printf("\t|0 - Sair                                     |\n");
    printf("\t---------------------------------------------------------------\n");
    printf("\t|Informe sua opcao: ");
}

int main(void) {
    Lista lista;
    int opcao;
    const char pastaBase[] = "Base de Dados";

    inicializarLista(&lista);

    if (!carregarTodosCSVsDaPasta(pastaBase, &lista)) {
        printf("\t>> Erro ao carregar os CSVs da pasta '%s'.\n", pastaBase);
        printf("\t>> Coloque a pasta Base de Dados no mesmo diretorio do executavel.\n");
        return 1;
    }

    do {
        char municipio[MAX_STR];
        char nomeArquivo[MAX_STR + 10];

        mostrarMenu();
        if (scanf("%d", &opcao) != 1) {
            printf("\t>> Entrada invalida.\n");
            while (getchar() != '\n');
            opcao = -1;
            continue;
        }
        while (getchar() != '\n');

        switch (opcao) {
            case 1:
                printf("\t>> Concatenando arquivos...\n");
                if (concatenarArquivos(pastaBase, "base_concatenada.csv")) {
                    printf("\t>> Arquivo 'base_concatenada.csv' gerado com sucesso.\n");
                } else {
                    printf("\t>> Falha ao concatenar os arquivos.\n");
                }
                break;

            case 2:
                printf("\t>> Gerando resumo por tribunal...\n");
                if (gerarResumoPorTribunal(&lista, "resumo_tribunais.csv")) {
                    printf("\t>> Arquivo 'resumo_tribunais.csv' gerado com sucesso.\n");
                } else {
                    printf("\t>> Falha ao gerar o resumo.\n");
                }
                break;

            case 3:
                printf("\t>> Gerando arquivo por municipio...\n");
                printf("\t>> Informe o municipio: ");
                fgets(municipio, sizeof(municipio), stdin);
                limparFimDeLinha(municipio);
                snprintf(nomeArquivo, sizeof(nomeArquivo), "%s.txt", municipio);
                if (gerarArquivoPorMunicipio(&lista, municipio, nomeArquivo)) {
                    printf("\t>> Arquivo '%s' gerado com sucesso.\n", nomeArquivo);
                } else {
                    printf("\t>> Nenhum registro encontrado para o municipio informado.\n");
                    remove(nomeArquivo);
                }
                break;

            case 0:
                printf("\t>> Encerrando...\n");
                break;

            default:
                printf("\t>> Opcao invalida.\n");
        }
    } while (opcao != 0);

    liberarLista(&lista);
    return 0;
}
