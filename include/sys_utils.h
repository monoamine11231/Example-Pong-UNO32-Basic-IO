#ifndef INCLUDE_SYS_UTILS_H
#define INCLUDE_SYS_UTILS_H

#include <stdint.h>

/* Used for setups only, should not be used for real time delay */
void quicksleep(int cyc);
uint8_t spi_send_recv(uint8_t data);
/* Self explanatory */

void update_textbuffer(int line, int offset, char *string);
void flush_textbuffer(void);
void clear_textbuffer(void);


char textbuffer[4][16];

extern const uint8_t const font[128*8];

#endif