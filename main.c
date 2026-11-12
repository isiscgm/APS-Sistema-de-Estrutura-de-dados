#include "timestamp.h"
#include "lista.h"
#include "lista_dupla.h"
#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int limite, impressos; } Contexto;
static void imprimir_ate_limite(const Timestamp *t, void *ctx) {
    Contexto *c = ctx;
    if (c->impressos < c->limite) { timestamp_imprimir(t); putchar('\n'); c->impressos++; }
}
static int carregar(const char *arquivo, Timestamp **dados, size_t *n) {
    FILE *f = fopen(arquivo, "r");
    if (!f) { perror("Não foi possível abrir o arquivo"); return 0; }
    size_t cap = 1024; *n = 0;
    *dados = malloc(cap * sizeof **dados);
    if (!*dados) { fclose(f); return 0; }
    char linha[128];
    while (fgets(linha, sizeof linha, f)) {
        linha[strcspn(linha, "\r\n")] = '\0';
        if (!linha[0]) continue;
        Timestamp t;
        if (!timestamp_parse(linha, &t)) { fprintf(stderr, "Linha inválida ignorada: %s\n", linha); continue; }
        if (*n == cap) {
            cap *= 2; Timestamp *tmp = realloc(*dados, cap * sizeof **dados);
            if (!tmp) { free(*dados); fclose(f); return 0; } *dados = tmp;
        }
        (*dados)[(*n)++] = t;
    }
    fclose(f); return 1;
}
int main(int argc, char **argv) {
    if (argc < 2) { printf("Uso: %s arquivo_timestamps.txt\n", argv[0]); return 1; }
    Timestamp *dados = NULL; size_t n = 0;
    if (!carregar(argv[1], &dados, &n)) return 1;
    Lista l; ListaDupla d; AVL a;
    lista_inicializar(&l); lista_dupla_inicializar(&d); avl_inicializar(&a);
    for (size_t i = 0; i < n; i++) {
        if (!lista_inserir(&l, dados[i]) || !lista_dupla_inserir(&d, dados[i]) || !avl_inserir(&a, dados[i])) {
            fprintf(stderr, "Falha de memória durante inserção.\n"); break;
        }
    }
    Contexto c;
    puts("\nLISTA SIMPLES - primeiros 10 em ordem:");
    c.limite = 10; c.impressos = 0; lista_listar_ordenado(&l, imprimir_ate_limite, &c);
    puts("\nAVL - primeiros 10 em ordem:");
    c.impressos = 0; avl_listar_ordenado(&a, imprimir_ate_limite, &c);
    puts("\nLISTA DUPLA - primeiros 10 em ordem:");
    c.impressos = 0; lista_dupla_listar_ordenado(&d, imprimir_ate_limite, &c);
    printf("\nRegistros carregados: %zu\n", n);
    lista_liberar(&l); lista_dupla_liberar(&d); avl_liberar(&a); free(dados);
    return 0;
}
