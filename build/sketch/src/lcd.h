#line 1 "c:\\Users\\HDP\\Desktop\\temp\\projects\\mhamad feris\\src\\lcd.h"
#pragma once

#include "config.h"
#include <LiquidCrystal.h>

// #define show_serial_debug

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_NSS, LCD_MOSI, LCD_MISO, LCD_SCK);

int lcd_pos = 0;
#define LCD_BUFFER_SIZE (LCD_COLS * LCD_ROWS)
char lcd_buffer[LCD_BUFFER_SIZE];

void lcd_clean();

void lcd_init()
{
	lcd.begin(LCD_COLS, LCD_ROWS);
	lcd.noBlink();
	lcd.noCursor();

	lcd_clean();
}

void lcd_clean()
{
	lcd_pos = 0;
	lcd.clear();
	lcd.setCursor(0, 0);

#ifdef show_serial_debug
	for (int i = 0; i < LCD_COLS * LCD_ROWS; i++)
	{
		lcd_buffer[i] = ' ';
	}
#endif
}

void lcd_set_cursor(int x, int y)
{
	lcd.setCursor(x, y);
	lcd_pos = y * LCD_COLS + x;
}

void lcd_print(char *str)
{
	lcd.print(str);

#ifdef show_serial_debug
	char *p = str;
	while (*p != '\0' && lcd_pos < LCD_BUFFER_SIZE)
	{
		lcd_buffer[lcd_pos++] = *p++;
	}
#endif
}

void lcd_print(int val)
{
	lcd_print((char *)String(val).c_str());
}

void lcd_flush()
{
#ifdef show_serial_debug
	Serial.println("\nLCD:");
	Serial.write(lcd_buffer, LCD_COLS);
	Serial.println();
	Serial.write(lcd_buffer + LCD_COLS, LCD_COLS);
	Serial.println();
#endif
}