#include <SPI.h>
#include <mcp_can.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CAN_CS 10
MCP_CAN CAN0(CAN_CS);

LiquidCrystal_I2C lcd(0x27,16,2);
unsigned int counter = 0;
float temperature = 0, humidity = 0;

void setup() {
  Serial.begin(115200);
  lcd.init(); lcd.backlight();

  if (CAN0.begin(MCP_ANY, 500000, MCP_8MHZ) != CAN_OK) while(1);
  CAN0.setMode(MCP_NORMAL);
  delay(200);
}

void loop() {
  if(CAN0.checkReceive() == CAN_MSGAVAIL){
    unsigned long rxId;
    unsigned char len;
    unsigned char buf[8];
    CAN0.readMsgBuf(&rxId, &len, buf);

    if(rxId == 0x100){
      counter = buf[0];
      uint16_t tempInt = ((uint16_t)buf[1]<<8)|buf[2];
      temperature = tempInt / 10.0;

      uint16_t humInt = ((uint16_t)buf[3]<<8)|buf[4];
      humidity = humInt / 10.0;
    }
  }

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Cnt:");
  lcd.print(counter);

  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.print(temperature,1);
  lcd.print("C H:");
  lcd.print(humidity,1);
  lcd.print("%");

  delay(300);
}
