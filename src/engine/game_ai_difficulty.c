#include "gui.h"
#include "engine.h"
#include "drivers/display.h"
#include "drivers/io.h"
#include "sys_utils.h"
#include "debug.h"


void render_game_ai_difficulty(enum engine_state *state) {
    struct io_shield_input st;
    /* If selected right */
    int selected = 0x00;

    for (;;) {
        clear_video();
        source_io_shield_inputs(&st);

        if (st.btn1) {
            switch (selected) {
                case 0:
                    *state = GAME_START_AI_EASY;
                    return;
                case 1:
                    *state = GAME_START_AI_MEDIUM;
                    return;
                case 2:
                    *state = GAME_START_AI_HARD;
                    return;
                default:
                    break;
            }
        }

        selected += st.btn2;
        selected %= 3;

        selected += st.btn3;
        selected %= 3;

        put_text(12, 0, "AI DIFFICULTY?");

        put_text(7, 16, "EASY");
        put_text(78, 16, "MEDIUM");
        put_text(44, 24, "HARD");

        switch (selected)
        {
        case 0:
            text_border(7, 16, 32);
            break;
        case 1:
            text_border(78, 16, 48);
            break;
        case 2:
            text_border(44, 24, 32);
            break;
        default:
            break;
        }

        flush_video();

        /* Artificial delay so that inputs are not acted on immediately*/
        quicksleep(INTERUPT_QUICKDELAY_VALUE);
    }   
}