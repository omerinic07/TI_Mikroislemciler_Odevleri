#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "lcd.h"

int main(void) {
    // Saat Ayarı: 50 MHz (PLL Kullanarak)
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // LCD Başlat
    LCD_Init();

    // Ekrana Yazı Yaz
    LCD_Git(1, 1);
    LCD_Yaz("omerfaruktz");

    LCD_Git(2, 1);
    LCD_Yaz("7");

    while(1) {
        // Sonsuz döngü (LED yakıp söndürülebilir çalıştığını görmek için)
    }
}