#include "simple_logger.h"

#include "g_item.h"
#include "g_font.h"
#include "g_mouse.h"
#include "w_building.h"

#include "m_build.h"


Menu *build_new(int buttonMax)
{
	Menu *buildMenu;

	buildMenu = menu_new();



	buildMenu->position = vector2d(510, 290);
	gfc_rect_set(buildMenu->windowSize, buildMenu->position.x, buildMenu->position.y, 900, 500);


	buildMenu->buttonMax = buttonMax;
	buildMenu->buttonList = (Button *)gfc_allocate_array(sizeof (Button), buildMenu->buttonMax);
	buildMenu->sprite = gf2d_sprite_load_image("assets/sprites/menus/buildMenu.png");

	buildMenu->think = build_think;
	buildMenu->draw = build_draw;


	buildMenu->tag = "build";

	build_create_slots(buildMenu);



	return buildMenu;
}


void build_create_slots(Menu *buildMenu)
{
	//buttons 0-29 are the different buildings

	for (int i = 0; i < 30; i++)
	{
		int testIndex;
		testIndex = menu_button_new(buildMenu);

		if (testIndex == -1)
		{
			slog("button not created");
			return NULL;
		}


		Button *button = buildMenu->buttonList[testIndex];

		button->position.x = buildMenu->position.x + 16 + 80 * (i % 5);
		button->position.y = buildMenu->position.y + 16 + (80 * (int)(i / 5));
		button->sprite2Position.x = button->position.x + 8;
		button->sprite2Position.y = button->position.y + 8;

		gfc_rect_set(button->buttonSize, button->position.x, button->position.y, 64, 64);

		button->buttonTag = "buildButton";
		button->sprite = gf2d_sprite_load_image("assets/sprites/menus/inventorySlot.png");
		button->onPress = build_choose_item;
	}

	//this is where I would have buildings load from file, would make the most sense, but for now I'll manually code them in
	//TODO, make global building list in w_building

	buildMenu->buttonList[0]->data = "testBuilding";
	buildMenu->buttonList[0]->sprite2 = gf2d_sprite_load_image("assets/sprites/buildings/testBuilding2x2.png");
}

void build_choose_item(Button *button, Menu *buildMenu)
{
	char *buildingName;

	buildingName = button->data;

	building_list_set_current_build(buildingName);

	build_set_inactive(NULL, buildMenu);
}


void build_set_active(Button *button, Menu *buildMenu)
{
	buildMenu->_active = 1;
}


void build_set_inactive(Button *button, Menu *buildMenu)
{
	buildMenu->_active = 0;
}


void build_think(Menu *buildMenu)
{

}


void build_button_select(Button *slot, Menu *buildMenu)
{

}


void build_deselect_all(Menu *buildMenu)
{

}


void build_draw(Menu *buildMenu)
{

}