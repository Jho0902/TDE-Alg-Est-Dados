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

## Comparativo: Tabela Hash x Árvore B x Índice Exaustivo

| Critério                  | Árvore B                           | Tabela Hash                        | Índice Exaustivo                 |
|---------------------------|-------------------------------------|-------------------------------------|-----------------------------------|
| **Tempo de Criação do Índice** | 0.089 segundos                    | 0.095 segundos                    | 0.01 segundos                    |
| **Tempo Médio de Criação**     | **0.092 segundos**               | **0.092 segundos**                | **0.01 segundos**                |
| **Tempo de Busca**         | ~0 segundos (para 20 mil itens)    | ~0 segundos (para 20 mil itens)    | 0.00 segundos                    |
| **Estrutura**              | Árvore balanceada                 | Array com encadeamento separado    | Arquivo com índices sequenciais  |
| **Organização dos Dados**  | Ordenada por chaves               | Sem ordem específica               | Ordenada sequencialmente         |
| **Eficiência de Busca**    | Logarítmica (O(log n))            | Constante (O(1), idealmente)       | Linear (O(n)) para leitura de índices |
| **Suporte a Intervalos**   | Sim                               | Não                                | Sim                              |
| **Desempenho com Colisões**| Não há colisões                   | Encadeamento separado resolve colisões | Não se aplica                   |
| **Uso de Memória**         | Mais alta (devido à estrutura da árvore) | Menor (apenas listas ligadas e array) | Depende do tamanho do índice e do arquivo binário |

### Análise de Desempenho
- **Criação do Índice**: O índice exaustivo foi extremamente eficiente na criação, com um tempo de apenas **0.01 segundos**, sendo significativamente mais rápido que as outras estruturas.
- **Busca**: O índice exaustivo também teve desempenho superior, com um tempo de busca de **0.00 segundos**.
- **Cenários de Uso**:
  - A **Árvore B** é ideal para buscas ordenadas e intervalos de valores em grandes bases de dados.
  - A **Tabela Hash** é mais eficiente para buscas pontuais em chaves específicas.
  - O **Índice Exaustivo** é extremamente rápido em cenários com dados menores ou onde o índice completo pode ser mantido em memória.


