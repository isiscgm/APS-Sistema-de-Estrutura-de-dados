# 🚀 APS — Sistema de Estruturas de Dados

<p align="center">
  <strong>Implementação e análise experimental de estruturas de dados em linguagem C.</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Linguagem-C-blue?style=for-the-badge&logo=c" alt="Linguagem C">
  <img src="https://img.shields.io/badge/Compilador-GCC-red?style=for-the-badge&logo=gnu" alt="GCC">
  <img src="https://img.shields.io/badge/Python-3-yellow?style=for-the-badge&logo=python" alt="Python">
  <img src="https://img.shields.io/badge/Projeto-APS-purple?style=for-the-badge" alt="Projeto APS">
</p>

---

## 📌 Sobre o projeto

Este projeto foi desenvolvido como parte da **Atividade Prática Supervisionada (APS)** da disciplina de Estruturas de Dados.

O objetivo é implementar e comparar diferentes estruturas de dados utilizando a linguagem C, realizando experimentos para analisar o desempenho das operações conforme o volume de registros aumenta.

Os testes consideram registros de timestamps recebidos fora de ordem cronológica, avaliando três operações principais:

- Inserção;
- Remoção;
- Listagem ordenada.

## 🌳 1. Estruturas de dados implementadas

O projeto utiliza três estruturas de dados:

| Estrutura | Descrição |
|:---|:---|
| **Lista simplesmente encadeada** | Estrutura linear formada por nós ligados por ponteiros. |
| **Lista duplamente encadeada** | Estrutura linear cujos nós possuem referências para o elemento anterior e o próximo. |
| **Árvore AVL** | Árvore binária de busca autobalanceada, que utiliza rotações para manter seu equilíbrio. |

> **Observação:** não é utilizada estrutura de vetor como uma das estruturas avaliadas no projeto.

## 🛠️ 2. Tecnologias e ferramentas

| Tecnologia | Utilização |
|:---|:---|
| C | Implementação das estruturas e operações |
| GCC | Compilação dos programas C |
| Python 3 | Geração de dados e análise dos resultados |
| NumPy | Cálculos e ajustes matemáticos |
| Matplotlib | Geração dos gráficos |
| CSV | Armazenamento das medições |
| Git e GitHub | Versionamento e hospedagem do projeto |

## 💻 3. Pré-requisitos

Antes de executar o projeto, é necessário instalar:

### 3.1 GCC — Compilador C

No Windows, pode ser utilizado o MinGW.

Após a instalação, adicione a pasta `bin` do compilador à variável de ambiente `PATH`.

Verifique a instalação no terminal:

```bash
gcc --version
```

Se aparecer a versão do GCC, a instalação está funcionando.

### 3.2 Python 3

Instale o Python pelo site oficial:

🔗 [Download Python](https://www.python.org/downloads/)

Durante a instalação, marque a opção **Add Python to PATH**.

Verifique a instalação:

```bash
python --version
```

Caso não funcione, tente:

```bash
py --version
```

### 3.3 Bibliotecas Python

O projeto utiliza NumPy e Matplotlib.

Instale as dependências:

```bash
python -m pip install numpy matplotlib
```

Verifique se foram instaladas corretamente:

```bash
python -c "import numpy; import matplotlib; print('Bibliotecas instaladas com sucesso!')"
```

> Não é necessário instalar bibliotecas Python para executar os programas compilados em C.

## 📥 4. Baixando o projeto

Existem duas maneiras de obter os arquivos do projeto.

### Opção 1 — Download ZIP

1. Acesse o repositório no GitHub.
2. Clique em **Code**.
3. Selecione **Download ZIP**.
4. Extraia o arquivo em uma pasta do computador.
5. Abra a pasta no VS Code.

### Opção 2 — Git

No terminal, execute:

```bash
git clone https://github.com/isiscgm/APS-Sistema-de-Estrutura-de-dados.git
```

Entre na pasta:

```bash
cd APS-Sistema-de-Estrutura-de-dados
```

## ⚙️ 5. Compilando e executando os programas

Abra o terminal dentro da pasta do projeto.

### 5.1 Compilar o programa principal

```bash
gcc -std=c11 -Wall -Wextra -O0 main.c timestamp.c lista.c avl.c lista_dupla.c -o main.exe
```

Se não aparecer nenhum erro, o arquivo `main.exe` será criado.

### 5.2 Executar o programa principal

```powershell
.\main.exe timestamps_1000.txt
```

O arquivo `timestamps_1000.txt` é passado como argumento para o programa, que utiliza esse arquivo como entrada para processar os registros nas três estruturas.

> **Importante:** o arquivo de timestamps precisa existir no caminho informado. Caso esteja em outra pasta, informe o caminho correto.

### 5.3 Compilar o benchmark

O benchmark é responsável por realizar os experimentos e registrar as medições de desempenho.

```bash
gcc -std=c11 -Wall -Wextra -O0 benchmark.c timestamp.c lista.c avl.c lista_dupla.c -o benchmark.exe
```

Se a compilação terminar sem erros, será criado o arquivo `benchmark.exe`.

### 5.4 Executar o benchmark

```powershell
.\benchmark.exe
```

Durante a execução, o programa poderá solicitar informações, como a frequência do processador.

Informe o valor solicitado, respeitando a unidade indicada no terminal.

## 📊 6. Gerando os arquivos de timestamps

O projeto possui um script Python para gerar os arquivos de entrada utilizados nos testes.

Execute:

```bash
python gerar_timestamps.py
```

O script gera os arquivos de timestamps utilizados pelo programa principal e pelos experimentos.

Os arquivos gerados devem permanecer nos diretórios esperados pelos programas.

## 🧪 7. Realizando os experimentos

Após gerar os arquivos de entrada e compilar o benchmark:

1. Execute o `benchmark.exe`.
2. Informe a frequência do processador quando solicitada.
3. Aguarde a realização das operações.
4. Verifique os arquivos CSV gerados na pasta `experiment_data`.

Os arquivos CSV armazenam os resultados das medições, relacionando o tamanho da entrada ao custo medido durante os experimentos.

Esses dados serão utilizados posteriormente na análise gráfica.

## 📈 8. Gerando os gráficos

Com os arquivos CSV gerados, execute:

```bash
python gerar_graficos.py
```

O script utiliza NumPy e Matplotlib para:

- Ler os arquivos CSV;
- Filtrar medições inválidas;
- Ajustar modelos matemáticos aos dados;
- Comparar diferentes classes de complexidade;
- Calcular erros relativos e R²;
- Gerar gráficos de comparação e resíduos.

Os gráficos são salvos na pasta `graficos`.

### 8.1 Modelos de complexidade analisados

| Modelo | Descrição |
|:---:|:---|
| O(1) | Constante |
| O(log n) | Logarítmica |
| O(n) | Linear |
| O(n log n) | Linearítmica |
| O(n^1.5) | Polinomial |
| O(n²) | Quadrática |
| O(n³) | Cúbica |

### 8.2 Gráficos produzidos

| Gráfico | Descrição |
|:---|:---|
| **Linear-linear** | Apresenta os dados medidos e os modelos ajustados em escalas convencionais. |
| **Log-linear** | Utiliza escala logarítmica no eixo horizontal, facilitando a visualização das tendências. |
| **Log-log** | Utiliza escala logarítmica nos dois eixos, ajudando a observar padrões de crescimento polinomial. |
| **Resíduos** | Apresenta a diferença percentual entre os valores medidos e os valores estimados pelos modelos. |

### 8.3 Interpretando os resultados

O programa informa qual modelo apresentou o melhor ajuste entre os modelos avaliados.

Também apresenta:

- **Erro relativo:** indica a diferença entre as medições e as previsões.
- **R²:** indica o quanto o modelo explica a variação dos dados.
- **Custo fixo:** representa a parcela constante estimada pelo ajuste.
- **Expoente global:** indica a tendência de crescimento estimada pelos dados.

> A complexidade identificada experimentalmente deve ser comparada com a complexidade teórica de cada operação. O ajuste matemático, isoladamente, não comprova a complexidade assintótica.

## 🔄 9. Ordem recomendada de execução

Para executar o projeto completo do zero, siga esta sequência:

| Etapa | Ação |
|:---:|:---|
| 1 | Instalar GCC, Python 3, NumPy e Matplotlib |
| 2 | Gerar os dados de entrada |
| 3 | Compilar o programa principal |
| 4 | Executar o programa principal |
| 5 | Compilar o benchmark |
| 6 | Executar os experimentos |
| 7 | Gerar os gráficos |
| 8 | Consultar os resultados |

**Comandos principais:**

```bash
# Gerar os dados
python gerar_timestamps.py

# Compilar o programa principal
gcc -std=c11 -Wall -Wextra -O0 main.c timestamp.c lista.c avl.c lista_dupla.c -o main.exe

# Executar o programa principal
.\main.exe timestamps_1000.txt

# Compilar o benchmark
gcc -std=c11 -Wall -Wextra -O0 benchmark.c timestamp.c lista.c avl.c lista_dupla.c -o benchmark.exe

# Executar os experimentos
.\benchmark.exe

# Gerar os gráficos
python gerar_graficos.py
```

### 📂 Onde encontrar os resultados?

| Arquivos | Localização |
|:---|:---|
| Resultados das medições (CSV) | `experiment_data` |
| Gráficos gerados | `graficos` |

## 🔧 10. Solução de problemas

| Problema | Possível solução |
|:---|:---|
| `gcc não é reconhecido` | Verificar a instalação do GCC e a variável PATH. |
| `python não é reconhecido` | Verificar a instalação do Python ou utilizar `py`. |
| `No module named numpy` | Executar `python -m pip install numpy`. |
| `No module named matplotlib` | Executar `python -m pip install matplotlib`. |
| `No such file or directory` | Verificar se está na pasta correta e se o arquivo existe. |
| `At least 3 points...` | Verificar se os CSVs possuem pelo menos três medições válidas. |
| Executável não encontrado | Compilar novamente o programa C. |

## 👥 11. Integrantes

- **Isis Cagliumi**
- **Cauã**
- **Yasmin**

---

<p align="center">
  <strong>📚 Projeto acadêmico — Atividade Prática Supervisionada (APS)</strong>
  <br>
  Desenvolvido para fins educacionais, com foco em estruturas de dados, análise de algoritmos e avaliação experimental de desempenho.
</p>
