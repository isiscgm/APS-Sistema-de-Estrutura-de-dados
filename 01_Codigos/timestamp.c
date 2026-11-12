#include "estruturas.h"
#include <stdio.h>

static int bissexto(int ano) {
    return (ano % 400 == 0) || (ano % 4 == 0 && ano % 100 != 0);
}

static int dias_no_mes(int mes, int ano) {
    static const int dias[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (mes < 1 || mes > 12) return 0;
    if (mes == 2 && bissexto(ano)) return 29;
    return dias[mes];
}

int timestamp_parse(const char *texto, Timestamp *saida) {
    int lidos;
    char extra;
    Timestamp t;

    if (texto == NULL || saida == NULL) return 0;
    lidos = sscanf(texto, "%4d-%2d-%2d %2d:%2d:%2d %c",
                   &t.ano, &t.mes, &t.dia, &t.hora, &t.minuto, &t.segundo, &extra);
    if (lidos != 6) return 0;
    if (t.ano < 1 || t.mes < 1 || t.mes > 12) return 0;
    if (t.dia < 1 || t.dia > dias_no_mes(t.mes, t.ano)) return 0;
    if (t.hora < 0 || t.hora > 23 || t.minuto < 0 || t.minuto > 59 ||
        t.segundo < 0 || t.segundo > 59) return 0;
    *saida = t;
    return 1;
}

int timestamp_comparar(Timestamp a, Timestamp b) {
    if (a.ano != b.ano) return (a.ano < b.ano) ? -1 : 1;
    if (a.mes != b.mes) return (a.mes < b.mes) ? -1 : 1;
    if (a.dia != b.dia) return (a.dia < b.dia) ? -1 : 1;
    if (a.hora != b.hora) return (a.hora < b.hora) ? -1 : 1;
    if (a.minuto != b.minuto) return (a.minuto < b.minuto) ? -1 : 1;
    if (a.segundo != b.segundo) return (a.segundo < b.segundo) ? -1 : 1;
    return 0;
}

void timestamp_imprimir(const Timestamp *t) {
    printf("%04d-%02d-%02d %02d:%02d:%02d",
           t->ano, t->mes, t->dia, t->hora, t->minuto, t->segundo);
}
