#ifndef H_UI_H
#define H_UI_H

#include "dynlist.h"
#include "SDL/SDL2.h"

typedef struct {
	Dynlist* fonts_avail;
	const char** fonts_name;
} LoadedFonts;

typedef struct {
	Uint8 font_index;	
} UiSceneComponent;

typedef struct {
	UiSceneComponent* components;
} UiScene;

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif
