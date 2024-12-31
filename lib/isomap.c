#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include "magic.h"
#include "isomap.h"

#define MAPD 1

const int tile_width = 32;
const int tile_height = 32;

int offsetx = 200;
int offsety = 100;

const int rendering_scale = 2;
SDL_Texture* tileset;



typedef unsigned char Uint8;
//Rehel en oo iha varma mitä nää kaikki rivit tekee mut se toimii
void loadMapFromFile(const char *filename, Uint8 ***map, int *MAPH, int *MAPW) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    char name[256];

    // Katotaa leveys korkeus ja nimi
    if (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, ",w%d,h%d,\"%[^\"]\",", MAPW, MAPH, name);
    }

    // Pistetää muistii (toivottavasti tää riittää also en koskaa free tätä)
    *map = (Uint8 **)malloc(*MAPH * sizeof(Uint8 *));
    for (int h = 0; h < *MAPH; h++) {
        (*map)[h] = (Uint8 *)malloc(*MAPW * sizeof(Uint8));
    }

    // Luetaan mitä ihmettä siel o
    for (int h = 0; h < *MAPH; h++) {
        if (fgets(line, sizeof(line), file) != NULL) {
            char *token = strtok(line, ",");
            for (int w = 0; w < *MAPW; w++) {
                if (token != NULL) {
                    (*map)[h][w] = atoi(token);
                    token = strtok(NULL, ",");
                }
            }
        }
    }

    fclose(file);
}
static void get_iso_coords(int gx, int gy, int gz, int* outX, int* outY)
{
  if (outX) *outX = offsetx + ((gx * tile_width / 2) + (gy * tile_width / 2)) * rendering_scale;
  if (outY) *outY = offsety + ((gy * tile_height / 4) - (gz * tile_height / 2) - (gx * tile_height / 4)) * rendering_scale;
}

static void convert_screenspace_into_isog(int x, int y, int* outX, int* outY, int* outZ)
{
  if (outX) *outX = (x + offsetx) / (tile_width * rendering_scale);
  if (outY) *outY = (y + offsety) / (tile_width * rendering_scale);
  if (outZ) return;
}

void draw_debug_cursor() // TODO: Optimisoi tää
{
  int screenx;
  int screeny;
  SDL_Rect dCursor = {0, 0, tile_width * rendering_scale, tile_height * rendering_scale};

  SDL_GetMouseState(&screenx, &screeny);
  dCursor.x = (screenx - tile_width / 2) / tile_width * tile_width + offsetx % tile_width;
  dCursor.y = (screeny - tile_height / 2) / (tile_height / 2) * (tile_height / 2) + offsety % tile_height;
  
  //printf("cx: %i, cy: %i\n", (screenx - tile_width / 2) / tile_width - offsetx / tile_width, (screeny - tile_height / 2) / (tile_height / 2) - offsety / tile_height);

  SDL_SetRenderDrawColor(sWindow->Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  if (SDL_RenderDrawRect(sWindow->Renderer, &dCursor) < 0) puts(SDL_GetError());
  SDL_SetRenderDrawColor(sWindow->Renderer, 0x00, 0x00, 0x00, 0x00);
}

int load_tileset(const Application* App)
{
  SDL_Surface* tilesurface = SDL_LoadBMP("res/testcube.bmp");
  tileset = SDL_CreateTextureFromSurface(App->Renderer, tilesurface);
  if (!tileset) return -1; else return 0;
}

void draw_tilemap(const Application* App, const char *mapname) {
    Uint8 **map = NULL;
    int MAPH, MAPW;

    // TOIMI! plz
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "maps/%s.juusto", mapname);

    loadMapFromFile(filepath, &map, &MAPH, &MAPW);

    Uint8 tile_data;
    SDL_Rect srcslice;
    SDL_Rect destblt = {0, 0, tile_width * rendering_scale, tile_height * rendering_scale};

    for (int ycell = 0; ycell < MAPH; ycell++) {
        for (int xcell = MAPW - 1; xcell >= 0; xcell--) {
            tile_data = map[ycell][xcell];
            if (!tile_data) continue;

            srcslice = (SDL_Rect){0, 0, tile_width, tile_height};
            get_iso_coords(xcell, ycell, 0, &destblt.x, &destblt.y);
            if (
                destblt.x > 0 - tile_width * rendering_scale && destblt.x < App->width &&
                destblt.y > 0 - tile_height * rendering_scale && destblt.y < App->height
            ) {
                SDL_RenderCopy(App->Renderer, tileset, &srcslice, &destblt);
            }
        }
    }
}


