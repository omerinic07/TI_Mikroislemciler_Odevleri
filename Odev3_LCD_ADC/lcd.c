#include "lcd.h"

// Mikrosaniye cinsinden bekleme fonksiyonu (Yaklaşık)
// SysCtlDelay(x) -> x * 3 döngü harcar. 50MHz'de 1us yaklaşık 16 döngüdür.
void DelayUs(uint32_t us) {
    SysCtlDelay(us * (SysCtlClockGet() / 3 / 1000000));
}

// LCD'ye 4-bit (Nibble) veri gönderme tetikleyicisi
void LCD_Pulse_Enable(void) {
    GPIOPinWrite(LCD_CTRL_PORT_BASE, LCD_E_PIN, LCD_E_PIN); // E = 1
    DelayUs(10);
    GPIOPinWrite(LCD_CTRL_PORT_BASE, LCD_E_PIN, 0);         // E = 0
    DelayUs(10);
}

// 4-bit modunda veri gönderme mekanizması
void LCD_Byte_Gonder(unsigned char data, unsigned char type) {
    // RS pini ayarla: type=0 -> Komut, type=1 -> Veri
    if (type == 0)
        GPIOPinWrite(LCD_CTRL_PORT_BASE, LCD_RS_PIN, 0);
    else
        GPIOPinWrite(LCD_CTRL_PORT_BASE, LCD_RS_PIN, LCD_RS_PIN);

    // Üst 4 bit (High Nibble) gönderiliyor
    GPIOPinWrite(LCD_DATA_PORT_BASE, LCD_DATA_PINS, (data & 0xF0));
    LCD_Pulse_Enable();

    // Alt 4 bit (Low Nibble) gönderiliyor (PB4-PB7'ye kaydırılarak)
    GPIOPinWrite(LCD_DATA_PORT_BASE, LCD_DATA_PINS, ((data & 0x0F) << 4));
    LCD_Pulse_Enable();

    DelayUs(50); // İşlem süresi beklemesi
}

void LCD_Komut(unsigned char cmd) {
    LCD_Byte_Gonder(cmd, 0);
    if(cmd < 4) DelayUs(2000); // Clear ve Return Home komutları uzun sürer
}

void LCD_Veri(unsigned char data) {
    LCD_Byte_Gonder(data, 1);
}

void LCD_Init(void) {
    // Portları Aktif Et
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Pinleri Çıkış Olarak Ayarla
    GPIOPinTypeGPIOOutput(LCD_CTRL_PORT_BASE, LCD_RS_PIN | LCD_E_PIN);
    GPIOPinTypeGPIOOutput(LCD_DATA_PORT_BASE, LCD_DATA_PINS);

    DelayUs(50000); // Açılış beklemesi (>15ms)

    // Başlangıç Resetleme Prosedürü (Magic Sequence)
    GPIOPinWrite(LCD_CTRL_PORT_BASE, LCD_RS_PIN, 0);
    
    // 3 kez 0x30 göndererek resetle
    GPIOPinWrite(LCD_DATA_PORT_BASE, LCD_DATA_PINS, 0x30); LCD_Pulse_Enable(); DelayUs(5000);
    GPIOPinWrite(LCD_DATA_PORT_BASE, LCD_DATA_PINS, 0x30); LCD_Pulse_Enable(); DelayUs(150);
    GPIOPinWrite(LCD_DATA_PORT_BASE, LCD_DATA_PINS, 0x30); LCD_Pulse_Enable();

    // 4-bit moduna geçiş
    GPIOPinWrite(LCD_DATA_PORT_BASE, LCD_DATA_PINS, 0x20); LCD_Pulse_Enable();

    // LCD Ayarları
    LCD_Komut(0x28); // 4-bit, 2 satır, 5x7 font
    LCD_Komut(0x06); // Yazdıkça imleci sağa kaydır
    LCD_Komut(0x0C); // Ekran AÇIK, İmleç KAPALI, Yanıp sönme KAPALI
    LCD_Temizle();   // Ekranı temizle
}

void LCD_Temizle(void) {
    LCD_Komut(0x01);
}

void LCD_Yaz(char *str) {
    while(*str) {
        LCD_Veri(*str++);
    }
}

void LCD_Git(int satir, int sutun) {
    // 1. Satır: 0x80, 2. Satır: 0xC0
    if (satir == 1)
        LCD_Komut(0x80 + (sutun - 1));
    else if (satir == 2)
        LCD_Komut(0xC0 + (sutun - 1));
}