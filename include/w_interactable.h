#ifndef __INTERACAIBLE_H__
#define __INTERACAIBLE_H__


typedef struct Interactable_s
{
	Bool		_inuse;
	Vector2D	pos;
	Vector2D	gridPos;
	SDL_Rect	interactionBox;

	void(*onInteract)(struct Interactable *interactible);

}Interactable;


Interactable *interactable_new(int width, int height, Vector2D position, Vector2D gridPos);



#endif