#include <stdio.h>
#include <string.h>

#define ARENA_IMPLEMENTATION
#include "arena.h"

#define KB(a) a*1024
#define MB(a) KB(a)*1024
#define GB(a) MB(a)*1024


#define STRLIT "Hello, World!"
#define STRLIT2 "Bye, World!"

int main() {
  arena_t *arena = ainit(MB(4));
  if (!arena) return 100;

  char *str = aalloc(arena, strlen(STRLIT)+1);
  if (!str) return 1;
  memcpy(str, STRLIT, strlen(STRLIT)+1);
  printf("%p: %s\n", str, str);
  afree(str);

  str = aalloc(arena, strlen(STRLIT2)+1);
  if (!str) return 2;
  memcpy(str, STRLIT2, strlen(STRLIT2)+1);
  printf("%p: %s\n", str, str);
  afree(str);

  str = aalloc(arena, strlen(STRLIT)+1);
  if (!str) return 3;
  memcpy(str, STRLIT, strlen(STRLIT)+1);
  printf("%p: %s\n", str, str);
  char *str2 = aalloc(arena, strlen(STRLIT2)+1);;
  if (!str2) return 4;
  memcpy(str2, STRLIT2, strlen(STRLIT2)+1);
  printf("%p: %s\n", str2, str2);
  afree(str);
  afree(str2);

  auninit(arena);
}
