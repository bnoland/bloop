#include "dynlist.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

void dyn_list_init(DynList* list, size_t type_size)
{
  const size_t initial_capacity = 32;
  list->buffer = malloc(initial_capacity * type_size);
  list->size = 0;
  list->capacity = initial_capacity;
  list->type_size = type_size;
}

void dyn_list_destroy(DynList* list)
{
  free(list->buffer);
  list->buffer = NULL;
  list->size = 0;
  list->capacity = 0;
  list->type_size = 0;
}

void* dyn_list_add(DynList* list, const void* data_ptr)
{
  void* dest = dyn_list_add_slot(list);
  memcpy(dest, data_ptr, list->type_size);
  return dest;
}

void* dyn_list_add_slot(DynList* list)
{
  if (list->size >= list->capacity) {
    const size_t new_capacity = 2 * list->capacity;
    const size_t num_bytes = new_capacity * list->type_size;
    list->buffer = realloc(list->buffer, num_bytes);
    list->capacity = new_capacity;
  }

  const size_t offset = list->size * list->type_size;
  unsigned char* dest = list->buffer + offset;
  list->size++;

  return dest;
}

const void* dyn_list_at(const DynList* list, size_t index)
{
  assert(index < list->size);
  const size_t offset = index * list->type_size;
  return list->buffer + offset;
}

bool dyn_list_search(const DynList* list, const void* value, size_t* index, bool equal(const void*, const void*))
{
  for (size_t i = 0; i < list->size; i++) {
    if (equal(dyn_list_at(list, i), value)) {
      *index = i;
      return true;
    }
  }
  return false;
}
