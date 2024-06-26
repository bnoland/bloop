#ifndef DYNLIST_H_
#define DYNLIST_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
  unsigned char* buffer;
  size_t size;
  size_t capacity;
  size_t type_size;  // Size of element type in bytes
} DynList;

DynList dyn_list_make(size_t type_size);
void dyn_list_destroy(DynList* list);
void* dyn_list_add(DynList* list, const void* data_ptr);
void* dyn_list_add_slot(DynList* list);
void* dyn_list_mutable_at(DynList* list, size_t index);
const void* dyn_list_at(const DynList* list, size_t index);
bool dyn_list_search(const DynList* list, const void* value, size_t* index, bool equal(const void*, const void*));

#endif
