#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
/////////////////////////////////////////////////
//AYARLAR ===
const int AÇI_MIN = 0; // servo minimum pozisyonu derece cinsinden
const int AÇI_MAX = 180; // servo maximum pozisyonu
const int AÇI_ADIM = 2; // servo her dönüşü 2 derecelik açı
const int ADIM_GECİKMESİ = 40; // her 2 dereceyi dönerken geçireceği vaktimiz 0.04 saniye

const int TEHLİKE_CM = 30;  // sensör tetik santimetresi
const int MAX_GÖRÜNTÜLEME = 150; // sensör maksimum görüş mesafesi sinyal boyu

//PINLER (uno üzerinde hangi kanala hangi cihaz ve sinyali bağlı)
const int PIN_TRIG  = 9; //hc-sr04 haberleşme
const int PIN_ECHO  = 10; //hc-sr04 haberleşme
const int PIN_SERVO = 6; //servo haberleşme
const int PIN_RED   = 3; //led K haberleşme
const int PIN_GREEN = 4; //led Y haberleşme
const int PIN_BUZZ  = 5; //buzzer haberleşme

//OLED ekran genişlik yükseklik ayarı yapımı.
#define ekran_genislik 128 //ekran yatay boyut pixel
#define ekran_yuksek 64 //ekran dikey boyut pixel
#define led_sifirla -1 // I2C haberleşme kullandığımız için resetlemelik bir işim yok
Adafruit_SSD1306 display(ekran_genislik, ekran_yuksek, &Wire, led_sifirla); //ekranı oluşturuyoruz
/////////////////////////////////////////////////

Servo motor; // servoyu motor olarak değiştirdik

/////////////////////////////////////////////////
void kur() {
  Serial.begin(115200); //haberleşme pc veri göndermek için

  pinMode(PIN_TRIG, OUTPUT); //sensör tetik
  pinMode(PIN_ECHO, INPUT); //sensör tetikten gelen sinyalimizin geri girişi
  pinMode(PIN_RED, OUTPUT); //algılama tetik
  pinMode(PIN_GREEN, OUTPUT); //güvenli tetik
  pinMode(PIN_BUZZ, OUTPUT); //algılama tetik

  motor.attach(PIN_SERVO); // motorumuzu çalıştırıyoruz ve döndürüyoruz

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 bulunamadi"));  //bağlantıyı bulamazsa if döndürecek ve uyarı mesajını verecek pcde
    while (true);
  }

  display.clearDisplay(); // ekran temizleme
  display.setTextColor(ekran_renk); // yazı rengi
  display.setTextSize(1);  // yazı boyutumuz
  display.setCursor(20, 25); // ekranın tam ortası
  display.println("RADAR SISTEMI ACILIYOR"); // açılış ekranım
  display.display(); // bastır
  delay(1000); // yazının ekranda kalma saniyemiz
  display.clearDisplay(); // sonra ekranı temizle

  motor.write(0); // 1. başlangıç pozisyonumuz
  digitalWrite(PIN_GREEN, HIGH); // sorunsuz çalıştığının göstergesi yeşil ışığı yakacak
}

/////////////////////////////////////////////////
// Mesafe Ölçümü
int mesafe_olcum() { //mesafeyi cm olarak ayarla
  digitalWrite(PIN_TRIG, LOW); // sensör haberleşmeyi sıfırla
  delayMicroseconds(2);  // 2 mikro saniye bekle sensör için
  digitalWrite(PIN_TRIG, HIGH); // sesöre dalgayı gönder
  delayMicroseconds(10); // 10 mikro saniye dalga gönder
  digitalWrite(PIN_TRIG, LOW); // sensör haberleşmesini tekrar sıfırla
  long sure = pulseIn(PIN_ECHO, HIGH, 30000); // dönen sinyali süresini ölç (max 30ms gecikme olacak)
  if (sure == 0) return -1; // sinyal dönmedi -1 yok birşey
  int mesafe = sure * 0.034 / 2; // süreyi cm ye çevirme hesaplama (ses hızı 0.034 cm/us, gidiş-dönüş /2)******---?
  if (mesafe > 300) return -1; // 300 cm max sensör mesafesi dalga alış verişi
  return mesafe; // döndür 
}

/////////////////////////////////////////////////
// RADAR TASARIMI
void cizim(int aci, int mesafe) {
  display.clearDisplay(); // temizle
  int cx = ekran_genislik / 2; // ekranın ortası
  int cy = ekran_yuksek - 1; // ekranın altı
  int maxR = 60; // radar sembol yarıçapı

  // Radar halkaları
  for (int r = 15; r <= maxR; r += 15) {        //15 pixel aralıklı 4 halka 60 120 180 240
    display.drawCircle(cx, cy, r, ekran_renk);
  }

  // Çizgi
  float rad = aci * (PI / 180.0);
  int lx = cx + cos(rad) * maxR; //1. x uçları çizgi
  int ly = cy - sin(rad) * maxR; //1. y uçları çizgi
  display.drawLine(cx, cy, lx, ly, ekran_renk); //bastır

  // Algılanan nesne
  if (mesafe > 0 && mesafe <= MAX_GÖRÜNTÜLEME) {
    float olcek = (float)maxR / (float)MAX_GÖRÜNTÜLEME;
    int px = cx + cos(rad) * mesafe * olcek; // hedef x konumu sol
    int py = cy - sin(rad) * mesafe * olcek; // hedef y konumu sağ
    display.fillCircle(px, py, 3, ekran_renk); // hedef tespiti için 3px boyunda bir nokta koy
  }

  // Bilgi
  display.fillRect(0, 0, ekran_genislik, 12, SSD1306_BLACK); // evet temizleeee radarı ve
  display.setCursor(0, 0);  //sol üst
  display.setTextSize(1);  // boyut
  display.print("Aci:");      // açı bilgisi için print 
  display.print(aci);         // anlık açı değeri
  display.print("°  Mesafe:");   //mesafe bilgisi için print
  if (mesafe > 0) {
    display.print(mesafe);       //mesafe bilgisini doldur
    display.print("cm");         // cm cinsinden belirt
  } else {
    display.print("---");        // radar boşsa boş bırak
  }

  display.display();    ////// hepsini bastır göster.
}

/////////////////////////////////////////////////
// LED + Buzzer
void uyari(int mesafe) {
  if (mesafe > 0 && mesafe <= TEHLİKE_CM) { // 0 ile 30 cm içinde birşey görürsen 
    digitalWrite(PIN_RED, HIGH);   // kırmızı ledi tetikle
    digitalWrite(PIN_GREEN, LOW);   // yeşil ledin tetiğini çek
    tone(PIN_BUZZ, 1000);          // buzzerı alarm ver 1000hz de 
  } else {                          //hayırsa
    digitalWrite(PIN_RED, LOW);      // kırmızı kapat
    digitalWrite(PIN_GREEN, HIGH);   // yeşil aktif
    noTone(PIN_BUZZ);                 // tetiksiz bekle
  }
}

/////////////////////////////////////////////////
void dongum() {
  // 0 → 180 derece
  for (int a = AÇI_MIN; a <= AÇI_MAX; a += AÇI_ADIM) { // motoru 0 dan başlat ve 180 e kadar 2 şer arttırarak devam et
    motor.write(a);  // motoru o pozisyona getir
    int d = mesafe_olcum();  //int mesafe_olcumü çalıştır
    uyari(d);                // mesafe olcume göre led ve buzzerı kontrol et
    cizim(a, d);              //ekrana 3pixel için kontrol et
    delay(ADIM_GECİKMESİ);    // 40 mikro saniye bekle
  }

  // 180 → 0 derece
  for (int a = AÇI_MAX; a >= AÇI_MIN; a -= AÇI_ADIM) {        //ÜSTTEKİ KODUN TAM TERSİ EKSİLTEREK DEVAM ET
    motor.write(a);
    int d = mesafe_olcum();
    uyari(d);
    cizim(a, d);
    delay(ADIM_GECİKMESİ);
  }
}
