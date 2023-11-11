#include <stdint.h>
#include <pic32mx.h>
#include "sys_utils.h"


#define EXIT_LOOP for(;;) {}

/* Non maskable handler */
void _nmi_handler() {
    EXIT_LOOP;
}

/* Is called before .data and .bss is set up */
void _on_reset() {
    /* Nothing */
}

/* Called before main, a nice setup can be set up */
void _on_bootstrap() {
        /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

    /* INITIALIZE DISPLAY!!!!! */
    
    /* Display change to command mode */
    PORTFCLR = 0x10;

	quicksleep(10);
    /* Display active VDD */
    PORTFCLR = 0x40;

	quicksleep(1000000);
	
	spi_send_recv(0xAE);

    /* Display activate reset*/
    PORTGCLR = 0x200;

	quicksleep(10);
	
	/* Display do not reset */
    PORTGSET = 0x200;

    quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
    /* Display activate VBAT */
    PORTFCLR = 0x20;

	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);

    /* CLEAR TEXT BUFFER!! */
    int l, c;
    for (l = 0; l < 4; ++l) {
        for (c = 0; c < 16; ++c) {
            textbuffer[l][c] = ' ';
        }
    }
    flush_textbuffer();
}