#pragma once

#ifndef h_isomap_h
#define h_isomap_h

#include "magic.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int x;
  int y;
  int ix;
  int iy;
} IsoTransform;

int load_tileset(const Application* App);

void draw_tilemap(const Application* App, const char *mapname);

void draw_debug_cursor();

#ifdef __cplusplus
}
#endif

#endif
