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
        	video[16*TRIM(offy,8)+cur+k] = font[(*c)*8+k]; 
		}
    }
}

void put_digit(int offx, int offy, int x) {
    if (offy > 24 || offy < 0 || offx < 0 || offx > 120) {
        return;
    }

    if (!x) {
        int k;
        for (k = 0; k < 8; ++k) {
            video[16*TRIM(offy,8)+offx+k] = font['0'*8+k];
        }
    }

    int cur = offx;
    while (x > 0) {
        int k;
        for (k = 0; k < 8; ++k) {
            video[16*TRIM(offy,8)+cur+k] = font[((x%10)+'0')*8+k];
        }

        cur -= 8;
        x /= 10;
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

void put_box(int x, int y, int width, int height) {
    /* If there's a upper part of the board not inside a 8-bit high frame */
    if (y & 0x07) {
        int shiftl = y & 0x07;
        int rest = MIN(8-shiftl, height);
        /* Complex boolean function 
         * Takes the number of pixels, N, and creates a binary value
         * where there are N leading ones
        */
        int upper_board = ((1<<rest)-1) << shiftl;

        int i;
        for (i = 0; i < width; ++i) {
            video[16*TRIM(y, 8)+x+i] = upper_board;
        }
    }

    /* If the upper board was drawen move to the next 8-bit frame
     * otherwise draw the current one
    */
    int i = (y & 0x07) ? TRIM(y + 8, 8) : y;
    for (; i < TRIM(y+height, 8); i += 8) {
        int j;
        for (j = 0; j < width; ++j) {
            video[16*TRIM(i, 8)+x+j] = 0xFF;
        }
    }

    /* If board height < 8 and on start of 8-bit high frame
     * the board needs be drawen manually otherwise it will
     * be invisible.
    */
    if (!(y & 0x07) && height < 8) {
        int i;
        for (i = 0; i < width; ++i) {
            video[16*TRIM(y, 8)+x+i] = (1<<height)-1;
        }
    }

    /* If there's a lower patt of the board not inside the 8-bit high frame.
     * Also take in account so that the lower and upper part is not the same
     * 8-bit high frame
    */
    if (((y+height) & 0x07) && (TRIM(y+height, 8) != TRIM(y, 8))) {
        int rest = (y+height) & 0x07; 
        int lower_board = (1<<rest)-1;

        int i;
        for (i = 0; i < width; ++i) {
            video[16*TRIM(y+height, 8)+x+i] = lower_board;
        }
    }
}