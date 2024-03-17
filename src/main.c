#include <locale.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "comprimir.h"
#include "descomprimir.h"

const char MSG_USO[] = "Uso: huffman.exe [comprimir|descomprimir] [entrada] [saída]\n";

int main(int argc, char *argv[]) {
  FILE *arquivo_in, *arquivo_out;
  char *acao, *caminho_in, *caminho_out;
  bool deve_descomprimir;

#ifdef _WIN32
  // Evita problemas com texto no terminal no Windows
  // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setlocale-wsetlocale?view=msvc-170#utf-8-support
  setlocale(LC_ALL, ".UTF8");
#endif /* WIN32 */

  // Posibilidades válidas:
  // argv = {"./huffman", "comprimir", "dados.txt", "dados.huff"}
  // argv = {"./huffman", "descomprimir", "dados.huff", "dados.txt"}

  if (argc != 4) {
    // Número incorreto de argumentos
    printf(MSG_USO);
    return EXIT_FAILURE;
  }

  acao = argv[1];
  caminho_in = argv[2];
  caminho_out = argv[3];

  if (!strcmp(acao, "comprimir")) {
    deve_descomprimir = false;
  } else if (!strcmp(acao, "descomprimir")) {
    deve_descomprimir = true;
  } else {
    // Ação desconhecida
    printf(MSG_USO);
    return EXIT_FAILURE;
  }

  arquivo_in = fopen(caminho_in, "rb");
  if (!arquivo_in) {
    perror("Não foi possível abrir o arquivo de entrada");
    return EXIT_FAILURE;
  }

  arquivo_out = fopen(caminho_out, "wb");
  if (!arquivo_out) {
    perror("Não foi possível abrir o arquivo de saída");
    return EXIT_FAILURE;
  }

  if (deve_descomprimir) {
    descomprimir(arquivo_in, arquivo_out);
  } else {
    comprimir(arquivo_in, arquivo_out);
  }

  fclose(arquivo_in);
  fclose(arquivo_out);
  return EXIT_SUCCESS;
}
