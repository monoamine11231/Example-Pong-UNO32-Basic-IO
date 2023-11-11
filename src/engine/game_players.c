#include "gui.h"
#include "engine.h"
#include "drivers/display.h"
#include "drivers/io.h"
#include "sys_utils.h"


void render_game_players(enum engine_state *state) {
    struct io_shield_input st;
    /* If selected right */
    int selected = 0x00;

    for (;;) {
        clear_video();
        source_io_shield_inputs(&st);

        if (st.btn1) {
            if (selected) {
                *state = START_AI_DIFFICULTY;
                break;
            }
            *state = GAME_START_TWO_PLAYERS;
            break;
        }

        selected += st.btn2;
        selected &= 0x01;

        selected += st.btn3;
        selected &= 0x01;

        put_text(18, 0, "AGAINST WHO?");

        put_text(7, 16, "FRIEND");
        put_text(63, 16, "AI");

        if (selected) {
            text_border(63, 16, 16);
        } else {
            text_border(7, 16, 48);
        }

        flush_video();

        /* Artificial delay so that inputs are not acted on immediately*/
        quicksleep(INTERUPT_QUICKDELAY_VALUE);
    }   
}