#include "estruturas.h"
#include <stdlib.h>

struct NoAVL {
    Timestamp valor;
    unsigned ocorrencias;
    int altura;
    struct NoAVL *esq, *dir;
};

static int maximo(int a, int b) { return a > b ? a : b; }
static int altura(NoAVL *n) { return n ? n->altura : 0; }
static void atualizar_altura(NoAVL *n) {
    n->altura = 1 + maximo(altura(n->esq), altura(n->dir));
}
static int fator_balanceamento(NoAVL *n) { return n ? altura(n->esq) - altura(n->dir) : 0; }

static NoAVL *rotacao_direita(NoAVL *y) {
    NoAVL *x = y->esq;
    NoAVL *meio = x->dir;
    x->dir = y;
    y->esq = meio;
    atualizar_altura(y);
    atualizar_altura(x);
    return x;
}
static NoAVL *rotacao_esquerda(NoAVL *x) {
    NoAVL *y = x->dir;
    NoAVL *meio = y->esq;
    y->esq = x;
    x->dir = meio;
    atualizar_altura(x);
    atualizar_altura(y);
    return y;
}
static NoAVL *balancear(NoAVL *n) {
    int fb;
    if (!n) return NULL;
    atualizar_altura(n);
    fb = fator_balanceamento(n);
    if (fb > 1) {
        if (fator_balanceamento(n->esq) < 0) n->esq = rotacao_esquerda(n->esq);
        return rotacao_direita(n);
    }
    if (fb < -1) {
        if (fator_balanceamento(n->dir) > 0) n->dir = rotacao_direita(n->dir);
        return rotacao_esquerda(n);
    }
    return n;
}

static NoAVL *inserir_rec(NoAVL *n, Timestamp t, int *ok) {
    int cmp;
    if (!n) {
        NoAVL *novo = (NoAVL *)malloc(sizeof(NoAVL));
        if (!novo) { *ok = 0; return NULL; }
        novo->valor = t; novo->ocorrencias = 1; novo->altura = 1;
        novo->esq = novo->dir = NULL;
        return novo;
    }
    cmp = timestamp_comparar(t, n->valor);
    if (cmp == 0) n->ocorrencias++;
    else if (cmp < 0) {
        n->esq = inserir_rec(n->esq, t, ok);
        if (!*ok) return n;
    } else {
        n->dir = inserir_rec(n->dir, t, ok);
        if (!*ok) return n;
    }
    return balancear(n);
}

void avl_inicializar(AVL *a) { a->raiz = NULL; a->tamanho = 0; }
int avl_inserir(AVL *a, Timestamp t) {
    int ok = 1;
    a->raiz = inserir_rec(a->raiz, t, &ok);
    if (ok) a->tamanho++;
    return ok;
}

static NoAVL *menor_no(NoAVL *n) {
    while (n && n->esq) n = n->esq;
    return n;
}
static NoAVL *remover_menor_no(NoAVL *n) {
    if (!n->esq) {
        NoAVL *direita = n->dir;
        free(n);
        return direita;
    }
    n->esq = remover_menor_no(n->esq);
    return balancear(n);
}
static NoAVL *remover_rec(NoAVL *n, Timestamp t, int *removido) {
    int cmp;
    if (!n) return NULL;
    cmp = timestamp_comparar(t, n->valor);
    if (cmp < 0) n->esq = remover_rec(n->esq, t, removido);
    else if (cmp > 0) n->dir = remover_rec(n->dir, t, removido);
    else {
        *removido = 1;
        if (n->ocorrencias > 1) {
            n->ocorrencias--;
            return n;
        }
        if (!n->esq || !n->dir) {
            NoAVL *filho = n->esq ? n->esq : n->dir;
            free(n);
            return filho;
        }
        {
            NoAVL *sucessor = menor_no(n->dir);
            n->valor = sucessor->valor;
            n->ocorrencias = sucessor->ocorrencias;
            n->dir = remover_menor_no(n->dir);
        }
    }
    return balancear(n);
}
int avl_remover(AVL *a, Timestamp t) {
    int removido = 0;
    a->raiz = remover_rec(a->raiz, t, &removido);
    if (removido) a->tamanho--;
    return removido;
}

static void listar_rec(const NoAVL *n, VisitarTimestamp visitar, void *contexto) {
    unsigned i;
    if (!n) return;
    listar_rec(n->esq, visitar, contexto);
    for (i = 0; i < n->ocorrencias; i++) visitar(&n->valor, contexto);
    listar_rec(n->dir, visitar, contexto);
}
void avl_listar_ordenado(const AVL *a, VisitarTimestamp visitar, void *contexto) {
    if (visitar) listar_rec(a->raiz, visitar, contexto);
}
static void liberar_rec(NoAVL *n) {
    if (!n) return;
    liberar_rec(n->esq);
    liberar_rec(n->dir);
    free(n);
}
void avl_liberar(AVL *a) { liberar_rec(a->raiz); a->raiz = NULL; a->tamanho = 0; }
