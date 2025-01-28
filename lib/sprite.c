#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "magic.h"
#include "dynlist.h"
#include "sprite.h"

/* BEGIN PRIVATE GLOBALS */

DynList* sprite_queue = NULL; 

/* END PRIVATE GLOBALS */

/* ---- BEGIN PUBLIC FUNCTIONS ---- */

int init_sprite_queue(size_t length)
{
  if (sprite_queue) return 2;
  sprite_queue = init_dynlist(length, sizeof(Sprite*));
  if (!sprite_queue) return -1;
  return 0;
}

Sprite* load_sprite(const Application* App, const char* path, SDL_Rect transform)
{
  if (!sprite_queue) return NULL;
	SDL_Surface* SpriteSurface = SDL_LoadBMP(path);
	if (!SpriteSurface) return NULL;
	SDL_Texture* SpriteTexture = SDL_CreateTextureFromSurface(App->Renderer, SpriteSurface);

	Sprite* out = malloc(sizeof(Sprite));
	if (dyn_push(sprite_queue, out) < 0) return NULL; 	
	out->Texture = SpriteTexture;
	out->Transform = transform;

	return out;
}

int free_sprite(Sprite* sprite_value)
{
	if (!sprite_value) return -1;
	SDL_DestroyTexture(sprite_value->Texture);
	if (dyn_rem(sprite_queue, sprite_value) == 0)
	{
		return 0;
	}

	return -2;
}

void render_sprite(const Application* App, Sprite* sprite)
{
	if (sprite && App)
	{
		SDL_RenderCopy(App->Renderer, sprite->Texture, NULL, &sprite->Transform);
	}
	else
	{
		puts("Tried to render a NULL sprite");
	}
}

void render_all(const Application* App)
{
	for (int i = 0; i < sprite_queue->clen; i++)
	{
		render_sprite(App, sprite_queue->mem[i]);	
	}
}

void list_loaded_sprites()
{
  for (int i = 0; i < sprite_queue->clen; i++)
  {
    printf("%p\n", sprite_queue->mem[i]);
  }
}

void destroy_sprite_queue()
{
  if (!sprite_queue) return;
  dyn_free(sprite_queue);
}

/* ---- END PUBLIC FUNCTIONS ---- */
