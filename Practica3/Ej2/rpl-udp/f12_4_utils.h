#ifndef F12_4_UTILS_H
#define F12_4_UTILS_H

#include <stdint.h>
#include "format_trama.h" 

// Funciones a implementar
float convert_f12_4_to_float(uint8_t msb, uint8_t lsb);

void convert_float_to_f12_4(float temp_float, uint8_t *msb, uint8_t *lsb);

#endif /* F12_4_UTILS_H */