#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

extern "C"
{
#include "lib/magic.h"
#include "lib/sprite.h"
#include "lib/dynlist.h"
}

const int wWidth = 640;
const int wHeight = 480;

Application* sWindow;
SDL_Event CurrentEvent;

int main(int argc, char **argv)
{
  // Initialize components
  if (init_application(&sWindow, wWidth, wHeight) != 0) return 1;
  if (init_sprite_queue(10) != 0) return 2;

  // Main activity loop
  while (true)
  {
    SDL_PollEvent(&CurrentEvent);
    if (CurrentEvent.type == SDL_QUIT)
    {
      break;
    }

    display_update(sWindow);
  }

  destroy_application(sWindow);
  return 0;
}
