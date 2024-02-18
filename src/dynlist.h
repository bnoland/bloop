#ifndef DYNLIST_H_
#define DYNLIST_H_

#include <stddef.h>

// XXX: Can we mimic C++ templates using `_Generic` and preprocessor
// shenanigans?
// XXX: Rather crude API. Only accepts *pointers* to data.

typedef struct
{
  char* buffer;
  size_t size;
  size_t capacity;
  size_t type_size;  // Size of element type in bytes
} DynList;

void dyn_list_initialize(DynList* list, size_t type_size);
void dyn_list_free(DynList* list);
void dyn_list_add(DynList* list, void* data_ptr);
void* dyn_list_at(DynList* list, size_t index);

#endif
