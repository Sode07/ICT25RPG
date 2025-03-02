#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

#include "lib/magic.h"
#include "lib/sprite.h"
#include "lib/control.h"
#include "lib/isomap.h"
#include "lib/console.h"
#include "game.h"

#include <thread>
#include <iostream>
#include <signal.h>

#ifdef _DEBUG
#include <mcheck.h>
#endif

const int wWidth = 720;
const int wHeight = 640;

Application* sWindow;
extern SDL_Event CurrentEvent;

float delta_time = 0;
int next_frame_in = 0;
bool console_running = true;
static std::thread console_thread;

void display_update() // Called for every frame
{
  if (sWindow)
    {
      SDL_RenderClear(sWindow->Renderer);
      draw_tilemap(sWindow);
      draw_debug_cursor();
      SDL_RenderPresent(sWindow->Renderer);
    }
}

void cleanup_main(int status)
{
  console_running = false;
  std::cin.putback('\n');
  console_thread.join();
  game_cleanup();
  cleanup_isomap();
  destroy_sprite_queue();
  destroy_application(sWindow);
  free(sWindow);
  exit(0);
}

void handle_sigsegv(int status)
{
    // PUMMM
}

int main(int argc, char **argv)
{
#ifdef _DEBUG
    mtrace();
#endif
    
  signal(SIGINT, cleanup_main);
  // signal(SIGSEGV, handle_sigsegv);

  console_thread = std::thread(konsoli);

  // Initialize components
  if (!(sWindow = init_application(wWidth, wHeight))) return 1;
  if (init_sprite_queue(10) != 0) return 2;
  if (load_tileset(sWindow)) return 4;
  if (game_init(sWindow) != 0) return 3;

  load_cursor_sprite();

  // Main activity loop
  while (true)
    {
      update_delta_time();
      next_frame_in = (0.016 - delta_time) * 1000;

      update_events();
      game_tick(delta_time);

      if (CurrentEvent.type == SDL_QUIT) cleanup_main(0);

      display_update();
      if (next_frame_in > 0)
	{
	  SDL_Delay(next_frame_in);
	}
    }
  return 0;
}
