#include "simple_logger.h"

#include "m_main.h"

Menu *main_new(int buttonMax)
{
	Menu *mainMenu;

	mainMenu = menu_new();

	mainMenu->position = vector2d(0, 0);
	gfc_rect_set(mainMenu->windowSize, mainMenu->position.x, mainMenu->position.y, 1920, 1080);


	mainMenu->buttonMax = buttonMax;
	mainMenu->buttonList = (Button *)gfc_allocate_array(sizeof (Button), mainMenu->buttonMax);
	mainMenu->sprite = gf2d_sprite_load_image("assets/sprites/menus/mainMenu.png");

	mainMenu->tag = "main";

	mainMenu->data = "menu";
	mainMenu->_active = true;

	main_create_buttons(mainMenu);

	return mainMenu;
}

void main_create_buttons(Menu *mainMenu)
{
	main_new_game(mainMenu);
	main_exit(mainMenu);
	main_editor(mainMenu);
}

void main_new_game(Menu *mainMenu)
{
	int newIndex;
	newIndex = menu_button_new(mainMenu);


	if (newIndex == -1)
	{
		slog("New Game Button Not Created");
	}


	Button *button = mainMenu->buttonList[newIndex];

	button->position.x = 772;
	button->position.y = 100;
	gfc_rect_set(button->buttonSize, button->position.x, button->position.y, 376, 192);
	button->sprite = gf2d_sprite_load_image("assets/sprites/menus/mainNewGame.png");


	button->buttonTag = "new_game";
	button->onPress = start_game;
}

void main_exit(Menu *mainMenu)
{
	int exitIndex;
	exitIndex = menu_button_new(mainMenu);


	if (exitIndex == -1)
	{
		slog("Exit Button Not Created");
	}


	Button *button = mainMenu->buttonList[exitIndex];

	button->position.x = 772;
	button->position.y = 300;
	gfc_rect_set(button->buttonSize, button->position.x, button->position.y, 376, 192);
	button->sprite = gf2d_sprite_load_image("assets/sprites/menus/mainMenuExit.png");


	button->buttonTag = "main_exit";
	button->onPress = close_game;
}

void main_editor(Menu *mainMenu)
{
	int editorIndex;
	editorIndex = menu_button_new(mainMenu);


	if (editorIndex == -1)
	{
		slog("Editor Button not created");
	}


	Button *button = mainMenu->buttonList[editorIndex];

	button->position.x = 772;
	button->position.y = 500;
	gfc_rect_set(button->buttonSize, button->position.x, button->position.y, 376, 192);
	button->sprite = gf2d_sprite_load_image("assets/sprites/menus/mainEditor.png");


	button->buttonTag = "main_editor";
	button->onPress = start_editor;
}

void start_game(Button* button, Menu *mainMenu)
{
	mainMenu->data = "start";
}

void start_editor(Button* button, Menu *mainMenu)
{
	mainMenu->data = "editor";
}

void close_game(Button* button, Menu *mainMenu)
{
	exit(0);
}

char *main_get_data(Menu *mainMenu)
{
	return mainMenu->data;
}



