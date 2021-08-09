#ifndef __MENU_OBJECT_H__
#define __MENU_OBJECT_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"



typedef struct Button_s
{
	Bool        _inuse;
	Bool		_active;
	Bool		_selceted;

	int			_id;

	Vector2D	position;
	Vector2D	sprite2Position;	//position as offset from main position of button
	SDL_Rect	buttonSize;
	Sprite		*sprite;
	Sprite		*sprite2;			//used for things like images over buttons, like for the inventory
	TextLine	text;
	char		*buttonTag;
	void(*onPress)(struct Button_s *self);
	void		*data;

}Button;

/**
* @brief creates a new button for use in a menu
* @return pointer to the created button
*/
Button *button_new();

/**
* @brief called by menu to check if button is clicked, calls button's onclick fucntion if clicked
* @param button the button to check
*/
void button_check_click(Button *button);

/**
* @brief frees the button
* @param button the button to be freed
*/
void button_free(Button *button);

/**
* @brief draws the button
* @param button the button to draw
*/
void button_draw(Button *button);

#endif