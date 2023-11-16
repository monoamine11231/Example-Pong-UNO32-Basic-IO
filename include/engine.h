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
    
    GAME_END_TWO_PLAYERS,
    GAME_END_AI
};

struct highscore_entry {
    /* Don't forget the NULL byte at the end */
    char name[7];
    int score;
};


int _GAME_SCORE;
enum engine_state _ENGINE_STATE;

void render_menu_global();
void render_menu_highscore();

void render_menu_players();
void render_menu_ai_difficulty();

void render_pong_game();

void render_menu_end_two_players();
void render_menu_end_ai();

#endif