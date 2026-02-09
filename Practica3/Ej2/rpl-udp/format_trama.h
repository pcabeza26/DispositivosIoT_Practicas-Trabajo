#include <stdint.h>

// --- Constantes de la Trama ---
#define START_FLAG_VAL   0x55       // Bandera de inicio 
#define NODE_ID          0x02       // Identificador nodo (Borrar?)
#define FRAME_ID_VAL     0x01       // Identificador de trama 
#define UNIT_C_VAL       0x01       // Unidad: Grados centígrados 
#define UNIT_F_VAL       0x02       // Unidad: Grados Fahrenheit 
#define ALARM_INACTIVE   0x00       // Alarma no activa 
#define ALARM_ACTIVE     0x01       // Alarma activa 
#define RESERVED_ID_1    0x00       // ID de nodo reservado 
#define RESERVED_ID_2    0xFF       // ID de nodo reservado 

// --- Estructura del Valor --- (2 BYTES)
typedef struct {
    uint8_t msb; // Byte más significativo
    uint8_t lsb; // Byte menos significativo
} temp_f12_4_t;

// Estructura de la Trama de Monitorización
typedef struct {
    uint8_t start_flag;    
    uint8_t frame_id;     
    uint8_t node_id;
    uint8_t unit;
    temp_f12_4_t temp_val;   // Valor temperatura 
    uint8_t alarm_type;    
    uint8_t alarm_status;  
} monitor_frame_t;

// Funciones de (des)serialización

// Serializa la estructura en un array de bytes
uint16_t serialize_frame(const monitor_frame_t *frame, uint8_t *buffer);

// Deserializa el array de bytes en la estructura
void deserialize_frame(monitor_frame_t *frame, const uint8_t *buffer);
