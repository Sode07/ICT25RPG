#ifndef h_isomap_h
#define h_isomap_h

#include "magic.h"

typedef struct {
  int x;
  int y;
  int ix;
  int iy;
} IsoTransform;

#ifdef __cplusplus
extern "C" {
#endif

int load_tileset(const Application* App);

void draw_tilemap(const Application* App);

void draw_debug_cursor();

void load_cursor_sprite();

int load_map_from_file(const char* mapname);

void cleanup_isomap();

#ifdef __cplusplus
}
#endif

#endif
