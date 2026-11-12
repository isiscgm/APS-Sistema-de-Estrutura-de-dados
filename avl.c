#include "avl.h"
#include <stdlib.h>

struct NoAVL { Timestamp valor; int altura; struct NoAVL *esq, *dir; };

static int altura(NoAVL *n) { return n ? n->altura : 0; }
static int maximo(int a, int b) { return a > b ? a : b; }
static void atualizar(NoAVL *n) { n->altura = 1 + maximo(altura(n->esq), altura(n->dir)); }
static int fator(NoAVL *n) { return n ? altura(n->esq) - altura(n->dir) : 0; }

static NoAVL *rotacao_dir(NoAVL *y) {
    NoAVL *x = y->esq, *t = x->dir;
    x->dir = y; y->esq = t; atualizar(y); atualizar(x); return x;
}
static NoAVL *rotacao_esq(NoAVL *x) {
    NoAVL *y = x->dir, *t = y->esq;
    y->esq = x; x->dir = t; atualizar(x); atualizar(y); return y;
}
static NoAVL *balancear(NoAVL *n) {
    if (!n) return n;
    atualizar(n);
    if (fator(n) > 1) {
        if (fator(n->esq) < 0) n->esq = rotacao_esq(n->esq);
        return rotacao_dir(n);
    }
    if (fator(n) < -1) {
        if (fator(n->dir) > 0) n->dir = rotacao_dir(n->dir);
        return rotacao_esq(n);
    }
    return n;
}
static NoAVL *inserir_no(NoAVL *r, Timestamp v, int *ok) {
    if (!r) {
        NoAVL *n = malloc(sizeof *n);
        if (!n) { *ok = 0; return NULL; }
        n->valor = v; n->altura = 1; n->esq = n->dir = NULL; return n;
    }
    int c = timestamp_comparar(&v, &r->valor);
    if (c < 0) r->esq = inserir_no(r->esq, v, ok);
    else if (c > 0) r->dir = inserir_no(r->dir, v, ok);
    else return r;
    return *ok ? balancear(r) : r;
}
void avl_inicializar(AVL *a) { a->raiz = NULL; a->tamanho = 0; }
static int contem(NoAVL *p, const Timestamp *v) {
    while (p) {
        int c = timestamp_comparar(v, &p->valor);
        if (!c) return 1;
        p = c < 0 ? p->esq : p->dir;
    }
    return 0;
}
int avl_inserir(AVL *a, Timestamp v) {
    if (contem(a->raiz, &v)) return 1;
    int ok = 1;
    a->raiz = inserir_no(a->raiz, v, &ok);
    if (ok) a->tamanho++;
    return ok;
}
static NoAVL *minimo(NoAVL *n) { while (n && n->esq) n = n->esq; return n; }
static NoAVL *remover_no(NoAVL *r, const Timestamp *v, int *removeu) {
    if (!r) return NULL;
    int c = timestamp_comparar(v, &r->valor);
    if (c < 0) r->esq = remover_no(r->esq, v, removeu);
    else if (c > 0) r->dir = remover_no(r->dir, v, removeu);
    else {
        *removeu = 1;
        if (!r->esq || !r->dir) { NoAVL *filho = r->esq ? r->esq : r->dir; free(r); return filho; }
        NoAVL *s = minimo(r->dir); r->valor = s->valor;
        int ignorar = 0; r->dir = remover_no(r->dir, &s->valor, &ignorar);
    }
    return balancear(r);
}
int avl_remover(AVL *a, const Timestamp *v) {
    int removeu = 0; a->raiz = remover_no(a->raiz, v, &removeu);
    if (removeu) a->tamanho--;
    return removeu;
}
static void percorrer(NoAVL *n, void (*visitar)(const Timestamp *, void *), void *ctx) {
    if (!n) return;
    percorrer(n->esq, visitar, ctx);
    visitar(&n->valor, ctx);
    percorrer(n->dir, visitar, ctx);
}
void avl_listar_ordenado(const AVL *a, void (*visitar)(const Timestamp *, void *), void *ctx) {
    if (a && visitar) percorrer(a->raiz, visitar, ctx);
}
static void liberar_no(NoAVL *n) { if (n) { liberar_no(n->esq); liberar_no(n->dir); free(n); } }
void avl_liberar(AVL *a) { liberar_no(a->raiz); a->raiz = NULL; a->tamanho = 0; }
