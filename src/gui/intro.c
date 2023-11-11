#include "gui.h"

#include <pic32mx.h>
#include "drivers/display.h"
#include "drivers/io.h"


void render_intro(void) {
    int x = 16;
    struct io_shield_input st;
    for (;;) {
        clear_video();
        source_io_shield_inputs(&st);
        x += st.sw3;
        x -= st.sw4;


        put_text(x, 0, "PONG UNO32");

        put_text(7, 16, "Play");
        put_text(46, 16, "Highscore");

        text_border(7, 16, 32);
        text_border(46, 16, 72);

        flush_video();
    }

}