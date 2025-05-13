// game.h
#ifndef GAME_H
#define GAME_H

#define EMPTY ' '
#define PLAYER_X 'X'
#define PLAYER_O 'O'

extern char board[9];

void reset_board();
void print_board();
int is_winner(char player);
int is_draw();
int is_valid_move(int pos);
void make_move(int pos, char player);

#endif
