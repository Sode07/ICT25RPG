#include <SDL2/SDL.h>
#include <string.h>

#include "magic.h"
#include "dynlist.h"
#include "stdbool.h"
#include "ui.h"
#include "console.h"

typedef const char* fname_t;
typedef const char* fpath_t

static DynList* loaded_fonts;

/* ---- BEGIN PRIVATE FUNCTIONS ---- */

static UiSceneComponent* get_ui_components();

static void load_ui_interface_modules(const char* fpath);

static char* get_fnt_fname_from_path(fpath_t file_path);

static int get_font_index(fname_t font_name)
{
    Font* font_manip;
   
    for (int i = 0; i < loaded_fonts->clen; i++)	
    {
	font_manip = dyn_get(loaded_fonts, i);
	if (strcmp(font_manip->name, font_name))
	    return i;
    }
    return -1;
}

static void load_font_with_append(fpath_t file_path)
{ 
    Font* _font;

    if (!loaded_fonts)
	return;

    if (!(_font = malloc(sizeof(Font))))
    {
	KLOG_WARNING("%s", "Malloc failed");
	return;
    }

    if (!(_font->fnt_surface = SDL_LoadBMP(file_path)))
    {
	KLOG_WARNING("Failed to load Font texture %s", file_path);
	return;
    }
	
    if (!(_font->fnt_texture = SDL_CreateTextureFromSurface(sWindow->Renderer, temp_surface)))
    {
	KLOG_WARNING("%s","Failed to create Font texture");
	return;
    }
    
    dyn_push(loaded_fonts, _font);
}

static void load_fonts(const char** fpaths, Uint8 fontc)
{
    if (!loaded_fonts) return;
    if (!sWindow) return;

    loaded_fonts = init_dynlist(fontc, sizeof(Font*));
	
    for (int i = 0; i < fontc; i++)
	load_font_with_append(fpaths[i]);
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
