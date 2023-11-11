#ifndef INCLUDE_ENGINE_H
#define INCLUDE_ENGINE_H


enum engine_state {
    START_MENU,
    START_PLAYERS,
    START_AI_DIFFICULTY,
    START_HIGHSCORE,

    GAME_START_TWO_PLAYERS,
    GAME_START_AI_EASY,
    GAME_START_AI_MEDIUM,
    GAME_START_AI_HARD,
    GAME_END  
};

void render_game_menu(enum engine_state *state);
void render_game_players(enum engine_state *state);
void render_game_ai_difficulty(enum engine_state *state);

#endif