#include <pic32mx.h>
#include "gui.h"
#include "engine.h"
#include "drivers/display.h"
#include "drivers/eeprom.h"
#include "sys_utils.h"


#define EXIT_LOOP for(;;) {}

int main() {
    enum engine_state state = START_MENU;
    for (;;) {
        switch (state)
        {
        case START_MENU:
            render_game_menu(&state);
            break;
        case START_PLAYERS:
            render_game_players(&state);
            break;
        case START_AI_DIFFICULTY:
            render_game_ai_difficulty(&state);
            break;
        default:
            print_error();
            break;
        }
        /* Artificial delay so that inputs are not acted on immediately*/
        quicksleep(INTERUPT_QUICKDELAY_VALUE);
    }

    return 0;
}