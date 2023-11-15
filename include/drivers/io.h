#ifndef INCLUDE_DRIVERS_IO_H
#define INCLUDE_DRIVERS_IO_H

struct io_shield_input {
    int btn1;
    int btn2;
    int btn3;
    int btn4;

    int sw1;
    int sw2;
    int sw3;
    int sw4;

    int pot;
};

void source_io_shield_inputs(struct io_shield_input *st);

#endif