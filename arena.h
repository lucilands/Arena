#ifndef __ARENA_H
#define __ARENA_H

typedef unsigned long asize_t;

typedef struct __arena_t arena_t;

arena_t *ainit(asize_t size);
void *aalloc(arena_t *arena, asize_t size);
void *aalloc0(arena_t *arena, asize_t size); // NOTE: This function also zeros the block of memory when allocating
void afree(void *ptr);
void auninit(arena_t *arena);

#ifdef ARENA_IMPLEMENTATION
#include <stdlib.h>
#include <string.h>

#define __SIGN_MASK 1U << (sizeof(signed int) * 8 - 1)

struct __arena_t{
  void *start;
  void *head;
  asize_t size;
};

struct __arena_info_t {
  signed int size;
};

void *__find_free_chunk(arena_t *arena, asize_t size) {
  struct __arena_info_t *arena_cast = (struct __arena_info_t*)arena->start;
  asize_t alloc_size = 0;
  while (arena_cast[0].size < 0 && (asize_t)abs(arena_cast[0].size) >= size) {
    arena_cast = (struct __arena_info_t*)((char*)arena_cast + abs(arena_cast[0].size));
    alloc_size = ((char*)arena_cast - (char*)arena->start);
    if (alloc_size + size + sizeof(struct __arena_info_t) >= arena->size) return NULL;
  }

  if ((asize_t)abs(arena_cast[0].size) > size) {
    struct __arena_info_t *ar_cast = arena_cast;
    ar_cast[0].size = abs(arena_cast[0].size) - size;
  }

  if (alloc_size + size + sizeof(struct __arena_info_t) >= arena->size) return NULL;
  return (void*)arena_cast;
}

arena_t *ainit(asize_t size) {
  if (!size) return NULL;
  arena_t *ret = malloc(sizeof(struct __arena_t));
  if (!ret) return NULL;

  //printf("%lu\n", size);
  ret->start = malloc(size);
  if (!ret->start) return NULL;

  ret->head = ret->start;
  ret->size = size;

  memset(ret->start, 0, size);

  return ret;
}

void *aalloc(arena_t *arena, asize_t size) {
  if (size == 0 || size + sizeof(struct __arena_info_t) > arena->size) return NULL;

  void *ret = (char*)__find_free_chunk(arena, size);
  if (!ret) return NULL;

  struct __arena_info_t *ret_cast = (struct __arena_info_t*)ret;
  ret_cast[0].size = -size;

  ret = (char*)ret + sizeof(struct __arena_info_t);
  return ret;
}

void *aalloc0(arena_t *arena, asize_t size) {
  void *ret = aalloc(arena, size);
  memset(ret, 0, size);

  return ret;
}

void afree(void *ptr) {
  struct __arena_info_t *ptr_cast = (struct __arena_info_t*)((char*)ptr - sizeof(struct __arena_info_t));
  ptr_cast[0].size *= -1;
}

void auninit(arena_t *arena) {
  free(arena->start);
  free(arena);
}

#endif //ARENA_IMPLEMENTATION
#endif //__ARENA_H
