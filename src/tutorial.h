#ifndef LINGO_TUTORIAL_H
#define LINGO_TUTORIAL_H

#include "player.h"

void lingo_tutorial_update_message(void * data);
void lingo_tutorial_center_message(void * data);

void lingo_tutorial_start(int mode, void * data);
void lingo_tutorial_transition_in_logic(void * data);
void lingo_tutorial_transition_out_logic(void * data);
void lingo_tutorial_logic(void * data);
void lingo_tutorial_render(void * data);

#endif
