# **Implementação de Índices com Árvore B e Tabela Hash**

Este projeto implementa um sistema híbrido de indexação utilizando **Árvore B** e **Tabela Hash** para realizar operações de busca eficiente em um arquivo de registros. A Árvore B é usada para indexar chaves numéricas, enquanto a Tabela Hash indexa colunas representadas por strings.

---

## **Descrição do Projeto**

### **1. Estruturas Utilizadas**

#### **Árvore B**
- **Descrição:** Estrutura de dados balanceada utilizada para busca e inserção eficiente em grandes volumes de dados.
- **Ordem da Árvore:** 4 (definida pela constante `ORDEM`).
- **Capacidade dos Nós:**
  - Cada nó pode conter no máximo `ORDEM - 1` chaves.
  - Cada nó pode ter no máximo `ORDEM` filhos.
- **Aplicação:** Busca eficiente baseada em chaves numéricas.
- **Operações Implementadas:**
  - Inserção de novos registros com divisão de nós.
  - Busca recursiva por chaves na árvore.

#### **Tabela Hash**
- **Descrição:** Estrutura utilizada para indexar colunas representadas por strings.
- **Tamanho da Tabela:** 1000 (definido pela constante `TAMANHO_TABELA_HASH`).
- **Função Hash:**
  - Fórmula: `hash = (hash << 5) + caractere_atual`.
  - Distribui as strings uniformemente nos índices da tabela.
- **Estratégia de Resolução de Colisões:**
  - **Encadeamento Separado:** Cada índice da tabela armazena uma lista ligada para gerenciar colisões.

---

## **Detalhamento das Funções**

### **1. Funções da Árvore B**

#### **Função `criar_no`**
- Cria um novo nó para a Árvore B.
- Inicializa o nó como folha ou interno, conforme necessário.

#### **Função `dividir_no`**
- Divide um nó que atingiu sua capacidade máxima.
- Move metade das chaves para um novo nó e ajusta os ponteiros no nó pai.

#### **Função `inserir_nao_cheio`**
- Insere uma chave em um nó que ainda não está cheio.
- Adiciona diretamente em nós folha ou delega para subárvores.

#### **Função `inserir`**
- Insere uma chave na Árvore B, começando pela raiz.
- Caso a raiz esteja cheia, cria uma nova raiz e divide o nó original.

#### **Função `buscar`**
- Realiza busca recursiva de uma chave na Árvore B.
- Caso a chave seja encontrada, retorna o registro correspondente no arquivo.

---

### **2. Funções da Tabela Hash**

#### **Função `funcao_hash`**
- Calcula o índice da tabela hash para uma string utilizando deslocamento de bits.

#### **Função `trim`**
- Remove espaços extras ao final de uma string para garantir consistência nas buscas.

#### **Função `inserir_tabela_hash`**
- Insere um registro na tabela hash.
- Cria um novo nó de hash e o conecta à lista ligada correspondente ao índice.

#### **Função `buscar_tabela_hash`**
- Realiza busca de uma coluna na tabela hash.
- Navega pela lista ligada no índice até encontrar a coluna ou retornar "não encontrada".

---

## **Características da Árvore B**

- **Ordem:** 4
- **Capacidade dos Nós:**
  - Máximo de 3 chaves por nó (`ORDEM - 1`).
  - Máximo de 4 filhos por nó (`ORDEM`).
- **Operações:**
  - **Inserção e Remoção:** \(O(\log n)\), onde \(n\) é o número de chaves.
  - **Busca:** \(O(\log n)\).

---

## **Características da Tabela Hash**

- **Tamanho da Tabela:** 1000
- **Função Hash:** Baseada em deslocamento de bits.
- **Resolução de Colisões:** Encadeamento separado (listas ligadas).
- **Complexidade:**
  - **Inserção e Busca:** \(O(1)\) no melhor caso, \(O(n)\) no pior caso.

---

## **Exemplo de Uso**

O sistema oferece as seguintes funcionalidades:

1. **Criar Índice na Árvore B:**
   - Indexa as chaves numéricas do arquivo de registros na Árvore B.

2. **Criar Índice na Tabela Hash:**
   - Indexa as colunas baseadas em strings na Tabela Hash.

3. **Buscar na Árvore B:**
   - Realiza busca eficiente de uma chave numérica e exibe o registro correspondente.

4. **Buscar na Tabela Hash:**
   - Busca uma coluna pela string associada e exibe o registro correspondente.

5. **Sair do Programa:**
   - Encerra o sistema.

---

## **Como Executar**

1. Compile o programa com um compilador C, por exemplo:
   ```bash
   gcc -o indices indices.c
