#include <locale.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "comprimir.h"
#include "descomprimir.h"
#include "extensoes.h"

const char MSG_USO[] = "Uso: huffman.exe [arquivo]\nCaso a extensão seja .huff, descomprime\n";

void configurar_terminal() {
#ifdef _WIN32
  // Evita problemas com texto no terminal no Windows
  // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setlocale-wsetlocale?view=msvc-170#utf-8-support
  setlocale(LC_ALL, ".UTF8");
#endif /* WIN32 */
}

int main(int argc, char *argv[]) {
  FILE *arquivo_in, *arquivo_out;
  int ret;
  const char *extensao_in;
  char *caminho_in, *caminho_out, *extensao_out;
  bool deve_descomprimir;

  // Inicializa as variáveis que precisam ser limpadas/desalocadas
  ret = EXIT_FAILURE;
  extensao_out = NULL;
  caminho_out = NULL;
  arquivo_in = NULL;
  arquivo_out = NULL;

  configurar_terminal();

  // Posibilidades válidas:
  // argv = {"./huffman", "dados.txt"}
  // argv = {"./huffman", "dados.huff"}

  if (argc != 2) {
    // Número incorreto de argumentos
    printf(MSG_USO);
    goto retorno;
  }

  caminho_in = argv[1];
  extensao_in = encontrar_extensao(caminho_in);
  deve_descomprimir = !strcmp(extensao_in, "huff");

  if (!deve_descomprimir) {
    if (strlen(extensao_in) > 6) {
      fprintf(stderr, "A extensão do arquivo é muito longa\n");
      goto retorno;
    }

    // Usa strdup para simplificar a desalocação do outro caso
    extensao_out = strdup("huff");
  }

  arquivo_in = fopen(caminho_in, "rb");
  if (!arquivo_in) {
    perror("Não foi possível abrir o arquivo de entrada");
    goto retorno;
  }

  if (deve_descomprimir) {
    // Lê a extensão e volta para o começo do arquivo
    extensao_out = ler_extensao(arquivo_in);
    fseek(arquivo_in, 0, SEEK_SET);
  }

  caminho_out = trocar_extensao(caminho_in, extensao_out);

  arquivo_out = fopen(caminho_out, "wb");
  if (!arquivo_out) {
    perror("Não foi possível abrir o arquivo de saída");
    goto retorno;
  }

  if (deve_descomprimir) {
    printf("Descomprimindo\n");
    descomprimir(arquivo_in, arquivo_out);
  } else {
    printf("Comprimindo\n");
    comprimir(arquivo_in, arquivo_out, extensao_in);
  }

  ret = EXIT_SUCCESS;

retorno:
  if (extensao_out) {
    free(extensao_out);
  }

  if (caminho_out) {
    free(caminho_out);
  }

  if (arquivo_in) {
    fclose(arquivo_in);
  }

  if (arquivo_out) {
    fclose(arquivo_out);
  }

  return ret;
}
