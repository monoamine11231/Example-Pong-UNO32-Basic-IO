#include "engine.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "gui.h"
#include "drivers/io.h"
#include "drivers/eeprom.h"
#include "sys_utils.h"


static int highscore_entry_cmp(const void *a, const void *b) {
    return ((struct highscore_entry*)b)->score - ((struct highscore_entry*)a)->score;
}

/* This function is runned when the "Ok" option is pressed */
static int ok_on_select(struct gui_instance *instance, struct gui_select *element,
                        struct io_shield_input *io) {
    /* Exit to the main menu */
    _ENGINE_STATE = MENU_GLOBAL;
    if (_GAME_SCORE <= 0) {
        _GAME_SCORE = 0x00;
        return GUI_EXIT;
    }

    /* Fetch all the highscore entries stored in EEPROM */
    struct highscore_entry entries[11];
    int i;
    for (i = 0; i < 10; ++i) {
        /* The first entry must have a name of INT_MAX which identifies
         * that the EEPROM area was zeroed for 10 highscore entries
         */
        eeprom_read_struct(&entries[i], sizeof(struct highscore_entry), i+1);
    }

    /* Add the current end game entry so that it can be sorted and do not
     * forget about the NULL byte.
     */
    memcpy(&entries[10].name[0], &instance->selects[0].text.text[0], 7);
    entries[10].score = _GAME_SCORE;

    /* Sort the 11 entries including the new one and store them in EEPROM */
    qsort(&entries[0], 11, sizeof(struct highscore_entry), highscore_entry_cmp);
    for (i = 0; i < 10; ++i) {
        eeprom_dump_struct(&entries[i], sizeof(struct highscore_entry), i+1);
    }

    _GAME_SCORE = 0x00;
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


void render_menu_end_ai() {
    struct gui_text title;
    init_text(&title, 18, 0, "YOUR NAME?");

    struct gui_select options[2];
    init_text(&options[0].text, 7, 16, "______");
    options[0].writable = 0x01;
    options[0].on_select = NULL;
    options[0].on_write = field_on_write;

    init_text(&options[1].text, 62, 16, "OK");
    options[1].writable = 0x00;
    options[1].on_select = ok_on_select;
    options[1].on_write = NULL;


    struct gui_instance instance;
    instance.texts = &title;
    instance.texts_size = 0x01;
    
    instance.selects = &options[0];
    instance.selects_size = 0x02;

    instance.select_index = 0x00;
    instance.field_index = -1;
    instance.field_write_char_index = -1;

    instance.on_exit = NULL;

    instance.mode = CHOOSE;

    gui_instance_loop(&instance);
}