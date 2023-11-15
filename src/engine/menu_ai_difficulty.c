#include "engine.h"

#include <stddef.h>

#include "gui.h"
#include "drivers/io.h"
#include "sys_utils.h"


/* This function is runned when the "Easy" option is pressed */
static int easy_on_select(struct gui_instance *instance, struct gui_select *element,
                          struct io_shield_input *io) {
    _ENGINE_STATE = GAME_START_AI_EASY;
    return GUI_EXIT;
}

/* This function is runned when the "Medium" option is pressed */
static int medium_on_select(struct gui_instance *instance, struct gui_select *element,
                            struct io_shield_input *io) {
    _ENGINE_STATE = GAME_START_AI_MEDIUM;
    return GUI_EXIT;
}

/* This function is runned when the "Hard" option is pressed */
static int hard_on_select(struct gui_instance *instance, struct gui_select *element,
                          struct io_shield_input *io) {
    _ENGINE_STATE = GAME_START_AI_HARD;
    return GUI_EXIT;
}


void render_menu_ai_difficulty() {
    struct gui_text title;
    init_text(&title, 12, 0, "AI DIFFICULTY?");

    struct gui_select options[3];
    init_text(&options[0].text, 7, 16, "Easy");
    options[0].writable = 0x00;
    options[0].on_select = easy_on_select;
    options[0].on_write = NULL;   /* `on_write` should not be runned but anyways.. */

    init_text(&options[1].text, 78, 16, "Medium");
    options[1].writable = 0x00;
    options[1].on_select = medium_on_select;
    options[1].on_write = NULL;

    init_text(&options[2].text, 44, 24, "Hard");
    options[2].writable = 0x00;
    options[2].on_select = hard_on_select;
    options[2].on_write = NULL;

    struct gui_instance instance;
    instance.texts = &title;
    instance.texts_size = 0x01;
    
    instance.selects = &options[0];
    instance.selects_size = 0x03;

    instance.select_index = 0x00;
    instance.field_index = -1;
    instance.field_write_char_index = -1;

    instance.mode = CHOOSE;

    gui_instance_loop(&instance);
}