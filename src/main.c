#include <pic32mx.h>
#include "gui.h"
#include "engine.h"
#include "drivers/display.h"
#include "drivers/eeprom.h"
#include "sys_utils.h"


#define EXIT_LOOP for(;;) {}

int main() {
    enum engine_state state = MENU_GLOBAL;
    for (;;) {
        switch (state)
        {
        case MENU_GLOBAL:
            render_menu_global(&state);
            break;
        case MENU_PLAYERS:
            render_menu_players(&state);
            break;
        case MENU_AI_DIFFICULTY:
            render_menu_ai_difficulty(&state);
            break;
        case GAME_START_TWO_PLAYERS:
        case GAME_START_AI_EASY:
        case GAME_START_AI_MEDIUM:
        case GAME_START_AI_HARD:
            render_pong_game(&state);
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