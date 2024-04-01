#include <stdio.h>

/**
 * Encontra a extensão de um caminho de arquivo
 * @param caminho o caminho do arquivo
 * @return a extensão, ou se não houver uma, uma string vazia
 */
const char *encontrar_extensao(const char *caminho);

/**
 * Lê a extensão salva no cabeçalho de um arquivo comprimido
 * @param arquivo o arquivo comprimido
 * @return a extensão
 */
char *ler_extensao(FILE *arquivo);

/**
 * Troca a extensão de um caminho de arquivo
 * @param caminho o caminho
 * @param extensao a nova extensão
 * @return o caminho com a nova extensão
 */
char *trocar_extensao(const char *caminho, const char *extensao);
