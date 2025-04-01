#include "t3f/t3f.h"
#include "modules/draw_text.h"
#include "instance.h"
#include "credits.h"
#include "main.h"
#include "menu_proc.h"

LINGO_CREDITS * lingo_create_credits(int max)
{
  LINGO_CREDITS * cp;

  cp = malloc(sizeof(LINGO_CREDITS));
  if(!cp)
  {
    goto fail;
  }
  cp->credit_size = max;
  cp->credit_count = 0;
  cp->credit = malloc(sizeof(LINGO_CREDIT) * cp->credit_size);
  if(!cp->credit)
  {
    goto fail;
  }
  memset(cp->credit, 0, sizeof(LINGO_CREDIT) * cp->credit_size);

  return cp;

  fail:
  {
    lingo_destroy_credits(cp);
    return NULL;
  }
}

void lingo_destroy_credits(LINGO_CREDITS * cp)
{
  int i;

  if(cp)
  {
    if(cp->credit)
    {
      for(i = 0; i < cp->credit_count; i++)
      {
        if(cp->credit[i].text)
        {
          free(cp->credit[i].text);
        }
      }
      free(cp->credit);
    }
    free(cp);
  }
}

void lingo_add_credit(LINGO_CREDITS * cp, T3F_FONT * fp, ALLEGRO_COLOR color, float x, float y, const char * text)
{
  cp->credit[cp->credit_count].font = fp;
  cp->credit[cp->credit_count].color = color;
  cp->credit[cp->credit_count].x = x;
  cp->credit[cp->credit_count].y = y;
  cp->credit[cp->credit_count].text = strdup(text);
  if(cp->credit[cp->credit_count].text)
  {
    cp->credit_count++;
  }
}

void lingo_start_credits(LINGO_CREDITS * cp, float top, float height, float x, float y, float vy)
{
  cp->top = top;
  cp->height = height;
  cp->x = x;
  cp->y = y;
  cp->vy = vy;
}

bool lingo_process_credits(LINGO_CREDITS * cp)
{
  int i = cp->credit_count - 1;
  cp->y += cp->vy;
  if(cp->y + cp->credit[i].y + t3f_get_font_line_height(cp->credit[i].font) < cp->top)
  {
    return false;
  }
  return true;
}

void lingo_render_credits(LINGO_CREDITS * cp)
{
  int i;

  al_hold_bitmap_drawing(true);
  for(i = 0; i < cp->credit_count; i++)
  {
    if(cp->y + cp->credit[i].y + t3f_get_font_line_height(cp->credit[i].font) >= cp->top && cp->y + cp->credit[i].y <= cp->top + cp->height)
    {
      lingo_draw_text_with_shadow(cp->credit[i].font, cp->credit[i].color, al_map_rgba_f(0.0, 0.0, 0.0, 0.5), cp->x + cp->credit[i].x, cp->y + cp->credit[i].y, 0, 1, 1, T3F_FONT_ALIGN_CENTER, cp->credit[i].text);
    }
  }
  al_hold_bitmap_drawing(false);
}

bool lingo_setup_credits(void * data)
{
  APP_INSTANCE * instance = (APP_INSTANCE *)data;
  float line_height;
  float pos_x;
  float pos_y;
  T3F_FONT * font;
  ALLEGRO_COLOR header_color;
  ALLEGRO_COLOR name_color;

  font = instance->font[LINGO_FONT_SPRINT_20];
  line_height = t3f_get_font_line_height(font);
  header_color = al_map_rgba_f(0.0, 1.0, 0.0, 1.0);
  name_color = t3f_color_white;
  pos_x = 0;
  pos_y = 0;

  instance->credits = lingo_create_credits(256);
  if(!instance->credits)
  {
    goto fail;
  }

  lingo_add_credit(instance->credits, font, header_color, pos_x, pos_y, "Production");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Todd Cope");
  pos_y += line_height * 2.0;

  lingo_add_credit(instance->credits, font, header_color, pos_x, pos_y, "Design");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Todd Cope");
  pos_y += line_height * 2.0;
 
  lingo_add_credit(instance->credits, font, header_color, pos_x, pos_y, "Programming");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Todd Cope");
  pos_y += line_height * 2.0;

  lingo_add_credit(instance->credits, font, header_color, pos_x, pos_y, "Graphics");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Todd Cope");
  pos_y += line_height * 2.0;

  lingo_add_credit(instance->credits, font, header_color, pos_x, pos_y, "Fonts");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Juan Montoreano  (Lilita One)");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Font Diner  (Permanent Marker)");
  pos_y += line_height * 2.0;

  lingo_add_credit(instance->credits, font, header_color, pos_x, pos_y, "Sound");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Todd Cope");
  pos_y += line_height * 2.0;

  lingo_add_credit(instance->credits, font, header_color, pos_x, pos_y, "Testing");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Todd Cope");
  pos_y += line_height * 2.0;

  lingo_add_credit(instance->credits, font, header_color, pos_x, pos_y, "Third Party Code");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Matthew Leverton (Nine Patch Bitmaps)");
  pos_y += line_height * 2.0;

  lingo_add_credit(instance->credits, font, header_color, pos_x, pos_y, "Code Libraries");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "zlib");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "PNG");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "FreeType");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Ogg Vorbis");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Allegro");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "OpenGL");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "DirectX");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "MacOS SDK");
  pos_y += line_height * 2.0;

  lingo_add_credit(instance->credits, font, header_color, pos_x, pos_y, "Third Party Tools");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "GCC");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "GNU Make");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "MSYS2");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Clang");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "PhotoPlus");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Paint Shop Pro");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "InkScape");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Crimson Editor");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Atom");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "Visual Studio Code");
  pos_y += line_height;
  lingo_add_credit(instance->credits, font, name_color, pos_x, pos_y, "nano");
  pos_y += line_height * 2.0;
 
  return true;

  fail:
  {
    return false;
  }
}

void lingo_credits_logic(void * data)
{
  APP_INSTANCE * instance = (APP_INSTANCE *)data;

  if(!lingo_process_credits(instance->credits))
  {
    lingo_menu_proc_credits_back(data);
  }
  lingo_menu_logic(data);
}

void lingo_credits_render(void * data)
{
  APP_INSTANCE * instance = (APP_INSTANCE *)data;

	t3f_select_view(t3f_default_view);
  al_hold_bitmap_drawing(false);
	t3f_draw_bitmap(instance->image[LINGO_IMAGE_BG], t3f_color_white, 0, 0, 0, 0);
	lingo_select_view(data);
	t3f_draw_bitmap(instance->image[LINGO_IMAGE_LOGO], t3f_color_white, 320 - instance->image[LINGO_IMAGE_LOGO]->target_width / 2.0, LINGO_LOGO_POS_Y, instance->title_logo_z, 0);
  t3f_set_clipping_rectangle(0, LINGO_MENU_POS_TOP, instance->view->virtual_width, LINGO_MENU_POS_BOTTOM - LINGO_MENU_POS_TOP);
  lingo_render_credits(instance->credits);
  t3f_set_clipping_rectangle(0, 0, 0, 0);
  lingo_menu_render(data, 1.0);
}
