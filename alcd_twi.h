/************************************************** 
  CodeVisionAVR C Compiler V3.32+
  (C) 1998-2017 Pavel Haiduc, HP InfoTech S.R.L.

  Universal alphanumeric LCD driver library V1.0
  for the HD44780 LCD controller using connected
  to a Philips PCF8574 I2C I/O expander using TWI.
**************************************************/

#ifndef _ALCD_TWI_INCLUDED_
#define _ALCD_TWI_INCLUDED_

#ifdef _ATXMEGA_DEVICE_
#include <twix.h>
#else
#include <twi.h>
#endif

#include <stdbool.h>

// initialize the LCD controller
// pcf8574_addr specifies the PCF8574 7-bit I2C slave address 
// set by the logic state of the A0, A1 and A2 pins:
// 0x20..0x27 for PCF8574 and 0x38..0x3F for PCF8574A
#if defined(_ATXMEGA_DEVICE_) || defined(_AVR8X_DEVICE_)
// twi_master - pointer to the structure that holds information used by the TWI master
// XMEGA or AVR8X module, used for communication with the PCF8574,
// for performing a TWI bus transaction
void lcd_twi_init(TWI_MASTER_INFO_t *twi_master, unsigned char pcf8574_addr, unsigned char lcd_columns);
#else
void lcd_twi_init(unsigned char pcf8574_addr, unsigned char lcd_columns);
#endif

void _lcd_write_data(unsigned char data);
// read a byte from the LCD character generator or display RAM
unsigned char lcd_read_byte(unsigned char addr);
// write a byte to the LCD character generator or display RAM
void lcd_write_byte(unsigned char addr, unsigned char data);
// set the LCD display position  x=0..39 y=0..3
void lcd_gotoxy(unsigned char x, unsigned char y);
// clear the LCD
void lcd_clear(void);
void lcd_putchar(char c);
// write the string str located in SRAM to the LCD
void lcd_puts(char *str);
// write the string str located in FLASH to the LCD
void lcd_putsf(char flash *str);
// write the string str located in EEPROM to the LCD
void lcd_putse(char eeprom *str);
// displays formatted output
int lcd_printf(flash char *fmtstr,...);
// displays formatted output at the specified x, y coordinates
int lcd_printfxy(unsigned char x, unsigned char y, flash char *fmtstr,...);
// turn LCD backlight on/off
void lcd_backlight(bool on);

#pragma library alcd_twi.lib

#endif

