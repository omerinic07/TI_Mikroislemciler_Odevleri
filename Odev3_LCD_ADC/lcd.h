#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

// Port ve Pin Tanımlamaları
// RS -> PA2, E -> PA3
#define LCD_CTRL_PORT_BASE  GPIO_PORTA_BASE
#define LCD_RS_PIN          GPIO_PIN_2
#define LCD_E_PIN           GPIO_PIN_3

// D4..D7 -> PB4..PB7
#define LCD_DATA_PORT_BASE  GPIO_PORTB_BASE
#define LCD_DATA_PINS       (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)

// Fonksiyon Prototipleri
void LCD_Init(void);                // LCD'yi başlatır
void LCD_Komut(unsigned char cmd);  // Komut gönderir (Örn: Ekranı temizle)
void LCD_Veri(unsigned char data);  // Karakter yazar
void LCD_Temizle(void);             // Ekranı siler
void LCD_Yaz(char *str);            // String (kelime) yazar
void LCD_Git(int satir, int sutun); // İmleci konumlandırır (1 veya 2. satır)

#endif /* LCD_H_ */