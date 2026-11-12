#include "estruturas.h"
#include <stdlib.h>

struct NoLista {
    Timestamp valor;
    struct NoLista *prox;
};

void lista_inicializar(Lista *l) {
    l->inicio = NULL;
    l->tamanho = 0;
}

int lista_inserir(Lista *l, Timestamp t) {
    NoLista *novo = (NoLista *)malloc(sizeof(NoLista));
    NoLista **atual;
    if (!novo) return 0;
    novo->valor = t;
    novo->prox = NULL;
    atual = &l->inicio;
    while (*atual && timestamp_comparar((*atual)->valor, t) <= 0)
        atual = &(*atual)->prox;
    novo->prox = *atual;
    *atual = novo;
    l->tamanho++;
    return 1;
}

int lista_remover(Lista *l, Timestamp t) {
    NoLista **atual = &l->inicio;
    while (*atual && timestamp_comparar((*atual)->valor, t) < 0)
        atual = &(*atual)->prox;
    if (!*atual || timestamp_comparar((*atual)->valor, t) != 0) return 0;
    {
        NoLista *removido = *atual;
        *atual = removido->prox;
        free(removido);
    }
    l->tamanho--;
    return 1;
}

void lista_listar_ordenado(const Lista *l, VisitarTimestamp visitar, void *contexto) {
    NoLista *atual;
    if (!visitar) return;
    for (atual = l->inicio; atual; atual = atual->prox)
        visitar(&atual->valor, contexto);
}

void lista_liberar(Lista *l) {
    NoLista *atual = l->inicio;
    while (atual) {
        NoLista *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    l->inicio = NULL;
    l->tamanho = 0;
}
