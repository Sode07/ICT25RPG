#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>

#include "magic.h"
#include "isomap.h"

#define MAPW 3
#define MAPH 3
#define MAPD 3

const int tile_width = 32;
const int tile_height = 32;

int offsetx = 100;
int offsety = 200;

const int rendering_scale = 2;
SDL_Texture* tileset;

Uint8 testmap[MAPD][MAPH][MAPW] =
{
  {
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 0},
  },
  {
    {0, 0, 0},
    {1, 1, 1},
    {0, 0, 0},
  },
  {
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 0},
  }
};

static void get_iso_coords(int x, int y, int z, int* outX, int* outY)
{
  if (outX) *outX = offsetx + ((x * tile_width / 2) + (y * tile_width / 2)) * rendering_scale;
  if (outY) *outY = offsety + ((y * tile_height / 4) - (z * tile_height / 2) - (x * tile_height / 4)) * rendering_scale;
}

int load_tileset(const Application* App)
{
  SDL_Surface* tilesurface = SDL_LoadBMP("res/testcube.bmp");
  tileset = SDL_CreateTextureFromSurface(App->Renderer, tilesurface);
  if (!tileset) return -1; else return 0;
}

void draw_tilemap(const Application* App)
{
  Uint8 tile_data;
  SDL_Rect srcslice;
  SDL_Rect destblt = {0, 0, tile_width * rendering_scale, tile_height * rendering_scale};

  for (int zcell = 0; zcell < MAPD; zcell++)
  {
    for (int ycell = 0; ycell < MAPH; ycell++)
    {
      for (int xcell = MAPW-1; xcell >= 0; xcell--)
      {
        tile_data = testmap[zcell][ycell][xcell];
        if (!tile_data) continue;

        srcslice = (SDL_Rect){0, 0, tile_width, tile_height};
        get_iso_coords(xcell, ycell, zcell, &destblt.x, &destblt.y);
        SDL_RenderCopy(App->Renderer, tileset, &srcslice, &destblt);
      }
    }
  }
}
