#ifndef AVL_H
#define AVL_H
#include "timestamp.h"
#include <stddef.h>

typedef struct NoAVL NoAVL;
typedef struct { NoAVL *raiz; size_t tamanho; } AVL;

void avl_inicializar(AVL *a);
int avl_inserir(AVL *a, Timestamp valor);
int avl_remover(AVL *a, const Timestamp *valor);
void avl_listar_ordenado(const AVL *a, void (*visitar)(const Timestamp *, void *), void *ctx);
void avl_liberar(AVL *a);

#endif
