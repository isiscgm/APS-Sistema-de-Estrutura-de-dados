#include "estruturas.h"
#include <stdlib.h>

struct NoListaDupla {
    Timestamp valor;
    struct NoListaDupla *ant;
    struct NoListaDupla *prox;
};

void lista_dupla_inicializar(ListaDupla *l) {
    l->inicio = NULL;
    l->fim = NULL;
    l->tamanho = 0;
}

int lista_dupla_inserir(ListaDupla *l, Timestamp t) {
    NoListaDupla *novo = malloc(sizeof(*novo));
    NoListaDupla *atual;
    if (!novo) return 0;
    novo->valor = t;
    novo->ant = NULL;
    novo->prox = NULL;
    /* Mesmo criterio da lista simples: duplicatas depois dos iguais. */
    atual = l->inicio;
    while (atual && timestamp_comparar(atual->valor, t) <= 0)
        atual = atual->prox;
    if (!atual) {
        novo->ant = l->fim;
        if (l->fim) l->fim->prox = novo;
        else l->inicio = novo;
        l->fim = novo;
    } else {
        novo->prox = atual;
        novo->ant = atual->ant;
        if (atual->ant) atual->ant->prox = novo;
        else l->inicio = novo;
        atual->ant = novo;
    }
    l->tamanho++;
    return 1;
}

int lista_dupla_remover(ListaDupla *l, Timestamp t) {
    NoListaDupla *atual = l->inicio;
    while (atual && timestamp_comparar(atual->valor, t) < 0)
        atual = atual->prox;
    if (!atual || timestamp_comparar(atual->valor, t) != 0) return 0;
    if (atual->ant) atual->ant->prox = atual->prox;
    else l->inicio = atual->prox;
    if (atual->prox) atual->prox->ant = atual->ant;
    else l->fim = atual->ant;
    free(atual);
    l->tamanho--;
    return 1;
}

void lista_dupla_listar_ordenado(const ListaDupla *l, VisitarTimestamp visitar, void *contexto) {
    NoListaDupla *atual;
    if (!visitar) return;
    for (atual = l->inicio; atual; atual = atual->prox)
        visitar(&atual->valor, contexto);
}

void lista_dupla_liberar(ListaDupla *l) {
    NoListaDupla *atual = l->inicio;
    while (atual) {
        NoListaDupla *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    lista_dupla_inicializar(l);
}
