#include "engine.h"

#include <stddef.h>

#include "gui.h"
#include "drivers/io.h"
#include "sys_utils.h"


/* This function is runned when the "Ok" option is pressed */
static int ok_on_select(struct gui_instance *instance, struct gui_select *element,
                        struct io_shield_input *io) {
    _ENGINE_STATE = MENU_GLOBAL;
    return GUI_EXIT;
}

/* This function is runned when the first field is written to */
static int field_on_write(struct gui_instance *instance, struct gui_select *element,
                           struct io_shield_input *io) {
    /* Potentiometer gives a maximal value of 1024, divide it by the
     * largest possible number from the potentiometer and multiply it
     * with the number of possible characters that we have.
     */
    char c = (io->pot / 1024.0f)*27+'@';
    /* The first possible character is '@' which should be replaced with a
     * space aka underscore in this case.
     */
    c = (c == '@') ? '_' : c;
    element->text.text[instance->field_write_char_index] = c;
    return GUI_CONTINUE;
}


void render_menu_end() {
    struct gui_text title;
    init_text(&title, 18, 0, "YOUR NAME?");

    struct gui_select options[3];
    init_text(&options[0].text, 7, 16, "______");
    options[0].writable = 0x01;
    options[0].on_select = NULL;
    options[0].on_write = field_on_write;

    init_text(&options[1].text, 63, 16, "______");
    options[1].writable = 0x01;
    options[1].on_select = NULL;
    options[1].on_write = field_on_write;

    init_text(&options[2].text, 50, 24, "OK");
    options[2].writable = 0x00;
    options[2].on_select = ok_on_select;
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