#include "simple_logger.h"

#include "g_mouse.h"
#include "g_camera.h"


static Mouse mouse = { 0 };

void mouse_init()
{

	mouse.sprite = gf2d_sprite_load_image("assets/sprites/mouse.png");

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

	if (mouse.sprite2 != NULL)
	{
		drawPosition = vector2d(mouse.mousePos.x - 32, mouse.mousePos.y - 32);

		gf2d_sprite_draw(
			mouse.sprite2,
			drawPosition,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			0);
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


	//slog("Mouse x:%f, y:%f", mouse_get_world_position().x, mouse_get_world_position().y);
}

void mouse_set_default_sprite2()
{
	mouse.sprite = NULL;
}

void mouse_set_sprite2(Sprite *sprite)
{
	mouse.sprite2 = sprite;
}

Mouse mouse_get_mouse()
{
	return mouse;
}

Vector2D mouse_get_position()
{
	return mouse.mousePos;
}

Vector2D mouse_get_world_position()
{
	Vector2D offset, worldPos, cameraScale, mouseOffset;

	offset = camera_get_offset();
	cameraScale = camera_get_scale();

	worldPos.x = (mouse.mousePos.x / cameraScale.x - offset.x);
	worldPos.y = (mouse.mousePos.y / cameraScale.y - offset.y);

	return worldPos;
}