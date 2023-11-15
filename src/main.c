#include <pic32mx.h>
#include "gui.h"
#include "engine.h"
#include "drivers/display.h"
#include "drivers/eeprom.h"
#include "sys_utils.h"



int main() {
    _ENGINE_STATE = MENU_GLOBAL;
    _GAME_SCORE = 0x00;

    for (;;) {
        switch (_ENGINE_STATE)
        {
        case MENU_GLOBAL:
            render_menu_global();
            break;
        case MENU_PLAYERS:
            render_menu_players();
            break;
        case MENU_AI_DIFFICULTY:
            render_menu_ai_difficulty();
            break;
        case GAME_START_TWO_PLAYERS:
        case GAME_START_AI_EASY:
        case GAME_START_AI_MEDIUM:
        case GAME_START_AI_HARD:
            render_pong_game();
            break;
        case GAME_END_TWO_PLAYERS:
        case GAME_END_AI:
            render_menu_end();
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