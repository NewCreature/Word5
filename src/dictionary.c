#include "t3f/t3f.h"
#include "dictionary.h"

/* import a dictionary from a plain text file */
LINGO_DICTIONARY * lingo_import_dictionary(const char * fn)
{
	LINGO_DICTIONARY * dp;
	ALLEGRO_FILE * fp;
	char * textbuffer;
	char wordbuffer[256] = {0};
	int wordpos = 0;
	int i;
	int fsize = 0;
	
	/* allocate the dictionary */
	dp = malloc(sizeof(LINGO_DICTIONARY));
	if(!dp)
	{
		return NULL;
	}
	
	/* open the file */
	fp = al_fopen(fn, "rb");
	if(!fp)
	{
		free(dp);
		return NULL;
	}
	fsize = al_fsize(fp);
	
	/* create buffer to read file into */
	textbuffer = malloc(fsize);
	if(!textbuffer)
	{
		free(dp);
		return NULL;
	}
	
	/* read the file into the buffer */
	if(al_fread(fp, textbuffer, fsize) != fsize)
	{
		free(textbuffer);
		free(dp);
		return NULL;
	}
	
	dp->words = 0;
	for(i = 0; i < fsize; i++)
	{
		wordbuffer[wordpos] = textbuffer[i];
		
		/* finish word */
		if(wordbuffer[wordpos] == '\n')
		{
			wordbuffer[wordpos - 1] = '\0';
			wordpos = -1;
			
			/* if it's a 5-letter word, add it to dictionary */
			if(strlen(wordbuffer) == 5)
			{
				strcpy(dp->word[dp->words].letter, wordbuffer);
				dp->used[dp->words] = 0;
				dp->useable[dp->words] = 1;
				dp->available[dp->words] = dp->words;
				dp->words++;
				dp->available_words++;
			}
		}
		wordpos++;
	}
	al_fclose(fp);
	return dp;
}

LINGO_DICTIONARY * lingo_load_old_dictionary(const char * fn)
{
	LINGO_DICTIONARY * dp;
	char header[6]; // header, last char is version
	ALLEGRO_FILE * fp;
	int i, j;
	
	/* allocate the dictionary */
	dp = malloc(sizeof(LINGO_DICTIONARY));
	if(!dp)
	{
		return NULL;
	}
	memset(dp, 0, sizeof(LINGO_DICTIONARY));
	
	dp->available_words = 0;
	
	/* open the file in binary mode */
	fp = al_fopen(fn, "rb");
	if(!fp)
	{
		free(dp);
		return NULL;
	}
	
	/* read the header */
	if(al_fread(fp, header, 6) != 6)
	{
		al_fclose(fp);
		free(dp);
		return NULL;
	}
	
	/* check file type */
	if(header[0] != 'L' || header[1] != 'D' || header[2] != 'I' || header[3] != 'C' || header[4] != 'T' || header[5] != 0)
	{
		al_fclose(fp);
		free(dp);
		return NULL;
	}
	
	/* read the words */
	dp->words = al_fread32le(fp);
	for(i = 0; i < dp->words; i++)
	{
		for(j = 0; j < LINGO_WORD_MAX_SIZE; j++)
		{
			dp->word[i].letter[j] = al_fgetc(fp) - 10;
		}
	}
	if(al_fread(fp, dp->used, dp->words) != dp->words)
	{
		al_fclose(fp);
		free(dp);
		return NULL;
	}
	if(al_fread(fp, dp->useable, dp->words) != dp->words)
	{
		al_fclose(fp);
		free(dp);
		return NULL;
	}
	al_fclose(fp);
	
	return dp;
}

/* load a dictionary file */
LINGO_DICTIONARY * lingo_load_dictionary(const char * fn)
{
	LINGO_DICTIONARY * dp;
	char header[6]; // header, last char is version
	ALLEGRO_FILE * fp;
	int i, j;
	
	/* allocate the dictionary */
	dp = malloc(sizeof(LINGO_DICTIONARY));
	if(!dp)
	{
		return NULL;
	}
	memset(dp, 0, sizeof(LINGO_DICTIONARY));
	
	dp->available_words = 0;
	
	/* open the file in binary mode */
	fp = al_fopen(fn, "rb");
	if(!fp)
	{
		free(dp);
		return NULL;
	}
	
	/* read the header */
	if(al_fread(fp, header, 6) != 6)
	{
		al_fclose(fp);
		free(dp);
		return NULL;
	}
	
	/* check file type */
	if(header[0] != 'L' || header[1] != 'D' || header[2] != 'I' || header[3] != 'C' || header[4] != 'T' || header[5] != 0)
	{
		al_fclose(fp);
		free(dp);
		return NULL;
	}
	
	/* read the words */
	dp->words = al_fread32le(fp);
	for(i = 0; i < dp->words; i++)
	{
		for(j = 0; j < LINGO_WORD_MAX_SIZE; j++)
		{
			dp->word[i].letter[j] = al_fgetc(fp) - 10;
		}
	}
	if(al_fread(fp, dp->useable, dp->words) != dp->words)
	{
		al_fclose(fp);
		free(dp);
		return NULL;
	}
	al_fclose(fp);
	
	return dp;
}

bool lingo_load_dictionary_config(LINGO_DICTIONARY * dp, const char * fn)
{
	char header[6]; // header, last char is version
	ALLEGRO_FILE * fp;
	int i;
	
	/* open the file in binary mode */
	fp = t3f_open_file(t3f_data_path, fn, "rb");
	if(!fp)
	{
		return false;
	}
	
	/* read the header */
	if(al_fread(fp, header, 6) != 6)
	{
		al_fclose(fp);
		return false;
	}
	
	/* check file type */
	if(header[0] != 'L' || header[1] != 'D' || header[2] != 'C' || header[3] != 'F' || header[4] != 'G' || header[5] != 0)
	{
		al_fclose(fp);
		return false;
	}
	
	/* read the words */
	if(al_fread(fp, dp->used, dp->words) != dp->words)
	{
		al_fclose(fp);
		return false;
	}
	al_fclose(fp);
	
	/* build available words list */
	for(i = 0; i < dp->words; i++)
	{
		if(dp->useable[i] && !dp->used[i])
		{
			dp->available[dp->available_words] = i;
			dp->available_words++;
		}
	}
	
	/* if we have used all the words, start over */
	if(dp->available_words <= 0)
	{
		for(i = 0; i < dp->words; i++)
		{
			dp->used[i] = 0;
			dp->available[i] = i;
			dp->available_words++;
		}
	}
	
	return true;
}

/* save a dictionary file */
int lingo_save_dictionary(LINGO_DICTIONARY * dp, const char * fn)
{
	char header[6] = {'L', 'D', 'I', 'C', 'T', 0}; // header, last char is version
	ALLEGRO_FILE * fp;
	int i, j;
	
	/* open the file in binary mode */
	fp = al_fopen(fn, "wb");
	if(!fp)
	{
		return 0;
	}
	
	/* write the header */
	if(al_fwrite(fp, header, 6) != 6)
	{
		al_fclose(fp);
		return 0;
	}
	
	/* write the words */
	al_fwrite32le(fp, dp->words);
	for(i = 0; i < dp->words; i++)
	{
		for(j = 0; j < LINGO_WORD_MAX_SIZE; j++)
		{
			al_fputc(fp, dp->word[i].letter[j] + 10);
		}
	}
	if(al_fwrite(fp, dp->useable, dp->words) != dp->words)
	{
		al_fclose(fp);
		return 0;
	}
	al_fclose(fp);
	return 1;
}

/* save a dictionary file */
bool lingo_save_dictionary_config(LINGO_DICTIONARY * dp, const char * fn)
{
	char header[6] = {'L', 'D', 'C', 'F', 'G', 0}; // header, last char is version
	ALLEGRO_FILE * fp;
	
	/* open the file in binary mode */
	fp = t3f_open_file(t3f_data_path, fn, "wb");
	if(!fp)
	{
		return false;
	}
	
	/* write the header */
	if(al_fwrite(fp, header, 6) != 6)
	{
		al_fclose(fp);
		return false;
	}
	
	/* write the words */
	al_fwrite32le(fp, dp->words);
	if(al_fwrite(fp, dp->used, dp->words) != dp->words)
	{
		al_fclose(fp);
		return false;
	}
	al_fclose(fp);
	return true;
}

/* delete the specified word from the dictionary */
int lingo_dictionary_delete_word(LINGO_DICTIONARY * dp, int word)
{
	int i;
	
	if(word >= dp->words)
	{
		return 0;
	}
	
	for(i = word; i < dp->words - 1; i++)
	{
		strcpy(dp->word[i].letter, dp->word[i + 1].letter);
		dp->used[i] = dp->used[i + 1];
	}
	dp->words--;
	return 1;
}

/* choose word from all available words */
int lingo_dictionary_choose_word(LINGO_DICTIONARY * dp)
{
	int pick, i;
	int rpick;
	
	if(dp->available_words <= 0)
	{
		dp->available_words = 0;
		for(i = 0; i < dp->words; i++)
		{
			dp->used[i] = 0;
			if(dp->useable[i] && !dp->used[i])
			{
				dp->available[dp->available_words] = i;
				dp->available_words++;
			}
		}
	}
	rpick = rand() % dp->available_words;
	pick = dp->available[rpick];
	dp->used[pick] = 1;
	for(i = rpick; i < dp->available_words - 1; i++)
	{
		dp->available[i] = dp->available[i + 1];
	}
	dp->available_words--;
	return pick;
}

int lingo_dictionary_check_word(LINGO_DICTIONARY * dp, char * word)
{
	int i;
	
	for(i = 0; i < dp->words; i++)
	{
		if(!strcmp(dp->word[i].letter, word))
		{
			return 1;
		}
	}
	return 0;
}
