/*
 * GCT053 - Estruturas de Dados I
 * Grupo: Bianca Nunes, Lucas Eduardo Carvalho, Ruby Marshall
 * Projeto Pratico: Sistema de Catalogo de Produtos
 * Implementacao baseada no enunciado da atividade.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_PRODUTOS 500
#define TAM_NOME 60
#define TAM_CATEGORIA 30

typedef struct {
    int codigo;
    char nome[TAM_NOME];
    char categoria[TAM_CATEGORIA];
    float preco;
    int estoque;
} Produto;

typedef struct {
    long comparacoes;
    long movimentacoes;
} Estatisticas;

typedef struct {
    int indice;
    long comparacoes;
} ResultadoBusca;

Produto produtos[MAX_PRODUTOS];
int quantidade = 0;
int ordenado_por_codigo = 0;

/* Le uma linha e remove o '\n'. */
void ler_linha(const char *mensagem, char *destino, size_t tamanho) {
    printf("%s", mensagem);
    if (fgets(destino, (int)tamanho, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        } else {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
        }
    } else {
        destino[0] = '\0';
    }
}

/* Le inteiro com validacao simples. */
int ler_inteiro(const char *mensagem) {
    char linha[100], *fim;
    long valor;
    for (;;) {
        ler_linha(mensagem, linha, sizeof(linha));
        valor = strtol(linha, &fim, 10);
        while (isspace((unsigned char)*fim)) fim++;
        if (linha[0] != '\0' && *fim == '\0' && valor >= -2147483647L && valor <= 2147483647L)
            return (int)valor;
        printf("Entrada invalida. Digite um numero inteiro.\n");
    }
}

/* Le float com ponto ou virgula decimal. */
float ler_float(const char *mensagem) {
    char linha[100], *fim;
    float valor;
    for (;;) {
        ler_linha(mensagem, linha, sizeof(linha));
        for (size_t i = 0; linha[i]; i++) if (linha[i] == ',') linha[i] = '.';
        valor = strtof(linha, &fim);
        while (isspace((unsigned char)*fim)) fim++;
        if (linha[0] != '\0' && *fim == '\0' && valor >= 0.0f)
            return valor;
        printf("Entrada invalida. Digite um valor numerico nao negativo.\n");
    }
}

void copiar_texto(char *destino, size_t tamanho, const char *origem) {
    if (tamanho == 0) return;
    strncpy(destino, origem, tamanho - 1);
    destino[tamanho - 1] = '\0';
}

void trocar(Produto *a, Produto *b, Estatisticas *e) {
    Produto temp = *a;
    *a = *b;
    *b = temp;
    if (e != NULL) e->movimentacoes += 3;
}

void exibir_produto(const Produto *p) {
    printf("%-7d %-25.25s %-18.18s %10.2f %8d\n",
           p->codigo, p->nome, p->categoria, p->preco, p->estoque);
}

void listar_produtos(Produto v[], int n) {
    if (n == 0) {
        printf("\nNenhum produto cadastrado.\n");
        return;
    }
    printf("\n%-7s %-25s %-18s %10s %8s\n",
           "CODIGO", "NOME", "CATEGORIA", "PRECO", "ESTOQUE");
    printf("-------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) exibir_produto(&v[i]);
}

int busca_sequencial_codigo(Produto v[], int n, int codigo) {
    for (int i = 0; i < n; i++) {
        if (v[i].codigo == codigo) return i;
    }
    return -1;
}

ResultadoBusca busca_sequencial_com_estatistica(Produto v[], int n, int codigo) {
    ResultadoBusca r = {-1, 0};
    for (int i = 0; i < n; i++) {
        r.comparacoes++;
        if (v[i].codigo == codigo) {
            r.indice = i;
            return r;
        }
    }
    return r;
}

int busca_sequencial_nome(Produto v[], int n, const char *nome) {
    for (int i = 0; i < n; i++) {
        if (strstr(v[i].nome, nome) != NULL) return i;
    }
    return -1;
}

ResultadoBusca busca_binaria_com_estatistica(Produto v[], int n, int codigo) {
    ResultadoBusca r = {-1, 0};
    int inicio = 0, fim = n - 1;
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        r.comparacoes++;
        if (v[meio].codigo == codigo) {
            r.indice = meio;
            return r;
        }
        r.comparacoes++;
        if (v[meio].codigo < codigo) inicio = meio + 1;
        else fim = meio - 1;
    }
    return r;
}

int busca_binaria_codigo(Produto v[], int n, int codigo) {
    return busca_binaria_com_estatistica(v, n, codigo).indice;
}

Estatisticas selection_sort(Produto v[], int n) {
    Estatisticas e = {0, 0};
    for (int i = 0; i < n - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < n; j++) {
            e.comparacoes++;
            if (v[j].codigo < v[menor].codigo) menor = j;
        }
        if (menor != i) trocar(&v[i], &v[menor], &e);
    }
    return e;
}

Estatisticas insertion_sort(Produto v[], int n) {
    Estatisticas e = {0, 0};
    for (int i = 1; i < n; i++) {
        Produto chave = v[i];
        e.movimentacoes++;
        int j = i - 1;
        while (j >= 0) {
            e.comparacoes++;
            if (v[j].codigo > chave.codigo) {
                v[j + 1] = v[j];
                e.movimentacoes++;
                j--;
            } else break;
        }
        v[j + 1] = chave;
        e.movimentacoes++;
    }
    return e;
}

Estatisticas bubble_sort(Produto v[], int n) {
    Estatisticas e = {0, 0};
    for (int fim = n - 1; fim > 0; fim--) {
        int trocou = 0;
        for (int j = 0; j < fim; j++) {
            e.comparacoes++;
            if (v[j].codigo > v[j + 1].codigo) {
                trocar(&v[j], &v[j + 1], &e);
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
    return e;
}

void merge(Produto v[], Produto tmp[], int inicio, int meio, int fim,
           Estatisticas *e) {
    int i = inicio, j = meio + 1, k = inicio;
    while (i <= meio && j <= fim) {
        e->comparacoes++;
        if (v[i].codigo <= v[j].codigo) tmp[k++] = v[i++];
        else tmp[k++] = v[j++];
        e->movimentacoes++;
    }
    while (i <= meio) {
        tmp[k++] = v[i++];
        e->movimentacoes++;
    }
    while (j <= fim) {
        tmp[k++] = v[j++];
        e->movimentacoes++;
    }
    for (i = inicio; i <= fim; i++) {
        v[i] = tmp[i];
        e->movimentacoes++;
    }
}

void merge_sort_rec(Produto v[], Produto tmp[], int inicio, int fim,
                    Estatisticas *e) {
    if (inicio >= fim) return;
    int meio = inicio + (fim - inicio) / 2;
    merge_sort_rec(v, tmp, inicio, meio, e);
    merge_sort_rec(v, tmp, meio + 1, fim, e);
    merge(v, tmp, inicio, meio, fim, e);
}

Estatisticas merge_sort(Produto v[], int n) {
    Estatisticas e = {0, 0};
    if (n <= 1) return e;
    Produto *tmp = (Produto *)malloc((size_t)n * sizeof(Produto));
    if (tmp == NULL) {
        printf("Memoria insuficiente para Merge Sort.\n");
        return e;
    }
    merge_sort_rec(v, tmp, 0, n - 1, &e);
    free(tmp);
    return e;
}

Estatisticas ordenar_por_preco(Produto v[], int n) {
    Estatisticas e = {0, 0};
    /* Insertion Sort proprio, usando preco como criterio. */
    for (int i = 1; i < n; i++) {
        Produto chave = v[i];
        e.movimentacoes++;
        int j = i - 1;
        while (j >= 0) {
            e.comparacoes++;
            if (v[j].preco > chave.preco) {
                v[j + 1] = v[j];
                e.movimentacoes++;
                j--;
            } else break;
        }
        v[j + 1] = chave;
        e.movimentacoes++;
    }
    return e;
}

void vender_produto(void) {
    if (quantidade == 0) {
        printf("Nao ha produtos cadastrados.\n");
        return;
    }
    int codigo = ler_inteiro("Codigo do produto vendido: ");
    int indice = busca_sequencial_codigo(produtos, quantidade, codigo);
    if (indice < 0) {
        printf("Produto nao encontrado. Nenhuma alteracao realizada.\n");
        return;
    }
    printf("Produto: %s | Estoque atual: %d\n", produtos[indice].nome, produtos[indice].estoque);
    int qtd_vendida = ler_inteiro("Quantidade vendida: ");
    if (qtd_vendida <= 0) {
        printf("A quantidade vendida deve ser maior que zero.\n");
        return;
    }
    if (qtd_vendida > produtos[indice].estoque) {
        printf("Estoque insuficiente. Venda nao realizada.\n");
        return;
    }
    produtos[indice].estoque -= qtd_vendida;
    printf("Venda registrada! Estoque atualizado: %d unidade(s).\n", produtos[indice].estoque);
    if (produtos[indice].estoque == 0)
        printf("Aviso: o produto ficou sem estoque, mas continua cadastrado no catalogo.\n");
}

void cadastrar_produto(void) {
    if (quantidade >= MAX_PRODUTOS) {
        printf("Limite de produtos atingido (%d).\n", MAX_PRODUTOS);
        return;
    }

    Produto novo;
    novo.codigo = ler_inteiro("Codigo: ");
    if (novo.codigo <= 0) {
        printf("O codigo deve ser positivo.\n");
        return;
    }
    if (busca_sequencial_codigo(produtos, quantidade, novo.codigo) != -1) {
        printf("ERRO: codigo de produto ja cadastrado.\n");
        return;
    }

    ler_linha("Nome: ", novo.nome, sizeof(novo.nome));
    if (novo.nome[0] == '\0') {
        printf("O nome nao pode ficar vazio.\n");
        return;
    }
    ler_linha("Categoria: ", novo.categoria, sizeof(novo.categoria));
    if (novo.categoria[0] == '\0') {
        printf("A categoria nao pode ficar vazia.\n");
        return;
    }
    novo.preco = ler_float("Preco: ");
    novo.estoque = ler_inteiro("Estoque: ");
    if (novo.estoque < 0) {
        printf("O estoque nao pode ser negativo.\n");
        return;
    }

    produtos[quantidade++] = novo;
    ordenado_por_codigo = 0;
    printf("Produto cadastrado com sucesso. A busca binaria precisa de nova ordenacao por codigo.\n");
}

void buscar_por_codigo(void) {
    int codigo = ler_inteiro("Digite o codigo: ");
    int indice = busca_sequencial_codigo(produtos, quantidade, codigo);
    if (indice >= 0) {
        printf("\nProduto encontrado:\n");
        listar_produtos(&produtos[indice], 1);
    } else printf("Produto nao encontrado.\n");
}

void buscar_por_nome(void) {
    char nome[TAM_NOME];
    ler_linha("Digite o nome ou parte do nome: ", nome, sizeof(nome));
    if (nome[0] == '\0') {
        printf("A busca nao pode ficar vazia.\n");
        return;
    }
    int indice = busca_sequencial_nome(produtos, quantidade, nome);
    if (indice >= 0) {
        printf("\nProduto encontrado:\n");
        listar_produtos(&produtos[indice], 1);
    } else printf("Produto nao encontrado.\n");
}

void ordenar_menu(int algoritmo) {
    Estatisticas e;
    switch (algoritmo) {
        case 1: e = selection_sort(produtos, quantidade); break;
        case 2: e = insertion_sort(produtos, quantidade); break;
        case 3: e = bubble_sort(produtos, quantidade); break;
        default: e = merge_sort(produtos, quantidade); break;
    }
    ordenado_por_codigo = 1;
    printf("Ordenacao por codigo concluida.\nComparacoes: %ld\nMovimentacoes: %ld\n",
           e.comparacoes, e.movimentacoes);
}

void listar_estoque_baixo(Produto v[], int n) {
    int limite = ler_inteiro("Estoque maximo para alerta: ");
    if (limite < 0) {
        printf("O limite nao pode ser negativo.\n");
        return;
    }
    int encontrados = 0;
    printf("\nProdutos com estoque menor que %d:\n", limite);
    for (int i = 0; i < n; i++) {
        if (v[i].estoque < limite) {
            printf("%d - %-25s - %d unidades\n", v[i].codigo, v[i].nome, v[i].estoque);
            encontrados++;
        }
    }
    if (!encontrados) printf("Nenhum produto atende ao criterio.\n");
}

void copiar_vetor(Produto destino[], Produto origem[], int n) {
    for (int i = 0; i < n; i++) destino[i] = origem[i];
}

void imprimir_estatisticas_cenario(const char *titulo, Produto base[], int n) {
    Produto *a = malloc((size_t)n * sizeof(Produto));
    Produto *b = malloc((size_t)n * sizeof(Produto));
    Produto *c = malloc((size_t)n * sizeof(Produto));
    Produto *d = malloc((size_t)n * sizeof(Produto));
    if (!a || !b || !c || !d) {
        printf("Memoria insuficiente para executar o cenario.\n");
        free(a); free(b); free(c); free(d);
        return;
    }
    copiar_vetor(a, base, n); copiar_vetor(b, base, n);
    copiar_vetor(c, base, n); copiar_vetor(d, base, n);
    Estatisticas es = selection_sort(a, n);
    Estatisticas ei = insertion_sort(b, n);
    Estatisticas eb = bubble_sort(c, n);
    Estatisticas em = merge_sort(d, n);
    printf("\n%s\n", titulo);
    printf("%-15s %15s %18s\n", "ALGORITMO", "COMPARACOES", "MOVIMENTACOES");
    printf("--------------------------------------------------\n");
    printf("%-15s %15ld %18ld\n", "Selection Sort", es.comparacoes, es.movimentacoes);
    printf("%-15s %15ld %18ld\n", "Insertion Sort", ei.comparacoes, ei.movimentacoes);
    printf("%-15s %15ld %18ld\n", "Bubble Sort", eb.comparacoes, eb.movimentacoes);
    printf("%-15s %15ld %18ld\n", "Merge Sort", em.comparacoes, em.movimentacoes);
    free(a); free(b); free(c); free(d);
}

void comparar_ordenado_e_invertido(void) {
    if (quantidade == 0) {
        printf("Nao ha produtos para testar.\n");
        return;
    }
    Produto *base = malloc((size_t)quantidade * sizeof(Produto));
    if (!base) {
        printf("Memoria insuficiente para preparar os testes.\n");
        return;
    }
    copiar_vetor(base, produtos, quantidade);
    selection_sort(base, quantidade);
    imprimir_estatisticas_cenario("TESTE 5 - VETOR INICIAL EM ORDEM CRESCENTE", base, quantidade);
    for (int i = 0; i < quantidade / 2; i++) {
        Produto temp = base[i];
        base[i] = base[quantidade - 1 - i];
        base[quantidade - 1 - i] = temp;
    }
    imprimir_estatisticas_cenario("TESTE 6 - VETOR INICIAL EM ORDEM DECRESCENTE", base, quantidade);
    printf("\nOs quatro algoritmos receberam copias identicas dentro de cada cenario.\n");
    free(base);
}

void comparar_ordenacoes(void) {
    if (quantidade == 0) {
        printf("Nao ha produtos para ordenar.\n");
        return;
    }
    Produto *base = malloc((size_t)quantidade * sizeof(Produto));
    Produto *a = malloc((size_t)quantidade * sizeof(Produto));
    Produto *b = malloc((size_t)quantidade * sizeof(Produto));
    Produto *c = malloc((size_t)quantidade * sizeof(Produto));
    Produto *d = malloc((size_t)quantidade * sizeof(Produto));
    if (!base || !a || !b || !c || !d) {
        printf("Memoria insuficiente para comparar algoritmos.\n");
        free(base); free(a); free(b); free(c); free(d);
        return;
    }

    copiar_vetor(base, produtos, quantidade);
    copiar_vetor(a, base, quantidade);
    copiar_vetor(b, base, quantidade);
    copiar_vetor(c, base, quantidade);
    copiar_vetor(d, base, quantidade);

    Estatisticas es = selection_sort(a, quantidade);
    Estatisticas ei = insertion_sort(b, quantidade);
    Estatisticas eb = bubble_sort(c, quantidade);
    Estatisticas em = merge_sort(d, quantidade);

    printf("\nComparacao dos algoritmos (mesma entrada inicial):\n");
    printf("%-15s %15s %18s\n", "ALGORITMO", "COMPARACOES", "MOVIMENTACOES");
    printf("--------------------------------------------------\n");
    printf("%-15s %15ld %18ld\n", "Selection Sort", es.comparacoes, es.movimentacoes);
    printf("%-15s %15ld %18ld\n", "Insertion Sort", ei.comparacoes, ei.movimentacoes);
    printf("%-15s %15ld %18ld\n", "Bubble Sort", eb.comparacoes, eb.movimentacoes);
    printf("%-15s %15ld %18ld\n", "Merge Sort", em.comparacoes, em.movimentacoes);
    printf("\nObservacao: cada algoritmo recebeu uma copia identica dos dados originais.\n");
    free(base); free(a); free(b); free(c); free(d);
}

void comparar_buscas(void) {
    if (quantidade == 0) {
        printf("Nao ha produtos cadastrados.\n");
        return;
    }
    if (!ordenado_por_codigo) {
        printf("A busca binaria esta indisponivel: o vetor precisa estar ordenado por codigo.\n");
        printf("Use uma das opcoes de ordenacao por codigo e tente novamente.\n");
        return;
    }

    int codigos[3];
    codigos[0] = produtos[0].codigo;
    codigos[1] = produtos[quantidade - 1].codigo;
    codigos[2] = 9999;

    printf("\nExperimento de busca (inicio, final e codigo 9999):\n");
    for (int i = 0; i < 3; i++) {
        ResultadoBusca seq = busca_sequencial_com_estatistica(produtos, quantidade, codigos[i]);
        ResultadoBusca bin = busca_binaria_com_estatistica(produtos, quantidade, codigos[i]);
        printf("\nCodigo: %d\n", codigos[i]);
        printf("Busca sequencial: comparacoes = %ld; encontrado = %s\n",
               seq.comparacoes, seq.indice >= 0 ? "SIM" : "NAO");
        printf("Busca binaria:    comparacoes = %ld; encontrado = %s\n",
               bin.comparacoes, bin.indice >= 0 ? "SIM" : "NAO");
    }

    int codigo = ler_inteiro("\nDigite outro codigo para comparar (ou 0 para voltar): ");
    if (codigo != 0) {
        ResultadoBusca seq = busca_sequencial_com_estatistica(produtos, quantidade, codigo);
        ResultadoBusca bin = busca_binaria_com_estatistica(produtos, quantidade, codigo);
        printf("Busca sequencial: comparacoes = %ld; encontrado = %s\n",
               seq.comparacoes, seq.indice >= 0 ? "SIM" : "NAO");
        printf("Busca binaria:    comparacoes = %ld; encontrado = %s\n",
               bin.comparacoes, bin.indice >= 0 ? "SIM" : "NAO");
    }
}

void inicializar_dados(void) {
    Produto iniciais[] = {
        {1042, "Teclado Mecanico", "Perifericos", 249.90f, 12},
        {1007, "Mouse Sem Fio", "Perifericos", 119.50f, 31},
        {1078, "Monitor 24", "Monitores", 899.00f, 8},
        {1015, "Cabo HDMI", "Cabos", 39.90f, 55},
        {1091, "SSD 1TB", "Armazenamento", 449.00f, 14},
        {1033, "Webcam Full HD", "Video", 189.90f, 9},
        {1064, "Headset USB", "Audio", 159.90f, 23},
        {1021, "Hub USB", "Acessorios", 89.90f, 17},
        {1085, "Memoria 16GB", "Memoria", 279.00f, 26},
        {1056, "Suporte Notebook", "Acessorios", 79.90f, 35},
        {1103, "Fonte 650W", "Energia", 389.00f, 7},
        {1011, "Adaptador Bluetooth", "Acessorios", 54.90f, 42},
        {1071, "HD Externo 2TB", "Armazenamento", 529.90f, 11},
        {1048, "Microfone USB", "Audio", 299.00f, 6},
        {1097, "Notebook Stand Pro", "Acessorios", 139.90f, 19}
    };
    quantidade = (int)(sizeof(iniciais) / sizeof(iniciais[0]));
    copiar_vetor(produtos, iniciais, quantidade);
    ordenado_por_codigo = 0;
}

void exibir_menu(void) {
    printf("\n=========================================\n");
    printf("       SISTEMA DE CATALOGO DE PRODUTOS\n");
    printf("=========================================\n");
    printf("1  - Listar produtos\n");
    printf("2  - Cadastrar produto\n");
    printf("3  - Busca sequencial por codigo\n");
    printf("4  - Busca sequencial por nome\n");
    printf("5  - Busca binaria por codigo\n");
    printf("6  - Ordenar por codigo - Selection Sort\n");
    printf("7  - Ordenar por codigo - Insertion Sort\n");
    printf("8  - Ordenar por codigo - Bubble Sort\n");
    printf("9  - Ordenar por codigo - Merge Sort\n");
    printf("10 - Ordenar produtos por preco\n");
    printf("11 - Exibir produtos com estoque baixo\n");
    printf("12 - Comparar algoritmos de ordenacao\n");
    printf("13 - Comparar metodos de busca\n");
    printf("14 - Comparar ordenacoes (vetor crescente e invertido)\n");
    printf("15 - Registrar venda / reduzir estoque\n");
    printf("0  - Sair\n");
}

int main(void) {
    inicializar_dados();
    int opcao;
    do {
        exibir_menu();
        opcao = ler_inteiro("Opcao: ");
        switch (opcao) {
            case 1:
                listar_produtos(produtos, quantidade);
                break;
            case 2:
                cadastrar_produto();
                break;
            case 3:
                buscar_por_codigo();
                break;
            case 4:
                buscar_por_nome();
                break;
            case 5: {
                if (!ordenado_por_codigo) {
                    printf("Busca binaria indisponivel.\nO vetor precisa estar ordenado por codigo.\n");
                    break;
                }
                int codigo = ler_inteiro("Digite o codigo: ");
                int indice = busca_binaria_codigo(produtos, quantidade, codigo);
                if (indice >= 0) {
                    printf("Produto encontrado:\n");
                    listar_produtos(&produtos[indice], 1);
                } else printf("Produto nao encontrado.\n");
                break;
            }
            case 6: ordenar_menu(1); break;
            case 7: ordenar_menu(2); break;
            case 8: ordenar_menu(3); break;
            case 9: ordenar_menu(4); break;
            case 10:
                ordenar_por_preco(produtos, quantidade);
                ordenado_por_codigo = 0;
                printf("Produtos ordenados por preco crescente. Busca binaria por codigo desativada.\n");
                listar_produtos(produtos, quantidade);
                break;
            case 11:
                listar_estoque_baixo(produtos, quantidade);
                break;
            case 12:
                comparar_ordenacoes();
                break;
            case 13:
                comparar_buscas();
                break;
            case 14:
                comparar_ordenado_e_invertido();
                break;
            case 15:
                vender_produto();
                break;
            case 0:
                printf("Encerrando o sistema. Ate mais!\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);
    return 0;
}
