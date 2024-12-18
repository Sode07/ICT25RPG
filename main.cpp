#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

#include "lib/magic.h"
#include "lib/sprite.h"
#include "lib/control.h"
#include "lib/isomap.h"

#include "game.h"

const int wWidth = 640;
const int wHeight = 480;

Application* sWindow;
extern SDL_Event CurrentEvent;

float delta_time = 0;
int next_frame_in = 0;

void display_update()
{
  if (sWindow)
  {
    SDL_RenderClear(sWindow->Renderer);
    render_all(sWindow);
    draw_tilemap(sWindow);
    draw_debug_cursor();
    SDL_RenderPresent(sWindow->Renderer);
  }
}

int main(int argc, char **argv)
{
  // Initialize components
  if (init_application(&sWindow, wWidth, wHeight) != 0) return 1;
  if (init_sprite_queue(10) != 0) return 2;
  if (load_tileset(sWindow)) return 4;
  if (game_init(sWindow) != 0) return 3;

  // Main activity loop
  while (true)
  {
    update_delta_time();
    next_frame_in = (0.016 - delta_time) * 1000;

    update_events();
    game_tick(delta_time);

    if (CurrentEvent.type == SDL_QUIT) break;

    display_update();
    if (next_frame_in > 0)
    {
      SDL_Delay(next_frame_in);
    }
  }

  destroy_application(sWindow);
  return 0;
}
