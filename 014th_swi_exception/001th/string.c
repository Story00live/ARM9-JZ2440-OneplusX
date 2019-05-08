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
