#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <cstdio>

#include "lib/magic.h"
#include "lib/sprite.h"
#include "lib/control.h"
#include "lib/asym.h"

#include "game.h"

const int wWidth = 640;
const int wHeight = 480;

Application* sWindow;
extern SDL_Event CurrentEvent;

float delta_time = 0;
int next_frame_in = 0;

int main(int argc, char **argv)
{
  // Initialize components
  if (init_application(&sWindow, wWidth, wHeight) != 0) return 1;
  if (init_sprite_queue(10) != 0) return 2;
  if (game_init(sWindow) != 0) return 3;

  AsymTileMap* atilemap = load_asym_tilemap(sWindow);
  if (!atilemap) return 4;

  // Main activity loop
  while (true)
  {
    update_delta_time();
    next_frame_in = (0.016 - delta_time) * 1000;

    update_events();
    game_tick(delta_time);

    if (CurrentEvent.type == SDL_QUIT) break;
    
    draw_asym_tilemap(sWindow, atilemap);
    display_update(sWindow);
    if (next_frame_in > 0)
    {
      SDL_Delay(next_frame_in);
    }
  }

  game_cleanup();
  destroy_application(sWindow);
  return 0;
}
