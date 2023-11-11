#include "debug.h"

#include "drivers/display.h"


void print_error(void) {
    clear_video();
    put_text(0,0, "WTF? ERROR!");

    flush_video();
}