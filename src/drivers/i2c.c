#include "drivers/i2c.h"

#include <pic32mx.h>
#include <stdint.h>


void i2c_wait_until_idle(void) {
    while (I2C1CON & 0x1F || I2C1STAT & (1 << 14)) {}
}

void i2c_start(void) {
    i2c_wait_until_idle();
    /* Set SEN to 1 */
    I2C1CONSET = 0x01;
    i2c_wait_until_idle();
}

void i2c_stop(void) {
    i2c_wait_until_idle();
    /* Set PEN to 1 */
    I2C1CONSET = (1 << 2);
    i2c_wait_until_idle();
}

void i2c_restart(void) {
    i2c_wait_until_idle();
    /* Set RSEN to 1 */
    I2C1CONSET = 0x02;
    i2c_wait_until_idle();
}

int i2c_send_u8(uint8_t value) {
    i2c_wait_until_idle();
    I2C1TRN = value;
    i2c_wait_until_idle();

    /* If received not acknowledge return 0, otherwise 1 */
    return !(I2C1STAT & (1 << 15));
}

uint8_t i2c_receive_u8(void) {
    i2c_wait_until_idle();
    /* Set RCEN to 1 */
    I2C1CONSET = (1 << 3);
    i2c_wait_until_idle();
    /* Set I2COV to 0 */
    I2C1STATCLR = (1 << 6);
    return I2C1RCV;
}

void i2c_send_acknowledge(void) {
    i2c_wait_until_idle();
    /* Set ACKDT to 0 */
    I2C1CONCLR = (1 << 5);
    /* Set ACKEN to 1 */
    I2C1CONSET = (1 << 4);
}

void i2c_send_not_acknowledge(void) {
    i2c_wait_until_idle();
    /* Set ACKDT to 1 */
    I2C1CONSET = (1 << 5);
    /* Set ACKEN to 1 */
    I2C1CONSET = (1 << 4);
}