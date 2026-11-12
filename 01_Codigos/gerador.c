#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int bissexto_local(int ano) {
    return (ano % 400 == 0) || (ano % 4 == 0 && ano % 100 != 0);
}
static int dias_mes(int mes, int ano) {
    static const int dias[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (mes == 2 && bissexto_local(ano)) return 29;
    return dias[mes];
}
static Timestamp gerar_timestamp(void) {
    Timestamp t;
    t.ano = 2000 + rand() % 36;       /* 2000 a 2035 */
    t.mes = 1 + rand() % 12;
    t.dia = 1 + rand() % dias_mes(t.mes, t.ano);
    t.hora = rand() % 24;
    t.minuto = rand() % 60;
    t.segundo = rand() % 60;
    return t;
}
static int gerar_arquivo(size_t quantidade) {
    char nome[64];
    FILE *arquivo;
    size_t i;
    snprintf(nome, sizeof(nome), "timestamps_%lu.txt", (unsigned long)quantidade);
    arquivo = fopen(nome, "w");
    if (!arquivo) { perror(nome); return 0; }
    for (i = 0; i < quantidade; i++) {
        Timestamp t = gerar_timestamp();
        if (fprintf(arquivo, "%04d-%02d-%02d %02d:%02d:%02d\n",
                    t.ano, t.mes, t.dia, t.hora, t.minuto, t.segundo) < 0) {
            fclose(arquivo); return 0;
        }
    }
    if (fclose(arquivo) != 0) return 0;
    printf("Gerado: %s (%lu registros)\n", nome, (unsigned long)quantidade);
    return 1;
}
int main(void) {
    const size_t tamanhos[] = {1000, 5000, 10000, 50000, 100000};
    size_t i;
    srand((unsigned)time(NULL));
    for (i = 0; i < sizeof(tamanhos) / sizeof(tamanhos[0]); i++)
        if (!gerar_arquivo(tamanhos[i])) return 1;
    return 0;
}
