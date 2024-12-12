#pragma once

#include <SDL2/SDL.h>
#include "magic.h"

#ifndef h_asym_h
#define h_asym_h

typedef struct {
  int* tiles;
  int mapw;
  int maph;
  SDL_Texture* tileset;
} AsymTileMap;

#ifdef __cplusplus
extern "C" {
#endif

AsymTileMap* load_asym_tilemap(const Application* App);

int draw_asym_tilemap(const Application* App, const AsymTileMap* Tilemap);

#ifdef __cplusplus
}
#endif

#endif
