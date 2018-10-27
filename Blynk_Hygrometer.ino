#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Arduino.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "8afd72d6c46a4b5f90588933ab083b25";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "BETSY";
char pass[] = "6969696969";

#define DHTPIN 17          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
int h = 0; // Global Humidity variable
int t = 0; // Global Temperature Variable

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  h = dht.readHumidity();
  t = dht.readTemperature(true); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  Serial.println("Humidity: " + String(h) + "% Temperature: " + String(t));
}

void drawLogo()
{
  if ( u8g2.getDisplayHeight() < 59 )
  u8g2.setFontMode(1);  // Transparent
  u8g2.setDrawColor(1);

  u8g2.setFontDirection(0);
  u8g2.setFont(u8g2_font_timR14_tf);
  u8g2.drawStr(10, 15, "Ohlone");
  u8g2.setFont(u8g2_font_timB24_tr);
  u8g2.drawStr(0, 40, "C");
  u8g2.setFont(u8g2_font_timB18_tr);
  u8g2.drawStr(24, 35, "IGAR");
  u8g2.setFont(u8g2_font_timB10_tr);
  u8g2.drawStr(25, 47, "Lounge");
  u8g2.setFont(u8g2_font_timR14_tr);
  u8g2.setCursor(95, 15);
  u8g2.print(h);
  u8g2.print("%");
  u8g2.setCursor(95, 30);
  u8g2.print(t);
  u8g2.print("F");
}

void drawURL(void)
{
  u8g2.setFont(u8g2_font_5x7_mr);
  u8g2.drawStr(0,64,"www.ohlonecigarlounge.com");
}

void setup()
{
  // DISPLAY SETUP
  u8g2.begin();
  u8g2.setFlipMode(0);
  
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(5000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();

  u8g2.firstPage();
  do {
    drawLogo();
    drawURL();
  } while ( u8g2.nextPage() );
}
