#include "drivers/eeprom.h"
#include "drivers/i2c.h"

#include <pic32mx.h>
#include <stdint.h>


void eeprom_reset() {
    i2c_start();
    i2c_send_not_acknowledge();
    i2c_stop();
}

void eeprom_dump_struct(const void *src, int st_size, int index) {
    int acknowledge;
    do {
        i2c_start();
        acknowledge = i2c_send_u8(EEPROM_OPERATION_WRITE);
    } while (!acknowledge);
    
    i2c_send_u8(0x00);
    i2c_send_u8(0x00 + index*st_size);
    
    int i;
    for (i = 0; i < st_size; ++i) {
        /* Type punning is the best feature, this is why C > C++ smh */
        i2c_send_u8( ((uint8_t*)src)[i] );
    }

    i2c_stop();
    /* 5ms delay after EEPROM write cycle is needed as defined in specifications */
    quicksleep(100000);
}

void eeprom_read_struct(void *dest, int st_size, int index) {
    if (st_size <= 0) {
        return;
    }

    i2c_start();
    i2c_send_u8(EEPROM_OPERATION_WRITE);
    i2c_send_u8(0x00);
    i2c_send_u8(0x00 + index*st_size);
    i2c_restart();
    i2c_send_u8(EEPROM_OPERATION_READ);
    int i;
    for (i = 0; i < st_size-1; ++i) {
        ((uint8_t*)dest)[i] = i2c_receive_u8();
        i2c_send_acknowledge();
    }

    /* On the last received byte it is important to NOT sent
     * the acknowledge bit, but rather the not acknowledge bit,
     * otherwise the EEPROM will bug!!!!!!!!!!.
     */
    ((uint8_t*)dest)[st_size-1] = i2c_receive_u8();
    i2c_send_not_acknowledge();
    i2c_stop();
}