#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>

SDL_Event CurrentEvent;
const Uint8* keyboard;

int cx = 0;
int cy = 0;

const SDL_Scancode upkey = 26;
const SDL_Scancode downkey = 22;
const SDL_Scancode leftkey = 4;
const SDL_Scancode rightkey = 7;

void update_events()
{
  SDL_PumpEvents(); 
  keyboard = SDL_GetKeyboardState(NULL);
  if (CurrentEvent.type == SDL_MOUSEBUTTONDOWN) {
    printf("Mouse button down at (%d, %d)\n", CurrentEvent.button.x, CurrentEvent.button.y);
  }
  if (keyboard[41]) CurrentEvent.type = SDL_QUIT;
  cx = -keyboard[leftkey] + keyboard[rightkey];
  cy = -keyboard[upkey] + keyboard[downkey];
}

void get_movement_vec(int* x, int* y)
{
  *x = cx;
  *y = cy;
}



