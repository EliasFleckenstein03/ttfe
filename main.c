#include "main.h"

#define DBG 0

bool move_possible_north(board *b);
bool move_possible_south(board *b);
bool move_possible_east(board *b);
bool move_possible_west(board *b);

void move_north(board *b);
void move_south(board *b);
void move_east(board *b);
void move_west(board *b);
void merge(board *b, const int d);
void merge_north(board *b);
void merge_south(board *b);
void merge_east(board *b);
void merge_west(board *b);

void place_new_piece(board *b);
void game_loop(board *b);

board* new_board() {
	board *tmp = malloc(sizeof(board));
	init_board(tmp);
	return tmp;
}

void init_board(board *b) {
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j)
			b->x[i][j] = 0;
	}
	b->points = 0;
	b->num_p = 0;
	srand((uint)time(NULL));
	place_new_piece(b);
	// second piece will be placed in game_loop
}

void game_start() {
	printf("\e[?1049h\e[?25l]");
	struct termios oldtio, newtio;
	tcgetattr(STDIN_FILENO, &oldtio);
	newtio = oldtio;
	newtio.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newtio);
	board *b = new_board();
	game_loop(b);
	printf("\e[?1049l\e[?25h");
	tcsetattr(STDIN_FILENO, TCSANOW, &oldtio);
	print_score(b);
	free_board(b);
}

enum direction get_input()
{
	switch(fgetc(stdin)) {
		case 'a':
			return west;
		case 's':
			return south;
		case 'w':
			return north;
		case 'd':
			return east;
		case 'q':
			return quit;
		default:
			return get_input();
	}
}

void game_loop(board *b) {
	while(move_possible_any(b)) {
		place_new_piece(b);
		print_board(b);
		while(true) {
			enum direction d = get_input();

			if (d == quit) {
				return;
			} else if(move_possible(b, d)) {
				make_move(b, d);
				break;
			}
		}
	}
}

void place_new_piece(board *b) {
	int prob = rand() % 100;
	int possibs[32];
	uint val = 0;
	int cnt = 0;
	int pair = 0;
	int rx = 0, ry = 0;
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			if(b->x[i][j] == 0) {
				possibs[2*cnt] = i;
				possibs[2*cnt+1] = j;
				cnt++;
			}
		}
	}
	assert(cnt > 0);
	pair = rand() % cnt;
	rx = possibs[2*pair];
	ry = possibs[2*pair+1];
	assert(b->x[rx][ry] == 0);
	if(prob > 9)
		val = 2;
	else
		val = 4;
	b->x[rx][ry] = val;
	b->num_p++;
}

void free_board(board *b) {
	free(b);
}

void make_move(board *b, const int d) {
	bool ok = (d == south) ||
		(d == east) ||
		(d == north) ||
		(d == west);
	assert(ok);
	/*
	 * checked before already
	 * if(!move_possible(b, d))
	 * 	return;
	*/
	move(b, d);
	merge(b, d);
	move(b, d);
}

bool move_possible_any(board *b) {
	if(b->num_p < 16)
		return true;
	if(move_possible_south(b))
		return true;
	if(move_possible_east(b))
		return true;
	if(move_possible_north(b))
		return true;
	if(move_possible_west(b))
		return true;
	return false;
}

bool move_possible(board *b, const int d) {
	bool ok = (d == south) ||
		(d == east) ||
		(d == north) ||
		(d == west);
	assert(ok);
	switch(d) {
		case north:
			return move_possible_north(b);
		case south:
			return move_possible_south(b);
		case east:
			return move_possible_east(b);
		case west:
			return move_possible_west(b);
		default: // will never execute anyway
			return false;
	}
}

void move(board *b, const int d) {
	bool ok = (d == south) ||
		(d == east) ||
		(d == north) ||
		(d == west);
	assert(ok);
	switch(d) {
		case north:
			move_north(b);
			break;
		case south:
			move_south(b);
			break;
		case east:
			 move_east(b);
			break;
		case west:
			move_west(b);
			break;
		default: // will never execute anyway
			return;
	}
}

void merge(board *b, const int d) {
	bool ok = (d == south) ||
		(d == east) ||
		(d == north) ||
		(d == west);
	assert(ok);
	switch(d) {
		case north:
			merge_north(b);
			break;
		case south:
			merge_south(b);
			break;
		case east:
			merge_east(b);
			break;
		case west:
			merge_west(b);
			break;
		default: // will never execute anyway
			return;
	}
}

bool move_possible_south(board *b) {
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 3; ++j) {
			if(b->x[i][j] != 0) {
				if(b->x[i][j+1] == 0)
					return true;
				if(b->x[i][j] == b->x[i][j+1])
					return true;
			}
		}
	}
	return false;
}

bool move_possible_north(board *b) {
	for(int i = 0; i < 4; ++i) {
		for(int j = 1; j < 4; ++j) {
			if(b->x[i][j] != 0) {
				if(b->x[i][j-1] == 0)
					return true;
				if(b->x[i][j] == b->x[i][j-1])
					return true;
			}
		}
	}
	return false;
}

bool move_possible_east(board *b) {
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 4; ++j) {
			if(b->x[i][j] != 0) {
				if(b->x[i+1][j] == 0)
					return true;
				if(b->x[i+1][j] == b->x[i][j])
					return true;
			}
		}
	}
	return false;
}

bool move_possible_west(board *b) {
	for(int i = 1; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			if(b->x[i][j] != 0) {
				if(b->x[i-1][j] == 0)
					return true;
				if(b->x[i-1][j] == b->x[i][j])
					return true;
			}
		}
	}
	return false;
}

void move_north(board *b) {
	int k;
	for(int i = 0; i < 4; ++i) {
		for(int j = 1; j < 4; ++j) {
			if(b->x[i][j] != 0) {
				k = j;
				while(b->x[i][k-1] == 0) {
					b->x[i][k-1] = b->x[i][k];
					b->x[i][k] = 0;
					k--;
					if(k < 1)
						break;
				}
			}
		}
	}
}

void move_south(board *b) {
	int k;
	for(int i = 0; i < 4; ++i) {
		for(int j = 2; j >= 0; --j) {
			if(b->x[i][j] != 0) {
				k = j;
				while(b->x[i][k+1] == 0) {
					b->x[i][k+1] = b->x[i][k];
					b->x[i][k] = 0;
					k++;
					if(k > 2)
						break;
				}
			}
		}
	}
}

void move_east(board *b) {
	int k;
	for(int i = 2; i >= 0; --i) {
		for(int j = 0; j < 4; ++j) {
			if(b->x[i][j] != 0) {
				k = i;
				while(b->x[k+1][j] == 0) {
					b->x[k+1][j] = b->x[k][j];
					b->x[k][j] = 0;
					k++;
					if(k > 2)
						break;
				}
			}
		}
	}
}

void move_west(board *b) {
	int k;
	for(int i = 1; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			if(b->x[i][j] != 0) {
				k = i;
				while(b->x[k-1][j] == 0) {
					b->x[k-1][j] = b->x[k][j];
					b->x[k][j] = 0;
					k--;
					if(k < 1)
						break;
				}
			}
		}
	}
}

void merge_north(board *b) {
	for(int i = 0; i < 4; ++i) {
		for(int j = 1; j < 4; ++j) {
			if(b->x[i][j] != 0 && b->x[i][j] == b->x[i][j-1]) {
				b->x[i][j-1] *= 2;
				b->x[i][j] = 0;
				b->num_p--;
				b->points += b->x[i][j-1];
			}
		}
	}
}

void merge_south(board *b) {
	for(int i = 0; i < 4; ++i) {
		for(int j = 2; j >= 0; --j) {
			if(b->x[i][j] != 0 && b->x[i][j] == b->x[i][j+1]) {
				b->x[i][j+1] *= 2;
				b->x[i][j] = 0;
				b->num_p--;
				b->points += b->x[i][j+1];
			}
		}
	}
}

void merge_east(board *b) {
	for(int i = 2; i >= 0; --i) {
		for(int j = 0; j < 4; ++j) {
			if(b->x[i][j] != 0 && b->x[i+1][j] == b->x[i][j]) {
				b->x[i+1][j] *= 2;
				b->points += b->x[i+1][j];
				b->num_p--;
				b->x[i][j] = 0;
			}
		}
	}
}

void merge_west(board *b) {
	for(int i = 1; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			if(b->x[i][j] != 0 && b->x[i-1][j] == b->x[i][j]) {
				b->x[i-1][j] *= 2;
				b->points += b->x[i-1][j];
				b->num_p--;
				b->x[i][j] = 0;
			}
		}
	}
}

void center_print(uint n, int width)
{
    char s[20] = {'\0'};
    int len;
    sprintf(s, "%u", n);
    len = strlen(s);
    if (len >= width)  {
        printf("%s", s);
    } else {
        int remaining = width - len;
        int spaces_right = remaining / 2;
        int spaces_left = remaining - spaces_right;
        printf("%*s%s%*s", spaces_left, "", s, spaces_right, "");
    }
}

void print_sep(const char *left, const char *right, const char *cross, const char *line)
{
	printf("%s", left);
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 6; j++)
			printf("%s", line);
		if(i == 3)
			printf("%s", right);
		else
			printf("%s", cross);
	}
	printf("\n");
}

uint mylog2(uint n) {
	uint i;

	for (i = 0; ! (n & 1); i++)
		n >>= 1;

	return i;
}

void print_board_line(board *b, int l) {
	printf("\u2503");

	for(int i = 0; i < 4; i++) {
		uint n = b->x[i][l];

		if(n == 0) {
			printf("      ");
		} else {
			uint c = mylog2(n);

			if (c > 6) {
				c -= 6;
				printf("\e[1m");
			}

			printf("\e[3%1um", c);
			center_print(n, 6);
			printf("\e[0m");
		}

		if(i == 3)
			printf("\u2503");
		else
			printf("\u2502");
	}

	printf("\n");
	print_sep("\u2503", "\u2503", "\u2502", " ");

	if(l == 3)
		print_sep("\u2517", "\u251B", "\u2537", "\u2501");
	else
		print_sep("\u2520", "\u2528", "\u253C", "\u2500");
}

void print_board(board *b) {
	printf("\e[2J\e[0;0H");
	printf("\e[1mScore: \e[0m%u\n", b->points);
	print_sep("\u250F", "\u2513", "\u252F", "\u2501");
	for(int i = 0; i < 4; ++i) {
		print_board_line(b, i);
	}
}

void print_score(board *b) {
	printf("\e[1m\e[91mGame Over\e[0m\n\e[1mScore: \e[0m%u\n", b->points);
}

void merge_test1() {
	board *b = new_board();
	b->x[2][0] = 2;
	b->x[3][0] = 2;
	merge_east(b);
	assert(b->x[3][0] == 4);
}

void merge_test2() {
	board *b = new_board();
	b->x[2][0] = 2;
	b->x[3][0] = 2;
	print_board(b);
	move_west(b);
	print_board(b);
	merge_west(b);
	print_board(b);
	assert(b->x[0][0] == 4);
}

void merge_test3() {
	board *b = new_board();
	b->x[0][0] = 2;
	b->x[0][1] = 2;
	print_board(b);
	merge_north(b);
	print_board(b);
	assert(b->x[0][0] == 4);
}

void merge_test4() {
	board *b = new_board();
	b->x[0][2] = 2;
	b->x[0][3] = 2;
	print_board(b);
	merge_south(b);
	print_board(b);
	assert(b->x[0][3] == 4);
}

void move_merge_test1() {
	board *b = new_board();
	b->x[2][0] = 4;
	b->x[3][0] = 2;
	b->x[3][2] = 2;
	print_board(b);
	move_south(b);
	print_board(b);
	merge_south(b);
	print_board(b);
	assert(b->x[3][3] == 4);
}

void move_north_test() {
	board *b = new_board();
	b->x[2][3] = 8;
	b->x[2][2] = 0;
	b->x[2][1] = 0;
	b->x[2][0] = 0;
	b->x[0][1] = 2;
	b->x[0][0] = 0;
	b->x[3][0] = 4;
	b->x[3][1] = 2;
	b->x[3][2] = 3;
	b->x[3][3] = 16;
	print_board(b);
	move_north(b);
	merge_north(b);
	move_north(b);
	print_board(b);
	assert(b->x[2][0] == 8);
	assert(b->x[0][0] == 2);
}

void move_merge_test2() {
	board *b = new_board();
	b->x[3][0] = 2;
	b->x[3][1] = 2;
	b->x[3][2] = 4;
	b->x[3][3] = 32;
	print_board(b);
	move_south(b);
	merge_south(b);
	move_south(b);
	print_board(b);
	assert(b->x[3][1] == 4);
}

void move_merge_test3() {
	board *b = new_board();
	b->x[0][0] = 0;
	b->x[0][1] = 0;
	b->x[0][2] = 0;
	b->x[0][3] = 2;
	b->x[1][0] = 0;
	b->x[1][1] = 0;
	b->x[1][2] = 0;
	b->x[1][3] = 8;
	b->x[2][0] = 0;
	b->x[2][1] = 0;
	b->x[2][2] = 0;
	b->x[2][3] = 4;
	b->x[3][0] = 2;
	b->x[3][1] = 2;
	b->x[3][2] = 4;
	b->x[3][3] = 16;
	print_board(b);
	move_south(b);
	merge_south(b);
	move_south(b);
	print_board(b);
	assert(b->x[3][1] == 4);
}

void run_tests() {
	/*
	merge_test1();
	merge_test2();
	merge_test3();
	merge_test4();
	move_merge_test1();
	move_north_test();
	*/
	move_merge_test2();
	move_merge_test3();
}

int main() {
#if DBG
	run_tests();
#else
	game_start();
#endif
}
