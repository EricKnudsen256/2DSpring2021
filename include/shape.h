#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "gfc_types.h"
#include "gfc_vector.h"

typedef struct
{
	float x, y, w, h;
}ShapeRect;

typedef struct
{
	float x, y, r;
}ShapeCircle;

typedef Vector2D ShapePoint;


ShapeRect shape_rect_from_sdl_rect(SDL_Rect r);
ShapeRect shape_rect_from_vector4d(Vector4D v);
SDL_Rect shape_rect_to_sdl_rect(ShapeRect r);

/**
* @brief check if the two circles have overlap
* @param a the first circle to check
* @param b the second circle to check
* @return true if there is overlap, false if there is not
*/
Bool shape_circle_collision(ShapeCircle a, ShapeCircle b);

/**
* @brief check if a given point is inside a given circle
* @param point the point in question
* @param c the circle in question
* @return true if there is overlap, false if there is not
*/
Bool shape_point_in_circle(ShapePoint point, ShapeCircle c);

/**
* @brief check if a given point is inside a given rectangle
* @param point the point in question
* @param rect the rectangle in question
* @return true if there is overlap, false if there is not
*/
Bool shape_point_in_rect(ShapePoint point, ShapeRect rect);


/**
* @brief check if the two rectangles have overlap
* @param a the first rectangle to check
* @param b the second rectangle to check
* @return true if there is overlap, false if there is not
*/
Bool shape_rect_collision(ShapeRect a, ShapeRect b);


#endif