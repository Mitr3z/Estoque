#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUTOS 100

void salvarNoArquivo(char nome[MAX_PRODUTOS][50], int codigo[MAX_PRODUTOS],
                     float valor[MAX_PRODUTOS], int qtdd[MAX_PRODUTOS],
                     int numProdutos);

void excluirProduto(char nome[MAX_PRODUTOS][50], int codigo[MAX_PRODUTOS],
                    float valor[MAX_PRODUTOS], int qtdd[MAX_PRODUTOS],
                    int *numProdutos);

int main() {
  char nome[MAX_PRODUTOS][50];
  int codigo[MAX_PRODUTOS];
  float valor[MAX_PRODUTOS];
  int qtdd[MAX_PRODUTOS];
  int numProdutos = 0;
  int opcao;

  FILE *arquivo = fopen("estoque.csv", "r");

  if (arquivo != NULL) {
    char buffer[256];
    fgets(buffer, sizeof(buffer), arquivo);

    while (fscanf(arquivo, "%49[^,],%d,%f,%d\n", nome[numProdutos],
                  &codigo[numProdutos], &valor[numProdutos],
                  &qtdd[numProdutos]) == 4) {
      numProdutos++;
    }

    fclose(arquivo);
  } else {
    printf("Arquivo estoque.csv não encontrado. Criando um novo arquivo.\n");

    FILE *newFile = fopen("estoque.csv", "w");

    if (newFile == NULL) {
      perror("Erro ao criar o arquivo");
      exit(EXIT_FAILURE);
    }

    fprintf(newFile, "Produto,Código,Valor,Quantidade\n");
    fclose(newFile);
  }

  do {
    printf("\n1. Cadastrar novo produto\n");
    printf("2. Exibir produtos cadastrados\n");
    printf("3. Modificar informações de um produto\n");
    printf("4. Excluir produto\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
      case 1:
        if (numProdutos < MAX_PRODUTOS) {
          printf("Digite o nome do produto: ");
          scanf("%s", nome[numProdutos]);
          printf("Digite o código do produto: ");
          scanf("%d", &codigo[numProdutos]);
          printf("Digite o valor do produto: ");
          scanf("%f", &valor[numProdutos]);
          printf("Digite a quantidade em estoque: ");
          scanf("%d", &qtdd[numProdutos]);
          numProdutos++;

          FILE *arquivo = fopen("estoque.csv", "a");

          if (arquivo == NULL) {
            perror("Erro ao abrir o arquivo");
            exit(EXIT_FAILURE);
          }

          fprintf(arquivo, "%s,%d,%.2f,%d\n", nome[numProdutos - 1],
                  codigo[numProdutos - 1], valor[numProdutos - 1],
                  qtdd[numProdutos - 1]);

          fclose(arquivo);

          printf("Produto cadastrado com sucesso!\n");
        } else {
          printf("Limite de produtos atingido. Não é possível cadastrar mais produtos.\n");
        }
        break;

      case 2:
        printf("\nProdutos Cadastrados: %d\n",numProdutos);
        for (int i = 0; i < numProdutos; i++) {
          printf("Produto: %s\n", nome[i]);
          printf("Código: %d\n", codigo[i]);
          printf("Valor: %.2f\n", valor[i]);
          printf("Quantidade em Estoque: %d\n\n", qtdd[i]);
        }
        break;

      case 3:
        {
          int codigoMod;
          int encontrado = 0;

          printf("Digite o código do produto que deseja modificar: ");
          scanf("%d", &codigoMod);

          for (int i = 0; i < numProdutos; i++) {
            if (codigo[i] == codigoMod) {
              printf("Produto encontrado. O que você deseja modificar?\n");
              printf("1. Produto\n");
              printf("2. Valor\n");
              printf("3. Quantidade em Estoque\n");
              printf("Escolha uma opção: ");

              int opcaoMod;
              scanf("%d", &opcaoMod);

              switch (opcaoMod) {
                case 1:
                  printf("Novo produto: ");
                  scanf("%s", nome[i]);
                  break;

                case 2:
                  printf("Novo valor: ");
                  scanf("%f", &valor[i]);
                  break;

                case 3:
                  printf("Nova quantidade em estoque: ");
                  scanf("%d", &qtdd[i]);
                  break;

                default:
                  printf("Opção inválida.\n");
              }

              encontrado = 1;

              salvarNoArquivo(nome, codigo, valor, qtdd, numProdutos);

              printf("Produto modificado com sucesso!\n");
              break;
            }
          }

          if (!encontrado) {
            printf("Produto não encontrado.\n");
          }
        }
        break;

      case 4:
        excluirProduto(nome, codigo, valor, qtdd, &numProdutos);
        break;

      case 0:
        printf("Saindo do programa.\n");
        break;

      default:
        printf("Opção inválida. Tente novamente.\n");
    }
  } while (opcao != 0);

  return 0;
}

void salvarNoArquivo(char nome[MAX_PRODUTOS][50], int codigo[MAX_PRODUTOS],
                     float valor[MAX_PRODUTOS], int qtdd[MAX_PRODUTOS],
                     int numProdutos) {
  FILE *arquivo = fopen("estoque.csv", "w");

  if (arquivo == NULL) {
    perror("Erro ao abrir o arquivo");
    exit(EXIT_FAILURE);
  }

  fprintf(arquivo, "Produto,Código,Valor,Quantidade\n");

  for (int j = 0; j < numProdutos; j++) {
    fprintf(arquivo, "%s,%d,%.2f,%d\n", nome[j], codigo[j], valor[j], qtdd[j]);
  }

  fclose(arquivo);
}

void excluirProduto(char nome[MAX_PRODUTOS][50], int codigo[MAX_PRODUTOS],
                    float valor[MAX_PRODUTOS], int qtdd[MAX_PRODUTOS],
                    int *numProdutos) {
  int codigoExcluir;
  int encontrado = 0;

  printf("Digite o código do produto que deseja excluir: ");
  scanf("%d", &codigoExcluir);

  for (int i = 0; i < *numProdutos; i++) {
    if (codigo[i] == codigoExcluir) {
      printf("Produto encontrado. Tem certeza que deseja excluir? (1 - Sim, 0 - Não): ");

      int confirmacao;
      scanf("%d", &confirmacao);

      if (confirmacao == 1) {
        
        strcpy(nome[i], nome[*numProdutos - 1]);
        codigo[i] = codigo[*numProdutos - 1];
        valor[i] = valor[*numProdutos - 1];
        qtdd[i] = qtdd[*numProdutos - 1];

        (*numProdutos)--;

        encontrado = 1;
        printf("Produto excluído com sucesso!\n");

        salvarNoArquivo(nome, codigo, valor, qtdd, *numProdutos);
      } else {
        printf("Exclusão cancelada.\n");
      }

      break;
    }
  }

  if (!encontrado) {
    printf("Produto não encontrado.\n");
  }
}
