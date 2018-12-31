#ifndef LINGO_DICTIONARY_H
#define LINGO_DICTIONARY_H

#define LINGO_WORD_MAX_SIZE            6
#define LINGO_DICTIONARY_MAX_WORDS 65536

typedef struct
{
	
	char letter[LINGO_WORD_MAX_SIZE];
	
} LINGO_WORD;

typedef struct
{
	
	LINGO_WORD word[LINGO_DICTIONARY_MAX_WORDS];
	char used[LINGO_DICTIONARY_MAX_WORDS]; // word has been used before
	char useable[LINGO_DICTIONARY_MAX_WORDS]; // word is usable (meaning the game can pick it)
	int available[LINGO_DICTIONARY_MAX_WORDS]; // available word list (so we can pick easily)
	int words; // total words
	int available_words; // total available words */
	
} LINGO_DICTIONARY;

LINGO_DICTIONARY * lingo_import_dictionary(const char * fn);
LINGO_DICTIONARY * lingo_load_old_dictionary(const char * fn);
LINGO_DICTIONARY * lingo_load_dictionary(const char * fn);
bool lingo_load_dictionary_config(LINGO_DICTIONARY * dp, const char * fn);
int lingo_save_dictionary(LINGO_DICTIONARY * dp, const char * fn);
bool lingo_save_dictionary_config(LINGO_DICTIONARY * dp, const char * fn);

/* utility functions */
int lingo_dictionary_choose_word(LINGO_DICTIONARY * dp);
int lingo_dictionary_check_word(LINGO_DICTIONARY * dp, char * word);

#endif
