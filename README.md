APS — Sistema de Estruturas de Dados
Projeto desenvolvido como parte da Atividade Prática Supervisionada (APS) da disciplina de Estruturas de Dados.
O objetivo é implementar e comparar diferentes estruturas de dados utilizando a linguagem C, realizando experimentos para analisar o desempenho das operações conforme o volume de registros aumenta.
1. Estruturas implementadas
O projeto utiliza três estruturas de dados:
Lista Encadeada: estrutura linear formada por nós ligados por ponteiros.
Lista Duplamente Encadeada: estrutura linear cujos nós possuem referências para o elemento anterior e o próximo.
Árvore AVL: árvore binária de busca autobalanceada, que utiliza rotações para manter seu equilíbrio.
Não é utilizada estrutura de vetor como uma das estruturas avaliadas no projeto.
2. Tecnologias e ferramentas
Tecnologia	Utilização
C	Implementação das estruturas e operações
GCC	Compilação dos programas C
Python 3	Geração de dados e análise dos resultados
NumPy	Cálculos e ajustes matemáticos
Matplotlib	Geração dos gráficos
CSV	Armazenamento das medições
Git e GitHub	Versionamento do projeto
3. Pré-requisitos
Antes de executar o projeto, é necessário instalar:
3.1. GCC (compilador C)
No Windows, pode ser utilizado o MinGW.
Após a instalação, adicione a pasta bin do compilador à variável de ambiente PATH.
Abra o terminal do VS Code ou PowerShell e execute:
gcc --version
Se aparecer a versão do GCC, a instalação está funcionando.
Caso apareça que o comando não foi reconhecido, verifique a instalação e a configuração do PATH.
3.2. Python
Instale o Python 3 pelo site oficial:
Download Python | Python.org
Durante a instalação, marque a opção Add Python to PATH.
Verifique a instalação:
python --version
Se o comando não funcionar, tente:
py --version
3.3. Bibliotecas Python
O projeto utiliza NumPy e Matplotlib.
Instale as dependências com:
python -m pip install numpy matplotlib
Verifique se foram instaladas:
python -c "import numpy; import matplotlib; print('Bibliotecas instaladas com sucesso!')"
Não é necessário instalar bibliotecas Python para executar os programas compilados em C.
4. Baixando o projeto
É possível baixar o projeto pelo GitHub ou utilizar o Git.
Opção 1 — Download ZIP
Acesse o repositório.
Clique em Code.
Selecione Download ZIP.
Extraia o arquivo em uma pasta do computador.
Abra essa pasta no VS Code.
Opção 2 — Git
No terminal, execute:
git clone https://github.com/isiscgm/APS-Sistema-de-Estrutura-de-dados.git
Entre na pasta:
cd APS-Sistema-de-Estrutura-de-dados
5. Compilando os programas em C
A compilação transforma os arquivos .c em executáveis .exe.
Abra o terminal dentro da pasta do projeto.
5.1. Compilar o programa principal
Execute:
gcc -std=c11 -Wall -Wextra -O0 main.c timestamp.c lista.c avl.c lista_dupla.c -o main.exe
Se não aparecer nenhum erro, o arquivo main.exe será criado.
5.2. Executar o programa principal
Para executar:
.\main.exe timestamps_1000.txt
O arquivo timestamps_1000.txt é passado como argumento para o programa.
O programa utiliza esse arquivo como entrada para processar os registros nas estruturas implementadas.
Importante: o arquivo de timestamps precisa existir no caminho informado. Se estiver em outra pasta, informe o caminho correto.
5.3. Compilar o benchmark
O benchmark é responsável por realizar os experimentos e registrar as medições de desempenho.
Execute:
gcc -std=c11 -Wall -Wextra -O0 benchmark.c timestamp.c lista.c avl.c lista_dupla.c -o benchmark.exe
Se a compilação terminar sem erros, será criado o arquivo benchmark.exe.
5.4. Executar o benchmark
Execute:
.\benchmark.exe
Durante a execução, o programa poderá solicitar informações, como a frequência do processador.
Informe o valor solicitado pelo programa, respeitando a unidade indicada na mensagem exibida no terminal.
O benchmark realiza as operações programadas e registra os tempos de execução.
6. Gerando os arquivos de timestamps
O projeto possui um script Python para gerar os arquivos de entrada utilizados nos testes.
Execute:
python gerar_timestamps.py
O script gera os arquivos de timestamps utilizados pelo programa principal e pelos experimentos.
Os arquivos gerados devem permanecer nos diretórios esperados pelos programas.
Caso o script solicite parâmetros, siga as instruções exibidas no terminal.
7. Realizando os experimentos
Após gerar os arquivos de entrada e compilar o benchmark:
Execute o benchmark.exe.
Informe a frequência do processador quando solicitada.
Aguarde a realização das operações.
Verifique os arquivos CSV gerados na pasta experiment_data.
Os CSVs armazenam os resultados das medições, relacionando o tamanho da entrada ao custo medido durante os experimentos.
Esses dados serão utilizados posteriormente na análise gráfica.
8. Gerando os gráficos
Com os arquivos CSV gerados, execute:
python gerar_graficos.py
O script utiliza as bibliotecas NumPy e Matplotlib para:
Ler os arquivos CSV.
Filtrar medições inválidas.
Ajustar modelos matemáticos aos dados.
Comparar diferentes classes de complexidade.
Calcular erros relativos e R².
Gerar gráficos de comparação e resíduos.
Os gráficos são salvos na pasta graficos.
8.1. Modelos de complexidade analisados
Modelo	Descrição
O(1)	Constante
O(log n)	Logarítmica
O(n)	Linear
O(n log n)	Linearítmica
O(n^1.5)	Polinomial
O(n²)	Quadrática
O(n³)	Cúbica
8.2. Gráficos produzidos
Linear-linear: apresenta os dados medidos e os modelos ajustados em escalas convencionais.
Log-linear: utiliza escala logarítmica no eixo horizontal, facilitando a visualização de tendências conforme o tamanho da entrada cresce.
Log-log: utiliza escala logarítmica nos dois eixos, ajudando a observar padrões de crescimento polinomial.
Resíduos: apresenta a diferença percentual entre os valores medidos e os valores estimados pelos modelos.
8.3. Interpretando os resultados
O programa informa qual modelo apresentou o melhor ajuste entre os modelos avaliados.
Também apresenta:
Erro relativo: indica a diferença entre as medições e as previsões.
R²: indica o quanto o modelo explica a variação dos dados.
Custo fixo: representa a parcela constante estimada pelo ajuste.
Expoente global: indica a tendência de crescimento estimada pelos dados.
A complexidade identificada experimentalmente deve ser comparada com a complexidade teórica de cada operação. O ajuste matemático, isoladamente, não comprova a complexidade assintótica.
9. Ordem recomendada de execução
Para executar o projeto completo do zero:
Etapa 1 — Instalar as ferramentas
GCC
Python 3
NumPy
Matplotlib
Etapa 2 — Gerar os dados de entrada
python gerar_timestamps.py
Etapa 3 — Compilar o programa principal
gcc -std=c11 -Wall -Wextra -O0 main.c timestamp.c lista.c avl.c lista_dupla.c -o main.exe
Etapa 4 — Executar o programa principal
.\main.exe timestamps_1000.txt
Etapa 5 — Compilar o benchmark
gcc -std=c11 -Wall -Wextra -O0 benchmark.c timestamp.c lista.c avl.c lista_dupla.c -o benchmark.exe
Etapa 6 — Executar os experimentos
.\benchmark.exe
Etapa 7 — Gerar os gráficos
python gerar_graficos.py
Etapa 8 — Consultar os resultados
Arquivos CSV: pasta experiment_data.
Gráficos: pasta graficos.
10. Solução de problemas
Problema	Possível solução
gcc não é reconhecido	Verificar instalação do GCC e variável PATH
python não é reconhecido	Verificar instalação do Python ou utilizar py
No module named numpy	Executar python -m pip install numpy
No module named matplotlib	Executar python -m pip install matplotlib
No such file or directory	Verificar se está na pasta correta e se o arquivo existe
At least 3 points...	Verificar se os CSVs possuem pelo menos três medições válidas
Executável não encontrado	Compilar novamente o programa C
11. Integrantes
Isis Cagliumi
Cauã
Yasmin
Projeto acadêmico — Atividade Prática Supervisionada (APS)
Desenvolvido para fins educacionais, com foco em estruturas de dados, análise de algoritmos e avaliação experimental de desempenho.
