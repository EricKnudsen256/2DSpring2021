#ifndef __INTERACAIBLE_H__
#define __INTERACAIBLE_H__


typedef struct Interactable_s
{
	Bool		_inuse;

	Bool		playerInside;
	SDL_Rect	intersectAmount;

	int			id;

	Vector2D	pos;
	Vector2D	gridPos;
	SDL_Rect	interactionBox;

	void		*parent;

	void(*onInteract)(struct Interactable *interactible);

	

}Interactable;


Interactable *interactable_new(int width, int height, Vector2D *position, Vector2D *gridPos);


void interactable_update(Interactable *interact);


void interactable_draw(Interactable *interact);


void interactable_free(Interactable *interact);



#endif