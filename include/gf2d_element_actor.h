#ifndef __GF2D_ACTOR_ELEMENT_H__
#define __GF2D_ACTOR_ELEMENT_H__

#include "gf2d_elements.h"
#include "gf2d_actor.h"

typedef struct
{
    Actor actor;
    Vector2D scale;
    Sprite *image;
}ActorElement;

ActorElement *gf2d_element_actor_new();

ActorElement *gf2d_element_actor_new_full(char *actorFile, char *action,Vector2D scale,const char *image);

/**
 * @brief get a pointer to the actor data in this element
 * @param e the element to get an actor from
 * @return NULL if it is not set or an error, the actor otherwise
 */
Actor *gf2d_element_actor_get_actor(Element *e);

/**
 * @brief set the actor for a given actor element
 * @param e the element to set the actor for
 * @param actorFile the file to set it to
 */
void gf2d_element_actor_set_actor(Element *e, char *actorFile);

/**
 * @brief set the action for a given actor element
 * @param e the element to set the action for
 * @param action the action to set the actor to
 */
void gf2d_element_actor_set_action(Element *e, char *action);

/**
 * @brief scale the actor to fit the element bounds
 * @param e the element to scale
 */
void gf2d_element_actor_auto_scale(Element *e);

/**
 * @brief set the action to the next action for the actor element
 * @param e the element to set the action for
 */
void gf2d_element_actor_next_action(Element *e);

/**
 * @brief get the name of the current action for the actor
 * @param e the element to set the action name for
 * @return NULL on error or not found, the name of the actor action otherwise
 */
const char *gf2d_element_actor_get_action_name(Element *e);

/**
 * @brief set an element to be the actor provided
 * @param e the element to set
 * @param actor the actor to set it too
 */
void gf2d_element_make_actor(Element *e,ActorElement *actor);

/**
 * @brief load actor configuration for an actor element from config
 * @param e the element to configure
 * @param json the json config to use
 */
void gf2d_element_load_actor_from_config(Element *e,SJson *json);

#endif
