#include <SDL.h>

#include <math.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_windows.h"
#include "gf2d_font.h"
#include "gf2d_actor.h"
#include "gf2d_windows.h"

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
	SDL_ShowCursor(SDL_DISABLE);

	init_random();


}

void init_game()
{
	entity_manager_init(100);
	projectile_manager_init(100);
	level_manager_init(64);
	player_inventory_init(32);
	gfc_audio_init(32, 6, 4, 8, true, false);

	room_manager_init(8, 8, 64);

	//room_test();
}

void game_main_menu()
{
	Menu *mainMenu;

	mainMenu = main_new(10);

	while (true)
	{

		//slog("%i", check_start_game(mainMenu));

		
		if (check_start_game(mainMenu))
		{
			mainMenu->_active = false;
			return;
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

void game_main()
{
	/*variable declarations*/
	int done = 0;
	const Uint8 * keys;

	Bool gameOver = false;

	TextLine fps_text, health_text, game_over_text, kills_text, hs_text;
	Entity *player;
	Menu *pauseMenu, *inventoryMenu;
	Sound *bgMusic;
	Sprite *bg;

	Vector2D room1Pos = vector2d(0, 0);

	room_init_all();

	player = player_spawn(vector2d(100, 100));




	player_inventory_add_item(item_new("testItem", 1, "assets/sprites/items/testItem.png"));
	//slog("maxItems: %i", player_inventory_get_max());

	//player_inventory_slog();

	//gf2d_window_load("config/testwindow.cfg");


	pauseMenu = pause_menu_new(10);
	inventoryMenu = inventory_new(50);

	bgMusic = gfc_sound_load("assets/audio/UFO Gang.wav", 1, 1);


	gfc_sound_play(bgMusic, -1, .01, -1, -1);

	bg = gf2d_sprite_load_image("assets/sprites/backgrounds/bg.png");


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

		menu_manager_draw_menus();

		mouse_draw();

		gf2d_grahics_next_frame();	// render current draw frame and skip to the next frame
	}


	slog("---==== END ====---");
	return 0;
}

int main(int argc, char * argv[])
{
	init_main_menu();
	game_main_menu();
	init_game();
	game_main();
}

/*eol@eof*/
