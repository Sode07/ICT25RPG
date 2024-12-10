#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>

SDL_Event CurrentEvent;
int cx = 0;
int cy = 0;

const SDL_Keycode upkey = SDLK_w;
const SDL_Keycode downkey = SDLK_s;
const SDL_Keycode leftkey = SDLK_a;
const SDL_Keycode rightkey = SDLK_d;

static void handle_keyboard_event(int multiplier)
{
  if (CurrentEvent.key.keysym.sym == upkey)
  {
    cy = -1 * multiplier;
  }
  else if (CurrentEvent.key.keysym.sym == downkey)
  {
    cy = 1 * multiplier; 
  }
  else if (CurrentEvent.key.keysym.sym == leftkey)
  {
    cx = -1 * multiplier;
  }
  else if (CurrentEvent.key.keysym.sym == rightkey)
  {
    cx = 1 * multiplier;
  }
}

void update_events()
{
  SDL_PollEvent(&CurrentEvent);
  switch (CurrentEvent.type)
  {
    case SDL_KEYDOWN:
      handle_keyboard_event(1);
      break;
    case SDL_KEYUP:
      handle_keyboard_event(0);
      break;
    case SDL_QUIT:
      break;
  }
}

void get_movement_vec(int* x, int* y)
{
  *x = cx;
  *y = cy;
}



