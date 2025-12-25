#include <stdint.h>
#include <stdbool.h>
#include <stdio.h> // sprintf fonksiyonu icin
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "lcd.h"

int main(void) {
    // Degiskenler
    uint32_t adc_degeri[1]; // ADC'den okunan  veri (0 - 4095)
    float voltaj;           // Pindeki voltaj
    float sicaklik;         // Hesaplanan sicaklik (Celcius)
    char ekran_buffer[16];  // LCD tamponu

    // 1. Saat Ayari: 50 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // 2. LCD Baslatma
    LCD_Init();
    LCD_Temizle();
    LCD_Git(1, 1);
    LCD_Yaz("Omer's home temp:");

    // 3. ADC Modulu Kurulumu (PE3 -> AIN0)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    // ADC Sequencer 3 Ayari (Tek okuma)
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);

    while(1) {
        // --- OKUMA ---
        ADCProcessorTrigger(ADC0_BASE, 3);
        while(!ADCIntStatus(ADC0_BASE, 3, false)) {}
        ADCIntClear(ADC0_BASE, 3);
        ADCSequenceDataGet(ADC0_BASE, 3, adc_degeri);

        // --- HESAPLAMA (LM35 Formulasyonu) ---
        // 1. Adım: ADC verisini Voltaja cevir (Referans 3.3V)
        voltaj = (adc_degeri[0] * 3.3f) / 4095.0f;

        // 2. Adım: Voltaji Sicakliga cevir
        // LM35 her 10mV (0.01V) icin 1 Derece verir.
        // Sicaklik = Voltaj / 0.01
        sicaklik = voltaj * 100.0f; 

        // --- EKRANA YAZDIRMA ---
        // Ekrana "Temp: 25.4 C" gibi yazar
        sprintf(ekran_buffer, "Temp: %.1f C   ", sicaklik);
        
        LCD_Git(2, 1); 
        LCD_Yaz(ekran_buffer);

        // Okuma hizini ayarla (Saniyede 2-3 kere guncellesin)
        SysCtlDelay(SysCtlClockGet() / 3 * 0.5); 
    }
}