/******************************************
*
* VINICIUS TAVARES GOMES
* Trabalho Prático
* Disciplina: Algortimos e Programação II *
* Professores: Ivone, Mauro e Ronaldo
*
******************************************/

/* Bibliotecas */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

/*Estruturas*/
typedef struct {
    char nome[50];
    int codigo;
    float preco;
    int quantidade;
} Produto;

typedef struct {
    Produto produtos[100];
    int quantidade;
} Estoque;

typedef struct {
    Produto produtos[100];
    int quantidade;
} Carrinho;

/*Operações*/
void inserir(Estoque *estoque, int codigo, char nome[], int quantidade, float preco) {
    int i;
    for (i = 0; i < estoque->quantidade; i++) {
        if (estoque->produtos[i].codigo == codigo) {
            printf("Erro ao inserir o produto %d.\n", codigo);
            return;
        }
    }
    estoque->produtos[estoque->quantidade].codigo = codigo;
    strcpy(estoque->produtos[estoque->quantidade].nome, nome);
    estoque->produtos[estoque->quantidade].quantidade = quantidade;
    estoque->produtos[estoque->quantidade].preco = preco;
    estoque->quantidade++;
    printf("Produto %d inserido com sucesso!\n", codigo);
}

void excluir(Estoque *estoque, int codigo) {
    int i, j;
    for (i = 0; i < estoque->quantidade; i++) {
        if (estoque->produtos[i].codigo == codigo) {
            for (j = i; j < estoque->quantidade - 1; j++) {
                estoque->produtos[j] = estoque->produtos[j + 1];
            }
            estoque->quantidade--;
            printf("Produto %d excluído com sucesso!\n", codigo);
            return;
        }
    }
    printf("Produto %d não cadastrado!\n", codigo);
}

void atualizar(Estoque *estoque, int codigo, char p, int novo_valor) {
    int i;
    for (i = 0; i < estoque->quantidade; i++) {
        if (estoque->produtos[i].codigo == codigo) {
            if (p == 'q') {
                estoque->produtos[i].quantidade = novo_valor;
            } else if (p == 'v') {
                estoque->produtos[i].preco = novo_valor;
            }
            printf("Produto %d atualizado!\n", codigo);
            return;
        }
    }
    printf("Produto %d não cadastrado!\n", codigo);
}

void consultar(Estoque *estoque, char sequencia[]) {
    int i, j;
    for (i = 0; i < estoque->quantidade; i++) {
        if (strstr(estoque->produtos[i].nome, sequencia) != NULL) {
            printf("%d - %s - %d - %.2f\n", estoque->produtos[i].codigo, estoque->produtos[i].nome, estoque->produtos[i].quantidade, estoque->produtos[i].preco);
        }
    }
}

void relatorio(Estoque *estoque) {
    int i, j;
    FILE *arquivo;
    arquivo = fopen("Relatorio.txt", "w");
    for (i = 0; i < estoque->quantidade; i++) {
        for (j = i + 1; j < estoque->quantidade; j++) {
            if (strcmp(estoque->produtos[i].nome, estoque->produtos[j].nome) > 0) {
                Produto aux = estoque->produtos[i];
                estoque->produtos[i] = estoque->produtos[j];
                estoque->produtos[j] = aux;
            }
        }
    }
    for (i = 0; i < estoque->quantidade; i++) {
        if (i == 0 || estoque->produtos[i].nome[0] != estoque->produtos[i - 1].nome[0]) {
            printf("%c\n", estoque->produtos[i].nome[0]);
            fprintf(arquivo, "%c\n", estoque->produtos[i].nome[0]);
        }
        printf("%d - %s - %d - %.2f\n", estoque->produtos[i].codigo, estoque->produtos[i].nome, estoque->produtos[i].quantidade, estoque->produtos[i].preco);
        fprintf(arquivo, "%d - %s - %d - %.2f\n", estoque->produtos[i].codigo, estoque->produtos[i].nome, estoque->produtos[i].quantidade, estoque->produtos[i].preco);
    }
    fclose(arquivo);
}

void importar(Estoque *estoque, char nome_arquivo[]) {
    int i, quantidade;
    char nome[50];
    int codigo, quantidade_produto;
    float preco;
    FILE *arquivo;
    arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao importar do arquivo %s.\n", nome_arquivo);
        return;
    }
    fscanf(arquivo, "%d", &quantidade);
    for (i = 0; i < quantidade; i++) {
        fscanf(arquivo, "%d;%[^;];%d;%f", &codigo, nome, &quantidade_produto, &preco);
        inserir(estoque, codigo, nome, quantidade_produto, preco);
    }
    fclose(arquivo);
    printf("%d produtos importados!\n", quantidade);
}

void vender(Estoque *estoque, Carrinho *carrinho) {
    int codigo, i, j;
    float soma = 0;
    while (1) {
        scanf("%d", &codigo);
        if (codigo == 0) {
            break;
        }
        for (i = 0; i < estoque->quantidade; i++) {
            if (estoque->produtos[i].codigo == codigo) {
                if (estoque->produtos[i].quantidade == 0) {
                    printf("%d - produto em falta no estoque\n", codigo);
                } else {
                    estoque->produtos[i].quantidade--;
                    soma += estoque->produtos[i].preco;
                    printf("%d - %s - %.2f\n", estoque->produtos[i].codigo, estoque->produtos[i].nome, estoque->produtos[i].preco);
                }
                break;
            }
        }
        if (i == estoque->quantidade) {
            printf("%d - produto não cadastrado\n", codigo);
        }
    }
    printf("---------\nTotal %.2f\n", soma);
}

void help() {
    printf("A estrutura escolhida foi a lista duplamente encadeada, pois ela permite a inserção e exclusão de elementos em qualquer posição da lista, além de permitir a consulta de elementos em qualquer posição da lista.\n");
}

void sair(Estoque *estoque) {
    int i, j;
    FILE *arquivo;
    arquivo = fopen("Comprar.txt", "w");
    for (i = 0; i < estoque->quantidade; i++) {
        for (j = i + 1; j < estoque->quantidade; j++) {
            if (estoque->produtos[i].quantidade > estoque->produtos[j].quantidade) {
                Produto aux = estoque->produtos[i];
                estoque->produtos[i] = estoque->produtos[j];
                estoque->produtos[j] = aux;
            }
        }
    }
    for (i = 0; i < estoque->quantidade; i++) {
        if (estoque->produtos[i].quantidade < 15) {
            printf("%d - %s - %d\n", estoque->produtos[i].codigo, estoque->produtos[i].nome, estoque->produtos[i].quantidade);
            fprintf(arquivo, "%d - %s - %d\n", estoque->produtos[i].codigo, estoque->produtos[i].nome, estoque->produtos[i].quantidade);
        }
    }
    fclose(arquivo);
    arquivo = fopen("Produtos.dat", "wb");
    fwrite(estoque, sizeof(Estoque), 1, arquivo);
    fclose(arquivo);
}

/* Menu */

void menu() {
    printf("1 - Inserir produto\n");
    printf("2 - Excluir produto\n");
    printf("3 - Atualizar produto\n");
    printf("4 - Consultar produto\n");
    printf("5 - Relatório\n");
    printf("6 - Importar produtos\n");
    printf("7 - Vender\n");
    printf("8 - Help\n");
    printf("9 - Sair\n");
}

/* Main */

int main(void){
    int opcao, codigo, quantidade, novo_valor;
    char nome[50], sequencia[50], nome_arquivo[50], p;
    float preco;
    Estoque estoque;
    Carrinho carrinho;
    estoque.quantidade = 0;
    FILE *arquivo;
    arquivo = fopen("Produtos.dat", "rb");
    if (arquivo != NULL) {
        fread(&estoque, sizeof(Estoque), 1, arquivo);
        fclose(arquivo);
    }
    while (1) {
        menu();
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                scanf("%d %s %d %f", &codigo, nome, &quantidade, &preco);
                inserir(&estoque, codigo, nome, quantidade, preco);
                break;
            case 2:
                scanf("%d", &codigo);
                excluir(&estoque, codigo);
                break;
            case 3:
                scanf("%d %c %d", &codigo, &p, &novo_valor);
                atualizar(&estoque, codigo, p, novo_valor);
                break;
            case 4:
                scanf("%s", sequencia);
                consultar(&estoque, sequencia);
                break;
            case 5:
                relatorio(&estoque);
                break;
            case 6:
                scanf("%s", nome_arquivo);
                importar(&estoque, nome_arquivo);
                break;
            case 7:
                vender(&estoque, &carrinho);
                break;
            case 8:
                help();
                break;
            case 9:
                sair(&estoque);
                return 0;
            default:
                printf("Opção inválida!\n");
                break;
        }
    }
    return 0;
}