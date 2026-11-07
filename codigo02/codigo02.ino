#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "esp_sleep.h"

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define SDA_I2C 4
#define SCL_I2C 3

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);


Adafruit_BMP280 bmp;
bool bmpOK = false;


#define TEMPO_ATIVO 10000ULL
#define INTERVALO_MEDICAO 60000000ULL

void setup() {

  Serial.begin(115200);
  delay(500);

  Serial.println("Estacao Meteorologica");
  Serial.println("ESP32 acordado!");

  Wire.begin(SDA_I2C, SCL_I2C);
  Wire.setClock(100000);

  dht.begin();
  if (bmp.begin(0x76)) {
    bmpOK = true;
  }
 
  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS
      )) {
    Serial.println("Erro ao iniciar OLED!");
  } else {
    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();
    float pressao = NAN;
    if (bmpOK) {
      pressao = bmp.readPressure() / 100.0F;
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(45, 0);
    display.println("DADOS");

    display.setCursor(0, 16);
    display.print("Temperatura: ");

    if (isnan(temperatura)) {
      display.println("--");
    } else {
      display.print(temperatura, 1);
      display.println(" C");
    }

   
    display.setCursor(0, 32);
    display.print("Umidade: ");

    if (isnan(umidade)) {
      display.println("--");
    } else {
      display.print(umidade, 1);
      display.println(" %");
    }

  
    display.setCursor(0, 48);
    display.print("Pressao: ");

    if (isnan(pressao)) {
      display.println("--");
    } else {
      display.print(pressao, 1);
      display.println(" hPa");
    }

    display.display();

    delay(TEMPO_ATIVO);

    display.ssd1306_command(SSD1306_DISPLAYOFF);

    
    Wire.end();
  }

  Serial.println("Entrando em Deep Sleep...");

  uint64_t tempoSleep =
    INTERVALO_MEDICAO - TEMPO_ATIVO;

  esp_sleep_enable_timer_wakeup(tempoSleep);

  Serial.flush();

  esp_deep_sleep_start();
}

void loop() {
 
}
