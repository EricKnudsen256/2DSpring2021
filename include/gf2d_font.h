#ifndef __GF2D_FONT_H__
#define __GF2D_FONT_H__
/**
 * gf2d_font
 * @license The MIT License (MIT)
   @copyright Copyright (c) 2015 EngineerOfLies
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

/**
 * @purpose the GF2d text library provides common string support for fixed length
 *  strings.  For use with unlimited strings mgl will use glib's GString type
 */
#include <string.h>
#include <SDL_ttf.h>

#include "gfc_types.h"
#include "gfc_color.h"

#include "gf2d_shape.h"

typedef enum
{
    FT_Normal,
    FT_Small,
    FT_H1,
    FT_H2,
    FT_H3,
    FT_H4,
    FT_H5,
    FT_H6,
    FT_MAX
}FontTypes;


typedef struct
{
    TextLine filename;
    TTF_Font *font;
    Uint32  pointSize;
}Font;

/**
 * @brief initialized text drawing system
 * @param configFile the file to load font information from
 */
void gf2d_font_init(char *configFile);

void gf2d_font_draw_line_named(char *text,char *filename,Color color, Vector2D position);
void gf2d_font_draw_line_tag(char *text,FontTypes tag,Color color, Vector2D position);
void gf2d_font_draw_line(char *text,Font *font,Color color, Vector2D position);

/**
 * @brief draw a word wrapped block of text to the sceen
 * @param thetext the text to draw
 * @param block the dimensions to keep to
 * @param color the color to draw with
 * @param font the font to use, IF NULL this is a no-op
 */
void gf2d_font_draw_text_wrap(
    char    *thetext,
    Rect     block,
    Color    color,
    Font    *font
);

/**
 * @brief draw a word wrapped block of text to the sceen by font tag
 * @param text the text to draw
 * @param tag the font tag to use to draw
 * @param color the color to draw with
 * @param block the dimensions to keep to
 */
void gf2d_font_draw_text_wrap_tag(char *text,FontTypes tag,Color color, Rect block);

/**
 * @brief get the bounds that will describe the text provided, with word wrap on
 * @param thetext the text to check
 * @param font the font to use
 * @param w the width of the desired bounds - will be used for word wrapping
 * @param h the height of the desired bounds
 */
Rect gf2d_font_get_text_wrap_bounds(
    char    *thetext,
    Font    *font,
    Uint32   w,
    Uint32   h
);

/**
 * @brief the above but by tag
 */
Rect gf2d_font_get_text_wrap_bounds_tag(
    char       *thetext,
    FontTypes   tag,
    Uint32      w,
    Uint32      h
);


Vector2D gf2d_font_get_bounds_tag(char *text,FontTypes tag);
Vector2D gf2d_font_get_bounds(char *text,Font *font);

#endif
