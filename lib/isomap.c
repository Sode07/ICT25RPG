#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include "magic.h"
#include "isomap.h"

const int tile_width = 32;
const int tile_height = 32;

int offsetx = 200;
int offsety = 100;

const int rendering_scale = 2;
//tää o tosi getto ratkasu ja joudutaa lisää näit joka tilee mu onneks jokases o yläpuoli ja sivu eli yks o kaks eri tekstuurii
SDL_Texture* tileset_loaded;

void loadMapFromFile(const char *filename, Uint8 ****map, int *MAPH, int *MAPW, int *MAPD)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		perror("Error opening file");
		return;
	}

	char line[256];
	char name[256];

	// Read width, height, depth, and name
	if (fgets(line, sizeof(line), file) != NULL) {
		sscanf(line, ",w%d,h%d,d%d,\"%[^\"]\",", MAPW, MAPH, MAPD, name);
	}

	// Allocate memory for the map
	*map = (Uint8 ***)malloc(*MAPD * sizeof(Uint8 **));
	for (int d = 0; d < *MAPD; d++) {
		(*map)[d] = (Uint8 **)malloc(*MAPH * sizeof(Uint8 *));
		for (int h = 0; h < *MAPH; h++) {
				(*map)[d][h] = (Uint8 *)malloc(*MAPW * sizeof(Uint8));
		}
	}

    // Read the map data
	for (int d = 0; d < *MAPD; d++) {
		for (int h = 0; h < *MAPH; h++) {
			if (fgets(line, sizeof(line), file) != NULL) {
				char *token = strtok(line, ",");
				for (int w = 0; w < *MAPW; w++) {
					if (token != NULL) {
						(*map)[d][h][w] = atoi(token);
						token = strtok(NULL, ",");
					}
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

void draw_debug_cursor() // TODO: Optimisoi tää
{
    int screenx;
    int screeny;
    SDL_Rect dCursor = {0, 0, tile_width/2 * rendering_scale, tile_height * rendering_scale};

    SDL_GetMouseState(&screenx, &screeny);
    dCursor.x = (screenx - tile_width / 2) / tile_width * tile_width + offsetx % tile_width+tile_width/2;
    dCursor.y = (screeny - tile_height / 2) / (tile_height / 2) * (tile_height / 2) + offsety % tile_height;

    //printf("cx: %i, cy: %i\n", (screenx - tile_width / 2) / tile_width - offsetx / tile_width, (screeny - tile_height / 2) / (tile_height / 2) - offsety / tile_height);

    SDL_SetRenderDrawColor(sWindow->Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    if (SDL_RenderDrawRect(sWindow->Renderer, &dCursor) < 0) puts(SDL_GetError());
    SDL_SetRenderDrawColor(sWindow->Renderer, 0x00, 0x00, 0x00, 0x00);
}

int load_tileset(const Application* App)
{
  SDL_Surface* tilesurface = SDL_LoadBMP("res/tileset.bmp");
  tileset_loaded = SDL_CreateTextureFromSurface(App->Renderer, tilesurface);
	return 0;
}

void draw_tilemap(const Application* App, const char *mapname) {
	Uint8 ***map = NULL;
	int MAPH, MAPW, MAPD;

	// Load the map from file
	char filepath[256];
	snprintf(filepath, sizeof(filepath), "maps/%s.juusto", mapname);
	loadMapFromFile(filepath, &map, &MAPH, &MAPW, &MAPD);

	Uint8 tile_data;
	SDL_Rect srcslice;
	SDL_Rect destblt = {0, 0, tile_width * rendering_scale, tile_height * rendering_scale};

	for (int zcell = 0; zcell < MAPD; zcell++) {
		for (int ycell = 0; ycell < MAPH; ycell++) {
			for (int xcell = MAPW-1; xcell >= 0; xcell--) {
				tile_data = map[zcell][ycell][xcell];
				if (!tile_data) continue;
				srcslice = (SDL_Rect){(tile_data - 1) * 32, 0, tile_width, tile_height};
				get_iso_coords(xcell, ycell, zcell, &destblt.x, &destblt.y);
				if(
					destblt.x > 0 - tile_width * rendering_scale && destblt.x < App->width &&
					destblt.y > 0 - tile_height * rendering_scale && destblt.y < App->height
					)
				{SDL_RenderCopy(sWindow->Renderer, tileset_loaded, &srcslice, &destblt);}
			}
		}
	}
}

