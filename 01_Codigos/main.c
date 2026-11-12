#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct LimiteImpressao { size_t impressos; size_t limite; };
static void imprimir_ate_limite(const Timestamp *t, void *contexto) {
    struct LimiteImpressao *c = (struct LimiteImpressao *)contexto;
    if (c->impressos < c->limite) {
        timestamp_imprimir(t);
        putchar('\n');
        c->impressos++;
    }
}
static int carregar_arquivo(const char *nome, Lista *l, AVL *a, ListaDupla *v) {
    FILE *f = fopen(nome, "r");
    char linha[128];
    size_t numero_linha = 0;
    if (!f) { perror(nome); return 0; }
    while (fgets(linha, sizeof(linha), f)) {
        Timestamp t;
        numero_linha++;
        linha[strcspn(linha, "\r\n")] = '\0';
        if (!timestamp_parse(linha, &t)) {
            fprintf(stderr, "Timestamp invalido na linha %lu: %s\n",
                    (unsigned long)numero_linha, linha);
            fclose(f); return 0;
        }
        /* O mesmo registro, na mesma ordem, entra nas tres estruturas. */
        if (!lista_inserir(l, t) || !avl_inserir(a, t) || !lista_dupla_inserir(v, t)) {
            fprintf(stderr, "Falha de memoria ao inserir a linha %lu.\n", (unsigned long)numero_linha);
            fclose(f); return 0;
        }
    }
    if (ferror(f)) { perror("Erro de leitura"); fclose(f); return 0; }
    fclose(f);
    return 1;
}
int main(int argc, char **argv) {
    const char *arquivo = argc > 1 ? argv[1] : "timestamps_1000.txt";
    Lista lista;
    AVL avl;
    ListaDupla lista_dupla;
    struct LimiteImpressao contexto;

    lista_inicializar(&lista);
    avl_inicializar(&avl);
    lista_dupla_inicializar(&lista_dupla);

    if (!carregar_arquivo(arquivo, &lista, &avl, &lista_dupla)) {
        lista_liberar(&lista); avl_liberar(&avl); lista_dupla_liberar(&lista_dupla);
        return 1;
    }

    printf("Arquivo: %s\n", arquivo);
    printf("Registros carregados - Lista: %lu | AVL: %lu | ListaDupla: %lu\n",
           (unsigned long)lista.tamanho, (unsigned long)avl.tamanho,
           (unsigned long)lista_dupla.tamanho);
    puts("\nPrimeiros 10 registros em ordem (lista):");
    contexto.impressos = 0; contexto.limite = 10;
    lista_listar_ordenado(&lista, imprimir_ate_limite, &contexto);
    puts("\nPrimeiros 10 registros em ordem (AVL):");
    contexto.impressos = 0;
    avl_listar_ordenado(&avl, imprimir_ate_limite, &contexto);
    puts("\nPrimeiros 10 registros em ordem (lista_dupla):");
    contexto.impressos = 0;
    lista_dupla_listar_ordenado(&lista_dupla, imprimir_ate_limite, &contexto);

    lista_liberar(&lista);
    avl_liberar(&avl);
    lista_dupla_liberar(&lista_dupla);
    return 0;
}
