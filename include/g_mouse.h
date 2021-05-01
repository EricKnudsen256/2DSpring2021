#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "gf2d_sprite.h"

typedef struct Mouse_s
{
	Vector2D mousePos;
	Sprite *sprite;

}Mouse;

void mouse_init();

void mouse_free();

void mouse_draw();

void mouse_update();

void mouse_set_default_sprite();

void mouse_set_sprite();

Mouse mouse_get_mouse();

Vector2D mouse_get_position();


#endif