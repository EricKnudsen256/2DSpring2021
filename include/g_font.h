#ifndef __FONT_H__
#define __FONT_H__

#include <SDL_ttf.h>

#include "gfc_vector.h"
#include "gfc_text.h"
#include "gfc_color.h"

typedef struct
{
	Uint32      _refCount;  /**<how many times this specific resource has been used*/
	TTF_Font   *font;       /**<the handle to the TTF_Font we will use*/
	TextLine    filename;   /**<filename of the font we loaded*/
	int         ptsize;     /**<the font size loaded.*/
}Font;

/**
* @brief initialize font resource manager
* @param maxFonts how many fonts to support
*/
void font_init(Uint32 maxFonts);

Font *font_load(const char *filename, int ptsize);
void font_free(Font * font);
void font_render(Font *font, char *text, Vector2D position, Color color);

#endif