#include <stdint.h>
#include <stdbool.h>
#include <stdio.h> // sprintf icin gerekli
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h" // Kesme vektor tanimlamalari
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "lcd.h"

// Saat degiskenleri
volatile uint32_t saniye = 0;
volatile uint32_t dakika = 0;
volatile uint32_t saat = 0;

// Ekranin guncellenmesi gerektigini soyleyen bayrak
volatile bool ekran_guncelle = false;

// Timer0 Kesme Fonksiyonu (Her 1 saniyede bir otomatik cagrilir)
void Timer0IntHandler(void) {
    // Kesme bayragini temizle (Cok onemli!)
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Zamani ilerlet
    saniye++;
    if(saniye >= 60) {
        saniye = 0;
        dakika++;
        if(dakika >= 60) {
            dakika = 0;
            saat++;
            if(saat >= 24) {
                saat = 0;
            }
        }
    }

    // Ana donguye ekranin yenilenmesi gerektigini bildir
    ekran_guncelle = true;
}

int main(void) {
    char zaman_buffer[16]; // Ekrana yazilacak metin icin tampon

    // 1. Saat Ayari: 50 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // 2. LCD'yi Baslat
    LCD_Init();
    LCD_Temizle();
    LCD_Git(1, 1);
    LCD_Yaz("Dijital Saat");

    // 3. Timer0 Kurulumu (Periyodik Mod)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    // Timer Periyodu Ayari: 1 Saniye
    // Sistem hizi (50MHz) = 1 saniyede 50.000.000 dongu
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());

    // 4. Kesme Ayarlari
    // Timer0 icin interrupt kaydi yapiliyor (startup dosyasini editlememek icin dinamik kayit)
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
    
    // Kesmeleri aktif et
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable(); // Islemci genel kesmelerini ac

    // Timer'i baslat
    TimerEnable(TIMER0_BASE, TIMER_A);

    while(1) {
        // Islemci burada baska isler yapabilir, surekli saati kontrol etmez.
        // Sadece 'ekran_guncelle' bayragi 1 oldugunda LCD'ye yazar.
        
        if(ekran_guncelle) {
            // sprintf ile sayilari "SS:DD:SN" formatina cevir (0 dolgulu)
            // %02d -> Sayi tek haneli ise basina 0 koyar (Orn: 5 -> 05)
            sprintf(zaman_buffer, "%02d:%02d:%02d", saat, dakika, saniye);

            LCD_Git(2, 4); // 2. Satir, ortalara dogru
            LCD_Yaz(zaman_buffer);

            ekran_guncelle = false; // Bayragi indir, bir sonraki saniyeyi bekle
        }
    }
}