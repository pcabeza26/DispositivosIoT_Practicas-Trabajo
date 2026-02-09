#ifndef FORMATO_TRAMA_H
#define FORMATO_TRAMA_H

#include <stdint.h>

/* Valores fijos del protocolo */
#define TRAMA_BANDERA_INICIO   0x55
#define TRAMA_ID_TEMPERATURA  0x01

/* Unidades de temperatura */
#define UNIDAD_CELSIUS     0x01
#define UNIDAD_FAHRENHEIT  0x02

/* Estado de la alarma */
#define ALARMA_NO_ACTIVA   0x00
#define ALARMA_ACTIVA      0x01

/* Estructura de la trama UDP
 * Tamaño total: 8 bytes
 */
typedef struct __attribute__((packed))
{
    uint8_t  bandera_inicio;   /* 0x55 */
    uint8_t  id_trama;          /* 0x01 */
    uint8_t  id_nodo;           /* 0x01 - 0xFE */
    uint8_t  unidad;            /* 0x01: ºC | 0x02: ºF */
    int16_t  temperatura;       /* Punto fijo F12.4 */
    uint8_t  id_alarma;         /* Por ahora siempre 0x01 */
    uint8_t  estado_alarma;     /* 0x00 o 0x01 */
} trama_temperatura_t;

#endif /* FORMATO_TRAMA_H */
