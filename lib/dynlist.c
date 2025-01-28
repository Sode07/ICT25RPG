#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "dynlist.h"

#define MAX_LENGTH 1024

int dyn_push(DynList* list, void* ptr)
{
	void* tmp_ptr;

  if (!list) return -1;
  if (list->clen >= list->tlen)
  {
    if (list->clen >= MAX_LENGTH) return -10;
    if (!(tmp_ptr = realloc(list->mem, list->unitsize))) return -20;
		list->mem = (void**)tmp_ptr;
  }
  ((char**)list->mem)[list->clen] = (char*)ptr;
  list->clen++;
  return 0;
}

int dyn_pop(DynList* list)
{
  if (!list) return -1;
  if (list->clen > 0) free(list->mem[--list->clen]);
  else return 1;
  return 0;
}

int dyn_rem(DynList* list, void* ptr)
{
	void* tmp_ptr;

  for (int i = 0; i < list->clen; i++)
  {
    if (i == 0)
      free(ptr);
    else if (list->mem[i] == ptr) {
      free(ptr);
      memcpy(list->mem+i, list->mem+i+1, list->clen - i);
      if (!(tmp_ptr = realloc(list->mem, --list->tlen))) return -40;
			list->mem = tmp_ptr;
    }
    list->clen--;
    return 0;
  }
  return 1;
}

void dyn_free(DynList* list)
{
  for (int i = 0; i < list->clen; i++)
  {
    free(list->mem[i]);
  }
	free(list);
}

DynList* init_dynlist(size_t len, size_t size)
{
  DynList* out = malloc(sizeof(DynList));
  if (!out) return NULL;

  out->mem = malloc(len * size);
  if (!out->mem) return NULL;

  out->tlen = len;
  out->clen = 0;
  out->unitsize = size;

  return out;
}

