#include "lista_dupla.h"
#include <stdlib.h>

struct NoDuplo { Timestamp valor; struct NoDuplo *ant, *prox; };

void lista_dupla_inicializar(ListaDupla *l) {
    l->inicio = l->fim = NULL; l->tamanho = 0;
}

int lista_dupla_inserir(ListaDupla *l, Timestamp valor) {
    NoDuplo *n = malloc(sizeof *n);
    if (!n) return 0;
    n->valor = valor; n->ant = NULL; n->prox = l->inicio;
    if (l->inicio) l->inicio->ant = n; else l->fim = n;
    l->inicio = n; l->tamanho++;
    return 1;
}

int lista_dupla_remover(ListaDupla *l, const Timestamp *valor) {
    for (NoDuplo *p = l->inicio; p; p = p->prox) {
        if (timestamp_comparar(&p->valor, valor) == 0) {
            if (p->ant) p->ant->prox = p->prox; else l->inicio = p->prox;
            if (p->prox) p->prox->ant = p->ant; else l->fim = p->ant;
            free(p); l->tamanho--; return 1;
        }
    }
    return 0;
}

static int comparar_qsort(const void *a, const void *b) {
    return timestamp_comparar((const Timestamp *)a, (const Timestamp *)b);
}

void lista_dupla_listar_ordenado(const ListaDupla *l, void (*visitar)(const Timestamp *, void *), void *ctx) {
    if (!l || !visitar || l->tamanho == 0) return;
    Timestamp *v = malloc(l->tamanho * sizeof *v);
    if (!v) return;
    size_t i = 0;
    for (NoDuplo *p = l->inicio; p; p = p->prox) v[i++] = p->valor;
    qsort(v, i, sizeof *v, comparar_qsort);
    for (size_t a = 0; a < i; a++) visitar(&v[a], ctx);
    free(v);
}

void lista_dupla_liberar(ListaDupla *l) {
    NoDuplo *p = l->inicio;
    while (p) { NoDuplo *prox = p->prox; free(p); p = prox; }
    l->inicio = l->fim = NULL; l->tamanho = 0;
}
