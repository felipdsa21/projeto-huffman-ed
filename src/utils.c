#include <stdlib.h>

#include "utils.h"

void *uint8_para_ptr(uint8_t valor) {
  uint8_t *valor_p = (uint8_t *)malloc(sizeof(*valor_p));
  *valor_p = valor;
  return valor_p;
}

uint8_t ptr_para_uint8(void *ptr) {
  return *((uint8_t *)ptr);
}
