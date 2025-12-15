#include <SPI.h>
#include <mcp_can.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CAN_CS 10
MCP_CAN CAN0(CAN_CS);

LiquidCrystal_I2C lcd(0x27, 16, 2);

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Receiver Ready");

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
  if (CAN0.checkReceive() == CAN_MSGAVAIL) {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);

    String msg = "";
    for (int i = 0; i < len; i++) {
      msg += (char)rxBuf[i];
    }

    Serial.print("Message received: ");
    Serial.println(msg);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Msg Recvd:");
    lcd.setCursor(0, 1);
    lcd.print(msg);
  }
}
