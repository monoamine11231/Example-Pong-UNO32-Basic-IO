#include "engine.h"

#include <pic32mx.h>
#include <stddef.h>

#include "drivers/eeprom.h"
#include "drivers/display.h"
#include "gui.h"


static void highscore_on_exit(struct gui_instance *instance) {
    instance->mode = CHOOSE;
    _ENGINE_STATE = MENU_GLOBAL;

    instance->page_offset = 0x00;
}

void render_menu_highscore() {
    struct highscore_entry id, entries[10];
    /* Get the identifying entry */
    eeprom_read_struct(&id, sizeof(struct highscore_entry), 0);

    struct gui_text tmp[21];
    init_text(&tmp[0], 24, 0, "LEADERBOARD");

    int i;
    /* Get the top ten entries */
    for (i = 0; i < 10; ++i) {
        /* The first entry must have a score of INT_MAX which identifies
         * that the EEPROM area was zeroed for 10 highscore entries
         */
        eeprom_read_struct(&entries[i], sizeof(struct highscore_entry), i+1);
        init_text(&tmp[i*2+0+1], 0, (i+1)*8, entries[i].name);
        char atoi[5] = {' ', ' ', ' ', ' ', '\0'};
        int j = 3;
        for (;entries[i].score > 0 && j >= 0; --j) {
            atoi[j] = (entries[i].score % 10) + '0';
            entries[i].score /= 10;
        }

        init_text(&tmp[i*2+1+1], 70, (i+1)*8, &atoi[0]);
    }

    struct gui_instance instance;
    instance.texts = &tmp[0];
    instance.texts_size = (id.score == INT32_MAX) ? 21 : 1;

    instance.selects = NULL;
    instance.selects_size = 0x00;

    instance.select_index = 0x00;
    instance.field_index = -1;
    instance.field_write_char_index = -1;

    instance.page_offset = 0x00;
    instance.page_max_offset = 8+8*10;

    instance.on_exit = highscore_on_exit;

    instance.mode = PAGE_SCROLL;

    gui_instance_loop(&instance);
    
}