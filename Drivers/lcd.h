#ifndef _LCD_H_
#define _LCD_H_


typedef struct {

	GPIO_TypeDef* dataPorts[8];
	uint16_t dataPins[8];

	GPIO_TypeDef* rsPort;
	uint16_t rsPin;

	GPIO_TypeDef* rwPort;
	uint16_t rwPin;

	GPIO_TypeDef* ePort;
	uint16_t ePin;

	enum{lcd_size4,lcd_size8} interfaceSize;
	enum{lcd_lastOpWrite,lcd_lastOpread} lastOp;



} lcd_struct_t;

enum lcd_opType{lcd_data,lcd_command};

void lcd_init(lcd_struct_t* lcd);

void lcd_setMode(lcd_struct_t* lcd, uint32_t onOff,uint32_t blink, uint32_t cursor);

void lcd_write(lcd_struct_t* lcd,enum lcd_opType op,uint8_t data);

void lcd_setCursor(lcd_struct_t* lcd,uint8_t x,uint8_t y);

void lcd_clear(lcd_struct_t* lcd);

void lcd_writeString(lcd_struct_t* lcd,uint8_t* str);

#endif //_LCD_H_
