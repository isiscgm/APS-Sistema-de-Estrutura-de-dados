APS - ESTRUTURAS DE DADOS
========================

Estruturas implementadas: Lista simplesmente encadeada, AVL e Lista duplamente encadeada.

ARQUIVOS
- timestamp.c/.h: parse, comparacao e impressao dos timestamps.
- lista.c/.h: lista simplesmente encadeada.
- avl.c/.h: arvore AVL.
- lista_dupla.c/.h: lista duplamente encadeada.
- main.c: teste de carga e exibicao dos 10 primeiros registros em ordem.
- benchmark.c: 10 repeticoes por tamanho e operacao; calcula mediana, minimo e maximo; grava CSV.
- gerar_timestamps.py: gera entradas de 1.000, 5.000, 10.000, 50.000 e 100.000 registros.
- analise_complexidade.py: script do material da APS, preservando os modelos e ajustes fornecidos; agora carrega os CSVs reais.

PASSO A PASSO (PowerShell/MinGW)
1) python gerar_timestamps.py
2) gcc -std=c11 -Wall -Wextra -O0 main.c timestamp.c lista.c avl.c lista_dupla.c -o main.exe
3) .\main.exe timestamps_1000.txt
4) gcc -std=c11 -Wall -Wextra -O0 benchmark.c timestamp.c lista.c avl.c lista_dupla.c -o benchmark.exe
5) .\benchmark.exe 3000000000
   Troque 3000000000 pela frequencia base em Hz que voces decidirem documentar/usar.
6) pip install numpy matplotlib
7) python analise_complexidade.py

SAIDAS
- experiment_data/estatisticas.csv: estrutura, operacao, n, mediana, minimo e maximo.
- experiment_data/*.csv: series n,cycles lidas pelo script de analise.
- O total ponderado usa 70% insercao, 15% remocao e 15% listagem ordenada.

CUIDADOS METODOLOGICOS
- Todos os testes usam a mesma sequencia de timestamps de cada arquivo.
- A preparacao da estrutura para remocao/listagem ocorre fora do intervalo cronometrado.
- A listagem ordenada e medida como operacao completa: nas listas, inclui copiar e ordenar os valores; na AVL, inclui percurso em ordem.
- A lista simples e a dupla usam qsort em uma copia temporaria, sem reorganizar os nos.
- Frequencia de CPU: informe em Hz ao executar benchmark.exe; nao use um valor inventado no relatorio.
- Compile com -O0 conforme instrucoes.


CORREÇÃO PARA WINDOWS/MINGW:
O benchmark usa _mkdir no Windows e mkdir em sistemas POSIX.
Os nomes dos arquivos usam %llu com conversão explícita para evitar avisos
em versões antigas do MinGW que não reconhecem %zu no printf.
