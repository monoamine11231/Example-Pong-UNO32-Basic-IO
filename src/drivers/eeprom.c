#include "drivers/eeprom.h"
#include "drivers/i2c.h"

#include <stdint.h>


#define EEPROM_OPERATION_WRITE 0xA0
#define EEPROM_OPERATION_READ 0xA1 
#define EEPROM_MEMORY_ADD 0x0000

void eeprom_dump_struct(const void *src, int st_size, int index) {
    int acknowledge;
    do {
        i2c_start();
        acknowledge = i2c_send_u8(EEPROM_OPERATION_WRITE);
    } while(!acknowledge);
    i2c_send_u8(EEPROM_MEMORY_ADD >> 2);
    i2c_send_u8(EEPROM_MEMORY_ADD + index*st_size);
    
    int i;
    for (i = 0; i < st_size; ++i) {
        /* Type punning is the best feature, this is why C > C++ smh */
        i2c_send_u8( ((uint8_t*)src)[i] );
    }

    i2c_stop();
}

void eeprom_read_struct(void *dest, int st_size, int index) {
    i2c_start();
    i2c_send_u8(EEPROM_OPERATION_WRITE);
    i2c_send_u8(EEPROM_MEMORY_ADD >> 2);
    i2c_send_u8(EEPROM_MEMORY_ADD + index*st_size);
    i2c_restart();
    i2c_send_u8(EEPROM_OPERATION_READ);
    
    int i;
    for (i = 0; i < st_size; ++i) {
        ((uint8_t*)dest)[i] = i2c_receive_u8();
        i2c_send_acknowledge();
    }

    i2c_stop();
}