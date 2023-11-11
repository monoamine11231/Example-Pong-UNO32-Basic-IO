#include <pic32mx.h>
#include "sys_utils.h"

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

void update_textbuffer(int line, int offset, char *string) {
    if (line > 3 || line < 0 || offset < 0 || offset > 15) {
        return;
    }

    int cur;
    char *c = string;
    for (cur = offset; cur < 16; ++cur, ++c) {
        if (c == '\0') {
            textbuffer[line][cur] = ' ';
            break;
        }

        textbuffer[line][cur] = *c; 
    }
}

void flush_textbuffer(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
        /* Display change to command mode */
		PORTFCLR = 0x10; 
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
        /* Display change to data mode */
		PORTFSET = 0x10; 
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

void clear_textbuffer(void) {
    int l, c;
    for (l = 0; l < 4; ++l) {
        for (c = 0; c < 16; ++c) {
            textbuffer[l][c] = ' ';
        }
    }
}