#pragma once
#include <stdbool.h>

/**
 * Tipo para funções usadas para desalocar os itens void* guardados nas estruturas.
 * @see no_fila_prio_desalocar, fila_prio_desalocar, arvore_bin_desalocar
 */
typedef void (*Desalocador)(void *);

/**
 * Uma árvore binária.
 */
typedef struct ArvoreBin {
  /**
   * A função usada para desalocar o item, se houver, ao chamar {@link arvore_bin_desalocar}
   */
  Desalocador desalocar;

  /**
   * O item associado a este nó.
   */
  void *item;

  /**
   * O filho esquerdo deste nó.
   */
  struct ArvoreBin *esquerda;

  /**
   * O filho direito deste nó.
   */
  struct ArvoreBin *direita;
} ArvoreBin;

/**
 * Um nó de uma fila de prioridade.
 */
typedef struct NoFilaPrio {
  /**
   * A função usada para desalocar o item, se houver, ao chamar {@link no_fila_prio_desalocar}
   */
  Desalocador desalocar;

  /**
   * O item associado a este nó.
   */
  void *item;

  /**
   * A prioridade deste nó.
   */
  long prioridade;

  /**
   * O próximo nó da fila.
   */
  struct NoFilaPrio *prox;
} NoFilaPrio;

/*
 * Uma fila de prioridade
 * Os nós são organizados de forma crescente. Os nós de menor prioridade são desenfileirados antes.
 */
typedef struct FilaPrio {
  /**
   * O primeiro nó da fila.
   */
  NoFilaPrio *inicio;
} FilaPrio;

/**
 * Cria um nó de árvore binária.
 * @memberof ArvoreBin
 * @param item o item a ser armazenado
 * @param desalocar uma função que possa desalocar o item
 * @param esquerda o filho esquerdo da nova árvore
 * @param direita o filho direito da nova árvore
 * @return um ponteiro para o nó criado
 */
ArvoreBin *arvore_bin_criar(void *item, Desalocador desalocar, ArvoreBin *esquerda, ArvoreBin *direita);

/**
 * Desaloca um nó e todos os filhos, e os itens, se houverem.
 * @memberof ArvoreBin
 * @param arvore a árvore
 */
void arvore_bin_desalocar(ArvoreBin *arvore);

/**
 * Calcula a quantidade de nós que existe em uma árvore.
 * @memberof ArvoreBin
 * @param arvore a árvore
 * @return a quantidade de nós
 */
size_t arvore_bin_tamanho(ArvoreBin *arvore);

/**
 * Verifica se um nó de árvore é folha (não tem filhos).
 * @memberof ArvoreBin
 * @param arvore o nó
 * @return true se o nó da arvore não tiver filhos, false se tiver
 */
bool arvore_bin_e_folha(ArvoreBin *arvore);

/**
 * Pega o item de um nó de árvore.
 * @memberof ArvoreBin
 * @param arvore o nó
 * @return o item dentro do nó
 */
void *arvore_bin_item(ArvoreBin *arvore);

/**
 * Retorna o filho esquerdo do nó atual.
 * @memberof ArvoreBin
 * @param arvore o nó
 * @return o nó filho esquerdo
 */
ArvoreBin *arvore_bin_esquerda(ArvoreBin *arvore);

/**
 * Retorna o filho direito do nó atual.
 * @memberof ArvoreBin
 * @param arvore o nó
 * @return o nó filho direito
 */
ArvoreBin *arvore_bin_direita(ArvoreBin *arvore);

/**
 * Cria uma fila de prioridade.
 * @memberof FilaPrio
 * @return um ponteiro para a fila criada
 */
FilaPrio *fila_prio_criar();

/**
 * Desaloca uma fila de prioridade, todos os nós, e os itens, se houverem.
 * @memberof FilaPrio
 * @param fila a fila
 */
void fila_prio_desalocar(FilaPrio *fila);

/**
 * Retorna se uma fila de prioridade está vazia.
 * @memberof FilaPrio
 * @param fila a fila
 * @return true se estiver vazia, false se não estiver
 */
bool fila_prio_esta_vazia(FilaPrio *fila);

/**
 * Enfileira/adiciona um item em uma fila de prioridade.
 * @memberof FilaPrio
 * @param fila a fila
 * @param item o item
 * @param desalocar uma função que possa desalocar o item
 * @param prioridade a prioridade
 */
void fila_prio_enfileirar(FilaPrio *fila, void *item, Desalocador desalocar, long prioridade);

/**
 * Desenfileira/retira o item de maior prioridade de uma fila de prioridade.
 * @memberof FilaPrio
 * @param fila a fila
 * @return o nó retirado
 */
NoFilaPrio *fila_prio_desenfileirar(FilaPrio *fila);

/**
 * Desaloca um nó de fila e o item dentro, se houver.
 * @memberof NoFilaPrio
 * @param no o nó
 */
void no_fila_prio_desalocar(NoFilaPrio *no);

/**
 * Pega o item armazenado em um nó de fila.
 * @memberof NoFilaPrio
 * @param no o nó
 * @return o item dentro do nó
 */
void *no_fila_prio_item(NoFilaPrio *no);

/**
 * Remove o item armazenado em um nó de fila e o retorna.
 *
 * Use em vez de {@link no_fila_prio_item} se for necessário que o item não seja desalocado ao chamar
 * {@link no_fila_prio_desalocar}.
 * @memberof NoFilaPrio
 * @param no o nó
 * @return o item dentro do nó
 */
void *no_fila_prio_tomar_item(NoFilaPrio *no);

/**
 * Retorna a proridade de um nó de fila.
 *
 * @memberof NoFilaPrio
 * @param no o nó
 * @returns a prioridade do nó
 */
long no_fila_prio_prioridade(NoFilaPrio *no);
