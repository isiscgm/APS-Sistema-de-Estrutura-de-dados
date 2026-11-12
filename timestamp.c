#include "timestamp.h"
#include <stdio.h>
#include <string.h>

int timestamp_parse(const char *texto, Timestamp *t) {
    if (!texto || !t) return 0;

    int n = sscanf(texto, "%4d-%2d-%2d %2d:%2d:%2d",
                   &t->ano, &t->mes, &t->dia,
                   &t->hora, &t->minuto, &t->segundo);

    if (n != 6) return 0;
    if (t->ano < 1 || t->mes < 1 || t->mes > 12 ||
        t->dia < 1 || t->dia > 31 || t->hora < 0 ||
        t->hora > 23 || t->minuto < 0 || t->minuto > 59 ||
        t->segundo < 0 || t->segundo > 59) return 0;

    return 1;
}

int timestamp_comparar(const Timestamp *a, const Timestamp *b) {
    if (a->ano != b->ano) return (a->ano < b->ano) ? -1 : 1;
    if (a->mes != b->mes) return (a->mes < b->mes) ? -1 : 1;
    if (a->dia != b->dia) return (a->dia < b->dia) ? -1 : 1;
    if (a->hora != b->hora) return (a->hora < b->hora) ? -1 : 1;
    if (a->minuto != b->minuto) return (a->minuto < b->minuto) ? -1 : 1;
    if (a->segundo != b->segundo) return (a->segundo < b->segundo) ? -1 : 1;
    return 0;
}

void timestamp_imprimir(const Timestamp *t) {
    printf("%04d-%02d-%02d %02d:%02d:%02d",
           t->ano, t->mes, t->dia, t->hora, t->minuto, t->segundo);
}
