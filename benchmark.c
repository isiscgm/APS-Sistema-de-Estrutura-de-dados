#include "timestamp.h"
#include "lista.h"
#include "lista_dupla.h"
#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <direct.h>
#define CRIAR_PASTA(nome) _mkdir(nome)
#else
#include <sys/stat.h>
#define CRIAR_PASTA(nome) mkdir(nome, 0777)
#endif

#define REPETICOES 10
#define QTD_TAMANHOS 5
static const size_t tamanhos[QTD_TAMANHOS] = {1000, 5000, 10000, 50000, 100000};
static volatile unsigned long long checksum = 0;
static void consumir(const Timestamp *t, void *ctx) {
    (void)ctx;
    checksum += (unsigned long long)t->ano + (unsigned long long)t->segundo;
}
typedef struct { double valores[REPETICOES]; } Medidas;
static int cmp_double(const void *a, const void *b) {
    double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}
static int carregar(const char *nome, Timestamp **dados, size_t *n) {
    FILE *f = fopen(nome, "r");
    if (!f) { perror(nome); return 0; }
    size_t cap = 1024; *n = 0; *dados = malloc(cap * sizeof **dados);
    if (!*dados) { fclose(f); return 0; }
    char linha[128];
    while (fgets(linha, sizeof linha, f)) {
        linha[strcspn(linha, "\r\n")] = '\0';
        if (!linha[0]) continue;
        Timestamp t;
        if (!timestamp_parse(linha, &t)) continue;
        if (*n == cap) {
            cap *= 2; Timestamp *tmp = realloc(*dados, cap * sizeof **dados);
            if (!tmp) { free(*dados); fclose(f); return 0; }
            *dados = tmp;
        }
        (*dados)[(*n)++] = t;
    }
    fclose(f); return 1;
}
static double ciclos(clock_t ini, clock_t fim, double freq_hz) {
    return ((double)(fim - ini) / CLOCKS_PER_SEC) * freq_hz;
}
static int executar_lista(const Timestamp *v, size_t n, double freq, Medidas *ins, Medidas *rem, Medidas *lis) {
    for (int r = 0; r < REPETICOES; r++) {
        Lista l; lista_inicializar(&l);
        clock_t a = clock();
        for (size_t i=0;i<n;i++) if (!lista_inserir(&l,v[i])) return 0;
        clock_t b = clock(); ins->valores[r] = ciclos(a,b,freq);
        for (size_t i=0;i<n;i++) { /* estrutura já populada: preparação fora da medição */ }
        a = clock();
        for (size_t i=0;i<n;i++) lista_remover(&l,&v[i]);
        b = clock(); rem->valores[r] = ciclos(a,b,freq);
        lista_liberar(&l);

        lista_inicializar(&l);
        for (size_t i=0;i<n;i++) if (!lista_inserir(&l,v[i])) return 0;
        a = clock(); lista_listar_ordenado(&l,consumir,NULL); b = clock();
        lis->valores[r] = ciclos(a,b,freq);
        lista_liberar(&l);
    }
    return 1;
}
static int executar_dupla(const Timestamp *v, size_t n, double freq, Medidas *ins, Medidas *rem, Medidas *lis) {
    for (int r=0;r<REPETICOES;r++) {
        ListaDupla l; lista_dupla_inicializar(&l);
        clock_t a=clock();
        for(size_t i=0;i<n;i++) if(!lista_dupla_inserir(&l,v[i])) return 0;
        clock_t b=clock(); ins->valores[r]=ciclos(a,b,freq);
        a=clock(); for(size_t i=0;i<n;i++) lista_dupla_remover(&l,&v[i]); b=clock();
        rem->valores[r]=ciclos(a,b,freq); lista_dupla_liberar(&l);
        lista_dupla_inicializar(&l);
        for(size_t i=0;i<n;i++) if(!lista_dupla_inserir(&l,v[i])) return 0;
        a=clock(); lista_dupla_listar_ordenado(&l,consumir,NULL); b=clock();
        lis->valores[r]=ciclos(a,b,freq); lista_dupla_liberar(&l);
    }
    return 1;
}
static int executar_avl(const Timestamp *v, size_t n, double freq, Medidas *ins, Medidas *rem, Medidas *lis) {
    for(int r=0;r<REPETICOES;r++) {
        AVL a; avl_inicializar(&a); clock_t ini=clock();
        for(size_t i=0;i<n;i++) if(!avl_inserir(&a,v[i])) return 0;
        clock_t fim=clock(); ins->valores[r]=ciclos(ini,fim,freq);
        ini=clock(); for(size_t i=0;i<n;i++) avl_remover(&a,&v[i]); fim=clock();
        rem->valores[r]=ciclos(ini,fim,freq); avl_liberar(&a);
        avl_inicializar(&a); for(size_t i=0;i<n;i++) if(!avl_inserir(&a,v[i])) return 0;
        ini=clock(); avl_listar_ordenado(&a,consumir,NULL); fim=clock();
        lis->valores[r]=ciclos(ini,fim,freq); avl_liberar(&a);
    }
    return 1;
}
static void salvar_linha(FILE *f, const char *estrutura, const char *operacao, size_t n, Medidas m) {
    double copia[REPETICOES]; memcpy(copia,m.valores,sizeof copia);
    qsort(copia,REPETICOES,sizeof(double),cmp_double);
    double med=(copia[4]+copia[5])/2.0;
    fprintf(f,"%s,%s,%zu,%.3f,%.3f,%.3f\n",estrutura,operacao,n,med,copia[0],copia[9]);
}
int main(int argc,char **argv) {
    if(argc!=2) { fprintf(stderr,"Uso: %s frequencia_base_em_Hz\nExemplo: %s 3000000000\n",argv[0],argv[0]); return 1; }
    double freq=strtod(argv[1],NULL); if(freq<=0) { fprintf(stderr,"Frequencia invalida. Informe em Hz.\n"); return 1; }
    (void)CRIAR_PASTA("experiment_data");
    FILE *det=fopen("experiment_data/estatisticas.csv","w");
    if(!det) { perror("estatisticas.csv"); return 1; }
    fprintf(det,"estrutura,operacao,n,mediana_ciclos,minimo_ciclos,maximo_ciclos\n");
    for(int k=0;k<QTD_TAMANHOS;k++) {
        char nome[64]; snprintf(nome,sizeof nome,"timestamps_%u.txt",(unsigned int)tamanhos[k]);
        Timestamp *v=NULL; size_t n=0;
        if(!carregar(nome,&v,&n)) { fclose(det); return 1; }
        if(n!=tamanhos[k]) fprintf(stderr,"Aviso: %s tem %zu registros (esperado %zu).\n",nome,n,tamanhos[k]);
        const char *nomes[3]={"lista_simples","avl","lista_dupla"};
        Medidas medidas[3][3];
        int ok=executar_lista(v,n,freq,&medidas[0][0],&medidas[0][1],&medidas[0][2]) &&
               executar_avl(v,n,freq,&medidas[1][0],&medidas[1][1],&medidas[1][2]) &&
               executar_dupla(v,n,freq,&medidas[2][0],&medidas[2][1],&medidas[2][2]);
        if(!ok) { fprintf(stderr,"Falha de memoria no benchmark.\n"); free(v); fclose(det); return 1; }
        const char *ops[3]={"insercao","remocao","listagem_ordenada"};
        for(int e=0;e<3;e++) for(int o=0;o<3;o++) salvar_linha(det,nomes[e],ops[o],n,medidas[e][o]);
        /* Arquivos separados no formato n,y esperado pelo script de análise. */
        for(int e=0;e<3;e++) for(int o=0;o<3;o++) {
            char caminho[160]; snprintf(caminho,sizeof caminho,"experiment_data/%s_%s.csv",nomes[e],ops[o]);
            FILE *f=fopen(caminho,k==0?"w":"a"); if(!f) continue;
            if(k==0) fprintf(f,"n,cycles\n");
            double copia[REPETICOES]; memcpy(copia,medidas[e][o].valores,sizeof copia); qsort(copia,REPETICOES,sizeof(double),cmp_double);
            fprintf(f,"%zu,%.3f\n",n,(copia[4]+copia[5])/2.0); fclose(f);
        }
        free(v); printf("Benchmark concluido para n=%zu\n",n);
    }
    fclose(det);
    /* Série ponderada: 70%% inserção, 15%% remoção e 15%% listagem. */
    for(int e=0;e<3;e++) {
        char caminho[160]; snprintf(caminho,sizeof caminho,"experiment_data/%s_total_ponderado.csv",(const char*[]){"lista_simples","avl","lista_dupla"}[e]);
        FILE *f=fopen(caminho,"w"); if(!f) continue; fprintf(f,"n,cycles\n");
        for(int k=0;k<QTD_TAMANHOS;k++) {
            char nome[64]; snprintf(nome,sizeof nome,"timestamps_%u.txt",(unsigned int)tamanhos[k]); Timestamp *v=NULL; size_t n=0;
            if(!carregar(nome,&v,&n)) continue;
            free(v);
            /* Releitura dos resumos por arquivo: cada um possui cabeçalho e k+1 linhas. */
            const char *base_nomes[3]={"insercao","remocao","listagem_ordenada"}; double vals[3]={0};
            for(int o=0;o<3;o++) {
                snprintf(caminho,sizeof caminho,"experiment_data/%s_%s.csv",(const char*[]){"lista_simples","avl","lista_dupla"}[e],base_nomes[o]);
                FILE *g=fopen(caminho,"r"); if(!g) continue; char linha[128]; fgets(linha,sizeof linha,g);
                for(int j=0;j<=k;j++) if(fgets(linha,sizeof linha,g) && j==k) { char *comma=strchr(linha,','); if(comma) vals[o]=atof(comma+1); }
                fclose(g);
            }
            fprintf(f,"%zu,%.3f\n",tamanhos[k],0.70*vals[0]+0.15*vals[1]+0.15*vals[2]);
        }
        fclose(f);
    }
    printf("CSVs salvos em experiment_data/. Checksum: %llu\n",checksum);
    return 0;
}
