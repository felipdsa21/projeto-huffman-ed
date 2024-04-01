#include <stdlib.h>

#include "descomprimir.h"
#include "estruturas.h"
#include "utils.h"

void descomprimir(FILE *arquivo_in, FILE *arquivo_out) {
  ArvoreBin *arvore;
  // uint16_t tamanho_arvore;
  uint8_t cabecalho[2], lixo;

  fread(&cabecalho, 1, sizeof(cabecalho), arquivo_in);
  lixo = cabecalho[0] >> 5;
  // tamanho_arvore = (uint16_t)((cabecalho[0] & 0x1F) << 8) | cabecalho[1];

  arvore = ler_arvore_preordem(arquivo_in);

  if (arvore_bin_e_folha(arvore)) {
    // Caso especial de existir apenas um tipo de caractere (repetido ou não) no arquivo
    descomprimir_com_caractere(arquivo_in, arquivo_out, arvore, lixo);
  } else {
    descomprimir_com_arvore(arquivo_in, arquivo_out, arvore, lixo);
  }

  arvore_bin_desalocar(arvore);
}

// Adaptado de https://stackoverflow.com/a/4909298
ArvoreBin *ler_arvore_preordem(FILE *arquivo) {
  ArvoreBin *esquerda, *direita;
  uint8_t caractere;
  bool escape;

  caractere = (uint8_t)fgetc(arquivo);
  escape = caractere == '\\'; // Testa se há um escape

  if (escape) {
    caractere = (uint8_t)fgetc(arquivo);
  }

  if (caractere == '*' && !escape) {
    esquerda = ler_arvore_preordem(arquivo);
    direita = ler_arvore_preordem(arquivo);
  } else {
    esquerda = direita = NULL;
  }

  return arvore_bin_criar(uint8_para_ptr(caractere), free, esquerda, direita);
}

long encontrar_tamanho_arquivo(FILE *arquivo) {
  long atual, ultima;

  atual = ftell(arquivo);          // Pega a posição atual
  fseek(arquivo, 0, SEEK_END);     // Vai para o final do arquivo
  ultima = ftell(arquivo);         // Pega a ultima posição/tamanho
  fseek(arquivo, atual, SEEK_SET); // Volta para onde estava

  return ultima;
}

void descomprimir_com_arvore(FILE *arquivo_in, FILE *arquivo_out, ArvoreBin *arvore, uint8_t lixo) {
  ArvoreBin *atual;
  long pos, tamanho;
  int caractere;
  unsigned i, qtd_bits;
  bool ultimo_byte;

  pos = ftell(arquivo_in);
  tamanho = encontrar_tamanho_arquivo(arquivo_in);

  // Começa a percorrer na raíz
  atual = arvore;

  while ((caractere = fgetc(arquivo_in)) != EOF) {
    ultimo_byte = pos == tamanho - 1;

    // Se for o último byte, ignora os bits de lixo
    qtd_bits = ultimo_byte ? 8 - lixo : 8;

    for (i = 0; i < qtd_bits; i++) {
      if (is_bit_i_set(caractere, 7 - i)) {
        atual = arvore_bin_direita(atual);
      } else {
        atual = arvore_bin_esquerda(atual);
      }

      if (arvore_bin_e_folha(atual)) {
        fputc(ptr_para_uint8(arvore_bin_item(atual)), arquivo_out);

        // Volta para a raíz da árvore
        atual = arvore;
      }
    }

    pos++;
  }
}

void descomprimir_com_caractere(FILE *arquivo_in, FILE *arquivo_out, ArvoreBin *arvore, uint8_t lixo) {
  long pos, tamanho, qtd_bits, i;
  uint8_t caractere;

  pos = ftell(arquivo_in);
  tamanho = encontrar_tamanho_arquivo(arquivo_in);

  // Calcula a quantidade de bits comprimidos. Explicação:
  // tamanho - pos -> A quantidade de bytes comprimidos, que é o tamanho de tudo depois da tabela (pos)
  // * 8 -> De bytes para bits
  // - lixo -> Remove o lixo
  qtd_bits = (tamanho - pos) * 8 - lixo;

  caractere = ptr_para_uint8(arvore_bin_item(arvore));

  for (i = 0; i < qtd_bits; i++) {
    // Já que sabemos que existe apenas um tipo de caractere no arquivo, não é necessário checar os bits da
    // entrada, simplesmente usamos a quantidade calculada anteriormente
    fputc(caractere, arquivo_out);
  }
}
