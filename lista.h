#ifndef LISTA_H
#define LISTA_H
#include "timestamp.h"
#include <stddef.h>

typedef struct NoLista NoLista;
typedef struct { NoLista *inicio; size_t tamanho; } Lista;

void lista_inicializar(Lista *l);
int lista_inserir(Lista *l, Timestamp valor);
int lista_remover(Lista *l, const Timestamp *valor);
void lista_listar_ordenado(const Lista *l, void (*visitar)(const Timestamp *, void *), void *ctx);
void lista_liberar(Lista *l);

#endif
