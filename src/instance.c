#include "t3f/t3f.h"
#include "instance.h"

APP_INSTANCE * w5_create_instance(int argc, char * argv[])
{
  APP_INSTANCE * instance = NULL;

  instance = malloc(sizeof(APP_INSTANCE));
  if(!instance)
  {
    goto fail;
  }
  memset(instance, 0, sizeof(APP_INSTANCE));
  instance->argc = argc;
  instance->argv = argv;
  instance->current_menu = LINGO_MENU_MAIN;
  instance->current_game_menu = LINGO_GAME_MENU_MAIN;
  instance->state = LINGO_STATE_TITLE_TRANSITION_IN;
  instance->leaderboard_place = -1;
  instance->title_logo_z = -640.0;
  instance->input_type = LINGO_INPUT_TYPE_KEYBOARD;

  return instance;

  fail:
  {
    return NULL;
  }
}

void w5_destroy_instance(APP_INSTANCE * app)
{
  free(app);
}
