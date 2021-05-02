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

#include "p_player.h"

#include "w_level.h"

static int _done = 0;


int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;

	Bool gameOver = false;

	Level *level;
	TextLine fps_text, health_text, game_over_text, kills_text, hs_text;
	Entity *player;
	Menu *pauseMenu, *inventoryMenu;

    

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1920,
        1080,
		1920,
		1080,
        vector4d(0,0,0,255),
        0,
		false);
    gf2d_graphics_set_frame_delay(16);

	camera_set_dimensions(vector2d(1920, 1080));
	camera_set_position(vector2d(0, 0));

	gf2d_sprite_init(1024);

	//my font system, going to delete eventually
	font_init(10);

	//dj's font system, will move to this

	gf2d_font_init("config/font.cfg");


	entity_manager_init(100);
	projectile_manager_init(100);
	level_manager_init(64);
	player_inventory_init(32);
	menu_manager_init(32);

	//gf2d_windows_init(10);

	mouse_init();

	
    SDL_ShowCursor(SDL_DISABLE);


	//init_random();

    /*demo setup*/
	level = level_hub();
	//level = level_random(16, 16);

	player = player_spawn(vector2d(32, 448));

	

	
	player_inventory_add_item(item_new("testItem", 1, "assets/sprites/items/testItem.png"));
	slog("maxItems: %i", player_inventory_get_max());

	player_inventory_slog();

	//gf2d_window_load("config/testwindow.cfg");


	pauseMenu = pause_menu_new(10);
	inventoryMenu = inventory_new(50);


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

			level = level_manager_get_current();

			level_update(level);
		}



		menu_manager_think_menus();
		menu_manager_update_menus();

		gf2d_mouse_update();
		mouse_update();


		gf2d_graphics_clear_screen();// clears drawing buffers
			// all drawing should happen betweem clear_screen and next_frame
			//backgrounds drawn first

		level_draw(level);

		entity_manager_draw_entities();
		projectile_manager_draw_projectiles();

		menu_manager_draw_menus();
	


		mouse_draw();
		

		gf2d_grahics_next_frame();	// render current draw frame and skip to the next frame



    }


    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
