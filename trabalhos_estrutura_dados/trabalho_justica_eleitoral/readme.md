📌 1. Estrutura do Projeto

O projeto foi organizado em três arquivos principais, conforme exigido:

lista.h   → Declarações do TAD Lista, struct Registro e protótipos das funções  
lista.c   → Implementação do TAD Lista, leitura dos CSVs e funções da atividade  
main.c    → Interface de menu no console  

⚙️ 2. Requisitos Atendidos

🔹 Item 1 – Concatenação de Arquivos CSV
>Função: concatenarArquivos

>Objetivo: Unir todos os arquivos CSV da pasta "Base de Dados"

📄 Saída:

**base_concatenada.csv**

🔹 Item 2 – Resumo por Sigla do Tribunal
>Função: gerarResumoPorTribunal

>Objetivo: Gerar estatísticas agregadas por sigla_tribunal

📄 Saída:

**resumo_tribunais.csv**

📊 Campos calculados:

>total_julgados_2026

>Meta1

>Meta2A

>Meta2Ant

>Meta4A

>Meta4B

⚠️ Observação Importante (Requisito do Enunciado)

O enunciado exige explicitamente que todas as subtrações sejam substituídas por adições.

Por isso, as fórmulas foram implementadas da seguinte forma:

Meta1

>julgados / (casos_novos + dessobrestados + suspensos) * 100

Meta2A

>julgm2_a / (distm2_a + suspm2_a) * 100

Meta2Ant

>julgm2_ant / (distm2_ant + suspm2_ant + desom2_ant) * 100

Meta4A

>julgm4_a / (distm4_a + suspm4_a) * 100

Meta4B

>julgm4_b / (distm4_b + suspm4_b) * 100

✔ Implementação fiel ao enunciado, mesmo com distorção matemática.

🔹 Item 3 – Geração de Arquivo por Município
>Função: gerarArquivoPorMunicipio

>Entrada: Nome do município informado pelo usuário

📄 Saída:

**MUNICIPIO.txt**

💻 3. Como Compilar (Windows – GCC/MinGW)

Execute o comando:

**gcc main.c lista.c -o trabalho.exe**


▶️ 4. Como Executar

Coloque os arquivos .c e .h na mesma pasta

Certifique-se de que a pasta "Base de Dados" esteja no mesmo diretório

Execute o programa

Escolha uma opção no menu:

1 → Concatenar arquivos  
2 → Gerar resumo por tribunal  
3 → Gerar arquivo por município

🧠 5. Funcionamento Interno

Os arquivos CSV são lidos e armazenados em memória utilizando Lista Encadeada (TAD Lista)

Cada linha do CSV é convertida em um Registro

Cada registro é armazenado em um nó da lista

O campo linha_original guarda a linha completa para reaproveitamento

✔ Benefícios:

>Facilidade de manipulação

>Reuso direto para exportação

🔎 6. Observações Técnicas

Separador utilizado: vírgula (,)
O cabeçalho é ignorado em cada arquivo

Comparação de município:

>Não diferencia maiúsculas/minúsculas

>Não trata acentos (ex: MACAPA ≠ MACAPÁ)

>Proteção contra divisão por zero:

>Resultado definido como 0.00

📊 7. Exemplo de Uso

Opção 1 → Gera base_concatenada.csv  
Opção 2 → Gera resumo_tribunais.csv  
Opção 3 → Usuário digita MACAPA → Gera MACAPA.txt

📌 Conclusão

O projeto atende integralmente aos requisitos propostos, com:

>Uso de TAD Lista

>Estrutura modular em 3 arquivos

>Implementação de todas as funcionalidades solicitadas

>Manipulação eficiente de arquivos CSV

>Geração correta de arquivos de saída