#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "magic.h"
#include "sprite.h"

/* BEGIN PRIVATE GLOBALS */

static char* errorstr;
extern float delta_time;

/* END PRIVATE GLOBALS */

/* ---- BEGIN PUBLIC FUNCTIONS ---- */

char* get_error()
{
	return  errorstr;	
}

Application* init_application(int window_width, int window_height)
{
	Application* outApp = malloc(sizeof(Application));
	if (!outApp) return NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) return NULL;
	
	outApp->Window = SDL_CreateWindow(
    "jai",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    window_width,
    window_height,
    SDL_WINDOW_SHOWN
  );
	if (!outApp->Window) return NULL;

	outApp->Renderer = SDL_CreateRenderer(outApp->Window, 0, SDL_RENDERER_ACCELERATED);
	if (!outApp->Renderer) return NULL;

	outApp->width = window_width;
	outApp->height = window_height;

	return outApp;
}

void destroy_application (Application* App)
{
	if (App)
	{
		if (App->Renderer) SDL_DestroyRenderer(App->Renderer);
		if (App->Window) SDL_DestroyWindow(App->Window);
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		SDL_Quit();
	}
}

void update_delta_time()
{
  static Uint32 PreviousFrame = 0;
  Uint32 CurrentFrame = SDL_GetTicks();
  delta_time = (CurrentFrame - PreviousFrame) * 0.001;
  PreviousFrame = CurrentFrame;
}

/* ---- BEGIN PUBLIC FUNCTIONS ---- */
