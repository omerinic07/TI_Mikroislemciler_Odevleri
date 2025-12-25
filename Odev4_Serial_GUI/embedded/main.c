#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"

// UART uzerinden metin gonderme fonksiyonu
void UART_Yaz(const char *str) {
    while(*str) {
        UARTCharPut(UART0_BASE, *str++);
    }
}

int main(void) {
    char gelen_veri;

    // 1. Saat Ayari: 50 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // 2. Periferiklerin Aktif Edilmesi
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); // UART0 (USB Kablosu uzerinden)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // UART pinleri icin (PA0, PA1)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // LED icin (PF2 - Mavi LED)

    // 3. Pin Ayarlari
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // LED Pini (PF2) Cikis Yap
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    // 4. UART Konfigurasyonu (9600 Baud, 8 bit, No Parity, 1 Stop bit)
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // Baslangic Mesaji
    UART_Yaz("Tiva C Haberlesme Basladi...\n");

    while(1) {
        // Veri gelmesini bekle (FIFO bos degilse gir)
        if(UARTCharsAvail(UART0_BASE)) {
            
            // Gelen karakteri oku
            gelen_veri = UARTCharGet(UART0_BASE);

            // Gelen veriye gore islem yap
            if(gelen_veri == '1') {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); // LED YAK
                UART_Yaz("LED: ACIK\n");
            }
            else if(gelen_veri == '0') {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); // LED SONDUR
                UART_Yaz("LED: KAPALI\n");
            }
        }
    }
}