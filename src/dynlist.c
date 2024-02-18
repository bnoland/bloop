#include "dynlist.h"

#include <stdlib.h>
#include <string.h>

void dyn_list_initialize(DynList* list, size_t type_size)
{
  const size_t initial_capacity = 32;
  list->buffer = malloc(initial_capacity * type_size);
  list->size = 0;
  list->capacity = initial_capacity;
  list->type_size = type_size;
}

void dyn_list_free(DynList* list)
{
  free(list->buffer);
  list->buffer = NULL;
  list->size = 0;
  list->capacity = 0;
  list->type_size = 0;
}

void dyn_list_add(DynList* list, void* data_ptr)
{
  if (list->size >= list->capacity) {
    const size_t new_capacity = 2 * list->capacity;
    const size_t num_bytes = new_capacity * list->type_size;
    list->buffer = realloc(list->buffer, num_bytes);
    list->capacity = new_capacity;
  }

  const size_t offset = list->size * list->type_size;
  memcpy(list->buffer + offset, data_ptr, list->type_size);

  list->size++;
}

void* dyn_list_at(DynList* list, size_t index)
{
  const size_t offset = index * list->type_size;
  return list->buffer + offset;
}
