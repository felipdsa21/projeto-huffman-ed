#include <CUnit/Basic.h>
#include <stdlib.h>
#include <string.h>

#include "comprimir.h"
#include "descomprimir.h"
#include "estruturas.h"
#include "extensoes.h"
#include "utils.h"

#define TAMANHO_ARRAY(x) (sizeof(x) / sizeof(*(x)))

typedef struct Caso {
  const char *nome;
  CU_TestFunc funcao;
} Caso;

typedef struct ArquivoTeste {
  const char *caminho_in, *caminho_out;
} ArquivoTeste;

const ArquivoTeste arquivos_teste[] = {
  {"data/a10.txt", "data/a10.huff"},
  {"data/abcdef.txt", "data/abcdef.huff"},
  {"data/flaflu.mp3", "data/flaflu.huff"},
  {"data/sky.jpg", "data/sky.huff"},
};

void testar_arvore_bin() {
  ArvoreBin *arvore, *esquerda, *direita;

  esquerda = arvore_bin_criar(uint8_para_ptr(0), free, NULL, NULL);
  direita = arvore_bin_criar(uint8_para_ptr(100), free, NULL, NULL);
  arvore = arvore_bin_criar(uint8_para_ptr(50), free, esquerda, direita);

  CU_ASSERT_FALSE(arvore_bin_e_folha(arvore));
  CU_ASSERT_EQUAL(ptr_para_uint8(arvore_bin_item(arvore)), 50);
  CU_ASSERT_PTR_EQUAL(arvore_bin_esquerda(arvore), esquerda);
  CU_ASSERT_PTR_EQUAL(arvore_bin_direita(arvore), direita);
  arvore_bin_desalocar(arvore);

  arvore = arvore_bin_criar(uint8_para_ptr(50), free, NULL, NULL);
  CU_ASSERT_TRUE(arvore_bin_e_folha(arvore));
  arvore_bin_desalocar(arvore);

  esquerda = arvore_bin_criar(uint8_para_ptr(0), free, NULL, NULL);
  arvore = arvore_bin_criar(uint8_para_ptr(50), free, esquerda, NULL);
  CU_ASSERT_FALSE(arvore_bin_e_folha(arvore));
  CU_ASSERT_PTR_EQUAL(arvore_bin_esquerda(arvore), esquerda);
  arvore_bin_desalocar(arvore);
}

void comparar_no_fila(NoFilaPrio *no, uint8_t item, long prioridade) {
  CU_ASSERT_PTR_NOT_NULL_FATAL(no);
  CU_ASSERT_EQUAL(ptr_para_uint8(no_fila_prio_item(no)), item);
  CU_ASSERT_EQUAL(no_fila_prio_prioridade(no), prioridade);
}

void testar_fila_prio() {
  FilaPrio *fila;
  NoFilaPrio *no;
  void *item;

  fila = fila_prio_criar();
  CU_ASSERT_PTR_NULL_FATAL(fila_prio_desenfileirar(fila));
  CU_ASSERT_TRUE(fila_prio_esta_vazia(fila));

  fila_prio_enfileirar(fila, uint8_para_ptr(6), free, 7);
  fila_prio_enfileirar(fila, uint8_para_ptr(9), free, 10);
  fila_prio_enfileirar(fila, uint8_para_ptr(3), free, 1);
  CU_ASSERT_FALSE(fila_prio_esta_vazia(fila));

  no = fila_prio_desenfileirar(fila);
  comparar_no_fila(no, 3, 1);
  item = no_fila_prio_tomar_item(no);
  CU_ASSERT_EQUAL(ptr_para_uint8(item), 3);
  CU_ASSERT_PTR_EQUAL_FATAL(no_fila_prio_item(no), NULL);
  no_fila_prio_desalocar(no);
  free(item);

  no = fila_prio_desenfileirar(fila);
  comparar_no_fila(no, 6, 7);
  no_fila_prio_desalocar(no);

  fila_prio_enfileirar(fila, uint8_para_ptr(100), free, 5);
  no = fila_prio_desenfileirar(fila);
  comparar_no_fila(no, 100, 5);
  no_fila_prio_desalocar(no);

  no = fila_prio_desenfileirar(fila);
  comparar_no_fila(no, 9, 10);
  no_fila_prio_desalocar(no);

  CU_ASSERT_PTR_NULL_FATAL(fila_prio_desenfileirar(fila));
  CU_ASSERT(fila_prio_esta_vazia(fila));

  fila_prio_desalocar(fila);
}

void testar_trocar_extensao(const char *caminho, const char *extensao, const char *caminho_esperado) {
  char *novo_caminho;

  novo_caminho = trocar_extensao(caminho, extensao);
  CU_ASSERT_STRING_EQUAL(novo_caminho, caminho_esperado);
  free(novo_caminho);
}

void testar_ler_extensao(const char *caminho, const char *extensao_esperada) {
  FILE *arquivo;
  char *extensao;

  arquivo = fopen(caminho, "rb");
  extensao = ler_extensao(arquivo);
  CU_ASSERT_STRING_EQUAL(extensao, extensao_esperada);

  free(extensao);
  fclose(arquivo);
}

void testar_extensoes() {
  size_t i;

  CU_ASSERT_STRING_EQUAL(encontrar_extensao("arquivo"), "");
  CU_ASSERT_STRING_EQUAL(encontrar_extensao("arquivo2."), "");
  CU_ASSERT_STRING_EQUAL(encontrar_extensao("arquivo_a.txt"), "txt");
  CU_ASSERT_STRING_EQUAL(encontrar_extensao("arquivo_b.huff"), "huff");

  testar_trocar_extensao("arquivo.txt", "huff", "arquivo.huff");
  testar_trocar_extensao("arquivo2", "txt", "arquivo2.txt");
  testar_trocar_extensao("arquivo_a.txt", "txt", "arquivo_a.txt");
  testar_trocar_extensao("arquivo_b.", "txt", "arquivo_b.txt");
  testar_trocar_extensao("arquivo_c.txt", "", "arquivo_c");
  testar_trocar_extensao("arquivo3.", "", "arquivo3");

  for (i = 0; i < TAMANHO_ARRAY(arquivos_teste); i++) {
    testar_ler_extensao(arquivos_teste[i].caminho_out, encontrar_extensao(arquivos_teste[i].caminho_in));
  }
}

void comparar_arquivos(FILE *arquivo1, FILE *arquivo2) {
  int caractere, caractere_esperado;

  fseek(arquivo1, 0, SEEK_SET);
  fseek(arquivo2, 0, SEEK_SET);

  while (true) {
    caractere = fgetc(arquivo1);
    caractere_esperado = fgetc(arquivo2);

    if (caractere != caractere_esperado) {
      // Impede o contador de asserts de aumentar desnecessariamente
      CU_ASSERT_EQUAL(caractere, caractere_esperado);
      break;
    }

    if (caractere == EOF || caractere_esperado == EOF) {
      break;
    }
  }
}

void testar_huffman_arquivo(
  const char *caminho_in, const char *caminho_out_esperado, bool deve_descomprimir
) {
  FILE *arquivo_in, *arquivo_out, *arquivo_out_esperado;

  arquivo_in = fopen(caminho_in, "rb");
  arquivo_out = tmpfile();
  arquivo_out_esperado = fopen(caminho_out_esperado, "rb");

  if (deve_descomprimir) {
    descomprimir(arquivo_in, arquivo_out);
  } else {
    comprimir(arquivo_in, arquivo_out, encontrar_extensao(caminho_in));
  }

  comparar_arquivos(arquivo_out, arquivo_out_esperado);

  fclose(arquivo_in);
  fclose(arquivo_out);
  fclose(arquivo_out_esperado);
}

void testar_comprimir() {
  size_t i;

  for (i = 0; i < TAMANHO_ARRAY(arquivos_teste); i++) {
    testar_huffman_arquivo(arquivos_teste[i].caminho_in, arquivos_teste[i].caminho_out, false);
  }
}

void testar_descomprimir() {
  size_t i;

  for (i = 0; i < TAMANHO_ARRAY(arquivos_teste); i++) {
    testar_huffman_arquivo(arquivos_teste[i].caminho_out, arquivos_teste[i].caminho_in, true);
  }
}

int main() {
  CU_pSuite suite;
  size_t i;

  if (CU_initialize_registry() != CUE_SUCCESS) {
    return (int)CU_get_error();
  }

  suite = CU_add_suite("Suite", NULL, NULL);
  if (!suite) {
    CU_cleanup_registry();
    return (int)CU_get_error();
  }

  Caso casos[] = {
    {"Árvore binária", testar_arvore_bin}, {"Fila de prioridade", testar_fila_prio},
    {"Extensões", testar_extensoes},       {"Comprimir", testar_comprimir},
    {"Descomprimir", testar_descomprimir},
  };

  for (i = 0; i < TAMANHO_ARRAY(casos); i++) {
    if (!CU_add_test(suite, casos[i].nome, casos[i].funcao)) {
      CU_cleanup_registry();
      return (int)CU_get_error();
    }
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return (int)CU_get_error();
}
