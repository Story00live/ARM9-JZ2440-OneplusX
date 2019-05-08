/**
  *********************************************************************************************************
  * @file    string.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-03-03
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "s3c2440_soc.h"

#define NULL 			((void *)0)

#define _U			0x01												/* upper */
#define _L			0x02												/* lower */
#define _D			0x04												/* digit */
#define _C			0x08												/* cntrl */
#define _P			0x10												/* punct */
#define _S			0x20												/* white space (space/lf/tab) */
#define _X			0x40												/* hex digit */
#define _SP			0x80												/* hard space (0x20) */

unsigned char _ctype[] = {
	_C,_C,_C,_C,_C,_C,_C,_C,												/* 0-7 */
	_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,									/* 8-15 */
	_C,_C,_C,_C,_C,_C,_C,_C,												/* 16-23 */
	_C,_C,_C,_C,_C,_C,_C,_C,												/* 24-31 */
	_S|_SP,_P,_P,_P,_P,_P,_P,_P,											/* 32-39 */
	_P,_P,_P,_P,_P,_P,_P,_P,												/* 40-47 */
	_D,_D,_D,_D,_D,_D,_D,_D,												/* 48-55 */
	_D,_D,_P,_P,_P,_P,_P,_P,												/* 56-63 */
	_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,								/* 64-71 */
	_U,_U,_U,_U,_U,_U,_U,_U,												/* 72-79 */
	_U,_U,_U,_U,_U,_U,_U,_U,												/* 80-87 */
	_U,_U,_U,_P,_P,_P,_P,_P,												/* 88-95 */
	_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,								/* 96-103 */
	_L,_L,_L,_L,_L,_L,_L,_L,												/* 104-111 */
	_L,_L,_L,_L,_L,_L,_L,_L,												/* 112-119 */
	_L,_L,_L,_P,_P,_P,_P,_C,												/* 120-127 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,										/* 128-143 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,										/* 144-159 */
	_S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,						/* 160-175 */
	_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,							/* 176-191 */
	_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,							/* 192-207 */
	_U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,							/* 208-223 */
	_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,							/* 224-239 */
	_L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L							/* 240-255 */
};

#define __ismask(x)		(_ctype[(int)(unsigned char)(x)])

#define isalnum(c)		((__ismask(c)&(_U|_L|_D))        != 0)
#define isalpha(c)		((__ismask(c)&(_U|_L))           != 0)
#define iscntrl(c)		((__ismask(c)&(_C))              != 0)
#define isdigit(c)		((__ismask(c)&(_D))              != 0)
#define isgraph(c)		((__ismask(c)&(_P|_U|_L|_D))     != 0)
#define islower(c)		((__ismask(c)&(_L))              != 0)
#define isprint(c)		((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c)		((__ismask(c)&(_P))              != 0)
#define isspace(c)		((__ismask(c)&(_S))              != 0)
#define isupper(c)		((__ismask(c)&(_U))              != 0)
#define isxdigit(c)		((__ismask(c)&(_D|_X))           != 0)

#define isblank(c)		(c == ' ' || c == '\t')

#define isascii(c)		(((unsigned char)(c))<=0x7F)
#define toascii(c)		(((unsigned char)(c)) &0x7F)

static inline unsigned char __tolower(unsigned char c)
{
	if (isupper(c)) c -= 'A'-'a';
	return c;
}

static inline unsigned char __toupper(unsigned char c)
{
	if (islower(c)) c -= 'a'-'A';
	return c;
}

#define tolower(c)		__tolower(c)
#define toupper(c)		__toupper(c)

typedef int (*string_putchar)(int );
typedef int (*string_getchar)(void);

string_putchar _string_putchar = NULL;
string_getchar _string_getchar = NULL;

/**********************************************************************************************************
 @Function			static unsigned long simple_strtoul(const char* cp, char** endp, unsigned int base)
 @Description			simple_strtoul
 @Input				
 @Return				
**********************************************************************************************************/
static unsigned long simple_strtoul(const char* cp, char** endp, unsigned int base)
{
	unsigned long result = 0, value;
	
	if (*cp == '0') {
		cp++;
		if ((*cp == 'x' || *cp == 'X') && isxdigit(cp[1])) {
			base = 16;
			cp++;
		}
		if (!base) {
			base = 8;
		}
	}
	if (!base) {
		base = 10;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp) ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp) *endp = (char *)cp;
	
	return result;
}

/**********************************************************************************************************
 @Function			static signed long simple_strtol(const char*cp, char** endp, unsigned int base)
 @Description			simple_strtol
 @Input				
 @Return				
**********************************************************************************************************/
static signed long simple_strtol(const char*cp, char** endp, unsigned int base)
{
	if (*cp == '-') return -simple_strtoul(cp+1, endp, base);
	
	return simple_strtoul(cp, endp, base);
}

/**********************************************************************************************************
 @Function			static char *string_gets(char s[])
 @Description			string_gets
 @Input				
 @Return				
**********************************************************************************************************/
static char *string_gets(char s[])
{
	int i = 0;
	char c;
	
	while (1) {
		c = _string_getchar();
		_string_putchar(c);
		if (c == '\n')
			_string_putchar('\r');
		else if (c == '\r')
			_string_putchar('\n');
		if ((c == '\n') || (c == '\r')) {
			s[i] = '\0';
			break;
		}
		else {
			s[i++] = c;
		}
	}
	
	return s;
}

/**********************************************************************************************************
 @Function			unsigned int S3C2440_StringGetUint(void)
 @Description			S3C2440_StringGetUint		: 从标准输入获取 unsigned int 值
 @Input				void
 @Return				unsigned int
**********************************************************************************************************/
unsigned int S3C2440_StringGetUint(void)
{
	char str[100];
	string_gets(str);
	return simple_strtoul(str, NULL, 0);
}

/**********************************************************************************************************
 @Function			signed int S3C2440_StringGetSint(void)
 @Description			S3C2440_StringGetSint		: 从标准输入获取 signed int 值
 @Input				void
 @Return				signed int
**********************************************************************************************************/
signed int S3C2440_StringGetSint(void)
{
	char str[100];
	string_gets(str);
	return simple_strtol(str, NULL, 0);
}

/**********************************************************************************************************
 @Function			void string_putchar_callback(int (*__putchar)(int ))
 @Description			string_putchar_callback
 @Input				__putchar
 @Return				void
**********************************************************************************************************/
void string_putchar_callback(int (*__putchar)(int ))
{
	_string_putchar = __putchar;
}

/**********************************************************************************************************
 @Function			void string_getchar_callback(int (*__getchar)(void))
 @Description			string_getchar_callback
 @Input				__getchar
 @Return				void
**********************************************************************************************************/
void string_getchar_callback(int (*__getchar)(void))
{
	_string_getchar = __getchar;
}

/**********************************************************************************************************
 @Function			void *memset(void* s, int c, size_t n)
 @Description			memset
 @Input				
 @Return				
**********************************************************************************************************/
void *memset(void* dest, int c, size_t n)
{
	char* tmp_dest = (char *)dest;
	
	if (NULL == dest || n < 0) return NULL;
	
	while (n-- > 0) *tmp_dest++ = c;
	
	return dest;
}

/**********************************************************************************************************
 @Function			void *memcpy(void* dest, const void* src, size_t count)
 @Description			memcpy
 @Input				
 @Return				
**********************************************************************************************************/
void *memcpy(void* dest, const void* src, size_t count)
{
	char* tmp_dest = (char *)dest;
	const char* tmp_src = (const char *)src;
	
	if ((NULL == dest) || (NULL == src) || (count < 0)) return NULL;
	
	while (count--) *tmp_dest++ = *tmp_src++;
	
	return dest;
}

/**********************************************************************************************************
 @Function			void *memchr(const void* src, int ch, size_t count)
 @Description			memchr
 @Input				
 @Return				
**********************************************************************************************************/
void *memchr(const void* src, int ch, size_t count)
{
	const char* tmp_src = (const char *)src;
	
	if ((NULL == src) || (count < 0)) return NULL;
	
	while (*tmp_src++ != '\0' && count) {
		if (*tmp_src - ch == 0) return (void *)tmp_src;
		--count;
	}
	
	return NULL;
}

/**********************************************************************************************************
 @Function			void *memccpy(void* dest, const void* src, int ch, size_t count)
 @Description			memccpy
 @Input				
 @Return				
**********************************************************************************************************/
void *memccpy(void* dest, const void* src, int ch, size_t count)
{
	void* result = NULL;
	char* tmp_dest = (char *)dest;
	const char* tmp_src = (const char *)src;
	
	if ((NULL == dest) || (NULL == src) || (count < 0)) return NULL;
	
	while (count--) {
		*tmp_dest++ = *tmp_src;
		if (*tmp_src - ch == 0) break;
		tmp_src++;
	}
	
	if (0 < count) result = tmp_dest; else result = NULL;
	
	return result;
}

/**********************************************************************************************************
 @Function			void *memmove(void* dest, const void* src, size_t count)
 @Description			memmove
 @Input				
 @Return				
**********************************************************************************************************/
void *memmove(void* dest, const void* src, size_t count)
{
	char* tmp_dest = (char *)dest;
	const char* tmp_src = (const char *)src;
	
	if ((NULL == dest) || (NULL == src) || (count < 0)) return NULL;
	
	if ((dest < src) || ((char*)dest >= (char*)src + count)) {
		while (count--) *tmp_dest++ = *tmp_src++;
	}
	else {
		tmp_src += count - 1;
		tmp_dest += count - 1;
		while (count--) *tmp_dest-- = *tmp_src--;
	}
	
	return dest;
}

/********************************************** END OF FLEE **********************************************/
