#include <stdio.h> // Biblioteca padrão de entrada e saída
#include <stdlib.h> // Biblioteca padrão para alocação de memória e funções gerais
#include <string.h> // Biblioteca para manipulação de strings
#include <time.h> // Biblioteca para medir o tempo

#define ORDEM 4 // Definindo a ordem da árvore B
#define TAMANHO_TABELA_HASH 40000 // Definindo o tamanho da tabela hash

// Estrutura de um registro na árvore
typedef struct Registro {
    int chave; // Chave do registro
    long int endereco; // Endereço no arquivo onde o registro está armazenado
} Registro;

// Estrutura de um nó da árvore B
typedef struct NoB {
    int num_chaves; // Número de chaves no nó
    Registro registros[ORDEM - 1]; // Array de registros no nó
    struct NoB *filhos[ORDEM]; // Array de ponteiros para os filhos
    int folha; // Flag indicando se o nó é uma folha
} NoB;

// Estrutura para um registro na tabela hash
typedef struct HashNode {
    char coluna[64]; // Nome da coluna
    long int endereco; // Endereço no arquivo onde o registro está armazenado
    struct HashNode *proximo; // Ponteiro para o próximo nó na lista
} HashNode;

// Tabela hash
HashNode *tabela_hash[TAMANHO_TABELA_HASH]; // Array de ponteiros para nós hash

// Função hash
unsigned int funcao_hash(char *coluna) {
    unsigned int hash = 0;
    while (*coluna) {
        hash = (hash << 5) + *coluna++; // Calcula o hash acumulando valores dos caracteres
    }
    return hash % TAMANHO_TABELA_HASH; // Retorna o índice da tabela hash
}

// Função para remover espaços extras no final de uma string
void trim(char *str) {
    int i = strlen(str) - 1;
    while (i >= 0 && str[i] == ' ') { // Remove espaços em branco do final da string
        str[i] = '\0';
        i--;
    }
}

// Inserir na tabela hash
void inserir_tabela_hash(char *coluna, long int endereco) {
    trim(coluna); // Remove espaços extras da coluna
    unsigned int indice = funcao_hash(coluna); // Calcula o índice da tabela hash
    HashNode *novo = (HashNode *)malloc(sizeof(HashNode)); // Aloca memória para um novo nó hash
    strcpy(novo->coluna, coluna); // Copia a coluna para o novo nó
    novo->endereco = endereco; // Define o endereço no novo nó
    novo->proximo = tabela_hash[indice]; // Insere o novo nó na lista ligada
    tabela_hash[indice] = novo; // Atualiza a tabela hash
}

// Buscar na tabela hash (modificada para retornar todos os resultados)
void buscar_tabela_hash(char *coluna, long int *resultados, int *num_resultados) {
    trim(coluna); // Remove espaços extras da coluna
    unsigned int indice = funcao_hash(coluna); // Calcula o índice da tabela hash
    HashNode *atual = tabela_hash[indice]; // Ponteiro para o nó atual na lista ligada
    *num_resultados = 0; // Inicializa o número de resultados encontrados
    while (atual) {
        if (strcmp(atual->coluna, coluna) == 0) { // Se a coluna corresponder
            resultados[(*num_resultados)++] = atual->endereco; // Armazena o endereço no array de resultados
        }
        atual = atual->proximo; // Avança para o próximo nó na lista
    }
}

// Função para criar um nó
NoB *criar_no(int folha) {
    NoB *no = (NoB *)malloc(sizeof(NoB)); // Aloca memória para um novo nó
    no->num_chaves = 0; // Inicializa o número de chaves
    no->folha = folha; // Define se o nó é uma folha
    for (int i = 0; i < ORDEM; i++) {
        no->filhos[i] = NULL; // Inicializa todos os ponteiros para os filhos como NULL
    }
    return no; // Retorna o nó criado
}

// Função para buscar na árvore B
int buscar(NoB *raiz, int chave, FILE *arquivo) {
    int i = 0;
    while (i < raiz->num_chaves && chave > raiz->registros[i].chave) {
        i++; // Encontra a posição da chave ou do filho apropriado
    }

    if (i < raiz->num_chaves && chave == raiz->registros[i].chave) {
        fseek(arquivo, raiz->registros[i].endereco, SEEK_SET); // Move o ponteiro do arquivo para o endereço do registro
        char linha[128];
        if (fgets(linha, sizeof(linha), arquivo)) { // Lê a linha do arquivo
            printf("Registro encontrado: %s", linha); // Imprime o registro encontrado
        }
        return 1; // Retorna sucesso
    }

    if (raiz->folha) { // Se for uma folha, a chave não está presente
        return 0; // Retorna falha
    }

    return buscar(raiz->filhos[i], chave, arquivo); // Recursivamente busca no filho apropriado
}

// Função para dividir um nó
void dividir_no(NoB *pai, int indice, NoB *no) {
    NoB *novo_no = criar_no(no->folha); // Cria um novo nó
    novo_no->num_chaves = ORDEM / 2 - 1; // Define o número de chaves no novo nó

    for (int j = 0; j < ORDEM / 2 - 1; j++) {
        novo_no->registros[j] = no->registros[j + ORDEM / 2]; // Copia as chaves para o novo nó
    }

    if (!no->folha) { // Se não for uma folha, copia os filhos
        for (int j = 0; j < ORDEM / 2; j++) {
            novo_no->filhos[j] = no->filhos[j + ORDEM / 2];
        }
    }

    no->num_chaves = ORDEM / 2 - 1; // Atualiza o número de chaves no nó original

    for (int j = pai->num_chaves; j >= indice + 1; j--) {
        pai->filhos[j + 1] = pai->filhos[j]; // Move os filhos do pai para dar espaço ao novo nó
    }

    pai->filhos[indice + 1] = novo_no; // Define o novo nó como filho do pai

    for (int j = pai->num_chaves - 1; j >= indice; j--) {
        pai->registros[j + 1] = pai->registros[j]; // Move as chaves do pai para dar espaço à nova chave
    }

    pai->registros[indice] = no->registros[ORDEM / 2 - 1]; // Copia a chave do meio para o pai
    pai->num_chaves++; // Incrementa o número de chaves no pai
}

// Função para inserir uma chave na árvore B
void inserir_nao_cheio(NoB *no, int chave, long int endereco) {
    int i = no->num_chaves - 1;

    if (no->folha) {
        while (i >= 0 && chave < no->registros[i].chave) {
            no->registros[i + 1] = no->registros[i]; // Move as chaves para dar espaço à nova chave
            i--;
        }

        no->registros[i + 1].chave = chave; // Insere a nova chave
        no->registros[i + 1].endereco = endereco; // Insere o endereço correspondente
        no->num_chaves++; // Incrementa o número de chaves no nó
    } else {
        while (i >= 0 && chave < no->registros[i].chave) {
            i--; // Encontra o filho apropriado para a nova chave
        }
        i++;

        if (no->filhos[i]->num_chaves == ORDEM - 1) {
            dividir_no(no, i, no->filhos[i]); // Divide o filho se estiver cheio

            if (chave > no->registros[i].chave) {
                i++; // Ajusta o índice após a divisão
            }
        }
        inserir_nao_cheio(no->filhos[i], chave, endereco); // Insere recursivamente no filho apropriado
    }
}

// Função principal para inserir uma chave na árvore B
void inserir(NoB **raiz, int chave, long int endereco) {
    NoB *r = *raiz;
    if (r->num_chaves == ORDEM - 1) { // Se a raiz estiver cheia, divide a raiz
        NoB *novo = criar_no(0); // Cria um novo nó
        *raiz = novo; // Atualiza a raiz
        novo->filhos[0] = r; // Define a antiga raiz como filho do novo nó
        dividir_no(novo, 0, r); // Divide a antiga raiz
        inserir_nao_cheio(novo, chave, endereco); // Insere a nova chave
    } else {
        inserir_nao_cheio(r, chave, endereco); // Insere a nova chave na raiz não cheia
    }
}

// Função principal
int main() {
    FILE *arquivo = fopen("produtos.txt", "r"); // Abre o arquivo de produtos
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n"); // Exibe mensagem de erro se o arquivo não puder ser aberto
        return 1;
    }

    NoB *raiz = criar_no(1); // Cria a raiz da árvore B
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
                fseek(arquivo, 0, SEEK_SET); // Posiciona o ponteiro no início do arquivo
                while (fgets(linha, sizeof(linha), arquivo)) { // Lê cada linha do arquivo
                    int chave;
                    sscanf(linha, "%d", &chave); // Extrai a chave da linha
                    inserir(&raiz, chave, endereco); // Insere a chave na árvore B
                    endereco = ftell(arquivo); // Atualiza o endereço para o próximo registro
                }
                clock_t fim = clock();
                printf("Indice na arvore B criado em %.3f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
                break;
            }
            case 2: {
                clock_t inicio = clock();
                endereco = 0;
                fseek(arquivo, 0, SEEK_SET); // Posiciona o ponteiro no início do arquivo
                while (fgets(linha, sizeof(linha), arquivo)) { // Lê cada linha do arquivo
                    char coluna[64];
                    sscanf(linha, "%*d;%63[^;]", coluna); // Extrai a coluna da linha
                    inserir_tabela_hash(coluna, endereco); // Insere a coluna na tabela hash
                    endereco = ftell(arquivo); // Atualiza o endereço para o próximo registro
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
                if (!buscar(raiz, chave_busca, arquivo)) { // Busca a chave na árvore B
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
                long int resultados[100]; // Array para armazenar os endereços dos resultados
                int num_resultados = 0;
                buscar_tabela_hash(coluna_busca, resultados, &num_resultados); // Busca a coluna na tabela hash
                if (num_resultados > 0) {
                    for (int i = 0; i < num_resultados; i++) {
                        fseek(arquivo, resultados[i], SEEK_SET); // Posiciona o ponteiro no endereço do resultado
                        if (fgets(linha, sizeof(linha), arquivo)) { // Lê a linha do arquivo
                            printf("Registro encontrado para coluna '%s': %s", coluna_busca, linha);
                        }
                    }
                } else {
                    printf("Coluna '%s' nao encontrada.\n", coluna_busca);
                }
                clock_t fim = clock();
                printf("Busca na tabela hash concluida em %.5f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
                break;
            }
            case 5:
                printf("Saindo...\n"); // Mensagem de saída
                break;
            default:
                printf("Opcao invalida!\n"); // Mensagem para opção inválida
        }
    } while (opcao != 5);

    fclose(arquivo); // Fecha o arquivo
    return 0; // Retorna sucesso
}

