#include <Arduino.h>
#include <Adafruit_SPIFlash.h>
#include <SPI.h>

// Define flash memory chip select pin
#define FLASH_CS_PIN 10

// Define the SPI flash object
Adafruit_FlashTransport_SPI flashTransport(FLASH_CS_PIN, SPI);
Adafruit_SPIFlash flash(&flashTransport);

uint8_t testData[256]; // Buffer to write (adjust size as needed)

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);

  Serial.println("Initializing flash...");

  if (!flash.begin()) {
    Serial.println("Flash initialization failed!");
    while (1);
  }

  Serial.println("Flash initialized.");

  // Fill testData with a pattern for writing
  for (int i = 0; i < sizeof(testData); i++) {
    testData[i] = i % 256;
  }

  // Erase a sector before testing writes
  uint32_t testSector = 0; // Start of memory
  Serial.println("Erasing sector...");
  if (!flash.eraseSector(testSector)) {
    Serial.println("Failed to erase sector!");
    while (1);
  }

  // Start measuring write performance
  unsigned long startTime = millis();
  uint32_t writes = 0;

  while (millis() - startTime < 1000) {
    // Write the buffer to flash memory
    if (!flash.writeBuffer(testSector, testData, sizeof(testData))) {
      Serial.println("Write failed!");
      while (1);
    }
    writes++;
  }

  // Print the results
  unsigned long elapsedTime = millis() - startTime;
  Serial.print("Writes completed in 1 second: ");
  Serial.println(writes);
}

void loop() {
  // Do nothing in the loop
}