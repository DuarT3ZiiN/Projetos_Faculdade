#ifndef LISTA_H
#define LISTA_H

#define MAX_STR 256
#define MAX_LINE 4096
#define NUM_COLS 33

typedef struct {
    char sigla_tribunal[MAX_STR];
    char procedimento[MAX_STR];
    char ramo_justica[MAX_STR];
    char sigla_grau[MAX_STR];
    char uf_oj[MAX_STR];
    char municipio_oj[MAX_STR];
    int id_ultimo_oj;
    char nome[MAX_STR];
    char mesano_cnm1[MAX_STR];
    char mesano_sent[MAX_STR];
    long casos_novos_2026;
    long julgados_2026;
    long prim_sent2026;
    long suspensos_2026;
    long dessobrestados_2026;
    double cumprimento_meta1;
    long distm2_a;
    long julgm2_a;
    long suspm2_a;
    double cumprimento_meta2a;
    long distm2_ant;
    long julgm2_ant;
    long suspm2_ant;
    long desom2_ant;
    double cumprimento_meta2ant;
    long distm4_a;
    long julgm4_a;
    long suspm4_a;
    double cumprimento_meta4a;
    long distm4_b;
    long julgm4_b;
    long suspm4_b;
    double cumprimento_meta4b;
    char linha_original[MAX_LINE];
} Registro;

typedef struct no {
    Registro dado;
    struct no *prox;
} No;

typedef struct {
    No *inicio;
    No *fim;
    int tamanho;
} Lista;

typedef struct {
    char sigla_tribunal[MAX_STR];
    long total_julgados_2026;
    long soma_casos_novos_2026;
    long soma_suspensos_2026;
    long soma_dessobrestados_2026;
    long soma_distm2_a;
    long soma_julgm2_a;
    long soma_suspm2_a;
    long soma_distm2_ant;
    long soma_julgm2_ant;
    long soma_suspm2_ant;
    long soma_desom2_ant;
    long soma_distm4_a;
    long soma_julgm4_a;
    long soma_suspm4_a;
    long soma_distm4_b;
    long soma_julgm4_b;
    long soma_suspm4_b;
} ResumoTribunal;

void inicializarLista(Lista *lista);
int listaVazia(const Lista *lista);
int inserirFim(Lista *lista, Registro reg);
void liberarLista(Lista *lista);
int carregarCSVParaLista(const char *nomeArquivo, Lista *lista, int ignorarCabecalho);
int carregarTodosCSVsDaPasta(const char *pasta, Lista *lista);
int concatenarArquivos(const char *pasta, const char *arquivoSaida);
int gerarResumoPorTribunal(const Lista *lista, const char *arquivoSaida);
int gerarArquivoPorMunicipio(const Lista *lista, const char *municipio, const char *arquivoSaida);
void limparFimDeLinha(char *s);
void normalizarTexto(char *destino, const char *origem);

#endif
