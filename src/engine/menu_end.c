#include "gui.h"
#include "engine.h"
#include "drivers/display.h"
#include "drivers/io.h"
#include "sys_utils.h"


struct writing {
    int lr; /* Left or right, 0 -> left, 1 -> right */
    int index;
};

void render_menu_end(enum engine_state *state) {
    char fields[2][7] = {
        {'_', '_', '_', '_', '_', '_', '\0'},
        {'_', '_', '_', '_', '_', '_', '\0'}
    };

    struct io_shield_input st;
    struct writing wr;
    wr.lr = -1;

    /* If selected right */
    int selected = 0x00;

    for (;;) {
        clear_video();
        source_io_shield_inputs(&st);

        put_text(18, 0, "YOUR NAME?");
        put_text(50, 24, "OK");
        
        put_text(7, 16, &fields[0][0]);
        put_text(63, 16, &fields[1][0]);

        /* If in name field selecting mode */
        if (wr.lr == -1) {
            selected += st.btn2;
            selected -= st.btn3;

            selected = MOD(selected, 3); 

            switch (selected)
            {
            case 0x00:
                text_border(7, 16, 48);
                break;
            case 0x01:
                text_border(63, 16, 48);
                break;
            case 0x02:
                text_border(50, 24, 16);
                break;
            default:
                break;
            }

            if (st.btn1) {
                if (selected == 0x02) {
                    /* Write to EEPROM highscore */
                    *state = MENU_GLOBAL;
                    break;
                }
                wr.lr = selected;
                wr.index = 0x00;
            }
        /* If in name writing mode */
        } else {
            /* Potentiometer gives a maximal value of 1024, divide it by the
             * largest possible number from the potentiometer and multiply it
             * with the number of possible characters that we have.
            */
            fields[wr.lr][wr.index] = (st.pot / 1024.0f)*27+'@';
            /* The first possible character is '@' which should be replaced with a
             * space aka underscore in this case.
            */
            if (fields[wr.lr][wr.index] == '@') {
                fields[wr.lr][wr.index] = '_';
            }

            /* Which character to denote that it is being written to currently */
            put_box(((wr.lr == 0) ? 7 : 63)+8*wr.index, 24, 8, 1);

            wr.index += st.btn2;
            wr.index -= st.btn3;
            wr.index = MOD(wr.index, 6);

            /* Leave name writting mode if BTN1 is pressed */
            if (st.btn1) {
                wr.lr = -1;
                wr.index = 0;
            }
        }

        flush_video();

        /* Artificial delay so that inputs are not acted on immediately*/
        quicksleep(INTERUPT_QUICKDELAY_VALUE);
    }   
}