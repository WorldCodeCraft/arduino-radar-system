# Arduino Radar System

## 🔹 Proje Hakkında
Bu proje, **HC-SR04 ultrasonik sensör**, **servo motor**, **OLED ekran** ve **LED + buzzer uyarı sistemi** kullanarak gerçek zamanlı radar simülasyonu oluşturur.  

- Nesnelerin mesafesini ölçer ve tarama açısına göre OLED ekranda görselleştirir.  
- Tehlikeli mesafeye yaklaşan nesnelerde LED ve buzzer ile uyarı verir.  

**Amaç:** Gömülü sistemler, sensör tabanlı projeler ve gerçek zamanlı görselleştirme becerilerini göstermek.

---

## 🔹 Kullanılan Malzemeler
- Arduino Uno  
- HC-SR04 Ultrasonik Sensör  
- Servo Motor 180 Derece  
- OLED Ekran (128x64) SSD1306 
- LED (Kırmızı ve Yeşil)  
- Buzzer  
- Jumper kablolar, breadboard  

---

## 🔹 Kütüphaneler
Arduino IDE’de şu kütüphaneler yüklü olmalı:
- `Servo.h` → Servo motor kontrolü  
- `Wire.h` → I2C iletişim  
- `Adafruit_GFX.h` → OLED grafik kütüphanesi  
- `Adafruit_SSD1306.h` → OLED ekran kontrolü  

Arduino IDE üzerinden **Library Manager** ile yüklenebilir.  

---

## 🔹 Kurulum ve Çalıştırma
1. Arduino IDE’yi yükleyin: [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)  
2. Kodu açın: `radar.ino`  
3. Devreyi aşağıdaki pinlere göre bağlayın:
   - TRIG → Pin 9  
   - ECHO → Pin 10  
   - Servo → Pin 6  
   - LED Kırmızı → Pin 3  
   - LED Yeşil → Pin 4  
   - Buzzer → Pin 5  
4. Arduino’ya yükleyin ve çalıştırın  

---

## 🔹 Özellikler
- Servo motor ile 0° → 180° tarama  
- OLED ekranda gerçek zamanlı radar görselleştirme  
- Nesnelerin mesafe bilgisi: **0 – 150 cm**  
- Tehlike uyarısı: mesafe ≤ 30 cm → kırmızı LED + buzzer  
- Yeşil LED → güvenli mesafe  
- Ölçülen açı ve mesafe OLED üzerinde gösterilir  

---

## 🔹 Örnek Görseller
![Arduino Radar]<img width="1097" height="514" alt="Ekran görüntüsü 2025-10-11 193046" src="https://github.com/user-attachments/assets/a30cadef-e719-4064-b46b-564498a21f8c" />


---

## 🔹 Katkıda Bulunanlar
- Batuhan (Sahibi / Geliştirici)

---

## 🔹 Lisans
MIT License
