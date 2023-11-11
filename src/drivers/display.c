#include <pic32mx.h>
#include "sys_utils.h"
#include "drivers/display.h"



void clear_video(void) {
	int i;
	for (i = 0; i < 512; ++i) {
		video[i] = 0x00;
	}
}

void flush_video(void) {
	int l;
	for (l = 0; l < 4; ++l) {
		/* Change display to command mode*/
		PORTFCLR = 0x10;

		/* Set page command */
		spi_send_recv(0x22);
		spi_send_recv(l);

		spi_send_recv(0x00);
		spi_send_recv(0x10);

		/* Change display to data mode*/
		PORTFSET = 0x10;

		int i;
		for (i = 0; i < 128; ++i) {
			spi_send_recv(video[l*128+i]);
		}
	}
}

void put_text(int offx, int offy, char *string) {
    if (offy > 24 || offy < 0 || offx < 0 || offx > 120) {
        return;
    }

    int cur;
    char *c = string;
    for (cur = offx; cur < 128; cur+=8, ++c) {
        if (*c == '\0') {
            break;
        }

		int rest = 128 - cur;
		rest = (rest < 8) ? rest : 8;

		int k;
		for (k = 0; k < rest; ++k) {
        	video[(offy/8)*128+cur+k] = font[(*c)*8+k]; 
		}
    }
}


void text_border(int offx, int offy, int dx) {
    if (offy > 24 || offy < 0 || offx < 0 || offx >= 128 || offx+dx >= 128) {
        return;
    }

	/* Draw the top border at the line before if it's not the first line */
	if (offy > 8) {
		int i;
		for (i = 0; i < dx; ++i) {
			video[128*((offy-8)/8)+offx+i] = 0x80;
		}
	}

	if (offy + 8 < 32) {
		int i;
		for (i = 0; i < dx; ++i) {
			video[128*((offy+8)/8)+offx+i] = 0x01;
		}
	}

	if (offx > 0) {
		video[128*(offy/8)+offx-1] = 0xFF;
	}

	if (offx+dx < 128) {
		video[128*(offy/8)+offx+dx] = 0xFF;
	}
}