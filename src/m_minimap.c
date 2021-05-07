#include "simple_logger.h"

#include "e_entity.h"

#include "m_minimap.h"

#include "w_room.h"

Menu *minimap_menu_new(int buttonMax)
{
	Menu *minimap;

	minimap = menu_new();



	minimap->position = vector2d(1560, 60);
	gfc_rect_set(minimap->windowSize, minimap->position.x, minimap->position.y, 320, 320);


	minimap->sprite = gf2d_sprite_load_image("assets/sprites/menus/minimap.png");

	minimap->buttonMax = buttonMax;
	minimap->buttonList = (Button *)gfc_allocate_array(sizeof (Button), minimap->buttonMax);
	minimap->draw = minimap_draw;

	minimap->tag = "minimap";

	minimap->data = false;

	minimap->_active = 1;

	return minimap;
}

void *minimap_menu_set_active(Button* button, Menu *minimap)
{
	minimap->_active = 1;
}

void *minimap_menu_set_inactive(Button* button, Menu *minimap)
{
	minimap->_active = 0;
}

void *minimap_draw(Menu *minimap)
{
	Room **roomList = room_manager_get_room_list();
	Room *room;
	Tile *tile;
	SDL_Rect tempDraw;
	Entity *player;
	int drawX, drawY;

	player = entity_manager_get_player_ent();

	drawX = player->position.x / 32;
	drawY = player->position.y / 32;

	tempDraw = gfc_sdl_rect(drawX + minimap->position.x + 9 - 2, drawY + minimap->position.y + 9 - 2, 4, 4);

	SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 40, 175, 40, 255);
	SDL_RenderFillRect(gf2d_graphics_get_renderer(), &tempDraw);

	for (int x = 0; x < room_manager_get_max_columns(); x++)
	{
		for (int y = 0; y < room_manager_get_max_rows(); y++)
		{
			room = room_manager_get_room(x, y);

			if (room)
			{
				for (int i = 0; i < room->roomHeight; i++)
				{
					for (int j = 0; j < room->roomWidth; j++)
					{
						if (room->tileArray[i][j])
						{
							tile = room->tileArray[i][j];

							if (tile->_inuse == true)
							{
								drawX = tile->pos.x / 32;
								drawY = tile->pos.y / 32;

								SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 255, 255, 255, 255);
								SDL_RenderDrawPoint(gf2d_graphics_get_renderer(), drawX + minimap->position.x + 8, drawY + minimap->position.y + 8);
							}
						}
					}
				}
			}
		}
	}

}