#include <SPI.h>
#include <SD.h>

// Pin connected to the SD card module's chip select (CS)


// Variables to store sensor data
float batt_voltage = 3.73;
int flight_num = 5;

float alt = 5648.23;
float pressure = 1010.12;
float temp = 21.34;

float accel_x = 39.12;
float accel_y = 31.23;
float accel_z = 30.12;

float high_g_x = 30.12;
float high_g_y = 30.34;
float high_g_z = 30.56;

float gyro_x = 30.12;
float gyro_y = 30.34;
float gyro_z = 30.56;

float mag_x = 30.12;
float mag_y = 30.34;
float mag_z = 30.56;

float vert_speed = 313.45;
float flight_time = 465.01;
int state = 3;
bool bat_connected = true;
String error = "No error";  // Set to "" if no error, otherwise limited to 30 chars

byte separator[] = {0xFF, 0xFF, 0xFF, 0xFF};  // 4-byte separator

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(100);
  delay(3000);
  Serial.println("starting setup...");



  const int chipSelect = 10;
  File dataFile;

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present.");
    return;  // Do nothing more
  }
  
  Serial.println("SD card initialized.");
  
  // Open the file for writing
  dataFile = SD.open("values.csv", FILE_WRITE);
  
  if (dataFile) {
    Serial.println("Writing to file...");
    unsigned long start_time = millis();  // Start timing the writing process
    
    // Write 300 lines of data
    for (int i = 0; i < 300; i++) {
      writeDataToFile();
    }
    
    unsigned long end_time = millis();  // End timing
    dataFile.close();
    
    // Output the time taken
    Serial.print("Time taken: ");
    Serial.print((end_time - start_time) / 1000.0);  // Time in seconds
    Serial.println(" seconds");
  } else {
    // If the file couldn't be opened, output an error
    Serial.println("Error opening values.csv");
  }
}

void loop() {
  Serial.println("done");
  delay(1000);
}

// Function to write the data to the file in CSV format
void writeDataToFile() {
  // Prepare the CSV row
  String row = String(batt_voltage) + "," + String(flight_num) + ","
               + String(alt) + "," + String(pressure) + "," + String(temp) + ","
               + String(accel_x) + "," + String(accel_y) + "," + String(accel_z) + ","
               + String(high_g_x) + "," + String(high_g_y) + "," + String(high_g_z) + ","
               + String(gyro_x) + "," + String(gyro_y) + "," + String(gyro_z) + ","
               + String(mag_x) + "," + String(mag_y) + "," + String(mag_z) + ","
               + String(vert_speed) + "," + String(flight_time) + "," + String(state) + ","
               + String(bat_connected) + "," + error + "\n";
  
  // Write the row to the file
  dataFile.print(row);
}

