#include <SPI.h>
#include <mcp_can.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define CAN_CS 10
MCP_CAN CAN0(CAN_CS);

#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27,16,2);
unsigned int counter = 0;

void setup() {
  Serial.begin(115200);
  lcd.init(); lcd.backlight();

  if (CAN0.begin(MCP_ANY, 500000, MCP_8MHZ) != CAN_OK) while(1);
  CAN0.setMode(MCP_NORMAL);
  dht.begin();
  delay(200);
}

void loop() {
  counter++;

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if(!isnan(temperature) && !isnan(humidity)){
    // Display on LCD
    lcd.setCursor(0,0);
    lcd.print("Cnt:");
    lcd.print(counter);

    lcd.setCursor(0,1);
    lcd.print("T:");
    lcd.print(temperature,1);
    lcd.print("C H:");
    lcd.print(humidity,1);
    lcd.print("%");

    // Send CAN frame (2 bytes per value)
    uint16_t tempInt = temperature*10;
    uint16_t humInt = humidity*10;

    uint8_t buf[8];
    buf[0] = counter & 0xFF;
    buf[1] = (tempInt >> 8) & 0xFF;
    buf[2] = tempInt & 0xFF;
    buf[3] = (humInt >> 8) & 0xFF;
    buf[4] = humInt & 0xFF;

    CAN0.sendMsgBuf(0x100, 0, 5, buf);
  }

  delay(2000); // DHT11 max read rate
}
