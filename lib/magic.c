#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "magic.h"
#include "sprite.h"

/* BEGIN PRIVATE GLOBALS */

static char* errorstr;

/* END PRIVATE GLOBALS */

/* ---- BEGIN PRIVATE FUNCTIONS ---- */
/* ---- END PRIVATE FUNCTIONS ---- */

/* ---- BEGIN PUBLIC FUNCTIONS ---- */

char* get_error()
{
	return  errorstr;	
}

ApplicationInitStatus init_application(Application** outApp, int window_width, int window_height)
{
	*outApp = malloc(sizeof(Application));
	if (!*outApp) return APP_UNKNOWN;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) return APP_VIDEO_SUBSYSTEM_FAIL;
	
	(*outApp)->Window = SDL_CreateWindow("jai", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
	if (!(*outApp)->Window) return APP_WINDOW_CREATION_FAIL;

	(*outApp)->Renderer = SDL_CreateRenderer((*outApp)->Window, 0, SDL_RENDERER_PRESENTVSYNC);
	if (!(*outApp)->Renderer) return APP_RENDERER_CREATION_FAIL;

	(*outApp)->width = window_width;
	(*outApp)->height = window_height;

	return APP_SUCCESS;
}

void destroy_application (Application* App)
{
	if (App)
	{
		if (App->Window) SDL_DestroyWindow(App->Window);
		if (App->Renderer) SDL_DestroyRenderer(App->Renderer);
		free(App);
		App = NULL;
	}
}

void display_update(const Application* App)
{
  if (App)
  {
    SDL_RenderClear(App->Renderer);
    render_all(App);
    SDL_RenderPresent(App->Renderer);
  }
}

/* ---- BEGIN PUBLIC FUNCTIONS ---- */
