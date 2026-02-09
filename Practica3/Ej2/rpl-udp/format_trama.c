#include "format_trama.h"
#include <string.h> 


#define FRAME_SIZE 8 //8 Bytes

//  Serialización (Cliente)
uint16_t serialize_frame(const monitor_frame_t *frame, uint8_t *buffer) {
    if (frame == NULL || buffer == NULL) {
        return 0; 
    }

    // Indice Buffer
    int index = 0;
    
    // Trama en orden: 
    buffer[index++] = frame->start_flag;
    buffer[index++] = frame->frame_id;
    buffer[index++] = frame->node_id;
    buffer[index++] = frame->unit;
    // Valor temperatura
    buffer[index++] = frame->temp_val.msb; 
    buffer[index++] = frame->temp_val.lsb; 
    
    buffer[index++] = frame->alarm_type;
    buffer[index++] = frame->alarm_status;

    
    return index; 
}

//  Deserialización (Servidor)
void deserialize_frame(monitor_frame_t *frame, const uint8_t *buffer) {
    if (frame == NULL || buffer == NULL) {
        return;
    }
    
    // Indice buffer
    int index = 0;

    //Trama en orden
    frame->start_flag = buffer[index++];
    frame->frame_id  = buffer[index++];
    frame->node_id  = buffer[index++];
    frame->unit = buffer[index++];
    // Valor temp
    frame->temp_val.msb = buffer[index++]; 
    frame->temp_val.lsb = buffer[index++];

    frame->alarm_type = buffer[index++];
    frame->alarm_status = buffer[index++];
}