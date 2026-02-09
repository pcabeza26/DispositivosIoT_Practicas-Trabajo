#include "f12_4_utils.h"
#include <math.h> 
float convert_f12_4_to_float(uint8_t msb, uint8_t lsb) {
   
    // El orden de bits: | 12 bits ENTERO | 4 bits FRACCIÓN |
    uint16_t fixed_val = ((uint16_t)msb << 8) | lsb;

    // El valor real es el valor fijo dividido por 2^4 = 16.0
    float temp_float = (float)fixed_val / 16.0f;

    return temp_float;
}


// --- 2. Función para el Cliente: De Float a F12,4 ---
// El cliente toma la lectura (float) y la empaqueta en 2 bytes (F12,4).
void convert_float_to_f12_4(float temp_float, uint8_t *msb, uint8_t *lsb) {

    uint16_t fixed_val = (uint16_t)(temp_float * 16.0f);
    
    // Separar en MSB y LSB 
    *msb = (uint8_t)(fixed_val >> 8); // Byte Alto
    *lsb = (uint8_t)(fixed_val & 0xFF);  // Byte Bajo
}