#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "magic.h"

#ifndef h_magic_h
#define h_magic_h

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
SDL_Window* Window;
	SDL_Renderer* Renderer;
	int width;
	int height;
} Application;

typedef enum {
 APP_SUCCESS = 0,
 APP_VIDEO_SUBSYSTEM_FAIL = 1,
 APP_WINDOW_CREATION_FAIL = 2,
 APP_RENDERER_CREATION_FAIL = 3,
 APP_UNKNOWN = 4,
} ApplicationInitStatus;

extern Application* sWindow;

/* Wrapper function for window creation. */
Application* init_application(int window_width, int window_height);

/* Wrapper for freeing the created window. Call when exiting. */
void destroy_application (Application* App);

/* No comment */
char* get_error();

void update_delta_time();

#ifdef __cplusplus
}
#endif

#endif
