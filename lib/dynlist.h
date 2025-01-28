/*
 * Dynamic Heap-Allocated Pointer List
 * 
 * Ei, emme käytä std::vectoria!
 *
 * copyleft 2024-12-10 Jussi
 * redistribute whenever possible
*/
#ifndef h_dynlist_h
#define h_dynlist_h

#include <stddef.h>
#include "magic.h"

typedef struct {
  void** mem;
  size_t tlen;
  size_t clen;
  size_t unitsize;
} DynList;

#ifdef __cplusplus
extern "C" {
#endif

/* Preallocates heap memory */
DynList* init_dynlist(size_t len, size_t size);

/* Appends a pointer onto the dynlist */
int dyn_push(DynList* list, void* ptr);

/* Removes the last pointer from the dynlist */
int dyn_pop(DynList* list);

/* Removes a specific pointer from the dynlist */
int dyn_rem(DynList* list, void* ptr);

/* Deletes the whole dynlist from memory */
void dyn_free(DynList* list);

#ifdef __cplusplus
}
#endif

#endif
