// PSoC 4 SPI Slave用のMasterプログラム
// 
// POTでTXデータを設定、NOKIA 5110でTX,RXデータを表示
//
// PSoC 4 Slave
//      SCK    13
//      MISO   12
//      MOSI   11
//      CS      9
//
// LCD5110 
//      SCK  - Pin 13
//      MOSI - Pin 11
//      DC   - Pin 2
//      RST  - Pin 3
//      CS   - Pin 10
//
// POT  A0
//
// 2016.05.09
#include <stdio.h>
#include <SPI.h>
#include <LCD5110_Graph.h>

#define PIN_PSOC_SS  9
#define PIN_POT      A0

#define TX_PACKET_SIZE  3
#define RX_PACKET_SIZE  3

LCD5110 myGLCD(13,11,2,3,10);  // SCK, MOSI, DC, RST, CS

extern uint8_t SmallFont[];

byte txBuffer[TX_PACKET_SIZE];
byte rxBuffer[RX_PACKET_SIZE];

char strBuffer[17];

void setup()
{
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  
  myGLCD.clrScr();
  myGLCD.print("SPI Master", 0, 0);
  myGLCD.update();
  
  pinMode(PIN_PSOC_SS, OUTPUT);
}

void loop()
{
  int txData;
  int rxData;
  
  txData = analogRead(PIN_POT);
  rxData = sendRecieveData(txData);
  displayData(txData, rxData);
  delay(100);
}

int sendRecieveData(int txData) {
  static byte cnt;

  txBuffer[0] = cnt++;
  txBuffer[1] = txData >> 8;
  txBuffer[2] = txData & 0xff;

  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(PIN_PSOC_SS, LOW);
  rxBuffer[0] = SPI.transfer(txBuffer[0]);
  rxBuffer[1] = SPI.transfer(txBuffer[1]);
  rxBuffer[2] = SPI.transfer(txBuffer[2]);
  digitalWrite(PIN_PSOC_SS, HIGH);
  SPI.endTransaction();
  SPI.end();

  return ((int)rxBuffer[1] << 8) | (rxBuffer[2]);
}

void displayData(int txData, int rxData) {
  myGLCD.clrScr();
  myGLCD.print("SPI Master", 0, 0);
  
  sprintf(strBuffer, "TXDATA: %d", txData);
  myGLCD.print(strBuffer, 0, 10);
  
  sprintf(strBuffer, "TX:%03d %03d %03d", txBuffer[0], txBuffer[1], txBuffer[2]);
  myGLCD.print(strBuffer, 0, 20);

  sprintf(strBuffer, "RX:%03d %03d %03d", rxBuffer[0], rxBuffer[1], rxBuffer[2]);
  myGLCD.print(strBuffer, 0, 30);

  sprintf(strBuffer, "RXDATA: %d", rxData);
  myGLCD.print(strBuffer, 0, 40);

  myGLCD.update();
}

