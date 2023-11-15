#include "engine.h"

#include <stddef.h>     /* NULL */

#include "gui.h"
#include "drivers/io.h"
#include "sys_utils.h"


/* This function is runned when the "Play" option is pressed */
static int play_on_select(struct gui_instance *instance, struct gui_select *element,
                          struct io_shield_input *io) {
    _ENGINE_STATE = MENU_PLAYERS;
    return GUI_EXIT;
} 

/* This function is runned when the "Highscore" option is pressed */
static int highscore_on_select(struct gui_instance *instance, struct gui_select *element,
                               struct io_shield_input *io) {
    _ENGINE_STATE = MENU_HIGHSCORE;
    return GUI_EXIT;
}


void render_menu_global() {
    struct gui_text title;
    init_text(&title, 18, 0, "PONG UNO32");

    struct gui_select options[2];
    init_text(&options[0].text, 7, 16, "Play");
    options[0].writable = 0x00;
    options[0].on_select = play_on_select;
    options[0].on_write = NULL;   /* `on_write` should not be runned but anyways.. */

    init_text(&options[1].text, 46, 16, "Highscore");
    options[1].writable = 0x00;
    options[1].on_select = highscore_on_select;
    options[1].on_write = NULL;


    struct gui_instance instance;
    instance.texts = &title;
    instance.texts_size = 0x01;
    
    instance.selects = &options[0];
    instance.selects_size = 0x02;

    instance.select_index = 0x00;
    instance.field_index = -1;
    instance.field_write_char_index = -1;

    instance.mode = CHOOSE;

    gui_instance_loop(&instance);
}