# Knight
* This is a 2D platformer game/engine built in OpenGL C++(Under Development). 
* AABB Collision System is used for collision detection between sprites.
* Sprite sheet based animation system is developed and used in it, with the feature of dynamically changing the collider size equal to the current animation frame size.
* Player has following actions : idle , walk, run, jump, slide and fall.
* Main Menu and pause menu is developed. all the sprites in the menus and the collider sprites can be resized and translated.
* all translations and resizes will be stored in the text files in the levels folder.
* IrrKlang library is used for sound
* GLFW is used as the windowing library
# How to Use
* In the main menu, use arrow key UP & DOWN to select option. Press ENTER to enter the Game.
* Hold LEFT ALT key and hover mouse pointer near the center of the sprite until cursor changes to hand symbol. keep holding and now press LEFT MOUSE BUTTON and drag the sprite anywhere you want.
* Hold Z key and hover the mouse pointer near the edges of the sprite until cursor changes to resize icon. keep holding and press the LMS and drag the sides to resize it.(Resize it slowly as the cursor might get out of the edge and you will have to again bring it back to edge).
* Currently for level design, the level must be designed in a tilemap software like Tiled. the final image must be imported into the program and according to the level design, Colliders can be placed in the program itself.

