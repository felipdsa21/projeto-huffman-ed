#include <stdlib.h>

#include "estruturas.h"

ArvoreBin *arvore_bin_criar(void *item, Desalocador desalocar, ArvoreBin *esquerda, ArvoreBin *direita) {
  ArvoreBin *novo_no = (ArvoreBin *)malloc(sizeof(*novo_no));
  novo_no->desalocar = desalocar;
  novo_no->item = item;
  novo_no->esquerda = esquerda;
  novo_no->direita = direita;
  return novo_no;
}

void arvore_bin_desalocar(ArvoreBin *arvore) {
  if (arvore) {
    arvore_bin_desalocar(arvore->esquerda);
    arvore_bin_desalocar(arvore->direita);
    arvore->desalocar(arvore->item);
    free(arvore);
  }
}

size_t arvore_bin_tamanho(ArvoreBin *arvore) {
  if (!arvore) {
    return 0;
  } else {
    return arvore_bin_tamanho(arvore->esquerda) + arvore_bin_tamanho(arvore->direita) + 1;
  }
}

bool arvore_bin_e_folha(ArvoreBin *arvore) {
  return !arvore->esquerda && !arvore->direita;
}

void *arvore_bin_item(ArvoreBin *arvore) {
  return arvore->item;
}

ArvoreBin *arvore_bin_esquerda(ArvoreBin *arvore) {
  return arvore->esquerda;
}

ArvoreBin *arvore_bin_direita(ArvoreBin *arvore) {
  return arvore->direita;
}

FilaPrio *fila_prio_criar() {
  FilaPrio *nova_fila = (FilaPrio *)malloc(sizeof(*nova_fila));
  nova_fila->inicio = NULL;
  return nova_fila;
}

void fila_prio_desalocar(FilaPrio *fila) {
  NoFilaPrio *atual, *prox;

  if (!fila) {
    return;
  }

  atual = fila->inicio;
  while (atual != NULL) {
    prox = atual->prox;
    no_fila_prio_desalocar(atual);
    atual = prox;
  }

  free(fila);
}

bool fila_prio_esta_vazia(FilaPrio *fila) {
  return !fila->inicio;
}

void fila_prio_enfileirar(FilaPrio *fila, void *item, Desalocador desalocar, long prioridade) {
  NoFilaPrio *novo_no, *atual, *anterior;

  novo_no = (NoFilaPrio *)malloc(sizeof(*novo_no));
  novo_no->desalocar = desalocar;
  novo_no->item = item;
  novo_no->prioridade = prioridade;

  anterior = NULL;
  atual = fila->inicio;

  while (atual && prioridade > atual->prioridade) {
    anterior = atual;
    atual = atual->prox;
  }

  novo_no->prox = atual;

  if (!anterior) {
    fila->inicio = novo_no;
  } else {
    anterior->prox = novo_no;
  }
}

NoFilaPrio *fila_prio_desenfileirar(FilaPrio *fila) {
  NoFilaPrio *no;

  if (fila_prio_esta_vazia(fila)) {
    return NULL;
  } else {
    no = fila->inicio;
    fila->inicio = no->prox;
    no->prox = NULL;
    return no;
  }
}

void no_fila_prio_desalocar(NoFilaPrio *no) {
  if (no) {
    no->desalocar(no->item);
    free(no);
  }
}

void *no_fila_prio_item(NoFilaPrio *no) {
  return no->item;
}

void *no_fila_prio_tomar_item(NoFilaPrio *no) {
  void *item = no->item;
  no->item = NULL;
  return item;
}

long no_fila_prio_prioridade(NoFilaPrio *no) {
  return no->prioridade;
}
