#include "simple_logger.h"

#include "g_mouse.h"

static Mouse mouse = { 0 };

void mouse_init()
{

	mouse_set_default_sprite();

	atexit(mouse_free);
	slog("mouse initialized");
}

void mouse_free()
{

	gf2d_sprite_free(mouse.sprite);

	mouse.sprite = NULL;

}

void mouse_draw()
{
	Vector2D drawPosition;


	if (mouse.sprite == NULL)
	{
		return;
	}


	//currently not drawing animations, edit this if working on that in the future
	gf2d_sprite_draw(
		mouse.sprite,
		mouse.mousePos,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		0);

}

void mouse_update()
{
	int mx, my;

	SDL_GetMouseState(&mx, &my);

	mouse.mousePos = vector2d(mx, my);
}

void mouse_set_default_sprite()
{
	mouse.sprite = gf2d_sprite_load_image("assets/sprites/mouse.png");
}

void mouse_set_sprite(Sprite *sprite)
{
	mouse.sprite = sprite;
}

Mouse mouse_get_mouse()
{
	return mouse;
}

Vector2D mouse_get_position()
{
	return mouse.mousePos;
}