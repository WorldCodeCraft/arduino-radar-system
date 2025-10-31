# 🛰️ Arduino Radar System  

## 🔹 Proje Hakkında  
Bu proje, **HC-SR04 ultrasonik sensör**, **servo motor**, **OLED ekran** ve **LED + buzzer uyarı sistemi** kullanarak gerçek zamanlı bir radar simülasyonu oluşturur.  

- Nesnelerin mesafesini ölçer ve tarama açısına göre OLED ekranda görselleştirir.  
- Tehlikeli mesafeye yaklaşan nesnelerde LED ve buzzer ile uyarı verir.  

**Amaç:** Gömülü sistemler, sensör tabanlı projeler ve gerçek zamanlı görselleştirme becerilerini geliştirmek.

---

## ⚙️ Kullanılan Malzemeler  
- Arduino Uno  
- HC-SR04 Ultrasonik Sensör  
- Servo Motor (180°)  
- OLED Ekran (128x64) SSD1306  
- LED (Kırmızı ve Yeşil)  
- Buzzer  
- Jumper kablolar, breadboard  

---

## 📚 Kullanılan Kütüphaneler  
Arduino IDE’de aşağıdaki kütüphaneler yüklü olmalıdır:  

- `Servo.h` → Servo motor kontrolü  
- `Wire.h` → I2C iletişimi  
- `Adafruit_GFX.h` → OLED grafik kütüphanesi  
- `Adafruit_SSD1306.h` → OLED ekran sürücüsü  

Bu kütüphaneler **Arduino IDE → Tools → Manage Libraries** menüsünden kolayca yüklenebilir.

---

## 🔧 Kurulum ve Çalıştırma  
1. Arduino IDE’yi yükleyin: [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)  
2. `radar.ino` dosyasını açın.  
3. Devreyi aşağıdaki pin bağlantılarına göre kurun:  
   - TRIG → Pin 9  
   - ECHO → Pin 10  
   - Servo → Pin 6  
   - LED (Kırmızı) → Pin 3  
   - LED (Yeşil) → Pin 4  
   - Buzzer → Pin 5  
4. Kodu karta yükleyin ve çalıştırın.  

---

## 📈 Proje Özellikleri  
- Servo motor ile **0° → 180°** arası tarama  
- OLED ekranda **gerçek zamanlı radar görselleştirme**  
- Nesne mesafesi aralığı: **0 – 150 cm**  
- Tehlike uyarısı: mesafe ≤ 30 cm → **Kırmızı LED + Buzzer**  
- Güvenli mesafe: **Yeşil LED aktif**  
- Açı ve mesafe bilgileri OLED ekranda görüntülenir  

---

## 🖼️ Örnek Görseller  
![Arduino Radar](https://github.com/user-attachments/assets/a30cadef-e719-4064-b46b-564498a21f8c)  

---

## 👨‍💻 Tasarım & Katkıda Bulunan  
**Proje Tasarımcısı & Geliştirici:** Batuhan  
**Katkılar:** Donanım tasarımı, sensör entegrasyonu, görselleştirme kodlaması ve test senaryoları.    

---

## 📁 Dosya İçeriği  
- `radar.ino` → Ana proje dosyası  

---

## 📜 Lisans  
**MIT License**  
Bu proje eğitim ve kişisel geliştirme amaçlıdır.  
© 2025 Batuhan — Tüm hakları saklıdır.
