#include <allegro5/allegro5.h>
#include "t3f/t3f.h"
#include "game.h"
#include "leaderboard.h"

int lingo_save_score(char * fn, int high_score)
{
	ALLEGRO_FILE * fp;
	
	fp = t3f_open_file(t3f_data_path, fn, "wb");
	if(!fp)
	{
		return 0;
	}
	al_fwrite32le(fp, high_score);
	al_fclose(fp);
	return 1;
}

int lingo_load_score(char * fn, int * high_score)
{
	ALLEGRO_FILE * fp;
	
	fp = t3f_open_file(t3f_data_path, fn, "rb");
	if(!fp)
	{
		return 0;
	}
	*high_score = al_fread32le(fp);
	al_fclose(fp);
	return 1;
}

