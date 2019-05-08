#ifndef __LCD_CONTROLLER_H
#define   __LCD_CONTROLLER_H

#include "s3c2440_soc.h"
#include "lcd.h"

/* LCD 控制方法 */
typedef struct lcd_controller
{
	char				*name;
	void				(*init)(p_lcd_params plcdparams);
	void				(*enable)(void);
	void				(*disable)(void);
	void				(*init_palette)(void);
}lcd_controller, *p_lcd_controller;

#endif /* __LCD_CONTROLLER_H */
