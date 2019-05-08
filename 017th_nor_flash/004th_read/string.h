#ifndef __STRING_H
#define   __STRING_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void    string_putchar_callback(int (*__putchar)(int ));
void    string_getchar_callback(int (*__getchar)(void));

unsigned int    S3C2440_StringGetUint(void);									//从标准输入获取 unsigned int 值
signed int      S3C2440_StringGetSint(void);									//从标准输入获取 signed int   值

void    *memset(void *, int, size_t);
void    *memcpy(void *, const void *, size_t);
void    *memchr(const void *, int, size_t);
void    *memccpy(void *, const void *, int, size_t);
void    *memmove(void *, const void *, size_t);





#ifdef __cplusplus
}
#endif

#endif /* __STRING_H */
