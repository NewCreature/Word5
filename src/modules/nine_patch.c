#include "t3f/t3f.h"
#include "nine_patch.h"

typedef struct nine_patch_mark_tag
{
	int offset;
	int length;
	int dest_offset;
	int dest_length;
	float ratio;
} NINE_PATCH_MARK;

typedef struct nine_patch_side_tag
{
	NINE_PATCH_MARK *m;
	int count;
	int fix;
} NINE_PATCH_SIDE;

struct nine_patch_bitmap_tag
{
	ALLEGRO_BITMAP *bmp;
	NINE_PATCH_SIDE h, v;
	NINE_PATCH_PADDING padding;
	bool destroy_bmp;
	int width, height;
	int cached_dw, cached_dh;
	float scale;
	ALLEGRO_MUTEX *mutex;
};

static bool init_nine_patch_side(NINE_PATCH_SIDE *ps, ALLEGRO_BITMAP *bmp, int vertical)
{
	const int len = vertical ? al_get_bitmap_height(bmp) : al_get_bitmap_width(bmp);
	int i, s, t, n, z;
	ALLEGRO_COLOR c;

	int alloc = 8;

	ps->m = al_malloc(alloc * sizeof(*ps->m));

	for (i = 1, s = -1, t = 0, n = 0, z = -1; i < len; ++i)
	{
		int zz;
		uint8_t r, g, b, a;
		c = vertical ? al_get_pixel(bmp, 0, i) : al_get_pixel(bmp, i, 0);
		al_unmap_rgba(c, &r, &g, &b, &a);

		if (i == len - 1)
			zz = -1;
		else if (r == 0 && g == 0 && b == 0 && a == 255)
			zz = 0;
		else if (a == 0 || r + g + b + a == 255 * 4)
			zz = 1;
		else
			return false;

		if (z != zz)
		{
			if (s != -1)
			{
				ps->m[n].offset = s;
				ps->m[n].length = i - s;
				if (z == 0)
				{
					ps->m[n].ratio = 1;
					t += ps->m[n].length;
				}
				else
				{
					ps->m[n].ratio = 0;
				}
				++n;
			}
			s = i;
			z = zz;
		}

		if (n == alloc)
		{
			alloc *= 2;
			ps->m = al_realloc(ps->m, alloc * sizeof(*ps->m));
		}
	}

	if (n != alloc)
	{
		ps->m = al_realloc(ps->m, n * sizeof(*ps->m));
	}

	ps->count = n;

	ps->fix = len - 2 - t;
	for (i = 0; i < n; ++i)
	{
		if (ps->m[i].ratio)
			ps->m[i].ratio = ps->m[i].length / (float) t;
	}

	return true;
}

NINE_PATCH_BITMAP *create_nine_patch_bitmap(ALLEGRO_BITMAP *bmp, bool owns_bitmap)
{
	int i;
	NINE_PATCH_BITMAP *p9;
	ALLEGRO_COLOR c;

	p9 = al_malloc(sizeof(*p9));
	p9->bmp = bmp;
	p9->destroy_bmp = owns_bitmap;
	p9->h.m = NULL;
	p9->v.m = NULL;
	p9->cached_dw = 0;
	p9->cached_dh = 0;
	p9->mutex = al_create_mutex();
	p9->width = al_get_bitmap_width(bmp) - 2;
	p9->height = al_get_bitmap_height(bmp) - 2;
	p9->scale = 1.0;

	al_lock_bitmap(bmp, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);

	if (p9->width <= 0 || p9->height <= 0)
		goto bad_bitmap;

	/* make sure all four corners are transparent */
#define _check_pixel(x, y) \
	c = al_get_pixel(bmp, x, y); \
  if (c.a != 0 && c.r + c.g + c.b + c.a != 4) goto bad_bitmap;

  _check_pixel(0,0);
  _check_pixel(al_get_bitmap_width(bmp) - 1, 0);
	_check_pixel(0, al_get_bitmap_height(bmp) - 1);
	_check_pixel(al_get_bitmap_width(bmp) - 1, al_get_bitmap_height(bmp) - 1);
#undef _check_pixel

	p9->padding.top = p9->padding.right = p9->padding.bottom = p9->padding.left = -1;

	i = 1;
	while (i < al_get_bitmap_width(bmp))
	{
		c = al_get_pixel(bmp, i, al_get_bitmap_height(bmp) - 1);

		if (c.r + c.g + c.b == 0 && c.a == 1)
		{
			if (p9->padding.left == -1)
				p9->padding.left = i - 1;
			else if (p9->padding.right != -1)
				goto bad_bitmap;
		}
		else if (c.a == 0 || c.r + c.g + c.b + c.a == 4)
		{
			if (p9->padding.left != -1 && p9->padding.right == -1)
				p9->padding.right = al_get_bitmap_width(bmp) - i - 1;
		}
		++i;
	}

	i = 1;
	while (i < al_get_bitmap_height(bmp))
	{
		c = al_get_pixel(bmp, al_get_bitmap_width(bmp) - 1, i);

		if (c.r + c.g + c.b == 0 && c.a == 1)
		{
			if (p9->padding.top == -1)
				p9->padding.top = i - 1;
			else if (p9->padding.bottom != -1)
				goto bad_bitmap;
		}
		else if (c.a == 0 || c.r + c.g + c.b + c.a == 4)
		{
			if (p9->padding.top != -1 && p9->padding.bottom == -1)
				p9->padding.bottom = al_get_bitmap_height(bmp) - i - 1;
		}
		++i;
	}

	if (!init_nine_patch_side(&p9->h, bmp, 0) || !init_nine_patch_side(&p9->v, bmp, 1))
	{
bad_bitmap:
		al_destroy_mutex(p9->mutex);
		if (p9->h.m) al_free(p9->h.m);
		if (p9->v.m) al_free(p9->v.m);
		al_free(p9);
		p9 = NULL;
	}

	al_unlock_bitmap(bmp);
	return p9;
}

void calc_nine_patch_offsets(NINE_PATCH_SIDE *ps, int len, float scale)
{
	int i, j;
	int dest_offset = 0;
	int slen = len * scale;
	int remaining_stretch = slen - ps->fix;

	for (i = 0, j = 0; i < ps->count; ++i)
	{
		ps->m[i].dest_offset = dest_offset;
		if (ps->m[i].ratio == 0)
		{
			ps->m[i].dest_length = ps->m[i].length;
		}
		else
		{
			ps->m[i].dest_length = (slen - ps->fix) * ps->m[i].ratio;
			remaining_stretch -= ps->m[i].dest_length;
			j = i;
		}

		dest_offset += ps->m[i].dest_length;
	}

	if (remaining_stretch)
	{
		ps->m[j].dest_length += remaining_stretch;
		if (j + 1 < ps->count)
			ps->m[j + 1].dest_offset += remaining_stretch;
	}

	for(i = 0; i < ps->count; i++)
	{
		ps->m[i].dest_offset /= scale;
		ps->m[i].dest_length /= scale;
	}
}

void draw_nine_patch_bitmap(NINE_PATCH_BITMAP *p9, ALLEGRO_COLOR tint, int dx, int dy, int dw, int dh)
{
	int i, j;
	bool release_drawing = false;

	/* don't draw bitmaps that are smaller than the fixed area */
	if (dw < p9->h.fix || dh < p9->v.fix) return;

	/* if the bitmap is the same size as the origin, then draw it as-is */
	if (dw == p9->width && dh == p9->height)
	{
		al_draw_tinted_bitmap_region(p9->bmp, tint, 1, 1, dw, dh, dx, dy, 0);
		return;
	}

	/* due to the caching mechanism, multiple threads cannot draw this image at the same time */
	al_lock_mutex(p9->mutex);

	/* only recalculate the offsets if they have changed since the last draw */
	if (p9->cached_dw != dw || p9->cached_dh != dh)
	{
		calc_nine_patch_offsets(&p9->h, dw, p9->scale);
		calc_nine_patch_offsets(&p9->v, dh, p9->scale);

		p9->cached_dw = dw;
		p9->cached_dh = dh;
	}

	if (!al_is_bitmap_drawing_held())
	{
		release_drawing = true;
		al_hold_bitmap_drawing(true);
	}

	/* draw each region */
	for (i = 0; i < p9->v.count; ++i)
	{
		for (j = 0; j < p9->h.count; ++j)
		{
			al_draw_tinted_scaled_bitmap(p9->bmp, tint,
				p9->h.m[j].offset, p9->v.m[i].offset,
				p9->h.m[j].length, p9->v.m[i].length,
				dx + p9->h.m[j].dest_offset, dy + p9->v.m[i].dest_offset,
				p9->h.m[j].dest_length, p9->v.m[i].dest_length,
				0
			);
		}
	}

	al_unlock_mutex(p9->mutex);

	if (release_drawing)
		al_hold_bitmap_drawing(false);
}

ALLEGRO_BITMAP *create_bitmap_from_nine_patch(NINE_PATCH_BITMAP *p9, int w, int h)
{
	ALLEGRO_BITMAP *bmp = al_create_bitmap(w, h);
	ALLEGRO_STATE s;

	if (!bmp) return NULL;

	al_store_state(&s, ALLEGRO_STATE_TARGET_BITMAP);
	al_set_target_bitmap(bmp);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	draw_nine_patch_bitmap(p9, al_map_rgba_f(1.0, 1.0, 1.0, 1.0), 0, 0, w, h);
	al_restore_state(&s);

	return bmp;
}

NINE_PATCH_BITMAP *load_nine_patch_bitmap(const char *filename)
{
	ALLEGRO_BITMAP *bmp = al_load_bitmap(filename);

	return bmp ? create_nine_patch_bitmap(bmp, true) : NULL;
}

int get_nine_patch_bitmap_width(const NINE_PATCH_BITMAP *p9)
{
	return p9->width;
}

int get_nine_patch_bitmap_height(const NINE_PATCH_BITMAP *p9)
{
	return p9->height;
}

int get_nine_patch_bitmap_min_width(const NINE_PATCH_BITMAP *p9)
{
	return p9->h.fix;
}

int get_nine_patch_bitmap_min_height(const NINE_PATCH_BITMAP *p9)
{
	return p9->v.fix;
}

ALLEGRO_BITMAP *get_nine_patch_bitmap_source(const NINE_PATCH_BITMAP *p9)
{
	return p9->bmp;
}

NINE_PATCH_PADDING get_nine_patch_padding(const NINE_PATCH_BITMAP *p9)
{
	return p9->padding;
}

void destroy_nine_patch_bitmap(NINE_PATCH_BITMAP *p9)
{
	if (p9->destroy_bmp) al_destroy_bitmap(p9->bmp);
	al_destroy_mutex(p9->mutex);
	al_free(p9->h.m);
	al_free(p9->v.m);
	al_free(p9);
}

static bool _np_bitmap_resource_handler_proc(void ** ptr, ALLEGRO_FILE * fp, const char * filename, int option, int flags, unsigned long offset, bool destroy)
{
	NINE_PATCH_BITMAP * bitmap = (NINE_PATCH_BITMAP *)*ptr;
	ALLEGRO_STATE old_state;
	bool openfp = false; // operating on already open file

	if(destroy)
	{
		destroy_nine_patch_bitmap(bitmap);
		return true;
	}

	al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS);
	al_set_new_bitmap_flags(al_get_new_bitmap_flags() | ALLEGRO_NO_PRESERVE_TEXTURE);
	if(fp)
	{
		openfp = true;
	}
	if(!openfp && offset == 0)
	{
		bitmap = load_nine_patch_bitmap(filename);
	}
	else
	{
	}
	*ptr = bitmap;
	al_restore_state(&old_state);
	return *ptr;
}

static const char * _get_config_value_fallback(ALLEGRO_CONFIG * cp, const char * section, const char * fallback_section, const char * key)
{
	const char * val;

	val = al_get_config_value(cp, section, key);
	if(!val)
	{
		val = al_get_config_value(cp, fallback_section, key);
	}
	return val;
}

NINE_PATCH_BITMAP * np_load_nine_patch_bitmap(NINE_PATCH_BITMAP ** npbp, const char * fn)
{
	ALLEGRO_CONFIG * cp = NULL;
	ALLEGRO_PATH * path = NULL;
	char * loading_fn = NULL;
  char * loading_section = NULL;
	char * base_fn = NULL;
	const char * val;

	loading_fn = strdup(fn);
	if(!loading_fn)
	{
		goto fail;
	}
	base_fn = strtok(loading_fn, "#");
	loading_section = strtok(NULL, "#");

	if(strstr(fn, ".ini"))
	{
		cp = al_load_config_file(base_fn);
		if(!cp)
		{
			goto fail;
		}

		/* get the source image filename from 'section' */
		val = _get_config_value_fallback(cp, loading_section, "Nine Patch Bitmap", "filename");
		if(!val)
		{
			goto fail;
		}

		/* populate 'loading_path' */
		path = al_create_path(base_fn);
		if(!path)
		{
			goto fail;
		}
		al_set_path_filename(path, val);
		t3f_load_resource((void **)npbp, _np_bitmap_resource_handler_proc, al_path_cstr(path, '/'), 0, 0, 0);

		val = _get_config_value_fallback(cp, loading_section, "Nine Patch Bitmap", "scale");
		if(val)
		{
			(*npbp)->scale = atof(val);
		}

		al_destroy_config(cp);
		cp = NULL;
	}
	else
	{
		t3f_load_resource((void **)npbp, _np_bitmap_resource_handler_proc, base_fn, 0, 0, 0);
	}
	return *npbp;

	fail:
	{
		if(path)
		{
			al_destroy_path(path);
		}
		if(cp)
		{
			al_destroy_config(cp);
		}
		if(loading_fn)
		{
			free(loading_fn);
		}
		return NULL;
	}
}

void np_destroy_nine_patch_bitmap(NINE_PATCH_BITMAP * npbp)
{
	if(!t3f_destroy_resource(npbp))
	{
		destroy_nine_patch_bitmap(npbp);
	}
}
