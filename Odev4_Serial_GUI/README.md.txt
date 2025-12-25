# Ödev 4: Seri Port LED Kontrol Arayüzü

Bu proje, Python ve Tkinter kütüphanesi kullanılarak geliştirilmiş bir PC arayüzü ile Tiva C üzerindeki Mavi LED'in (PF2) kontrolünü sağlar.

## Çalışma Mantığı
1. **Bağlantı:** Kullanıcı programı açar ve Tiva C'nin bağlı olduğu COM portunu seçer.
2. **Veri Gönderimi:**
   - "LED AÇ" butonuna basıldığında seri port üzerinden **'1'** karakteri gönderilir.
   - "LED KAPAT" butonuna basıldığında seri port üzerinden **'0'** karakteri gönderilir.
3. **Tiva C Tarafı:** Mikrodenetleyici UART0 üzerinden gelen karakteri dinler. '1' gelirse PF2 pinini HIGH, '0' gelirse LOW yapar.

## Kurulum
Python kodunu çalıştırmak için terminale şu komutu yazarak kütüphaneyi yükleyin:
`pip install pyserial`

## Kullanılan Teknolojiler
- **Dil:** Python 3.x
- **Kütüphane:** Tkinter (GUI), Pyserial (Haberleşme)
- **Haberleşme Hızı (Baudrate):** 9600