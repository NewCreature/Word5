#ifndef LINGO_TUTORIAL_H
#define LINGO_TUTORIAL_H

#include "player.h"

void lingo_tutorial_update_message(void);
void lingo_tutorial_center_message(void);

void lingo_tutorial_start(int mode);
void lingo_tutorial_transition_in_logic(void);
void lingo_tutorial_transition_out_logic(void);
void lingo_tutorial_logic(void);
void lingo_tutorial_render(void);

#endif
