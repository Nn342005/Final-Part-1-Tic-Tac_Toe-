// game.c
#include <stdio.h>
#include "game.h"

char board[9];

void reset_board() {
    for (int i = 0; i < 9; i++)
        board[i] = EMPTY;
}

void print_board() {
    printf("\n");
    for (int i = 0; i < 9; i += 3) {
        printf(" %c | %c | %c \n", board[i], board[i+1], board[i+2]);
        if (i < 6) printf("---|---|---\n");
    }
    printf("\n");
}

int is_winner(char player) {
    int wins[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    };
    for (int i = 0; i < 8; i++) {
        if (board[wins[i][0]] == player &&
            board[wins[i][1]] == player &&
            board[wins[i][2]] == player)
            return 1;
    }
    return 0;
}

int is_draw() {
    for (int i = 0; i < 9; i++)
        if (board[i] == EMPTY)
            return 0;
    return 1;
}

int is_valid_move(int pos) {
    return pos >= 0 && pos < 9 && board[pos] == EMPTY;
}

void make_move(int pos, char player) {
    if (is_valid_move(pos))
        board[pos] = player;
}
