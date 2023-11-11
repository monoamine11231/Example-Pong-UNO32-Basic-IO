#include <pic32mx.h>
#include "drivers/io.h"


void source_io_shield_inputs(struct io_shield_input *st) {
    st->btn1    = (PORTF >> 1) & 0x01;
    st->btn2    = (PORTD >> 5) & 0x01;
    st->btn3    = (PORTD >> 6) & 0x01;
    st->btn4    = (PORTD >> 7) & 0x01;

    st->sw1     = (PORTD >> 8) & 0x01;
    st->sw2     = (PORTD >> 9) & 0x01;
    st->sw3     = (PORTD >> 10) & 0x01;
    st->sw4     = (PORTD >> 11) & 0x01;

    /* Start sampling, wait until conversion is done */
    AD1CON1 |= (0x1 << 1);
    while(!(AD1CON1 & (0x1 << 1)));
    while(!(AD1CON1 & 0x1));

    PORTE = ADC1BUF0 / 4;
}