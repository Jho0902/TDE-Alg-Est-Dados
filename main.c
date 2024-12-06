#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ORDEM 4 // Defina a ordem da arvore B
#define TAMANHO_TABELA_HASH 1000

// Estrutura de um registro na arvore
typedef struct Registro {
    int chave;
    long int endereco;
} Registro;

// Estrutura de um no da arvore B
typedef struct NoB {
    int num_chaves;
    Registro registros[ORDEM - 1];
    struct NoB *filhos[ORDEM];
    int folha;
} NoB;

// Estrutura para um registro na tabela hash
typedef struct HashNode {
    char coluna[64];
    long int endereco;
    struct HashNode *proximo;
} HashNode;

// Tabela hash
HashNode *tabela_hash[TAMANHO_TABELA_HASH];

// Função hash
unsigned int funcao_hash(char *coluna) {
    unsigned int hash = 0;
    while (*coluna) {
        hash = (hash << 5) + *coluna++;
    }
    return hash % TAMANHO_TABELA_HASH;
}

// Função para remover espaços extras no final de uma string
void trim(char *str) {
    int i = strlen(str) - 1;
    while (i >= 0 && str[i] == ' ') {
        str[i] = '\0';
        i--;
    }
}

// Inserir na tabela hash
void inserir_tabela_hash(char *coluna, long int endereco) {
    trim(coluna);
    unsigned int indice = funcao_hash(coluna);
    HashNode *novo = (HashNode *)malloc(sizeof(HashNode));
    strcpy(novo->coluna, coluna);
    novo->endereco = endereco;
    novo->proximo = tabela_hash[indice];
    tabela_hash[indice] = novo;
}

// Buscar na tabela hash
long int buscar_tabela_hash(char *coluna) {
    trim(coluna);
    unsigned int indice = funcao_hash(coluna);
    HashNode *atual = tabela_hash[indice];
    while (atual) {
        if (strcmp(atual->coluna, coluna) == 0) {
            return atual->endereco;
        }
        atual = atual->proximo;
    }
    return -1; // Não encontrado
}

// Função para criar um nó
NoB *criar_no(int folha) {
    NoB *no = (NoB *)malloc(sizeof(NoB));
    no->num_chaves = 0;
    no->folha = folha;
    for (int i = 0; i < ORDEM; i++) {
        no->filhos[i] = NULL;
    }
    return no;
}

// Função para buscar na árvore B
int buscar(NoB *raiz, int chave, FILE *arquivo) {
    int i = 0;
    while (i < raiz->num_chaves && chave > raiz->registros[i].chave) {
        i++;
    }

    if (i < raiz->num_chaves && chave == raiz->registros[i].chave) {
        fseek(arquivo, raiz->registros[i].endereco, SEEK_SET);
        char linha[128];
        if (fgets(linha, sizeof(linha), arquivo)) {
            printf("Registro encontrado: %s", linha);
        }
        return 1;
    }

    if (raiz->folha) {
        return 0;
    }

    return buscar(raiz->filhos[i], chave, arquivo);
}

// Função para dividir um nó
void dividir_no(NoB *pai, int indice, NoB *no) {
    NoB *novo_no = criar_no(no->folha);
    novo_no->num_chaves = ORDEM / 2 - 1;

    for (int j = 0; j < ORDEM / 2 - 1; j++) {
        novo_no->registros[j] = no->registros[j + ORDEM / 2];
    }

    if (!no->folha) {
        for (int j = 0; j < ORDEM / 2; j++) {
            novo_no->filhos[j] = no->filhos[j + ORDEM / 2];
        }
    }

    no->num_chaves = ORDEM / 2 - 1;

    for (int j = pai->num_chaves; j >= indice + 1; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }

    pai->filhos[indice + 1] = novo_no;

    for (int j = pai->num_chaves - 1; j >= indice; j--) {
        pai->registros[j + 1] = pai->registros[j];
    }

    pai->registros[indice] = no->registros[ORDEM / 2 - 1];
    pai->num_chaves++;
}

// Função para inserir uma chave na árvore B
void inserir_nao_cheio(NoB *no, int chave, long int endereco) {
    int i = no->num_chaves - 1;

    if (no->folha) {
        while (i >= 0 && chave < no->registros[i].chave) {
            no->registros[i + 1] = no->registros[i];
            i--;
        }

        no->registros[i + 1].chave = chave;
        no->registros[i + 1].endereco = endereco;
        no->num_chaves++;
    } else {
        while (i >= 0 && chave < no->registros[i].chave) {
            i--;
        }
        i++;

        if (no->filhos[i]->num_chaves == ORDEM - 1) {
            dividir_no(no, i, no->filhos[i]);

            if (chave > no->registros[i].chave) {
                i++;
            }
        }
        inserir_nao_cheio(no->filhos[i], chave, endereco);
    }
}

void inserir(NoB **raiz, int chave, long int endereco) {
    NoB *r = *raiz;
    if (r->num_chaves == ORDEM - 1) {
        NoB *novo = criar_no(0);
        *raiz = novo;
        novo->filhos[0] = r;
        dividir_no(novo, 0, r);
        inserir_nao_cheio(novo, chave, endereco);
    } else {
        inserir_nao_cheio(r, chave, endereco);
    }
}

int main() {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    NoB *raiz = criar_no(1);
    int opcao;
    char linha[128];
    long int endereco = 0;

    do {
        printf("\nEscolha uma opcao:\n");
        printf("1. Criar indice na arvore B\n");
        printf("2. Criar indice na tabela hash\n");
        printf("3. Buscar chave na arvore B\n");
        printf("4. Buscar coluna na tabela hash\n");
        printf("5. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                clock_t inicio = clock();
                endereco = 0;
                fseek(arquivo, 0, SEEK_SET);
                while (fgets(linha, sizeof(linha), arquivo)) {
                    int chave;
                    sscanf(linha, "%d", &chave);
                    inserir(&raiz, chave, endereco);
                    endereco = ftell(arquivo);
                }
                clock_t fim = clock();
                printf("Indice na arvore B criado em %.3f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
                break;
            }
            case 2: {
                clock_t inicio = clock();
                endereco = 0;
                fseek(arquivo, 0, SEEK_SET);
                while (fgets(linha, sizeof(linha), arquivo)) {
                    char coluna[64];
                    sscanf(linha, "%*d;%63[^;]", coluna);
                    inserir_tabela_hash(coluna, endereco);
                    endereco = ftell(arquivo);
                }
                clock_t fim = clock();
                printf("Indice na tabela hash criado em %.3f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
                break;
            }
            case 3: {
                int chave_busca;
                printf("Digite a chave para buscar: ");
                scanf("%d", &chave_busca);
                clock_t inicio = clock();
                if (!buscar(raiz, chave_busca, arquivo)) {
                    printf("Chave %d nao encontrada.\n", chave_busca);
                }
                clock_t fim = clock();
                printf("Busca na arvore B concluida em %.5f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
                break;
            }
            case 4: {
                char coluna_busca[64];
                printf("Digite a coluna para buscar: ");
                scanf("%s", coluna_busca);
                clock_t inicio = clock();
                long int endereco_busca = buscar_tabela_hash(coluna_busca);
                if (endereco_busca != -1) {
                    fseek(arquivo, endereco_busca, SEEK_SET);
                    if (fgets(linha, sizeof(linha), arquivo)) {
                        printf("Registro encontrado para coluna '%s': %s", coluna_busca, linha);
                    }
                } else {
                    printf("Coluna '%s' nao encontrada.\n", coluna_busca);
                }
                clock_t fim = clock();
                printf("Busca na tabela hash concluida em %.5f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
                break;
            }
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 5);

    fclose(arquivo);
    return 0;
}
