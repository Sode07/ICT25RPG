#ifndef H_UI_H
#define H_UI_H

#include "dynlist.h"
#include <SDL2/SDL.h>

#define GET_FONT_NAME(FC_LIST, INDEX) ((Font*)dyn_get(FC_LIST, INDEX))->name
#define GET_FONT_TEXTURE(FC_LIST, INDEX) ((Font*)dyn_get(FC_LIST, INDEX))->fnt_texture

typedef struct {
    SDL_Texture* fnt_texture;
    char* name;
} Font;

typedef struct {
    Uint8 font_index;	
} UiSceneComponent;

typedef struct {
    UiSceneComponent* components;
} UiScene;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __DEBUG__
    void list_loaded_ui_components();
#endif

#ifdef __cplusplus
}
#endif

#endif
