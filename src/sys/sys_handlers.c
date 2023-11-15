#include <stdint.h>
#include <pic32mx.h>
#include "sys_utils.h"


int errno;

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
	
	spi_send_recv(0x20);
	spi_send_recv(0x0);

	spi_send_recv(0xAF);
	quicksleep(1000);

	/* CLEAR TEXT BUFFER!! */
	clear_video();
	flush_video();

	/* INITIALIZE ANALOG -> DIGITAL POTENTIOMETER */
	TRISFSET = 0x02;
	TRISDSET = (0b1111111 << 5);

	/* PORTB.2 is analog pin with potentiometer*/
	AD1PCFG = ~(1 << 2);
	TRISBSET = (1 << 2);
	/* Use pin 2 for positive */
	AD1CHS = (0x2 << 16);
	
	/* Data format in uint32, 0 - 1024
	Manual sampling, auto conversion when sampling is done
	FORM = 0x4; SSRC = 0x7; CLRASAM = 0x0; ASAM = 0x0; */
	AD1CON1 = (0x4 << 8) | (0x7 << 5);
	
	AD1CON2 = 0x0;
	AD1CON3 |= (0x1 << 15);
	
	/* Set up output pins */
	ODCE = 0x0;
	TRISECLR = 0xFF;
	
	/* Turn on ADC */
	AD1CON1 |= (0x1 << 15);

	/* Reset I2C */
	I2C1CON = 0x00;
	/* Set I2C BRG for approximately 100kHz I2C bus (SCL), on 80MHz peripheral clock speed */
	I2C1BRG = 0x186;
	I2C1STAT = 0x00;
	/* Stop I2C when idle*/
	I2C1CONSET = (1 << 13);
	/* Enable I2C */
	I2C1CONSET = (1 << 15);
	/* Flush I2C receive buffer */
	int tmp = I2C1RCV;
}