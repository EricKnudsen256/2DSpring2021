#ifndef __MENU_OBJECT_H__
#define __MENU_OBJECT_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"



typedef struct Button_s
{
	Bool        _inuse;
	Bool		_active;
	Vector2D	position;
	Vector2D	sprite2Position;	//position as offset from main position of button
	SDL_Rect	buttonSize;
	Sprite		*sprite;
	Sprite		*sprite2;			//used for things like images over buttons, like for the inventory
	char		*buttonTag;
	void(*onPress)(struct Button_s *self);

}Button;


Button *button_new();

void button_check_click(Button *button);

void button_free(Button *button);

void button_draw(Button *button);

#endif