#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stddef.h>

typedef struct {
    int ano, mes, dia;
    int hora, minuto, segundo;
} Timestamp;

/* Retorna 1 se a data for valida e 0 caso contrario. */
int timestamp_parse(const char *texto, Timestamp *saida);
int timestamp_comparar(Timestamp a, Timestamp b);
void timestamp_imprimir(const Timestamp *t);

typedef void (*VisitarTimestamp)(const Timestamp *t, void *contexto);

/* Lista simplesmente encadeada, mantida em ordem cronologica. */
typedef struct NoLista NoLista;
typedef struct {
    NoLista *inicio;
    size_t tamanho;
} Lista;
void lista_inicializar(Lista *l);
int lista_inserir(Lista *l, Timestamp t);
int lista_remover(Lista *l, Timestamp t);
void lista_listar_ordenado(const Lista *l, VisitarTimestamp visitar, void *contexto);
void lista_liberar(Lista *l);

/* Arvore AVL: balanceamento garante altura O(log n). */
typedef struct NoAVL NoAVL;
typedef struct {
    NoAVL *raiz;
    size_t tamanho; /* quantidade total de registros, incluindo duplicatas */
} AVL;
void avl_inicializar(AVL *a);
int avl_inserir(AVL *a, Timestamp t);
int avl_remover(AVL *a, Timestamp t);
void avl_listar_ordenado(const AVL *a, VisitarTimestamp visitar, void *contexto);
void avl_liberar(AVL *a);

/* Lista duplamente encadeada, mantida em ordem cronologica. */
typedef struct NoListaDupla NoListaDupla;
typedef struct {
    NoListaDupla *inicio;
    NoListaDupla *fim;
    size_t tamanho;
} ListaDupla;
void lista_dupla_inicializar(ListaDupla *l);
int lista_dupla_inserir(ListaDupla *l, Timestamp t);
int lista_dupla_remover(ListaDupla *l, Timestamp t);
void lista_dupla_listar_ordenado(const ListaDupla *l, VisitarTimestamp visitar, void *contexto);
void lista_dupla_liberar(ListaDupla *l);

#endif
