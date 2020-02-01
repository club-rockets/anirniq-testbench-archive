#include "cmsis_os.h"
#include "gpio.h"

#include "stdio.h"

#include "main.h"
#include "app_lcd.h"
#include "lcd.h"


  lcd_struct_t lcd1 = {
		  .dataPorts = {0,0,0,0,LCD_D4_GPIO_Port,LCD_D5_GPIO_Port,LCD_D6_GPIO_Port,LCD_D7_GPIO_Port},
		  .dataPins = {0,0,0,0,LCD_D4_Pin,LCD_D5_Pin,LCD_D6_Pin,LCD_D7_Pin},
		  .rsPort = LCD_RS_GPIO_Port,
		  .rsPin = LCD_RS_Pin,
		  .rwPort = LCD_RW_GPIO_Port,
		  .rwPin = LCD_RW_Pin,
		  .ePort = LCD_E_GPIO_Port,
		  .ePin = LCD_E_Pin,
		  .interfaceSize = lcd_size4
  };


void tsk_lcd(void*arg)
{
	osDelay(100);
	lcd_init(&lcd1);
	lcd_write(&lcd1,lcd_data,'a');

	lcd_setMode(&lcd1,1,0,0);
	uint8_t buff[30] = {0};

	float load1 = 0;
	float load2 = 0;
	float load3 = 0;

	lcd_clear(&lcd1);
	lcd_setCursor(&lcd1,0,0);
	lcd_writeString(&lcd1,"load 1 :        g");
	lcd_setCursor(&lcd1,0,1);
	lcd_writeString(&lcd1,"load 2 :        g");

	while (1) {
    	osDelay(500);
    	loadcellGet(&load1,&load2,&load3);

    	sprintf(buff,"%06lu",(uint32_t)(load1 * 1000.0));
        lcd_setCursor(&lcd1,9,0);
        lcd_writeString(&lcd1,buff);

    	sprintf(buff,"%06u",(uint32_t)(load2 * 1000.0));
        lcd_setCursor(&lcd1,9,1);
        lcd_writeString(&lcd1,buff);


    }
}
