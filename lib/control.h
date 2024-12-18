#pragma once

#ifndef h_control_h
#define h_control_h

#include "magic.h"

#ifdef __cplusplus
extern "C" {
#endif

void update_events();

void get_movement_vec(int* x, int* y);

#ifdef __cplusplus
}
#endif

#endif
