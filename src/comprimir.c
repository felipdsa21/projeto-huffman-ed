#include <stdlib.h>
#include <string.h>

#include "comprimir.h"
#include "estruturas.h"
#include "utils.h"

void comprimir(FILE *arquivo_in, FILE *arquivo_out) {
  ArvoreBin *arvore;
  Caminho tabela[256];
  long freqs[256];
  uint16_t cabecalho, tamanho_arvore;
  uint8_t lixo;

  ler_frequencias(arquivo_in, freqs);
  arvore = transformar_frequencias_em_arvore(freqs);

  if (arvore_bin_e_folha(arvore)) {
    // Caso especial de existir apenas um tipo de caractere (repetido ou não) no arquivo
    tabela[ptr_para_uint8(arvore_bin_item(arvore))].qtd_bits = 1;
    tabela[ptr_para_uint8(arvore_bin_item(arvore))].bits = 0;
  } else {
    transformar_arvore_em_tabela(arvore, tabela, 0, 0);
  }

  // Pula os dois primeiros bytes no arquivo de saída para salvar a tabela
  fseek(arquivo_out, 2, SEEK_CUR);
  tamanho_arvore = salvar_arvore_preordem(arvore, arquivo_out);

  // Volta para o começo do arquivo de entrada e comprime o contéudo
  fseek(arquivo_in, 0, SEEK_SET);
  lixo = comprimir_com_tabela(arquivo_in, arquivo_out, tabela);

  // Volta para o começo do arquivo de saída e salva o cabeçalho
  fseek(arquivo_out, 0, SEEK_SET);
  cabecalho = (uint16_t)(lixo << 13) | tamanho_arvore;
  fputc(cabecalho >> 8, arquivo_out);
  fputc(cabecalho & 0xFF, arquivo_out);

  arvore_bin_desalocar(arvore);
}

void ler_frequencias(FILE *arquivo, long freqs[]) {
  int caractere;

  // Zera a tabela
  memset(freqs, 0, sizeof(*freqs) * 256);

  while ((caractere = fgetc(arquivo)) != EOF) {
    freqs[caractere]++;
  }
}

ArvoreBin *transformar_frequencias_em_arvore(long freqs[]) {
  ArvoreBin *huffman, *novo_no, *esquerda, *direita;
  FilaPrio *fila;
  NoFilaPrio *menor1, *menor2;
  long freq, nova_prioridade;
  unsigned i;

  fila = fila_prio_criar();

  // Coloca as frequências em uma fila de prioridade
  for (i = 0; i < 256; i++) {
    freq = freqs[i];
    if (freq > 0) {
      novo_no = arvore_bin_criar(uint8_para_ptr((uint8_t)i), free, NULL, NULL);
      fila_prio_enfileirar(fila, novo_no, (Desalocador)arvore_bin_desalocar, freq);
    }
  }

  huffman = NULL;

  while (!fila_prio_esta_vazia(fila)) {
    // Desenfileira os nós com as menores frequências/prioridades
    menor1 = fila_prio_desenfileirar(fila);
    menor2 = fila_prio_desenfileirar(fila);

    // Se houver apenas um nó, o algoritmo finaliza
    if (!menor2) {
      huffman = (ArvoreBin *)no_fila_prio_tomar_item(menor1);
      no_fila_prio_desalocar(menor1);
      break;
    }

    // Cria um novo nó da árvore e torna menor1 e menor2 filhos dele
    esquerda = (ArvoreBin *)no_fila_prio_tomar_item(menor1);
    direita = (ArvoreBin *)no_fila_prio_tomar_item(menor2);
    novo_no = arvore_bin_criar(uint8_para_ptr('*'), free, esquerda, direita);

    // Coloca o nó na fila de prioridade
    nova_prioridade = no_fila_prio_prioridade(menor1) + no_fila_prio_prioridade(menor2);
    fila_prio_enfileirar(fila, novo_no, (Desalocador)arvore_bin_desalocar, nova_prioridade);

    no_fila_prio_desalocar(menor1);
    no_fila_prio_desalocar(menor2);
  }

  fila_prio_desalocar(fila);
  return huffman;
}

void transformar_arvore_em_tabela(ArvoreBin *no, Caminho tabela[], unsigned bits, unsigned qtd_bits) {
  uint8_t caractere;

  if (arvore_bin_esquerda(no)) {
    transformar_arvore_em_tabela(arvore_bin_esquerda(no), tabela, bits, qtd_bits + 1);
  }

  if (arvore_bin_direita(no)) {
    transformar_arvore_em_tabela(arvore_bin_direita(no), tabela, set_bit(bits, qtd_bits), qtd_bits + 1);
  }

  if (arvore_bin_e_folha(no)) {
    caractere = ptr_para_uint8(arvore_bin_item(no));
    tabela[caractere].qtd_bits = qtd_bits;
    tabela[caractere].bits = bits;
  }
}

uint16_t salvar_arvore_preordem(ArvoreBin *arvore, FILE *arquivo) {
  uint16_t tamanho;
  uint8_t caractere;

  tamanho = 0;

  if (arvore) {
    caractere = ptr_para_uint8(arvore_bin_item(arvore));

    if (caractere == '\\' || (caractere == '*' && arvore_bin_e_folha(arvore))) {
      fputc('\\', arquivo);
      tamanho++;
    }

    fputc(caractere, arquivo);
    tamanho++;

    tamanho += salvar_arvore_preordem(arvore_bin_esquerda(arvore), arquivo);
    tamanho += salvar_arvore_preordem(arvore_bin_direita(arvore), arquivo);
  }

  return tamanho;
}

uint8_t comprimir_com_tabela(FILE *arquivo_in, FILE *arquivo_out, Caminho tabela[]) {
  Caminho caminho;
  int caractere;
  uint8_t bits, bit_atual, i;

  bits = 0;
  bit_atual = 0;

  // Para cada caractere
  while ((caractere = fgetc(arquivo_in)) != EOF) {
    caminho = tabela[caractere];

    for (i = 0; i < caminho.qtd_bits; i++) {
      if (is_bit_i_set(caminho.bits, i)) {
        // Se o bit for 1, usa set_bit no byte compactado
        bits = (uint8_t)set_bit(bits, 7 - bit_atual);
      }

      bit_atual++;

      // Se o byte compactado estiver completo
      if (bit_atual == 8) {
        fputc(bits, arquivo_out);
        bits = 0;
        bit_atual = 0;
      }
    }
  }

  // Se um byte incompleto houver sobrado
  if (bit_atual > 0) {
    fputc(bits, arquivo_out);
  }

  // Retorna a quantidade de bits incompletos como lixo
  return 8 - bit_atual;
}
