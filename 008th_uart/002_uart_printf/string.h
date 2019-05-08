#ifndef __STRING_H
#define   __STRING_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void    *memset(void *, int, size_t);
void    *memcpy(void *, const void *, size_t);




#ifdef __cplusplus
}
#endif

#endif /* __STRING_H */
