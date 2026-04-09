#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "lista.h"

static void copiarCampo(char *destino, const char *origem) {
    strncpy(destino, origem, MAX_STR - 1);
    destino[MAX_STR - 1] = '\0';
}

void limparFimDeLinha(char *s) {
    if (s == NULL) return;
    s[strcspn(s, "\r\n")] = '\0';
}

void normalizarTexto(char *destino, const char *origem) {
    int i = 0, j = 0;
    while (origem[i] != '\0' && j < MAX_STR - 1) {
        unsigned char c = (unsigned char)origem[i];
        if (c != '"') {
            destino[j++] = (char)toupper(c);
        }
        i++;
    }
    destino[j] = '\0';
}

void inicializarLista(Lista *lista) {
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

int listaVazia(const Lista *lista) {
    return lista->inicio == NULL;
}

int inserirFim(Lista *lista, Registro reg) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) return 0;

    novo->dado = reg;
    novo->prox = NULL;

    if (listaVazia(lista)) {
        lista->inicio = novo;
    } else {
        lista->fim->prox = novo;
    }

    lista->fim = novo;
    lista->tamanho++;
    return 1;
}

void liberarLista(Lista *lista) {
    No *atual = lista->inicio;
    while (atual != NULL) {
        No *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    inicializarLista(lista);
}

static long paraLong(const char *s) {
    if (s == NULL || *s == '\0') return 0;
    return strtol(s, NULL, 10);
}

static double paraDouble(const char *s) {
    if (s == NULL || *s == '\0') return 0.0;
    return strtod(s, NULL);
}

static int parseCSVLine(const char *linha, char campos[][MAX_STR], int maxCampos) {
    int i = 0, j = 0, k = 0;
    int dentroAspas = 0;
    char atual[MAX_STR];

    while (linha[i] != '\0' && linha[i] != '\n' && linha[i] != '\r') {
        char c = linha[i];

        if (c == '"') {
            if (dentroAspas && linha[i + 1] == '"') {
                if (j < MAX_STR - 1) atual[j++] = '"';
                i++;
            } else {
                dentroAspas = !dentroAspas;
            }
        } else if (c == ',' && !dentroAspas) {
            atual[j] = '\0';
            if (k < maxCampos) {
                copiarCampo(campos[k], atual);
                k++;
            }
            j = 0;
            atual[0] = '\0';
        } else {
            if (j < MAX_STR - 1) atual[j++] = c;
        }
        i++;
    }

    atual[j] = '\0';
    if (k < maxCampos) {
        copiarCampo(campos[k], atual);
        k++;
    }

    return k;
}

static Registro montarRegistro(char campos[][MAX_STR]) {
    Registro reg;
    memset(&reg, 0, sizeof(Registro));

    copiarCampo(reg.sigla_tribunal, campos[0]);
    copiarCampo(reg.procedimento, campos[1]);
    copiarCampo(reg.ramo_justica, campos[2]);
    copiarCampo(reg.sigla_grau, campos[3]);
    copiarCampo(reg.uf_oj, campos[4]);
    copiarCampo(reg.municipio_oj, campos[5]);
    reg.id_ultimo_oj = (int)paraLong(campos[6]);
    copiarCampo(reg.nome, campos[7]);
    copiarCampo(reg.mesano_cnm1, campos[8]);
    copiarCampo(reg.mesano_sent, campos[9]);
    reg.casos_novos_2026 = paraLong(campos[10]);
    reg.julgados_2026 = paraLong(campos[11]);
    reg.prim_sent2026 = paraLong(campos[12]);
    reg.suspensos_2026 = paraLong(campos[13]);
    reg.dessobrestados_2026 = paraLong(campos[14]);
    reg.cumprimento_meta1 = paraDouble(campos[15]);
    reg.distm2_a = paraLong(campos[16]);
    reg.julgm2_a = paraLong(campos[17]);
    reg.suspm2_a = paraLong(campos[18]);
    reg.cumprimento_meta2a = paraDouble(campos[19]);
    reg.distm2_ant = paraLong(campos[20]);
    reg.julgm2_ant = paraLong(campos[21]);
    reg.suspm2_ant = paraLong(campos[22]);
    reg.desom2_ant = paraLong(campos[23]);
    reg.cumprimento_meta2ant = paraDouble(campos[24]);
    reg.distm4_a = paraLong(campos[25]);
    reg.julgm4_a = paraLong(campos[26]);
    reg.suspm4_a = paraLong(campos[27]);
    reg.cumprimento_meta4a = paraDouble(campos[28]);
    reg.distm4_b = paraLong(campos[29]);
    reg.julgm4_b = paraLong(campos[30]);
    reg.suspm4_b = paraLong(campos[31]);
    reg.cumprimento_meta4b = paraDouble(campos[32]);

    return reg;
}

int carregarCSVParaLista(const char *nomeArquivo, Lista *lista, int ignorarCabecalho) {
    FILE *arq = fopen(nomeArquivo, "r");
    char linha[MAX_LINE];
    int primeiraLinha = 1;

    if (arq == NULL) {
        printf("Erro ao abrir: %s\n", nomeArquivo);
        return 0;
    }

    while (fgets(linha, sizeof(linha), arq) != NULL) {
        char campos[NUM_COLS][MAX_STR];
        int qtdCampos;
        Registro reg;

        if (primeiraLinha && ignorarCabecalho) {
            primeiraLinha = 0;
            continue;
        }
        primeiraLinha = 0;

        limparFimDeLinha(linha);
        if (linha[0] == '\0') continue;

        qtdCampos = parseCSVLine(linha, campos, NUM_COLS);
        if (qtdCampos != NUM_COLS) continue;

        reg = montarRegistro(campos);
        copiarCampo(reg.linha_original, linha);

        if (!inserirFim(lista, reg)) {
            fclose(arq);
            return 0;
        }
    }

    fclose(arq);
    return 1;
}

static int ehCSV(const char *nome) {
    const char *ext = strrchr(nome, '.');
    if (ext == NULL) return 0;
    return (tolower((unsigned char)ext[0]) == '.' &&
            tolower((unsigned char)ext[1]) == 'c' &&
            tolower((unsigned char)ext[2]) == 's' &&
            tolower((unsigned char)ext[3]) == 'v' &&
            ext[4] == '\0');
}

int carregarTodosCSVsDaPasta(const char *pasta, Lista *lista) {
    DIR *dir = opendir(pasta);
    struct dirent *entrada;
    char caminho[512];
    int carregou = 0;

    if (dir == NULL) {
        printf("Nao foi possivel abrir a pasta: %s\n", pasta);
        return 0;
    }

    while ((entrada = readdir(dir)) != NULL) {
        if (ehCSV(entrada->d_name)) {
            snprintf(caminho, sizeof(caminho), "%s/%s", pasta, entrada->d_name);
            if (carregarCSVParaLista(caminho, lista, 1)) {
                carregou = 1;
            }
        }
    }

    closedir(dir);
    return carregou;
}

int concatenarArquivos(const char *pasta, const char *arquivoSaida) {
    DIR *dir = opendir(pasta);
    struct dirent *entrada;
    FILE *saida;
    char caminho[512];
    char linha[MAX_LINE];
    int primeiraLinhaGeral = 1;

    if (dir == NULL) {
        printf("Nao foi possivel abrir a pasta: %s\n", pasta);
        return 0;
    }

    saida = fopen(arquivoSaida, "w");
    if (saida == NULL) {
        closedir(dir);
        printf("Nao foi possivel criar o arquivo: %s\n", arquivoSaida);
        return 0;
    }

    while ((entrada = readdir(dir)) != NULL) {
        if (ehCSV(entrada->d_name)) {
            FILE *origem;
            int primeiraLinhaArquivo = 1;
            snprintf(caminho, sizeof(caminho), "%s/%s", pasta, entrada->d_name);
            origem = fopen(caminho, "r");
            if (origem == NULL) continue;

            while (fgets(linha, sizeof(linha), origem) != NULL) {
                if (primeiraLinhaArquivo) {
                    primeiraLinhaArquivo = 0;
                    if (primeiraLinhaGeral) {
                        fputs(linha, saida);
                        primeiraLinhaGeral = 0;
                    }
                    continue;
                }
                fputs(linha, saida);
            }
            fclose(origem);
        }
    }

    fclose(saida);
    closedir(dir);
    return 1;
}

static int buscarIndiceTribunal(ResumoTribunal vet[], int n, const char *sigla) {
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(vet[i].sigla_tribunal, sigla) == 0) return i;
    }
    return -1;
}

static double calculaPercentual(double numerador, double denominador) {
    if (denominador == 0.0) return 0.0;
    return (numerador / denominador) * 100.0;
}

int gerarResumoPorTribunal(const Lista *lista, const char *arquivoSaida) {
    FILE *saida = fopen(arquivoSaida, "w");
    No *atual = lista->inicio;
    ResumoTribunal resumos[128];
    int qtd = 0;
    int i;

    if (saida == NULL) {
        printf("Nao foi possivel criar o arquivo: %s\n", arquivoSaida);
        return 0;
    }

    memset(resumos, 0, sizeof(resumos));

    while (atual != NULL) {
        int idx = buscarIndiceTribunal(resumos, qtd, atual->dado.sigla_tribunal);
        if (idx == -1) {
            idx = qtd++;
            copiarCampo(resumos[idx].sigla_tribunal, atual->dado.sigla_tribunal);
        }

        resumos[idx].total_julgados_2026 += atual->dado.julgados_2026;
        resumos[idx].soma_casos_novos_2026 += atual->dado.casos_novos_2026;
        resumos[idx].soma_suspensos_2026 += atual->dado.suspensos_2026;
        resumos[idx].soma_dessobrestados_2026 += atual->dado.dessobrestados_2026;

        resumos[idx].soma_distm2_a += atual->dado.distm2_a;
        resumos[idx].soma_julgm2_a += atual->dado.julgm2_a;
        resumos[idx].soma_suspm2_a += atual->dado.suspm2_a;

        resumos[idx].soma_distm2_ant += atual->dado.distm2_ant;
        resumos[idx].soma_julgm2_ant += atual->dado.julgm2_ant;
        resumos[idx].soma_suspm2_ant += atual->dado.suspm2_ant;
        resumos[idx].soma_desom2_ant += atual->dado.desom2_ant;

        resumos[idx].soma_distm4_a += atual->dado.distm4_a;
        resumos[idx].soma_julgm4_a += atual->dado.julgm4_a;
        resumos[idx].soma_suspm4_a += atual->dado.suspm4_a;

        resumos[idx].soma_distm4_b += atual->dado.distm4_b;
        resumos[idx].soma_julgm4_b += atual->dado.julgm4_b;
        resumos[idx].soma_suspm4_b += atual->dado.suspm4_b;

        atual = atual->prox;
    }

    fprintf(saida, "sigla_tribunal,total_julgados_2026,Meta1,Meta2A,Meta2Ant,Meta4A,Meta4B\n");

    for (i = 0; i < qtd; i++) {
        double meta1 = calculaPercentual(
            (double)resumos[i].total_julgados_2026,
            (double)(resumos[i].soma_casos_novos_2026 + resumos[i].soma_dessobrestados_2026 + resumos[i].soma_suspensos_2026)
        );

        double meta2a = calculaPercentual(
            (double)resumos[i].soma_julgm2_a,
            (double)(resumos[i].soma_distm2_a + resumos[i].soma_suspm2_a)
        );

        double meta2ant = calculaPercentual(
            (double)resumos[i].soma_julgm2_ant,
            (double)(resumos[i].soma_distm2_ant + resumos[i].soma_suspm2_ant + resumos[i].soma_desom2_ant)
        );

        double meta4a = calculaPercentual(
            (double)resumos[i].soma_julgm4_a,
            (double)(resumos[i].soma_distm4_a + resumos[i].soma_suspm4_a)
        );

        double meta4b = calculaPercentual(
            (double)resumos[i].soma_julgm4_b,
            (double)(resumos[i].soma_distm4_b + resumos[i].soma_suspm4_b)
        );

        fprintf(saida, "%s,%ld,%.2f,%.2f,%.2f,%.2f,%.2f\n",
            resumos[i].sigla_tribunal,
            resumos[i].total_julgados_2026,
            meta1,
            meta2a,
            meta2ant,
            meta4a,
            meta4b
        );
    }

    fclose(saida);
    return 1;
}

int gerarArquivoPorMunicipio(const Lista *lista, const char *municipio, const char *arquivoSaida) {
    FILE *saida = fopen(arquivoSaida, "w");
    No *atual = lista->inicio;
    char municipioNormalizado[MAX_STR];
    int encontrou = 0;

    if (saida == NULL) {
        printf("Nao foi possivel criar o arquivo: %s\n", arquivoSaida);
        return 0;
    }

    normalizarTexto(municipioNormalizado, municipio);

    fprintf(saida, "\"sigla_tribunal\",\"procedimento\",\"ramo_justica\",\"sigla_grau\",\"uf_oj\",\"municipio_oj\",\"id_ultimo_oj\",\"nome\",\"mesano_cnm1\",\"mesano_sent\",\"casos_novos_2026\",\"julgados_2026\",\"prim_sent2026\",\"suspensos_2026\",\"dessobrestados_2026\",\"cumprimento_meta1\",\"distm2_a\",\"julgm2_a\",\"suspm2_a\",\"cumprimento_meta2a\",\"distm2_ant\",\"julgm2_ant\",\"suspm2_ant\",\"desom2_ant\",\"cumprimento_meta2ant\",\"distm4_a\",\"julgm4_a\",\"suspm4_a\",\"cumprimento_meta4a\",\"distm4_b\",\"julgm4_b\",\"suspm4_b\",\"cumprimento_meta4b\"\n");

    while (atual != NULL) {
        char municipioRegistro[MAX_STR];
        normalizarTexto(municipioRegistro, atual->dado.municipio_oj);

        if (strcmp(municipioRegistro, municipioNormalizado) == 0) {
            fprintf(saida, "%s\n", atual->dado.linha_original);
            encontrou = 1;
        }
        atual = atual->prox;
    }

    fclose(saida);
    return encontrou;
}
