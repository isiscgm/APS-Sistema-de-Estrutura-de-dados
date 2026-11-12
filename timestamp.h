#ifndef TIMESTAMP_H
#define TIMESTAMP_H

typedef struct {
    int ano, mes, dia, hora, minuto, segundo;
} Timestamp;

int timestamp_parse(const char *texto, Timestamp *t);
int timestamp_comparar(const Timestamp *a, const Timestamp *b);
void timestamp_imprimir(const Timestamp *t);

#endif
