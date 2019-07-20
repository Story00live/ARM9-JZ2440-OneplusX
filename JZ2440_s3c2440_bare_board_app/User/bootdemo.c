/**
  *********************************************************************************************************
  * @file    bootdemo.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-07-19
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *					 
  *					 
  *********************************************************************************************************
  */

#include "bootdemo.h"
#include "setup.h"
#include "led.h"
#include "key.h"
#include "uart.h"

static struct tag * params;

/**********************************************************************************************************
 @Function			void S3C2440_Boot_Setup_startTag(void)
 @Description			S3C2440_Boot_Setup_startTag
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_Boot_Setup_startTag(void)
{
	params = (struct tag *) 0x30000100;
	
	params->hdr.tag		= ATAG_CORE;
	params->hdr.size		= tag_size( tag_core );
	
	params->u.core.flags	= 0;
	params->u.core.pagesize	= 0;
	params->u.core.rootdev	= 0;
	
	params = tag_next( params );
}

/**********************************************************************************************************
 @Function			void S3C2440_Boot_Setup_memoryTag(void)
 @Description			S3C2440_Boot_Setup_memoryTag
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_Boot_Setup_memoryTag(void)
{
	params->hdr.tag		= ATAG_MEM;
	params->hdr.size		= tag_size( tag_mem32 );
	
	params->u.mem.start		= 0x30000000;
	params->u.mem.size		= 64 * 1024 * 1024;
	
	params = tag_next( params );
}

/**********************************************************************************************************
 @Function			void S3C2440_Boot_Setup_commandlineTag(char* cmdline)
 @Description			S3C2440_Boot_Setup_commandlineTag
 @Input				cmdline
 @Return				void
**********************************************************************************************************/
void S3C2440_Boot_Setup_commandlineTag(char* cmdline)
{
	int len = strlen(cmdline) + 1;
	
	params->hdr.tag		= ATAG_CMDLINE;
	params->hdr.size		= (sizeof(struct tag_header) + len + 3) >> 2;
	
	strcpy(params->u.cmdline.cmdline, cmdline);
	
	params = tag_next( params );
}

/**********************************************************************************************************
 @Function			void S3C2440_Boot_Setup_endTag(void)
 @Description			S3C2440_Boot_Setup_endTag
 @Input				void
 @Return				void
**********************************************************************************************************/
void S3C2440_Boot_Setup_endTag(void)
{
	params->hdr.tag		= ATAG_NONE;
	params->hdr.size		= 0;
}

/********************************************** END OF FLEE **********************************************/
