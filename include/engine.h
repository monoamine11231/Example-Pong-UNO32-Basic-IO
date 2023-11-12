#ifndef INCLUDE_ENGINE_H
#define INCLUDE_ENGINE_H


enum engine_state {
    MENU_GLOBAL,
    MENU_PLAYERS,
    MENU_AI_DIFFICULTY,
    MENU_HIGHSCORE,

    GAME_START_TWO_PLAYERS,
    GAME_START_AI_EASY,
    GAME_START_AI_MEDIUM,
    GAME_START_AI_HARD,
    GAME_END  
};

void render_menu_global(enum engine_state *state);
void render_menu_players(enum engine_state *state);
void render_menu_ai_difficulty(enum engine_state *state);

void render_pong_game(enum engine_state *engine);

#endif