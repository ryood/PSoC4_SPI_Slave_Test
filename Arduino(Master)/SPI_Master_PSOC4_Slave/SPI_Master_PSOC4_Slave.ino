/*
  SPI_Master_PSOC4_Slave.c
  Master for PSoC 4 SPI Slave
  
  SCK   pin13
  MISO  pin12
  MOSI  pin11
  SS    pin10
  
  2016.05.09
*/
#include <SPI.h>

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;

#define TX_PACKET_SIZE  1
#define RX_PACKET_SIZE  1

void setup() {
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin();
  Serial.begin(9600);
  Serial.println("SPI Master test");
}

void loop() {
  static byte cnt;
  byte txBuffer[TX_PACKET_SIZE];
  byte rxBuffer[RX_PACKET_SIZE];
  
  // Master(Arduino) -> Slave(PSoC)
  txBuffer[0] = cnt++;
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(txBuffer[0]);
  digitalWrite(slaveSelectPin, HIGH);
  SPI.endTransaction();
  
  // Master(Arduino) <- Slave(PSoC)
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(slaveSelectPin, LOW);
  rxBuffer[0] = SPI.transfer(0);
  digitalWrite(slaveSelectPin, HIGH);
  SPI.endTransaction();
  
  Serial.print(rxBuffer[0]);
  /*
  Serial.print("\t");
  Serial.print(rxBuffer[1]);
  Serial.print("\t");
  Serial.print(rxBuffer[2]);
  */
  Serial.print("\r\n");
  delay(100);
}

