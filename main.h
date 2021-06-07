#ifndef BOARD_H_
#define BOARD_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

typedef struct board board;
typedef unsigned int uint;
typedef unsigned long ulong;

enum direction {
	north,
	south,
	west,
	east,
	quit,
};

struct board {
	uint x[4][4];
	uint points;
	uint num_p;
};

board *new_board();
void init_board(board*);
void free_board(board*);
bool move_possible_any(board*);
bool move_possible(board*, const int d);
void make_move(board*, const int d);
void move(board*, const int d);

void print_board(board*);
void print_score(board*);

#endif
