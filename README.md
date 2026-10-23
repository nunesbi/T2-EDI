# 📦 Sistema de Catálogo de Produtos

### GCT053 — Estruturas de Dados I | Projeto Prático

<p align="center">
  <img src="https://img.shields.io/badge/Linguagem-C-A8B9CC?style=for-the-badge&logo=c&logoColor=white" alt="Linguagem C">
  <img src="https://img.shields.io/badge/Disciplina-Estruturas%20de%20Dados%20I-6C63FF?style=for-the-badge" alt="Disciplina">
  <img src="https://img.shields.io/badge/Status-Concluído-green?style=for-the-badge" alt="Status do projeto">
</p>

Sistema desenvolvido em linguagem C para o gerenciamento de um catálogo de produtos, aplicando conceitos de **estruturas de dados, algoritmos de busca, algoritmos de ordenação e análise de desempenho**.

O projeto permite cadastrar e consultar produtos, controlar o estoque, registrar vendas e comparar o comportamento de diferentes algoritmos de ordenação e busca.

---

## 📑 Sumário

- [Sobre o projeto](#-sobre-o-projeto)
- [Funcionalidades](#-funcionalidades)
- [Tecnologias utilizadas](#-tecnologias-utilizadas)
- [Estrutura dos dados](#-estrutura-dos-dados)
- [Algoritmos implementados](#-algoritmos-implementados)
- [Como executar](#-como-executar)
- [Menu do sistema](#-menu-do-sistema)
- [Testes e análise](#-testes-e-análise)
- [Integrantes](#-integrantes)
- [Disciplina e objetivo acadêmico](#-disciplina-e-objetivo-acadêmico)

---

## 🎯 Sobre o projeto

O sistema foi desenvolvido como parte da disciplina **GCT053 — Estruturas de Dados I**, com o objetivo de demonstrar como a organização dos dados influencia a eficiência das operações de busca e ordenação.

Os produtos são armazenados em um vetor de estruturas (`struct`), contendo informações como código, nome, categoria, preço e quantidade disponível em estoque.

Além das operações de gerenciamento, o sistema permite comparar algoritmos de ordenação por meio da contagem de comparações e movimentações, contribuindo para a compreensão de seu comportamento em diferentes cenários.

## ⚙️ Funcionalidades

| Funcionalidade | Descrição |
|---|---|
| 📋 Listagem | Exibe os produtos cadastrados de forma organizada. |
| ➕ Cadastro | Adiciona produtos e impede códigos duplicados. |
| 🔎 Busca sequencial por código | Pesquisa produtos percorrendo o vetor. |
| 🔎 Busca sequencial por nome | Localiza produtos pelo nome ou por parte dele. |
| ⚡ Busca binária | Pesquisa por código quando o vetor está ordenado por código. |
| ↕️ Ordenação por código | Disponibiliza quatro algoritmos de ordenação. |
| 💲 Ordenação por preço | Organiza os produtos em ordem crescente de preço. |
| 📉 Estoque baixo | Lista produtos abaixo do limite de estoque informado. |
| 🛒 Registro de venda | Registra uma venda e atualiza a quantidade em estoque. |
| 📊 Comparação de ordenações | Compara o número de comparações e movimentações dos algoritmos. |
| 🧪 Comparação de buscas | Compara busca sequencial e binária em diferentes situações. |
| 🔬 Testes de ordenação | Avalia os algoritmos com entradas crescentes e decrescentes. |

## 🧰 Tecnologias utilizadas

- **Linguagem:** C
- **Compilador:** GCC
- **Controle de versão:** Git
- **Hospedagem do código:** GitHub

## 🗂️ Estrutura dos dados

Cada produto é representado por uma estrutura `Produto`:

```c
typedef struct {
    int codigo;
    char nome[60];
    char categoria[30];
    float preco;
    int estoque;
} Produto;
```

Os registros são armazenados em um vetor com capacidade máxima de **500 produtos**.

O sistema começa com 15 produtos de exemplo para facilitar os testes das funcionalidades.

## 🧠 Algoritmos implementados

### Algoritmos de ordenação

| Algoritmo | Característica principal |
|---|---|
| Selection Sort | Seleciona o menor elemento da parte ainda não ordenada. |
| Insertion Sort | Insere cada elemento na posição correta da parte ordenada. |
| Bubble Sort otimizado | Compara elementos adjacentes e pode encerrar antecipadamente quando não há trocas. |
| Merge Sort | Divide o vetor, ordena as partes e intercala os resultados. |

Todos os quatro algoritmos são implementados manualmente, sem utilizar `qsort()`.

### Algoritmos de busca

**Busca sequencial:** percorre o vetor até encontrar o produto desejado ou atingir o final.

**Busca binária:** reduz sucessivamente o intervalo de pesquisa pela metade. Para funcionar corretamente, exige que o vetor esteja ordenado pelo código do produto.

O sistema controla essa condição e bloqueia a busca binária após operações que invalidam a ordenação por código.

## 📊 Análise de desempenho

Os algoritmos de ordenação são avaliados por meio de duas métricas:

- **Comparações:** quantidade de comparações entre chaves utilizadas para determinar a ordem dos elementos.
- **Movimentações:** quantidade de atribuições ou trocas contabilizadas pela implementação durante a ordenação.

Na comparação entre algoritmos, todos recebem cópias dos mesmos dados iniciais dentro de cada cenário, evitando que um algoritmo seja favorecido por receber um vetor previamente ordenado por outro.

### Resultados experimentais

Os resultados obtidos durante a execução do programa foram:

| Algoritmo | Entrada crescente: comparações | Entrada crescente: movimentações | Entrada decrescente: comparações | Entrada decrescente: movimentações |
|---|---:|---:|---:|---:|
| Selection Sort | 105 | 0 | 105 | 21 |
| Insertion Sort | 14 | 28 | 105 | 133 |
| Bubble Sort otimizado | 14 | 0 | 105 | 315 |
| Merge Sort | 31 | 118 | 28 | 118 |

Nos testes realizados, Insertion Sort e Bubble Sort otimizado apresentaram 14 comparações cada na entrada crescente. O Selection Sort manteve 105 comparações nos dois cenários, enquanto o Merge Sort apresentou 31 comparações na entrada crescente e 28 na decrescente.

### Comparação entre os métodos de busca

Após ordenar o vetor por código, foram obtidos os seguintes resultados:

| Cenário | Código pesquisado | Busca sequencial | Busca binária |
|---|---:|---|---|
| Produto próximo ao início | 1007 | 1 comparação — encontrado | 7 comparações — encontrado |
| Produto próximo ao final | 1097 | 14 comparações — encontrado | 7 comparações — encontrado |
| Código inexistente | 88888 | 17 comparações — não encontrado | 10 comparações — não encontrado |

O código `88888` foi utilizado como código inexistente porque os códigos `9999` e `99999` estavam cadastrados durante a sessão de testes.

Os resultados mostram que a busca binária realizou menos comparações nos três cenários observados. Entretanto, seu uso depende da ordenação prévia do vetor por código.

### Verificações complementares

- **Código duplicado:** o sistema rejeitou o cadastro do código `1097`, que já existia.
- **Valores repetidos:** o sistema aceitou o produto de código `1200`, mesmo com preço, categoria e estoque iguais aos de outro produto.
- **Estoque baixo:** com limite de 10 unidades, o programa listou os produtos com estoque inferior a esse limite.
- **Busca binária após ordenação por código:** o sistema encontrou o produto de código `1007`.
- **Busca binária após ordenação por preço:** o sistema bloqueou a busca e informou que o vetor precisava estar ordenado por código.
- **Cadastro após ordenação:** o sistema informou a necessidade de ordenar novamente o vetor por código antes de utilizar a busca binária.

## 🚀 Como executar

### Pré-requisitos

- GCC instalado e disponível no terminal.
- Git, caso deseje clonar o repositório.

### 1. Clone o repositório

```bash
git clone https://github.com/nunesbi/T2-EDI.git
```

### 2. Acesse a pasta do projeto

```bash
cd T2-EDI
```

### 3. Compile o programa

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c -o catalogo
```

No Windows, caso o GCC esteja instalado pelo MSYS2 UCRT64, o comando também pode ser executado no PowerShell se o compilador estiver configurado no `PATH`.

### 4. Execute o sistema

**Windows — PowerShell:**

```powershell
.\catalogo.exe
```

**Linux/macOS:**

```bash
./catalogo
```

## 🖥️ Menu do sistema

O menu principal reúne as operações disponíveis:

| Opção | Operação |
|---:|---|
| 1 | Listar produtos |
| 2 | Cadastrar produto |
| 3 | Busca sequencial por código |
| 4 | Busca sequencial por nome |
| 5 | Busca binária por código |
| 6 | Ordenar por código — Selection Sort |
| 7 | Ordenar por código — Insertion Sort |
| 8 | Ordenar por código — Bubble Sort |
| 9 | Ordenar por código — Merge Sort |
| 10 | Ordenar produtos por preço |
| 11 | Exibir produtos com estoque baixo |
| 12 | Comparar algoritmos de ordenação |
| 13 | Comparar métodos de busca |
| 14 | Comparar ordenações em entradas crescentes e decrescentes |
| 15 | Registrar venda e atualizar estoque |
| 0 | Encerrar o programa |

## 🧪 Testes e análise

Os testes verificam o funcionamento das buscas, o tratamento de códigos inexistentes e duplicados, o controle da condição necessária à busca binária e o comportamento dos algoritmos em diferentes ordens de entrada.

Também são avaliados casos com preços, nomes, categorias e quantidades repetidos, mantendo a unicidade dos códigos dos produtos.

Os resultados experimentais e as respostas às questões conceituais são apresentados no relatório do projeto.

> **Observação:** os produtos cadastrados são mantidos em memória durante a execução. O armazenamento permanente em arquivos não faz parte desta versão do sistema.

## 👥 Integrantes

| Integrante |
|---|
| Bianca Lara Nunes |
| Lucas Eduardo de Carvalho Ferreira |
| Ruby Novaes Marshall |

## 🎓 Disciplina e objetivo acadêmico

**Universidade Federal de Lavras (UFLA)**  
**Disciplina:** GCT053 — Estruturas de Dados I  
**Projeto:** Sistema de Catálogo de Produtos

Este projeto busca consolidar os conhecimentos de estruturas de dados, implementação de algoritmos, busca, ordenação e análise de desempenho, valorizando a compreensão das soluções desenvolvidas e a capacidade de explicar seu funcionamento.

---

<p align="center">
  Desenvolvido como projeto acadêmico da disciplina GCT053 — Estruturas de Dados I.
</p>
