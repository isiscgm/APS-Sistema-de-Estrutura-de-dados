#ifndef LISTA_DUPLA_H
#define LISTA_DUPLA_H
#include "timestamp.h"
#include <stddef.h>

typedef struct NoDuplo NoDuplo;
typedef struct { NoDuplo *inicio, *fim; size_t tamanho; } ListaDupla;

void lista_dupla_inicializar(ListaDupla *l);
int lista_dupla_inserir(ListaDupla *l, Timestamp valor);
int lista_dupla_remover(ListaDupla *l, const Timestamp *valor);
void lista_dupla_listar_ordenado(const ListaDupla *l, void (*visitar)(const Timestamp *, void *), void *ctx);
void lista_dupla_liberar(ListaDupla *l);

#endif
