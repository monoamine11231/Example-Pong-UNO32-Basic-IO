#ifndef INCLUDE_SYS_UTILS_H
#define INCLUDE_SYS_UTILS_H

#include <stdint.h>

/* Used for setups only, should not be used for real time delay */
void quicksleep(int cyc);
uint8_t spi_send_recv(uint8_t data);

#endif