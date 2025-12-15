#include <SPI.h>
#include <mcp_can.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CAN_CS 10
MCP_CAN CAN0(CAN_CS); 
LiquidCrystal_I2C lcd(0x27,16,2); // change 0x27 to 0x3F if needed

void setup() {
  Serial.begin(115200);
  lcd.init(); lcd.backlight();
  lcd.setCursor(0,0); lcd.print("Sender Ready");

  if (CAN0.begin(MCP_ANY, 500000, MCP_8MHZ) == CAN_OK) {
    Serial.println("CAN OK");
    lcd.setCursor(0,1); lcd.print("CAN OK");
  } else {
    Serial.println("CAN FAIL");
    lcd.setCursor(0,1); lcd.print("CAN FAIL");
    while(1);
  }
  CAN0.setMode(MCP_NORMAL);
  delay(200);
}

void loop() {
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n'); // read typed message
    int len = msg.length();
    if (len > 8) len = 8; // max 8 bytes per CAN frame

    uint8_t buf[8];
    for (int i=0;i<len;i++) buf[i] = msg[i];

    if (CAN0.sendMsgBuf(0x100, 0, len, buf) == CAN_OK) {
      Serial.print("Sent: "); Serial.println(msg);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Sent Msg:");
      lcd.setCursor(0,1);
      lcd.print(msg);
    } else {
      Serial.println("Send fail");}}}
