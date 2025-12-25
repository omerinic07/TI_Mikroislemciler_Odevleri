#include <stdint.h>
import tkinter as tk
from tkinter import messagebox
import serial
import serial.tools.list_ports
import threading

# --- AYARLAR ---
BAUDRATE = 9600

class LEDKontrolUygulamasi:
    def __init__(self, root):
        self.root = root
        self.root.title("Tiva C LED Kontrol Paneli")
        self.root.geometry("300x250")
        
        self.ser = None # Seri port degiskeni

        # 1. Port Secimi
        tk.Label(root, text="COM Port Seçiniz:").pack(pady=5)
        self.port_listesi = [port.device for port in serial.tools.list_ports.comports()]
        self.secili_port = tk.StringVar(root)
        if self.port_listesi:
            self.secili_port.set(self.port_listesi[0])
            self.port_menu = tk.OptionMenu(root, self.secili_port, *self.port_listesi)
            self.port_menu.pack()
        else:
            self.secili_port.set("Port Bulunamadı")
            tk.Label(root, text="Lütfen cihazı bağlayın.").pack()

        # 2. Baglan Butonu
        self.btn_baglan = tk.Button(root, text="Bağlan", command=self.baglan, bg="lightgray")
        self.btn_baglan.pack(pady=5)

        # 3. Kontrol Butonlari (Baslangicta pasif)
        self.btn_ac = tk.Button(root, text="LED AÇ (Gönder: 1)", command=lambda: self.veri_gonder('1'), state="disabled", bg="green", fg="white")
        self.btn_ac.pack(pady=10, fill="x", padx=20)

        self.btn_kapat = tk.Button(root, text="LED KAPAT (Gönder: 0)", command=lambda: self.veri_gonder('0'), state="disabled", bg="red", fg="white")
        self.btn_kapat.pack(pady=0, fill="x", padx=20)

        # 4. Durum Etiketi
        self.lbl_durum = tk.Label(root, text="Durum: Bağlantı Yok", fg="red")
        self.lbl_durum.pack(pady=20)

    def baglan(self):
        port = self.secili_port.get()
        try:
            self.ser = serial.Serial(port, BAUDRATE, timeout=1)
            self.lbl_durum.config(text=f"Bağlandı: {port}", fg="green")
            self.btn_baglan.config(state="disabled")
            self.btn_ac.config(state="normal")
            self.btn_kapat.config(state="normal")
        except Exception as e:
            messagebox.showerror("Hata", f"Bağlantı hatası:\n{str(e)}")

    def veri_gonder(self, veri):
        if self.ser and self.ser.is_open:
            self.ser.write(veri.encode())
            print(f"Gönderildi: {veri}")
        else:
            messagebox.showwarning("Uyarı", "Bağlantı koptu!")

# --- ANA PROGRAM ---
if __name__ == "__main__":
    pencere = tk.Tk()
    uygulama = LEDKontrolUygulamasi(pencere)
    pencere.mainloop()
}