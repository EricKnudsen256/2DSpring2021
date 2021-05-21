#include <SDL.h>

#include <math.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_windows.h"
#include "gf2d_font.h"
#include "gf2d_actor.h"
#include "gf2d_mouse.h"

#include "gfc_input.h"
#include "gfc_audio.h"


#include "e_entity.h"

#include "g_menu.h"
#include "g_camera.h"
#include "g_random.h"
#include "g_windows.h"
#include "g_projectile.h"
#include "g_input.h"
#include "g_mouse.h"

#include "m_pause.h"
#include "m_inventory.h"
#include "m_main.h"
#include "m_editor.h"

#include "p_player.h"

#include "w_level.h"
#include "w_room.h"

static int _done = 0;

void init_main_menu()
{

	/*program initializtion*/
	init_logger("gf2d.log");
	slog("---==== BEGIN ====---");
	gf2d_graphics_initialize(
		"gf2d",
		1920,
		1080,
		1920,
		1080,
		vector4d(0, 0, 0, 255),
		0,
		false);
	gf2d_graphics_set_frame_delay(16);

	camera_set_dimensions(vector2d(1920, 1080));
	camera_set_position(vector2d(0, 0));

	gf2d_sprite_init(1024);

	//my font system, going to delete eventually
	font_init(10);
	mouse_init();
	menu_manager_init(32);
	gfc_audio_init(32, 6, 4, 8, true, false);
	SDL_ShowCursor(SDL_DISABLE);

	init_random();


}

void init_game()
{
	entity_manager_init(300);
	projectile_manager_init(100);
	level_manager_init(64);
	player_inventory_init(32);

	room_manager_init(8, 8, 64, 100);
}

void close_main_game()
{
	entity_manager_free();
	projectile_manager_free();
	level_manager_free();
	player_inventory_free();

	room_manager_free();
}

void init_editor()
{
	room_manager_init(8, 8, 64, 100);
}

void close_editor()
{
	room_manager_free();
}

void game_main()
{
	/*variable declarations*/
	int done = 0;
	const Uint8 * keys;

	TextLine fps_text, health_text, game_over_text, kills_text, hs_text;
	Entity *player;
	Menu *pauseMenu, *inventoryMenu, *minimap;
	Sound *bgMusic;
	Sprite *bg;

	Font *font;

	font = font_load("assets/fonts/DotGothic16-Regular.ttf", 24);


	//json test files below
	//Vector2D roomPos = vector2d(0, 0);
	//Room *room = room_template_load(roomPos, "templates/testRoom.json");

	room_init_all();
	//room_slog();

	Vector2D spawnRoomPos = room_manager_get_start_pos();
	Vector2D spawnPos = vector2d(500, 500);

	vector2d_add(spawnPos, spawnPos, spawnRoomPos);

	player = player_spawn(spawnPos);

	slog("Spawning player at x:%f, y:%f", spawnPos.x, spawnPos.y);

	player_inventory_add_item(item_new("testItem", 1, "assets/sprites/items/testItem.png"));
	//slog("maxItems: %i", player_inventory_get_max());

	//player_inventory_slog();

	//gf2d_window_load("config/testwindow.cfg");


	pauseMenu = pause_menu_new(10);
	inventoryMenu = inventory_new(50);
	minimap = minimap_menu_new(1);

	bgMusic = gfc_sound_load("assets/audio/UFO Gang.wav", 1, 1);


	//gfc_sound_play(bgMusic, -1, .01, -1, -1);

	bg = gf2d_sprite_load_image("assets/sprites/backgrounds/grayBg.png");


	/*main game loop*/
	while (!pause_menu_check_end_game(pauseMenu))
	{
		//slog("Loop");

		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		/*update things here*/

		input_update();

		if (!pauseMenu->_active)
		{
			entity_manager_think_entities();
			entity_manager_check_collions();
			entity_manager_update_entities();

			projectile_manager_check_collisions();
			projectile_manager_update_projectiles();

			room_manager_update();

			slog("Camera pos: x:%f, y:%f", camera_get_position().x, camera_get_position().y);
			slog("Player pos: x:%f, y:%f", player->position.x, player->position.y);
		}


		menu_manager_think_menus();
		menu_manager_update_menus();

		gf2d_mouse_update();
		mouse_update();


		gf2d_graphics_clear_screen();// clears drawing buffers
		// all drawing should happen betweem clear_screen and next_frame
		//backgrounds drawn first

		gf2d_sprite_draw(bg, vector2d(0, 0), NULL, NULL, NULL, NULL, NULL, 0);

		room_manager_draw();

		entity_manager_draw_entities();
		projectile_manager_draw_projectiles();

		gfc_line_sprintf(fps_text, "FPS:%i", (int)gf2d_graphics_get_frames_per_second());
		font_render(font, fps_text, vector2d(32, 32), gfc_color8(255, 255, 255, 255));


		menu_manager_draw_menus();

		mouse_draw();

		gf2d_grahics_next_frame();	// render current draw frame and skip to the next frame
	}

	menu_free(pauseMenu);
	menu_free(inventoryMenu);
	menu_free(minimap);

	gfc_sound_clear_all();


	slog("---==== END ====---");
	return 0;
}

void editor_main()
{
	/*variable declarations*/
	const Uint8 * keys;
	int mx, my;
	int x, y;
	

	Menu *pauseMenu, *editorMenu;
	Sprite *bg;
	SDL_Rect drawPos;
	Vector2D offset;
	Vector2D mouseGamePos;
	Vector2D mouseGridPos;
	Vector2D tilePos;

	Vector2D spawnPos = vector2d(0, 0);
	Room *room = room_empty(spawnPos);

	room_open_door(true, true, true, true, room);

	Vector2D cameraPos = vector2d(-300, 100);
	camera_set_position(cameraPos);

	pauseMenu = pause_menu_new(10);
	editorMenu = editor_new(5);

	bg = gf2d_sprite_load_image("assets/sprites/backgrounds/bg.png");


	/*main game loop*/
	while (!pause_menu_check_end_game(pauseMenu))
	{

		offset = camera_get_offset();


		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		SDL_GetMouseState(&mx, &my);

		/*update things here*/

		if (!strcmp(editor_get_data(editorMenu), "clear"))
		{
			room_free(0, 0, room);

			room = room_empty(spawnPos);
			room_open_door(true, true, true, true, room);

			editor_clear_done(NULL, editorMenu);
		}

		input_update();

		if (!pauseMenu->_active)
		{
			room_manager_update();
		}



		menu_manager_think_menus();
		menu_manager_update_menus();

		mouseGamePos = vector2d(mx - offset.x, my - offset.y);
		mouseGridPos = vector2d((int)(mx - offset.x) / 32, (int)(my - offset.y) / 32);
		gf2d_mouse_update();
		mouse_update();

		//check to see if click in range of tiles, make on if possible

		if (input_get_mouse_down())
		{
			if (mouseGridPos.x > 3 && mouseGridPos.x < room->roomWidth - 4 && mouseGridPos.y > 3 && mouseGridPos.y < room->roomHeight - 4)
			{
				x = mouseGridPos.x;
				y = mouseGridPos.y;

				if (room->tileArray[x][y])
				{
					room_free_tile(x, y, room);
				}
				else
				{
					tilePos = vector2d(x * room->tileWidth, y * room->tileHeight);

					room_new_tile(room, tilePos, mouseGridPos);

					//slog("new tile at x:%f, y:%f", mouseGridPos.x, mouseGridPos.y);
				}
			}
		}
	



		gf2d_graphics_clear_screen();// clears drawing buffers
		// all drawing should happen betweem clear_screen and next_frame
		//backgrounds drawn first

		gf2d_sprite_draw(bg, vector2d(0, 0), NULL, NULL, NULL, NULL, NULL, 0);

		room_manager_draw();

		//draw rectangles to show where to place tile

		for (int x = 0; x < room->roomHeight; x++)
		{
			for (int y = 0; y < room->roomWidth; y++)
			{
				if (x < 4 || x > room->roomHeight - 5 || y < 4 || y > room->roomWidth - 5)
				{
					continue;
				}

				drawPos.x = 32 * x + offset.x;
				drawPos.y = 32 * y + offset.y;
				drawPos.h = room->tileHeight;
				drawPos.w = room->tileWidth;

				SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), 0, 0, 0, 255);

				SDL_RenderDrawRect(gf2d_graphics_get_renderer(), &drawPos);
			}
		}

		menu_manager_draw_menus();

		mouse_draw();

		

		

		gf2d_grahics_next_frame();	// render current draw frame and skip to the next frame
	}


	menu_free(pauseMenu);
	menu_free(editorMenu);

	slog("---==== END ====---");
	return 0;
}

void game_main_menu()
{
	Menu *mainMenu;

	mainMenu = main_new(10);

	while (true)
	{

		//slog("%i", check_start_game(mainMenu));

		
		if (main_get_data(mainMenu) != "menu")
		{
			mainMenu->_active = false;
			
			if (main_get_data(mainMenu) == "start")
			{
				init_game();
				game_main();
				close_main_game();

				mainMenu->_active = true;
				back_to_main(mainMenu);
			}
			else if (main_get_data(mainMenu) == "editor")
			{
				init_editor();
				editor_main();
				close_editor();

				mainMenu->_active = true;
				back_to_main(mainMenu);
			}
			else
			{
				slog("Menu data not found");
			}

		}
		

		
		//slog("Loop");

		SDL_PumpEvents();   // update SDL's internal event structures
		/*update things here*/

		input_update();

		menu_manager_think_menus();
		menu_manager_update_menus();

		gf2d_mouse_update();
		mouse_update();


		gf2d_graphics_clear_screen();// clears drawing buffers
		// all drawing should happen betweem clear_screen and next_frame
		//backgrounds drawn first

		menu_manager_draw_menus();

		mouse_draw();

		gf2d_grahics_next_frame();	// render current draw frame and skip to the next frame

	}
}


int main(int argc, char * argv[])
{
	init_main_menu();

	game_main_menu();

}

/*eol@eof*/
