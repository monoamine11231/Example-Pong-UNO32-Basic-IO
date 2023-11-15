#ifndef INCLUDE_DRIVERS_I2C_H
#define INCLUDE_DRIVERS_I2C_H

#include <stdint.h>


void i2c_wait_until_idle(void);

void i2c_start(void);
void i2c_stop(void);
void i2c_restart(void);

/* Returns acknowledge/~acknowledge */
int i2c_send_u8(uint8_t value);
uint8_t i2c_receive_u8(void);
void i2c_send_acknowledge(void);
void i2c_send_not_acknowledge(void);

#endif