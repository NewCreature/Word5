#ifndef LINGO_PLAYER_H
#define LINGO_PLAYER_H

#define LINGO_MAX_PLAYERS 4

typedef struct
{
	
	char name[32];
	char word[16];
	int word_pos;
	char letter;
	int profile;
	int score;
	int guess_count;
	
	int lives;
	int bonus_letters;
	
} LINGO_PLAYER;

void lingo_reset_player(LINGO_PLAYER * pp);

#endif
