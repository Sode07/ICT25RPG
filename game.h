#pragma once

#include "lib/magic.h"

int game_init(const Application* App);

void game_tick(float delta_time);

void move();

void game_cleanup();
