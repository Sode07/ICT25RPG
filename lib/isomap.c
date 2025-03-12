#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include "console.h"
#include "magic.h"
#include "isomap.h"
#include "sprite.h"

#include <math.h>
#include <signal.h>
#include <stdbool.h>

const int tile_width = 32;
const int tile_height = 32;

int offsetx = 200;
int offsety = 100;

float rendering_scale = 2;
static SDL_Texture* tileset_loaded;
static SDL_Surface* tileset_surface;

Sprite* cursor_sprite;

map_t loaded_map;

static void get_tile_screenspace_dimensions(int* tw, int* th)
{
    static int xdim_cache = 0;
    static int ydim_cache = 0;
    static float rendering_scale_cache = 0;

    if (rendering_scale_cache != rendering_scale)
    {
	xdim_cache = tile_width / 2 * rendering_scale;
	ydim_cache = tile_height / 4 * rendering_scale;
	rendering_scale_cache = rendering_scale;
    }

    *tw = xdim_cache;
    *th = ydim_cache;
}

static void get_iso_coords(int gx, int gy, int gz, int* outX, int* outY)
{
    int ss_tile_width;
    int ss_tile_height;

    get_tile_screenspace_dimensions(&ss_tile_width, &ss_tile_height);
    
    if (outX)
	*outX = offsetx + (gx + gy) * ss_tile_width;

    if (outY)
	*outY = offsety + (gy - gx - gz * 2) * ss_tile_height;
}

static void unload_map()
{
    if (!loaded_map.map) return;

    for (int d = 0; d < loaded_map.map_d; d++)
    {
	for (int h = 0; h < loaded_map.map_h; h++)
	{
	    free(loaded_map.map[d][h]);
	}
	free(loaded_map.map[d]);
    }
    free(loaded_map.map);

    loaded_map.map_w = 0;
    loaded_map.map_h = 0;
    loaded_map.map_d = 0;
}

static SDL_Rect get_viewable_tiles_rect()
{
    int x_index_start, y_index_start;
    int ss_tile_width, ss_tile_height;
    int x_length, y_length;
    const int magic = 180;
    const int magic2 = 90;
    
    if (!sWindow)
	raise(SIGSEGV);

    get_tile_screenspace_dimensions(&ss_tile_width, &ss_tile_height);
    
    x_index_start = (-offsetx/2 + offsety - (magic * (int)rendering_scale)) / ss_tile_width;
    y_index_start = (-offsety/2 + abs(offsetx/4) + (magic2 * (int)rendering_scale )) / ss_tile_height;

    /* x_length = (offsetx < 0 ? offsetx : 0) + sWindow->width / ss_tile_width; */
    /* y_length = (offsety < 0 ? offsety : 0) + sWindow->height / ss_tile_height * 2; */

    x_length = 10;
    y_length = 10;
    
    return (SDL_Rect){x_index_start, y_index_start, x_length, y_length};
}

void load_cursor_sprite()
{
    if (!cursor_sprite && sWindow)
	cursor_sprite = load_sprite
	    (
	     sWindow,
	     "res/cursor.bmp",
	     (SDL_Rect){0, 0, 32 * rendering_scale, 32 * rendering_scale}
	     );

    if (!cursor_sprite)
	KLOG_WARNING("%s", "Failed to initialize cursor Sprite"); 
}

void draw_debug_cursor() // TODO: Optimisoi tää
{
    if (!cursor_sprite || ! sWindow)
    {
	KLOG_WARNING("%s", "Cursor sprite isn't loaded");
	return;
    }
    int screenx;
    int screeny;

    SDL_GetMouseState(&screenx, &screeny);

    cursor_sprite->Transform.x =
	(screenx - tile_width / 2) / tile_width * tile_width + offsetx % tile_width+tile_width/2;

    cursor_sprite->Transform.y =
	(screeny - tile_height / 2) / (tile_height / 2) * (tile_height / 2) + offsety % tile_height;

    render_sprite(sWindow, cursor_sprite);
} 

int load_tileset(const Application* App)
{
    tileset_surface = SDL_LoadBMP("res/tileset.bmp");
    tileset_loaded = SDL_CreateTextureFromSurface(App->Renderer, tileset_surface);
    return 0;
}

int load_map_from_file(const char* mapname)
{
  char filepath[256];
  char line[256];

  snprintf(filepath, sizeof(filepath), "maps/%s.juusto", mapname);

  KLOG_INFO("loading map: %s", filepath);
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    perror("Error opening file");
    return -1;
  }
	
  unload_map();

  // Read width, height, depth, and name
  if (fgets(line, sizeof(line), file) != NULL) {
    sscanf(line, ",w%d,h%d,d%d,\"%[^\"]\",", &loaded_map.map_w, &loaded_map.map_h, &loaded_map.map_d, loaded_map.name);
  }

  // Allocate memory for the map
  loaded_map.map = (Uint8 ***)malloc(loaded_map.map_d * sizeof(Uint8 **));
  for (int d = 0; d < loaded_map.map_d; d++)
  {
    loaded_map.map[d] = (Uint8 **)malloc(loaded_map.map_h * sizeof(Uint8 *));
    for (int h = 0; h < loaded_map.map_h; h++)
    {
      loaded_map.map[d][h] = (Uint8 *)malloc(loaded_map.map_w);
      for (int w = 0; w < loaded_map.map_w; w++)
      {
	loaded_map.map[d][h][w] = 0;
      }
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

void draw_tilemap(const Application* App)
{
    
    if (!loaded_map.map) return;

    Uint8 tile_data;
    SDL_Rect srcslice;
    SDL_Rect destblt = {0, 0, tile_width * rendering_scale, tile_height * rendering_scale};
    SDL_Rect viewable_tiles = get_viewable_tiles_rect();

    CLAMP(viewable_tiles.x, 0, loaded_map.map_w);
    CLAMP_MAX(viewable_tiles.w, loaded_map.map_w - viewable_tiles.x);

    CLAMP(viewable_tiles.y, 0, loaded_map.map_h);
    CLAMP_MAX(viewable_tiles.h, loaded_map.map_h - viewable_tiles.y);
    
    for (int zcell = 0; zcell < loaded_map.map_d; zcell++)
    {
	for (int ycell = viewable_tiles.y; ycell < +viewable_tiles.y + viewable_tiles.h; ycell++)
	{
	    for (int xcell = viewable_tiles.x + viewable_tiles.w; xcell >= viewable_tiles.x; xcell--)
	    {
		tile_data = loaded_map.map[zcell][ycell][xcell];
		if (!tile_data) continue;
		srcslice = (SDL_Rect){(tile_data - 1) * 32, 0, tile_width, tile_height};
		get_iso_coords(xcell, ycell, zcell, &destblt.x, &destblt.y);
		{SDL_RenderCopy(sWindow->Renderer, tileset_loaded, &srcslice, &destblt);}
	    }
	}
    }
}

void cleanup_isomap()
{
  if (cursor_sprite) free_sprite(cursor_sprite);
  if (tileset_surface) SDL_FreeSurface(tileset_surface);
  if (tileset_loaded) SDL_DestroyTexture(tileset_loaded);
  unload_map();
}

bool get_loaded_map_name(map_t* outMap)
{
    if (!loaded_map.map || !outMap)
	return false;

    *outMap = loaded_map;
    return true;    
}
