#pragma once
#include <stdint.h>
#include <stdio.h>

#include "estruturas.h"

/**
 * Um caminho da codificação Huffman de um caractere.
 * Exemplo: Se o caractere for codificado como 1011, qtd_bits será 4 e bits terá o valor binário 1101.
 * Razão da representação invertida: é mais simples desse jeito.
 */
typedef struct Caminho {
  /**
   * A quantidade de bits no caminho.
   */
  unsigned qtd_bits;

  /**
   * Os bits do caminho.
   */
  unsigned bits;
} Caminho;

/**
 * Comprime um arquivo usando codificação de Huffman e escreve um cabeçalho e a tabela.
 * @param arquivo_in o arquivo descomprimido
 * @param arquivo_out o arquivo onde o conteúdo comprimido será salvo
 */
void comprimir(FILE *arquivo_in, FILE *arquivo_out);

/**
 * Lê um arquivo por completo e salva a frequência de cada caractere em um array.
 * @param arquivo o arquivo a ser lido
 * @param freqs o array de frequências, de tamanho 256
 */
void ler_frequencias(FILE *arquivo, long freqs[]);

/**
 * Transforma um array de frequência de caracteres em uma árvore Huffman.
 * @param freqs o array de frequências
 * @return a árvore Huffman
 */
ArvoreBin *transformar_frequencias_em_arvore(long freqs[]);

/**
 * Percorre a árvore por completo. Ao chegar nas folhas, salva os caminhos desde a raíz em uma tabela.
 * @param no o nó atual, para iniciar, passe a raíz
 * @param tabela a tabela mapeando caracteres para caminhos
 * @param bits os bits do caminho atual, para iniciar, passe 0
 * @param qtd_bits a quantidade de bits no caminho atual, para iniciar, passe 0
 */
void transformar_arvore_em_tabela(ArvoreBin *no, Caminho tabela[], unsigned bits, unsigned qtd_bits);

/**
 * Salva a árvore em representação pré-ordem no arquivo especificado
 *
 * Escreve '*' para os nós que não são folha e usa \ como escape para um '*' folha.
 * @param arvore a árvore
 * @param arquivo o arquivo
 * @return o tamanho da tabela, incluindo os escapes
 */
uint16_t salvar_arvore_preordem(ArvoreBin *arvore, FILE *arquivo);

/**
 * Percorre um arquivo de entrada e salva os caminhos de cada caractere em um arquivo de saída de acordo com a
 * tabela passada.

 * O lixo são os bits que devem ser ignorados no último byte comprimido.
 * @param arquivo_in o arquivo de entrada
 * @param arquivo_out o arquivo de saída
 * @param tabela a tabela com os caminhos
 * @return a quantidade de bits de lixo no último byte.
 */
uint8_t comprimir_com_tabela(FILE *arquivo_in, FILE *arquivo_out, Caminho tabela[]);
