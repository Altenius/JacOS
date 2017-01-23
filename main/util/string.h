#ifndef STRING_H
#define STRING_H
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *string);
void strcat(char *dst, const char *str2);
const char *uitos(uint32_t num); // convert an unsigned int to a string (decimal)
const char *uqtos(uint64_t num); // cover an unsigned quad to a string (decimal)
const char *uitohs(uint32_t num); // convert an unsigned int to a string (hex)
const char *uqtohs(uint64_t num); // convert an unsigned quad to a string (hex)

#endif
