/*
 * The Laatu sprite system ver. 1.02
 *
 * for use in laatu applications
 *
 * copyleft 2024-12-10 Jussi
 * redistribute whenever possible
*/

#ifndef h_sprite_h
#define h_sprite_h

#include <SDL2/SDL.h>

#include "magic.h"

typedef struct {
	SDL_Texture* Texture;
	SDL_Rect Transform;
} Sprite;

#ifdef __cplusplus
extern "C" {
#endif

/* Allocates heap memory to store the sprites. Must be called before any sprites are loaded. */
int init_sprite_queue(size_t length);

/* Free the allocated sprite memory */
void destroy_sprite_queue();

/* Wrapper for loading a bitmap image with heap allocation. */
Sprite* load_sprite(const Application* App, const char* path, SDL_Rect transform);

/* Free a single sprite from memory */
int free_sprite(Sprite* sprite_value);

/* Copy the sprite for rendering */
void render_sprite(const Application* App, Sprite* sprite);

/* Renders all loaded sprites */
void render_all(const Application* App);

#ifdef __DEBUG__
/* Debug function for listing valid sprite pointers */
void list_loaded_sprites();
#endif

#ifdef __cplusplus
}
#endif

#endif
