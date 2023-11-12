#define _USE_MATH_DEFINES

#include "engine.h"
#include "drivers/display.h"
#include "drivers/io.h"
#include "sys_utils.h"

#include <math.h>


struct board {
    float x;
    float y;
    int width;
};

static void render_board(struct board *b) {
    put_box((int)b->x, (int)b->y, 2, b->width);
}


struct ball {
    float x;
    float y;
    int size;
    float dx;
    float dy;
    float speed;
};

static void render_ball(struct ball *b) {
    put_box((int)b->x, (int)b->y, b->size, b->size);
}

static int _board_ball_colision(struct board *board, struct ball *ball) {
    return  ( ((board->x+2) >= (int)(ball->x)) && 
              (board->x <= ((int)(ball->x)+ball->size)) &&
              ((board->y+board->width) >= (int)(ball->y)) &&
              (board->y <= ((int)(ball->y)+ball->size)) );
}

void render_pong_game(enum engine_state *state) {
    struct io_shield_input st;
    struct board boards[2];
    struct ball ball;
    
    int score = 0;
    int ai_mode = 0;
    ai_mode |= (*state==GAME_START_AI_EASY);
    ai_mode |= (*state==GAME_START_AI_MEDIUM);
    ai_mode |= (*state==GAME_START_AI_HARD);
    
    /* How fast will the AI board respond to the ball's y axis change? */
    float ai_response = 0.0f;
    switch (*state)
    {
    case GAME_START_AI_EASY:
        ai_response = 0.3f;
        break;
    case GAME_START_AI_MEDIUM:
        ai_response = 0.5f;
        break;
    case GAME_START_AI_HARD:
        ai_response = 0.7f;
        break;
    default:
        break;
    }


    boards[0].x = 0;
    boards[0].y = 0;
    boards[0].width = 16;

    boards[1].x = 126;
    boards[1].y = 15;
    boards[1].width = 10;

    ball.x = 128/2-1;
    ball.y = 32/2-1;
    ball.size = 2;
    ball.dx = cosf(0.5f);
    ball.dy = sinf(0.5f);
    ball.speed = 1;

    for (;;) {
        clear_video();
        source_io_shield_inputs(&st);

        if (ai_mode) {
            boards[1].y += (int)((ball.y-(float)(boards[1].y))*ai_response);
            boards[1].y = MIN(boards[1].y, 32-boards[1].width);
            boards[1].y = MAX(boards[1].y, 0);

            /* Control so that the boards are not going out of the screen */
            boards[0].y = MIN(boards[0].y+st.btn1, 32-st.btn1-boards[0].width);
            boards[0].y = MAX(boards[0].y-st.btn2, 0);
        } else {
            /* Control so that the boards are not going out of the screen */
            boards[0].y = MIN(boards[0].y+st.btn3, 32-st.btn3-boards[0].width);
            boards[0].y = MAX(boards[0].y-st.btn4, 0);

            boards[1].y = MIN(boards[1].y+st.btn1, 32-st.btn1-boards[1].width);
            boards[1].y = MAX(boards[1].y-st.btn2, 0);
        }

        /* Make sure that the ball is inside of the screen, do not allow overflow */
        ball.y = MIN(ball.y, 30);
        ball.y = MAX(ball.y, 0);

        /* Bounce of top and bottom of the screen */
        if ((((int)(ball.y)+ball.size-1) == 31) || ((int)(ball.y) == 0)) {
            ball.dy *= -1;
        }

        /* If hit the left or right screen walls => game over */
        if ((((int)(ball.x)+ball.size-1) == 128) || ((int)ball.x == 0)) {
            *state = MENU_GLOBAL;
            break;
        }

        if (_board_ball_colision(&boards[0], &ball)) {
            ball.x = (int)(ball.x)+1;
            ball.dx *= -1;
            ++score;
        }

        if (_board_ball_colision(&boards[1], &ball)) {
            ball.x = (int)(ball.x)-1;
            ball.dx *= -1;
            ++score;
        }

        ball.x += ball.speed*ball.dx;
        ball.y += ball.speed*ball.dy;

        render_board(&boards[0]);
        render_board(&boards[1]);
        render_ball(&ball);

        put_digit(70, 0, score);

        flush_video();

        /* Artificial delay so that inputs are not acted on immediately*/
        quicksleep(INTERUPT_QUICKDELAY_VALUE / 3);
    }
}