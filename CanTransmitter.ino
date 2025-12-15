#include <SPI.h>
#include <mcp_can.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CAN_CS 10
MCP_CAN CAN0(CAN_CS);

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned int counter = 0;

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sender Ready");

  if (CAN0.begin(MCP_ANY, 500000, MCP_8MHZ) == CAN_OK) {
    Serial.println("CAN init ok!");
    lcd.setCursor(0, 1);
    lcd.print("CAN OK");
  } else {
    Serial.println("CAN init fail");
    lcd.setCursor(0, 1);
    lcd.print("CAN FAIL");
    while (1);
  }

  CAN0.setMode(MCP_NORMAL);
}

void loop() {
  counter++;
  char msg[8];
  sprintf(msg, "%u", counter);  // counter ko string banaya

  CAN0.sendMsgBuf(0x100, 0, strlen(msg), (byte*)msg);

  Serial.print("Message Sent: ");
  Serial.println(msg);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Msg Sent:");
  lcd.setCursor(0, 1);
  lcd.print(counter);

  delay(1000); // 1 sec delay
}
