#include "t3f/t3f.h"
#include "player.h"

void lingo_reset_player(LINGO_PLAYER * pp)
{
	strcpy(pp->word, "");
	pp->word_pos = 0;
	pp->score = 0;
	pp->letter = 0;
	pp->guess_count = 0;
}
