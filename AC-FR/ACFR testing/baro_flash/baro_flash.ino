#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_SPIFlash.h>
#include <Adafruit_BMP3XX.h>
#include <Wire.h>

// Software SPI pins for flash chip
#define FLASH_SCK 9
#define FLASH_MISO 6
#define FLASH_MOSI 8
#define FLASH_CS 7

// Initialize flash with software SPI
Adafruit_FlashTransport_SPI flashTransport(FLASH_CS, FLASH_SCK, FLASH_MISO, FLASH_MOSI);
Adafruit_SPIFlash flash(&flashTransport);

// Initialize BMP390 with I2C
Adafruit_BMP3XX bmp;
TwoWire myWire = TwoWire();

// Data structure for each reading
struct DataPoint {
  uint32_t timestamp;
  float pressure;
  float temperature;
  uint32_t sensor_read_time;  // Time taken to read sensor
  uint32_t flash_write_time;  // Time taken to write to flash
  uint32_t total_loop_time;   // Total loop iteration time
};

// Keep track of where we are in flash
uint32_t currentAddress = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(100);
  Serial.println("Flash + BMP390 Data Logger with Timing");

  myWire.begin(17, 18);

  if (!flash.begin()) {
    Serial.println("Could not find flash chip!");
    while (1) delay(100);
  }

  if (!bmp.begin_I2C(0x77, &myWire)) {
    Serial.println("Could not find BMP390!");
    while (1) delay(100);
  }

  //bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  //bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  //bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

  Serial.println("Erasing flash...");
  flash.eraseBlock(0);
  Serial.println("Flash erased!");

  Serial.println("\nTimestamp,Pressure(hPa),Temp(C),SensorTime(us),FlashTime(us),TotalTime(us)");
}

void loop() {
  uint32_t loop_start = micros();

  if (currentAddress + sizeof(DataPoint) > flash.size()) {
    Serial.println("Flash is full!");
    while (1) delay(1000);
  }

  // Time the sensor reading
  uint32_t sensor_start = micros();
  bool success = bmp.performReading();
  uint32_t sensor_time = micros() - sensor_start;

  if (!success) {
    Serial.println("Failed to read sensor!");
    return;
  }

  // Create data point
  DataPoint data = {
    .timestamp = millis(),
    .pressure = bmp.pressure / 100.0,
    .temperature = bmp.temperature,
    .sensor_read_time = sensor_time
  };

  // Time the flash write
  uint32_t flash_start = micros();
  bool write_success = flash.writeBuffer(currentAddress, (void*)&data, sizeof(DataPoint));
  data.flash_write_time = micros() - flash_start;

  if (!write_success) {
    Serial.println("Failed to write to flash!");
    return;
  }

  // Calculate total loop time
  data.total_loop_time = micros() - loop_start;

  // Update address for next write
  currentAddress += sizeof(DataPoint);

  // Print in CSV format for easy data analysis
  Serial.println("Timestamp,Pressure(hPa),Temp(C),SensorTime(us),FlashTime(us),TotalTime(us)")
  Serial.print(data.timestamp);
  Serial.print(",");
  Serial.print(data.pressure, 2);
  Serial.print(",");
  Serial.print(data.temperature, 2);
  Serial.print(",");
  Serial.print(data.sensor_read_time);
  Serial.print(",");
  Serial.print(data.flash_write_time);
  Serial.print(",");
  Serial.println(data.total_loop_time);

  // Print flash usage every 10 readings
  static int readingCount = 0;
  if (++readingCount % 10 == 0) {
    Serial.printf("\nFlash used: %lu/%lu bytes (%.1f%%)\n\n",
                  currentAddress,
                  flash.size(),
                  (currentAddress * 100.0) / flash.size());
    // Reprint CSV header
    Serial.println("Timestamp,Pressure(hPa),Temp(C),SensorTime(us),FlashTime(us),TotalTime(us)");
  }

  //delay(1000);  // Log once per second
}