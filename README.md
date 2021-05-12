# 2D Factory Final

## Basics

In order to start the game, simply run the 2dGameFramework.exe

# Midterm Specific Deliverables 

## 2D Platforming Factory To Explore

The player spawns in a hub room with 3 doors, 3 the left side of the room, and 3 on the right side of the room. Each door will lead to a different room, and when the player enters a room for the first time, enemies will spawn for them to kill. If the player leaves a room and goes back through the same door, the layout of the room will be the same as before.

## Different Factory Puzzles / Features To Configure / Enable

Did not complete.

## 5 Different Enemies

4 of the 5 enemy types were completed

* Walker: simply walks in one direction until it hits a wall, then turns around. Deals contact damage if the player walks into it.
* Flyer: Will hover a small distance above the ground, otherwise has similar behavior to the walker
* Archer: Walks around similarly to the walker, if the player stands in front of it for long enough, it will fire a projectile at them that deals damage. Does not deal contact damage.
* Bouncer: Not affected by gravity, will float in a diagonal pattern and deal damage to the player on contact.

## Randomly Generated Rooms

When a room is first entered, platforms will be randomly placed around the room. Similarly, enemies will also be randomly distributed. 

## 5 Different Player Abilities

The player has 5 different types of abilities to move or fight  the enemies:

- Double Jump: The player can jump a second time when they are in the air
- Wall Jump: The player can slide down walls, and if they press **Space**, they will preform a jump, even if they do not have a double jump ready
- Sprint: If the player presses **Shift**, they will run at 1.5 times their normal walking speed
- Melee Attack: If the player presses **Left Click**, any enemy in the direction the player last moved will take damage. The yellow rectangles on either side of the player shows the hitboxes for the attack
- Ranged Attack: If the player presses **Right Click**, they will fire a projectile that deals a small amount of damage to the first enemy it hits

# Midterm Common Deliverables

## Functioning Entity System

The entity system is very easy to see. All enemies (currently using sprites with E# on them, with # being the enemy type) and the player use entities to function. There is also an entity manager that stores all entities that have been created

## Fully Interactable Environment

Every level is composed of tiles. These tiles can be walked on, walked into, jumped on, wall jumped on, and so forth. Enemies will also collide with these tiles. There are a few bug in the collision system still, but they are being worked on.

## User Interface

There are a few different types of UI in this game

- FPS counter, showing how many frames per second the game is running at
- An HP counter, showing how much health the player has. If it hits zero, the game ends
- A total kills counter, used for scoring
- Every enemy has a health bar above them

## File Read/Write Access

After the player dies, they will be shown the top 3 scores on their system. Scores are equal to the amount of enemies killed before dying. For testing purposes, pressing **P** will give the player 10 kills every frame. This list will be saved even if the game is closed.

## Readme

This file, the Read.me, is located in the game directory. It is also available on the github page at:

https://github.com/EricKnudsen256/2DSpring2021

# Final Specific Deliverables 

## Minimap

Shown on the right side of the screen, shows all rooms and the position of the player. The colors determine what kind of room is in each spot.

* Green: starting room where the player spawns
* Teal: Rooms that are on the generated path to the end of the level. Path can cross over itself
* Red: The ending room of the level
* White: All non-path rooms that were created as part of the recursive function to populate the empty doorways of the level

## Inventory / Crafting System

Partially completed. Inventory has full functionality and menu, but the crafting system was not completed.

## Resources

Also partially completed. Basic resource system fully working, interacts properly with the player's inventory, but specific items and drops have not been created yet.

## Editor: Room Editor

Opened in the main menu by hitting the EDIT button. When pressing left click inside of the grid, creates a tile, left clicking on a tile that exists removes it. The save button on the bottom right saves the room template to the templates folder. Clear removes all changes to the room, allowing for another room to be easily created.

## Research Component: Procedural Level Generation

The level generator has a few different steps:

1. Creates spawn room and room to the right of the spawn,
2. Randomly decides if the solution path should move up, down, or to the right, creates rooms as it moves. Each room created has a room type to show which doors should be opened. If the path hits the top or bottom wall, moves to the right.
3. Once the solution path attempts to move to the right, but is on the far right side of the level, creates the end room.
4. Every room is check to see if it still has an open door, then begins a recursive function to randomly generate a room branch from that each open door.

# Final Common Deliverables

## Code Compiles Clean

Has a few standard warnings from Visual Studios, such as lossy conversions that I am aware of and should probably cast instead, but otherwise compiles well.

## Fully Interactable Environment

Every level is composed of tiles. These tiles can be walked on, walked into, jumped on, wall jumped on, and so forth. Enemies will also collide with these tiles. There are a few bug in the collision system still, but they are being worked on.

## Steady Frame Rate of Rendering (constant FPS)

On my hardware, constantly runs at 60 FPS, which is max.

## Audio Implementation

Fairly barebones at the moment, but is fully functional. Sound effects that are currently implemented are:

- Background music
- Player melee sound
- Player ranged attack sound

## Menu System

All menus in the game utilize the menu system that I built. The currently implemented menus are as follows:

- Main Menu
- Pause Menu
- Inventory Menu
- Minimap
- Template Editor

## Research Component

Included in specific deliverables.

## Seamless Level Transitions

Not very applicable for current system, but all rooms can be moved through without any transitions.

## First Few Levels

First level, generated by level generation script, is fully traversable. Not much to play as resource and building systems not implemented yet.

## Content Editor

Also mentioned above in specific deliverables.

## Journal

Not a journal in the literal sense, but the github project board has been used to track progress and is available at:

https://github.com/EricKnudsen256/2DSpring2021/projects/1

## Readme

This file, the Read.me, is located in the game directory. It is also available on the github page at:

https://github.com/EricKnudsen256/2DSpring2021