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
	
	if ((NULL == dest) || (NULL == src)) return NULL;
	
	while (count--) *tmp_dest++ = *tmp_src++;
	
	return dest;
}

/********************************************** END OF FLEE **********************************************/
