#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

/* Benchmark APS: 10 repeticoes, sem E/S no trecho cronometrado.
   Regra provisoria: remover os primeiros 10% dos registros, na ordem original.
   Listagem mede N registros antes de remocoes; percorre lista dupla ja ordenada.
   Ciclos estimados = segundos de CPU de clock() * frequencia informada (Hz).
   Nao sao ciclos reais medidos pelo processador. */
static volatile unsigned long long checksum = 0;
static void visitar(const Timestamp *t, void *ctx) {
    size_t *cont = (size_t *)ctx;
    (*cont)++;
    checksum += (unsigned)t->segundo;
}
static Timestamp *ler(const char *nome, size_t esperado) {
    FILE *f = fopen(nome, "r");
    Timestamp *dados = malloc(esperado * sizeof(*dados));
    char linha[128]; size_t i = 0;
    if (!f || !dados) { perror(nome); if (f) fclose(f); free(dados); return NULL; }
    while (fgets(linha, sizeof linha, f)) {
        if (i >= esperado || !timestamp_parse(linha, &dados[i])) {
            fprintf(stderr, "Arquivo invalido: %s, linha %zu\n", nome, i + 1);
            fclose(f); free(dados); return NULL;
        }
        i++;
    }
    fclose(f);
    if (i != esperado) { fprintf(stderr, "Esperados %zu, encontrados %zu em %s\n", esperado, i, nome); free(dados); return NULL; }
    return dados;
}
static int iniciar(int tipo, Lista *l, AVL *a, ListaDupla *v) {
    (void)tipo; lista_inicializar(l); avl_inicializar(a); lista_dupla_inicializar(v); return 1;
}
static void liberar(int tipo, Lista *l, AVL *a, ListaDupla *v) {
    if (tipo == 0) lista_liberar(l);
    else if (tipo == 1) avl_liberar(a);
    else lista_dupla_liberar(v);
}
static int inserir(int tipo, Lista *l, AVL *a, ListaDupla *v, Timestamp t) {
    if (tipo == 0) return lista_inserir(l, t);
    if (tipo == 1) return avl_inserir(a, t);
    return lista_dupla_inserir(v, t);
}
static int remover(int tipo, Lista *l, AVL *a, ListaDupla *v, Timestamp t) {
    if (tipo == 0) return lista_remover(l, t);
    if (tipo == 1) return avl_remover(a, t);
    return lista_dupla_remover(v, t);
}
static void listar(int tipo, Lista *l, AVL *a, ListaDupla *v, size_t *cont) {
    if (tipo == 0) lista_listar_ordenado(l, visitar, cont);
    else if (tipo == 1) avl_listar_ordenado(a, visitar, cont);
    else lista_dupla_listar_ordenado(v, visitar, cont);
}
int main(int argc, char **argv) {
    const size_t ns[] = {1000,2500,5000,7500,10000,15000,20000,25000,30000,40000,50000,60000,70000,80000,100000};
    const char *nomes[] = {"Lista", "AVL", "ListaDupla"};
    const char *ops[] = {"Insercao", "Remocao", "Listagem"};
    double freq; FILE *csv; size_t z;
    if (argc != 2 || (freq = atof(argv[1])) <= 0) {
        fprintf(stderr, "Uso: %s FREQUENCIA_HZ (ex.: 3000000000)\n", argv[0]); return 1;
    }
    csv = fopen("medicoes_brutas.csv", "w");
    if (!csv) { perror("medicoes_brutas.csv"); return 1; }
    fprintf(csv, "estrutura,N,operacao,repeticao,segundos_cpu,ciclos_estimados,frequencia_hz\n");
    for (z = 0; z < sizeof(ns)/sizeof(ns[0]); z++) {
        char arquivo[80]; Timestamp *dados; size_t n = ns[z]; int tipo, op, rep;
        snprintf(arquivo, sizeof arquivo, "timestamps_100000.txt");
        dados = ler(arquivo, 100000); /* Cada N usa o prefixo da MESMA massa aleatoria. */
        if (!dados) { fclose(csv); return 1; }
        for (tipo = 0; tipo < 3; tipo++) for (op = 0; op < 3; op++) for (rep = 1; rep <= 10; rep++) {
            Lista l; AVL a; ListaDupla v; size_t i, limite = n / 10, cont = 0;
            clock_t inicio, fim; double segundos; int ok = 1; size_t vezes_listagem = 1;
            iniciar(tipo, &l, &a, &v);
            /* Preenchimento de remocao e listagem ocorre fora da medicao. */
            if (op != 0) for (i = 0; i < n; i++) {
                if (!inserir(tipo, &l, &a, &v, dados[i])) { ok = 0; break; }
            }
            if (!ok) { fprintf(stderr, "Falha de memoria na preparacao\n"); liberar(tipo,&l,&a,&v); free(dados); fclose(csv); return 1; }
            inicio = clock();
            if (op == 0) for (i = 0; i < n; i++) {
                if (!inserir(tipo, &l, &a, &v, dados[i])) { ok = 0; break; }
            }
            else if (op == 1) for (i = 0; i < limite; i++) {
                if (!remover(tipo, &l, &a, &v, dados[i])) { ok = 0; break; }
            }
            else {
                /* Repetir a mesma travessia por pelo menos 0,25 s de CPU para reduzir ruído de clock().
                   O valor final e a media por travessia, nao a soma. */
                vezes_listagem = 0;
                do {
                    listar(tipo, &l, &a, &v, &cont);
                    vezes_listagem++;
                } while ((double)(clock()-inicio)/CLOCKS_PER_SEC < 0.25 && vezes_listagem < 100000);
            }
            fim = clock();
            if (!ok || (op == 2 && cont != n * vezes_listagem)) {
                fprintf(stderr, "Falha de operacao/verificacao: %s %s N=%zu rep=%d\n", nomes[tipo], ops[op], n, rep);
                liberar(tipo,&l,&a,&v); free(dados); fclose(csv); return 1;
            }
            segundos = (double)(fim - inicio) / CLOCKS_PER_SEC / (double)vezes_listagem;
            fprintf(csv, "%s,%zu,%s,%d,%.9f,%.3f,%.0f\n", nomes[tipo],n,ops[op],rep,segundos,segundos*freq,freq);
            fflush(csv);
            liberar(tipo, &l, &a, &v);
        }
        fprintf(stderr, "Concluido N=%zu\n", n);
        free(dados);
    }
    fclose(csv);
    fprintf(stderr, "Concluido: medicoes_brutas.csv (checksum=%llu)\n", checksum);
    return 0;
}
