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
char*           S3C2440_StringGets(char s[]);								//从标准输入获取字符串

void    *memset(void* dest, int c, size_t n);
void    *memcpy(void* dest, const void* src, size_t count);
void    *memchr(const void* src, int ch, size_t count);
int      memcmp(const void* cs, const void* ct, size_t count);
void    *memccpy(void* dest, const void* src, int ch, size_t count);
void    *memmove(void* dest, const void* src, size_t count);

char    *strcpy(char* dest, const char* src);
char    *strncpy(char* dest, const char* src, size_t count);
char    *strcat(char* dest, const char* src);
char    *strncat(char* dest, const char* src, size_t count);
int      strcmp(const char* cs, const char* ct);
int      strncmp(const char* cs, const char* ct, size_t count);
char    *strchr(const char* s, int c);
char    *strrchr(const char* s, int c);
size_t   strlen(const char* s);
size_t   strnlen(const char* s, size_t count);
char    *strstr(const char* s1, const char* s2);

#ifdef __cplusplus
}
#endif

#endif /* __STRING_H */
