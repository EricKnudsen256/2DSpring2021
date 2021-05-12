#ifndef __INPUT_H__
#define __INPUT_H__

/**
* @brief called every frame to update game inputs
*/
void input_update();

/**
* @brief returns if the left mouse was pressed this frame
* @return true if left click pressed, false otherwise
*/
Bool input_get_mouse_down();

#endif