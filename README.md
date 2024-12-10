## Descrição dos Índices de Memória

Este código utiliza duas estruturas principais para índices de memória: uma Árvore B e uma Tabela Hash. Abaixo, seguem as especificações detalhadas de cada estrutura:

### 1. Árvore B
- **Tipo de Árvore**: Árvore B.
- **Ordem da Árvore**: 4 (cada nó pode ter no máximo 3 chaves e 4 filhos).
- **Quantidade de Elementos por Nodo (Grau da Árvore)**:
  - Máximo: `ORDEM - 1` (3 chaves por nó).
  - Mínimo (exceto para a raiz): `(ORDEM / 2) - 1` (1 chave por nó).
- **Utilização**: A árvore B é usada para indexar registros do arquivo de entrada com base em suas chaves inteiras. Cada chave é associada a um endereço no arquivo.
- **Operações Implementadas**:
  - Inserção de chaves.
  - Busca de chaves.
  - Divisão de nós quando cheios.

### 2. Tabela Hash
- **Tamanho da Tabela**: 40.000 posições (definido pela constante `TAMANHO_TABELA_HASH`).
- **Estratégia de Resolução de Colisões**: Encadeamento separado (cada posição da tabela contém uma lista ligada de nós `HashNode`).
- **Função Hash**: A função utiliza uma operação de deslocamento de bits (`hash = (hash << 5) + *coluna`) e calcula o índice como o módulo do tamanho da tabela (`hash % TAMANHO_TABELA_HASH`).
- **Utilização**: A tabela hash é usada para indexar registros do arquivo de entrada com base em valores textuais (nomes de colunas).
- **Operações Implementadas**:
  - Inserção de valores textuais.
  - Busca de valores textuais com suporte para múltiplos resultados.

### Observações Adicionais
Ambas as estruturas indexam registros por endereços físicos no arquivo de entrada (`produtos.txt`). A árvore B é ideal para buscas ordenadas e intervalos, enquanto a tabela hash oferece buscas rápidas para chaves específicas.

## Comparativo: Tabela Hash x Árvore B

| Critério                  | Árvore B                           | Tabela Hash                        |
|---------------------------|-------------------------------------|-------------------------------------|
| **Tempo de Criação do Índice** | 0.089 segundos                    | 0.095 segundos                    |
| **Tempo de Busca**         | ~0 segundos (para 20 mil itens)    | ~0 segundos (para 20 mil itens)    |
| **Estrutura**              | Árvore balanceada                 | Array com encadeamento separado    |
| **Organização dos Dados**  | Ordenada por chaves               | Sem ordem específica               |
| **Eficiência de Busca**    | Logarítmica (O(log n))            | Constante (O(1), idealmente)       |
| **Suporte a Intervalos**   | Sim                               | Não                                |
| **Desempenho com Colisões**| Não há colisões                   | Encadeamento separado resolve colisões |
| **Uso de Memória**         | Mais alta (devido à estrutura da árvore) | Menor (apenas listas ligadas e array) |

### Análise de Desempenho
- **Criação do Índice**: A tabela hash foi ligeiramente mais lenta na criação do índice devido ao tempo de alocação de nós na lista encadeada.
- **Busca**: Ambas as buscas foram extremamente rápidas (~0 segundos) no arquivo testado, que continha cerca de 20 mil itens.
- **Cenários de Uso**:
  - A **Árvore B** é ideal para buscas ordenadas e intervalos de valores.
  - A **Tabela Hash** é mais eficiente para buscas pontuais, mas não suporta operações baseadas em intervalos.

