#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <stdlib.h>
#include <string.h>

#include "magic.h"
#include "asym.h"

const int map[] = 
{
  0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const int mapw = 9;
const int maph = 9;

const int tilew = 16;
const int tileh = 16;

const char tilemap[] = "res/testcube.bmp";

AsymTileMap* load_asym_tilemap(const Application* App)
{
  AsymTileMap* out = malloc(sizeof(AsymTileMap));
  if (!out) return NULL;

  out->tiles = malloc(mapw * maph * sizeof(int));
  if (!out->tiles) return NULL;

  memcpy(out->tiles, &map, mapw * maph * sizeof(int));
  out->mapw = mapw;
  out->maph = maph;
  
  SDL_Surface* tilesurface = SDL_LoadBMP(tilemap);
  out->tileset = SDL_CreateTextureFromSurface(App->Renderer, tilesurface);
  if (!out->tileset) return NULL;

  return out;
}

int draw_asym_tilemap(const Application* App, const AsymTileMap* Tilemap)
{
  for (int ycell = 0; ycell < Tilemap->maph; ycell++)
  {
    for (int xcell = 0; xcell < Tilemap->mapw; xcell++)
    {
      int tile_index = ycell * Tilemap->mapw + xcell;
      int tiledata = Tilemap->tiles[tile_index];
      SDL_Rect srcslice = {(tiledata - 1) * tilew, 0, tilew, tileh};
      SDL_Rect destblt = {xcell * tilew, ycell * tileh, tilew, tileh};
      if (tiledata == 0) continue;
      SDL_RenderCopy(App->Renderer, Tilemap->tileset, &srcslice, &destblt);
    }
  } 
  return 0;
}
