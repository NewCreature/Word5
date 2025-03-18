#ifndef LINGO_LEADERBOARD_H
#define LINGO_LEADERBOARD_H

int lingo_obfuscate_score(int score);
int lingo_unobfuscate_score(int score);
bool lingo_verify_score(int score);

bool lingo_get_leaderboard(void * data);
void lingo_leaderboard_logic(void * data);
void lingo_leaderboard_render(void * data);

#endif
