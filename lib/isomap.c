#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include "magic.h"
#include "isomap.h"
#include "sprite.h"

typedef struct {
	char* name;
	int map_w;
	int map_h;
	int map_d;
	Uint8*** map;
} map_t;

const int tile_width = 32;
const int tile_height = 32;

int offsetx = 200;
int offsety = 100;

const int rendering_scale = 2;
SDL_Texture* tileset_loaded;
Sprite* cursor_sprite;

map_t loaded_map;


static void get_iso_coords(int gx, int gy, int gz, int* outX, int* outY)
{
  if (outX) *outX = offsetx + ((gx * tile_width / 2) + (gy * tile_width / 2)) * rendering_scale;
  if (outY) *outY = offsety + ((gy * tile_height / 4) - (gz * tile_height / 2) - (gx * tile_height / 4)) * rendering_scale;
}

void load_cursor_sprite()
{
	if (!cursor_sprite && sWindow) cursor_sprite = load_sprite(sWindow, "res/cursor.bmp", (SDL_Rect){0, 0, 32 * rendering_scale, 32 * rendering_scale});
}

void draw_debug_cursor() // TODO: Optimisoi tää
{
	if (!cursor_sprite) return;
	int screenx;
	int screeny;

	SDL_GetMouseState(&screenx, &screeny);
	cursor_sprite->Transform.x = (screenx - tile_width / 2) / tile_width * tile_width + offsetx % tile_width+tile_width/2;
	cursor_sprite->Transform.y = (screeny - tile_height / 2) / (tile_height / 2) * (tile_height / 2) + offsety % tile_height;
}

int load_tileset(const Application* App)
{
  SDL_Surface* tilesurface = SDL_LoadBMP("res/tileset.bmp");
  tileset_loaded = SDL_CreateTextureFromSurface(App->Renderer, tilesurface);
	return 0;
}

int load_map_from_file(const char* mapname)
{	
	char filepath[256];
	char line[256];
	char name[256];
	
	snprintf(filepath, sizeof(filepath), "maps/%s.juusto", mapname);

	printf("loading map" + *filepath);
	FILE *file = fopen(filepath, "r");
	if (file == NULL) {
		perror("Error opening file");
		return -1;
	}

	// Read width, height, depth, and name
	if (fgets(line, sizeof(line), file) != NULL) {
		sscanf(line, ",w%d,h%d,d%d,\"%[^\"]\",",&loaded_map.map_w, &loaded_map.map_h, &loaded_map.map_d); // SS can F!
		loaded_map.name = name;
	}

	// Allocate memory for the map
	loaded_map.map = (Uint8 ***)malloc(loaded_map.map_h * sizeof(Uint8 **));
	for (int d = 0; d < loaded_map.map_d; d++) {
		loaded_map.map[d] = (Uint8 **)malloc(loaded_map.map_h * sizeof(Uint8 *));
		for (int h = 0; h < loaded_map.map_h; h++) {
				loaded_map.map[d][h] = (Uint8 *)malloc(loaded_map.map_w * sizeof(Uint8));
		}
	}

    // Read the map data
	for (int d = 0; d < loaded_map.map_d; d++) {
		for (int h = 0; h < loaded_map.map_h; h++) {
			if (fgets(line, sizeof(line), file) != NULL) {
				char *token = strtok(line, ",");
				for (int w = 0; w < loaded_map.map_w; w++) {
					if (token != NULL) {
						loaded_map.map [d][h][w] = atoi(token);
						token = strtok(NULL, ",");
					}
				}
			}
		}
	}

	fclose(file);

	return 0;
}

void draw_tilemap(const Application* App) {

	if (!loaded_map.map) return;

	int MAPH, MAPW, MAPD;
	MAPH = loaded_map.map_h;
	MAPW = loaded_map.map_w;
	MAPD = loaded_map.map_d;

	Uint8 tile_data;
	SDL_Rect srcslice;
	SDL_Rect destblt = {0, 0, tile_width * rendering_scale, tile_height * rendering_scale};

	for (int zcell = 0; zcell < MAPD; zcell++) {
		for (int ycell = 0; ycell < MAPH; ycell++) {
			for (int xcell = MAPW-1; xcell >= 0; xcell--) {
				tile_data = loaded_map.map[zcell][ycell][xcell];
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

