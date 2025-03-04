/*
 * backlight_ctrl.h
 *
 *  Created on: Mar 4, 2025
 *      Author: molnar
 */

#ifndef ILI9341_INC_BACKLIGHT_CTRL_H_
#define ILI9341_INC_BACKLIGHT_CTRL_H_

void LCD_BacklightInit_v(void);
void LCD_BacklightEnable_v(void);
void LCD_BacklightDisable_v(void);
void LCD_BacklightSetBrightness_v(f32 brightness__per_cent__f32);

#endif /* ILI9341_INC_BACKLIGHT_CTRL_H_ */
