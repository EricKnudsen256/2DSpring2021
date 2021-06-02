#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "gf2d_sprite.h"

typedef struct Mouse_s
{
	Vector2D mousePos;
	Sprite *sprite;
	Sprite *sprite2;

}Mouse;

/**
* @brief starts mouse system
*/
void mouse_init();

/**
* @brief frees mouse system
*/
void mouse_free();

/**
* @brief draws mouse
*/
void mouse_draw();

/**
* @brief updates position and values of mouse
*/
void mouse_update();

/**
* @brief sets mouse sprite back to default sprite
*/
void mouse_set_default_sprite2();

/**
* @brief not in use, but sets sprite of the mouse
*/
void mouse_set_sprite2();

/**
* @brief returns the mouse object
* @return the mouse
*/
Mouse mouse_get_mouse();

/**
* @brief gets the position of the mouse on the screen
* @return the poision of the mouse
*/
Vector2D mouse_get_position();

Vector2D mouse_get_world_position();


#endif