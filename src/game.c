#include <SDL.h>

#include <math.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "font.h"
#include "camera.h"
#include "entity.h"
#include "player.h"
#include "level.h"
#include "random.h"
#include "globals.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;

	Bool gameOver = false;

	Level *level;
	Font *font;
	TextLine fps_text, health_text, game_over_text, kills_text, hs_text;
	Entity *player;

    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);

	camera_set_dimensions(vector2d(1200, 720));
	camera_set_position(vector2d(0, 0));

	gf2d_sprite_init(1024);
	font_init(10);
	globals_init();

	entity_manager_init(100);
	projectile_manager_init(100);
	level_manager_init(64);
	
    SDL_ShowCursor(SDL_DISABLE);


	init_random();

    /*demo setup*/
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	level = level_hub();
	//level = level_random(16, 16);

	player = player_spawn(vector2d(32, 448));

	font = font_load("assets/fonts/DotGothic16-Regular.ttf", 24);

	

    /*main game loop*/
    while(!done)
    {
		
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;



		entity_manager_think_entities();
		entity_manager_check_collions();
		entity_manager_update_entities();

		projectile_manager_check_collisions();
		projectile_manager_update_projectiles();
		

		level = level_manager_get_current();

		level_update(level);

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first

		level_draw(level);

			entity_manager_draw_entities();
			projectile_manager_draw_projectiles();

            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);

		gfc_line_sprintf(fps_text, "FPS:%f", gf2d_graphics_get_frames_per_second());
		font_render(font, fps_text, vector2d(32, 32), gfc_color8(0, 0, 0, 255));

		gfc_line_sprintf(health_text, "Health:%i", player->health);
		font_render(font, health_text, vector2d(32, 64), gfc_color8(0, 0, 0, 255));

		gfc_line_sprintf(kills_text, "Kills:%i", totalKills);
		font_render(font, kills_text, vector2d(32, 96), gfc_color8(0, 0, 0, 255));


		if (entity_manager_get_player_ent()->health <= 0)
		{
			gameOver = true;
		}

		if (gameOver)
		{
			gfc_line_sprintf(game_over_text, "GAME OVER");
			font_render(font, game_over_text, vector2d(500, 250), gfc_color8(0, 0, 0, 255));

			if (totalKills >= highScoreList[0])
			{
				gfc_line_sprintf(hs_text, "High Scores:1. %i, 2. %i, 3 %i", totalKills, highScoreList[0], highScoreList[1]);
			}
			else if (totalKills >= highScoreList[1])
			{
				gfc_line_sprintf(hs_text, "High Scores:1. %i, 2. %i, 3 %i", highScoreList[0], totalKills, highScoreList[1]);
			}
			else if (totalKills >= highScoreList[2])
			{
				gfc_line_sprintf(hs_text, "High Scores:1. %i, 2. %i, 3 %i", highScoreList[0], highScoreList[1], totalKills);
			}
			else
			{
				gfc_line_sprintf(hs_text, "High Scores:1. %i, 2. %i, 3 %i", highScoreList[0], highScoreList[1], highScoreList[2]);
			}

			
			font_render(font, hs_text, vector2d(400, 350), gfc_color8(0, 0, 0, 255));
		}


        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }

    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
