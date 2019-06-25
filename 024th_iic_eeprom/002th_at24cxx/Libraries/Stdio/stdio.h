#ifndef __STDIO_H
#define   __STDIO_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void    standard_putchar_callback(int (*__putchar)(int cByte));

#define printf		printf_
int     printf_(const char* format, ...);

#define printfln	printfln_
int     printfln_(const char* format, ...);

#define printfnull
int     printfnull_(const char* format, ...);

#define sprintf	sprintf_
int     sprintf_(char* buffer, const char* format, ...);

#define snprintf	snprintf_
int     snprintf_(char* buffer, size_t count, const char* format, ...);






#ifdef __cplusplus
}
#endif

#endif /* __STDIO_H */
