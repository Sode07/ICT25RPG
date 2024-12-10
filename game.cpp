#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>

#include "lib/magic.h"
#include "lib/sprite.h"
#include "lib/control.h"

Sprite* chocobo;

int game_init(const Application* App)
{
  chocobo = load_sprite(App, "res/chocobo.bmp", (SDL_Rect){0, 0, 100, 100});
  if (!chocobo) return -1;
  return 0;
}

void game_tick(float delta_time)
{
  int vx, vy;
  SDL_Rect* tf = &chocobo->Transform;

  get_movement_vec(&vx, &vy);
  tf->x += 600 * vx * delta_time;
  tf->y += 600 * vy * delta_time;
  printf("vx: %i, vy: %i\n", vx, vy);
}

void game_cleanup()
{
  free_sprite(chocobo);
}
