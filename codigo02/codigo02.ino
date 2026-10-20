#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 9
#define DHTTYPE DHT22



DHT dht(DHTPIN, DHTTYPE);
#define SDA_I2C 20
#define SCL_I2C 21

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
uint8_t enderecoBMP = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("============================");
  Serial.println("INICIANDO SENSORES");
  Serial.println("============================");


  Wire.begin(SDA_I2C, SCL_I2C);
  Wire.setClock(100000);

  
  dht.begin();
  Serial.println("DHT22 iniciado!");

  
  if (bmp.begin(0x76)) {
    bmpOK = true;
    enderecoBMP = 0x76;
  } else if (bmp.begin(0x77)) {
    bmpOK = true;
    enderecoBMP = 0x77;
  }

  if (bmpOK) {
    Serial.print("BMP280 encontrado em 0x");
    Serial.println(enderecoBMP, HEX);

    bmp.setSampling(
      Adafruit_BMP280::MODE_NORMAL,
      Adafruit_BMP280::SAMPLING_X2,
      Adafruit_BMP280::SAMPLING_X16,
      Adafruit_BMP280::FILTER_X16,
      Adafruit_BMP280::STANDBY_MS_500
    );
  } else {
    Serial.println("ERRO: BMP280 nao encontrado!");
  }


  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS,
        false,
        false
      )) {
    Serial.println("ERRO: OLED nao iniciado!");

    while (true) {
      delay(100);
    }
  }

  Serial.println("OLED iniciado!");

  // Tela inicial
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(48, 15);
  display.println("DADOS");

  display.setCursor(25, 35);
  display.println("INICIANDO...");

  display.display();

  // Aguarda o DHT22 estabilizar
  delay(2000);
}

// ========================================
// LOOP
// ========================================
void loop() {
  // Lê temperatura e umidade do DHT22
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  // Lê pressão do BMP280
  float pressao = NAN;

  if (bmpOK) {
    pressao = bmp.readPressure() / 100.0F;
  }


  Serial.println("----------------------------");

  Serial.print("Temperatura: ");

  if (isnan(temperatura)) {
    Serial.println("ERRO");
  } else {
    Serial.print(temperatura, 1);
    Serial.println(" C");
  }

  Serial.print("Umidade: ");

  if (isnan(umidade)) {
    Serial.println("ERRO");
  } else {
    Serial.print(umidade, 1);
    Serial.println(" %");
  }

  Serial.print("Pressao: ");

  if (isnan(pressao)) {
    Serial.println("ERRO");
  } else {
    Serial.print(pressao, 1);
    Serial.println(" hPa");
  }


  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  
  display.setCursor(48, 0);
  display.println("DADOS");

  
  display.drawLine(0, 11, 127, 11, SSD1306_WHITE);

  
  display.setCursor(0, 18);
  display.print("Temp: ");

  if (isnan(temperatura)) {
    display.println("ERRO");
  } else {
    display.print(temperatura, 1);
    display.println(" C");
  }

  display.setCursor(0, 34);
  display.print("Umid: ");

  if (isnan(umidade)) {
    display.println("ERRO");
  } else {
    display.print(umidade, 1);
    display.println(" %");
  }

 
  display.setCursor(0, 50);
  display.print("Press: ");

  if (isnan(pressao)) {
    display.println("ERRO");
  } else {
    display.print(pressao, 1);
    display.println(" hPa");
  }
  display.display();


  delay(2000);
}