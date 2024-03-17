#pragma once
#include <stdint.h>

/**
 * Testa se o bit em um índice é 1.
 *
 * Por que um macro e não uma função? Para permitir qualquer tipo inteiro (uint8_t, unsigned etc)
 * @param c um inteiro
 * @param i o indíce do bit a partir do final, último bit -> i == 0
 * @return se o bit é 1
 */
#define is_bit_i_set(c, i) ((1 << (i)) & (c))

/**
 * Define o bit em um índice como 1.
 *
 * Por que um macro e não uma função? Para permitir qualquer tipo inteiro (uint8_t, unsigned etc)
 * @param c um inteiro
 * @param i o indíce do bit a partir do final, último bit -> i == 0
 * @return o inteiro com o bit mudado
 */
#define set_bit(c, i) ((1 << (i)) | (c))

/**
 * Guarda um uint8_t em um ponteiro para void.
 *
 * Para uso com estruturas que aceitam void*.
 * Use {@link ptr_para_uint8} no ponteiro para obter o valor.
 * @param valor um valor uint8_t
 * @return um ponteiro
 */
void *uint8_para_ptr(uint8_t valor);

/**
 * Pega o uint8_t guardado em um ponteiro para void.
 * @param ptr um ponteiro
 * @return o valor uint8_t
 */
uint8_t ptr_para_uint8(void *ptr);
