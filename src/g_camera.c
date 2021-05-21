#include "gf2d_graphics.h"

#include "shape.h"
#include "g_camera.h"

static SDL_Rect Camera = { 0 };
static Vector2D CameraScale = { 0 };

void camera_set_dimensions(Vector2D size)
{
	CameraScale = vector2d(1, 1);

	Camera.w = size.x;
	Camera.h = size.y;
}

Vector2D camera_get_dimensions()
{
	Vector2D size;
	size.x = Camera.w;
	size.y = Camera.h;
	return size;
}


void camera_set_position(Vector2D position) 
{
	Camera.x = (int)position.x;
	Camera.y = (int)position.y;
}

void camera_center_on_position(Vector2D position)
{
	Camera.x = ((int)position.x - Camera.w * .5 / CameraScale.x);
	Camera.y = ((int)position.y - Camera.h * .5 / CameraScale.y);
}

void camera_move(Vector2D move)
{
	Camera.x += move.x;
	Camera.y += move.y;
}


Bool camera_point_on_screen(Vector2D point)
{
	return shape_point_in_rect(point, shape_rect_from_sdl_rect(Camera));
}

Bool camera_rect_on_screen(SDL_Rect rect)
{
	return shape_rect_collision(shape_rect_from_sdl_rect(rect), shape_rect_from_sdl_rect(Camera));
}

SDL_Rect camera_get_rect()
{
	return Camera;
}

Vector2D camera_get_position()
{
	Vector2D position;
	position.x = Camera.x;
	position.y = Camera.y;
	return position;
}


Vector2D camera_get_offset()
{
	Vector2D position;
	position.x = -Camera.x;
	position.y = -Camera.y;
	return position;
}

Vector2D camera_get_scale()
{
	return CameraScale;
}

Vector2D camera_set_scale(Vector2D scale)
{
	CameraScale = scale;
	//SDL_RenderSetScale(gf2d_graphics_get_renderer(), scale.x, scale.y);
}

void camera_zoom_in()
{
	Vector2D currentScale;
	Vector2D newScale;

	currentScale = CameraScale;

	if (currentScale.x >= 1 || currentScale.y >= 1)
	{
		currentScale.x = 1;
		currentScale.y = 1;
		return;
	}

	newScale = vector2d(currentScale.x + 0.03125, currentScale.y + 0.03125);
	camera_set_scale(newScale);
}

void camera_zoom_out()
{
	Vector2D currentScale;
	Vector2D newScale;

	currentScale = CameraScale;

	if (currentScale.x <= .25 || currentScale.y <= .25)
	{
		currentScale.x = .25;
		currentScale.y = .25;
		return;
	}

	newScale = vector2d(currentScale.x - 0.03125, currentScale.y - 0.03125);
	camera_set_scale(newScale);
}


/*eol@eof*/