#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>

#include "lib/magic.h"
#include "lib/sprite.h"
#include "lib/control.h"
#include "lib/isomap.h"
#include "lib/pos.h"

Sprite* chocobo;
extern int offsetx;
extern int offsety;

int playerx = 0;
int playery = 0;

int game_init(const Application* App)
{
  chocobo = load_sprite(App, "res/sprites/pelaaja.bmp", (SDL_Rect){playerx, playery, 16*2, 32*2});
  if (!chocobo) return -1;
  return 0;
}

void game_tick(float delta_time)
{
  int vx, vy;
  SDL_Rect* tf = &chocobo->Transform;

  get_movement_vec(&vx, &vy);
  offsetx += -1 * 600 * vx * delta_time;
  offsety += -1 * 600 * vy * delta_time;
}

void game_cleanup()
{
  free_sprite(chocobo);
}
