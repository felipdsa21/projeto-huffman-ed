#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "extensoes.h"

const char *encontrar_extensao(const char *caminho) {
  const char *ponto;

  // Tenta encontrar o último ponto do caminho
  ponto = strrchr(caminho, '.');

  // Se encontrar, retorna a partir do próximo caractere
  return ponto ? ponto + 1 : "";
}

char *ler_extensao(FILE *arquivo) {
  char extensao[6];
  uint16_t tamanho_arvore;
  uint8_t cabecalho[2], tamanho_extensao;

  // Pula o cabeçalho e a árvore
  fread(&cabecalho, 1, sizeof(cabecalho), arquivo);
  tamanho_arvore = (uint16_t)((cabecalho[0] & 0x1F) << 8) | cabecalho[1];
  fseek(arquivo, tamanho_arvore, SEEK_CUR);

  // Lê a extensão
  tamanho_extensao = (uint8_t)fgetc(arquivo) >> 5;
  fread(extensao, 1, tamanho_extensao, arquivo);
  extensao[tamanho_extensao] = '\0';

  return strdup(extensao);
}

char *trocar_extensao(const char *caminho, const char *extensao) {
  const char *ponto;
  char *novo_caminho;
  size_t novo_tamanho, tamanho_ate_ponto, tamanho_extensao;

  // Encontra o último ponto
  ponto = strrchr(caminho, '.');

  // Encontra o tamanho até o ponto
  tamanho_ate_ponto = ponto ? (size_t)(ponto - caminho) : strlen(caminho);

  novo_tamanho = tamanho_ate_ponto + 1;
  tamanho_extensao = strlen(extensao);

  // Adiciona espaço para a nova extensão
  if (tamanho_extensao) {
    novo_tamanho += tamanho_extensao + 1;
  }

  // Copia caminho para novo_caminho até o ponto
  novo_caminho = (char *)malloc(novo_tamanho);
  strncpy(novo_caminho, caminho, tamanho_ate_ponto);
  novo_caminho[tamanho_ate_ponto] = '\0';

  // Adiciona a nova extensão
  if (tamanho_extensao) {
    strcat(novo_caminho, ".");
    strcat(novo_caminho, extensao);
  }

  return novo_caminho;
}
