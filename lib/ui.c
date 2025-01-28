#include <SDL2/SDL.h>
#include <string.h>

#include "magic.h"
#include "dynlist.h"
#include "ui.h"

LoadedFonts loaded_fonts;
	
/* ---- BEGIN PRIVATE FUNCTIONS ---- */

static UiSceneComponent* get_ui_components();

static void load_ui_interface_modules(const char* fpath);

static int get_font_index(const char* fname)
{
	for (int i = 0; i < loaded_fonts.fonts_avail->clen)	
	{
		if (strcmp(loaded_fonts.fonts_name[i], fname))
			return i;
	}
	return -1;
}

static void load_fonts(const char** fpaths, Uint8 fontc)
{
	SDL_Surface* temp_surface;
	SDL_Texture* font_texture;

	if (loaded_fonts) return;
	if (!sWindow) return;

	loaded_fonts = LoadedFonts {
		init_dynlist(fontc, sizeof(Sprite*)),
		malloc(fontc * sizeof(char*)),
	};
	
	for (int i = 0; i < fontc; i++)
	{
		if (!(temp_surface = SDL_LoadBMP(fpaths[i]))) return; // Pistä konsoli error tähän
		if (!(font_texture = SDL_CreateTextureFromSurface(sWindow->Renderer, temp_surface)));	
		dyn_push(loaded_fonts.fonts_avail, (void*)font_texture);
		loaded_fonts.fonts_name[i] = fpaths[i];
	}
}

static void unload_fonts()
{
	if (!loaded_fonts) return;
}

/* ---- END PRIVATE FUNCTIONS ---- */

/* ---- BEGIN PUBLIC FUNCTIONS ---- */

int init_ui();

void cleanup_ui();

void draw_ui();

/* ---- END PUBLIC FUNCTIONS ---- */
