#include <SDL2/SDL.h>
#include <string.h>

#include "magic.h"
#include "dynlist.h"
#include "stdbool.h"
#include "ui.h"
#include "console.h"

DynList* loaded_fonts;

/* ---- BEGIN PRIVATE FUNCTIONS ---- */

static UiSceneComponent* get_ui_components();

static void load_ui_interface_modules(const char* fpath);

static int get_font_index(const char* fname)
{
    Font* font_manip;
   
    for (int i = 0; i < loaded_fonts->clen; i++)	
    {
	font_manip = dyn_get(loaded_fonts, i);
	if (strcmp(font_manip->name, fname))
	    return i;
    }
    return -1;
}

static void load_font_with_append(const char* fpath)
{ 
    SDL_Surface* temp_surface;
    SDL_Texture* font_texture;

    if (!fntarr || !fntarr->present)
	return;
    
    if (!(temp_surface = SDL_LoadBMP(fpath)))
    {
	KLOG_WARNING("Failed to load Font texture %s", fpath);
	return;
    }
	
    if (!(font_texture = SDL_CreateTextureFromSurface(sWindow->Renderer, temp_surface)))
    {
	KLOG_WARNING("%s","Failed to create Font texture");
	return;
    }
    
    dyn_push(fntarr->fonts_avail, font_texture);
    
}

static void load_fonts(const char** fpaths, Uint8 fontc)
{
    if (!loaded_fonts.present) return;
    if (!sWindow) return;

    loaded_fonts = (LoadedFonts) {
	init_dynlist(fontc, sizeof(SDL_Texture*)),
	malloc(fontc * sizeof(char*)),
    };
	
    for (int i = 0; i < fontc; i++)
    {
	
  
	dyn_push(loaded_fonts.fonts_avail, font_texture);
    }
}

static void unload_fonts()
{ 
    if (!loaded_fonts.present) return;
}

/* ---- END PRIVATE FUNCTIONS ---- */

/* ---- BEGIN PUBLIC FUNCTIONS ---- */

int init_ui();

void cleanup_ui();

void draw_ui();

#ifdef __DEBUG__
void list_loaded_ui_components()
{

};
#endif

/* ---- END PUBLIC FUNCTIONS ---- */
