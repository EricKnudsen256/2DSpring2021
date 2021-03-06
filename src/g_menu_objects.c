#include "simple_logger.h"

#include "g_menu_objects.h"



Button *button_new()
{
	Button *button;
	button = (Button *)malloc(sizeof(Button));
	if (!button)
	{
		slog("failed to allocate button!");
		return NULL;
	}
	memset(button, 0, sizeof(Button));

	button->_active = true;
	button->_inuse = true;

	return button;
}

void button_check_click(Button *button)
{

}

void button_free(Button *button)
{

}

void button_draw(Button *button)
{
	Vector2D drawPosition;
	if (!button)
	{
		slog("cannot draw a NULL button");
		return;
	}
	else
	{
		if (button->sprite == NULL)
		{

			SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 255, 0, 0, 255);

			SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &button->buttonSize);
			return;// nothing to draw
		}
		//currently not drawing animations, edit this if working on that in the future
		gf2d_sprite_draw(
			button->sprite,
			button->position,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			0);
	}

	

	if (button->sprite2 != NULL)
	{
		//slog("Draw Sprite");

		Vector4D colorShift;

		Vector2D drawPos = vector2d(button->position.x + button->sprite2Position.x, button->position.y + button->sprite2Position.y);

		if (button->_selceted)
		{
			colorShift = vector4d(255, 255, 255, 125);

			gf2d_sprite_draw(
				button->sprite2,
				button->position,
				NULL,
				NULL,
				NULL,
				NULL,
				&colorShift,
				0);
		}
		else
		{
			gf2d_sprite_draw(
				button->sprite2,
				button->position,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				0);
		}



	}


}