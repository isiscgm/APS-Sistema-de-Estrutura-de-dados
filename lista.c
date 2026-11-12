#include "lista.h"
#include <stdlib.h>

struct NoLista { Timestamp valor; struct NoLista *prox; };

void lista_inicializar(Lista *l) { l->inicio = NULL; l->tamanho = 0; }

int lista_inserir(Lista *l, Timestamp valor) {
    NoLista *novo = malloc(sizeof *novo);
    if (!novo) return 0;
    novo->valor = valor;
    novo->prox = l->inicio;
    l->inicio = novo;
    l->tamanho++;
    return 1;
}

int lista_remover(Lista *l, const Timestamp *valor) {
    NoLista **p = &l->inicio;
    while (*p) {
        if (timestamp_comparar(&(*p)->valor, valor) == 0) {
            NoLista *rem = *p;
            *p = rem->prox;
            free(rem);
            l->tamanho--;
            return 1;
        }
        p = &(*p)->prox;
    }
    return 0;
}

static int comparar_qsort(const void *a, const void *b) {
    return timestamp_comparar((const Timestamp *)a, (const Timestamp *)b);
}

void lista_listar_ordenado(const Lista *l, void (*visitar)(const Timestamp *, void *), void *ctx) {
    if (!l || !visitar || l->tamanho == 0) return;
    Timestamp *v = malloc(l->tamanho * sizeof *v);
    if (!v) return;
    size_t i = 0;
    for (NoLista *p = l->inicio; p; p = p->prox) v[i++] = p->valor;
    qsort(v, i, sizeof *v, comparar_qsort);
    for (size_t a = 0; a < i; a++) visitar(&v[a], ctx);
    free(v);
}

void lista_liberar(Lista *l) {
    NoLista *p = l->inicio;
    while (p) { NoLista *prox = p->prox; free(p); p = prox; }
    l->inicio = NULL; l->tamanho = 0;
}
