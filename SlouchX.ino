#include <Wire.h>
#include <MPU6050.h>
#include <BluetoothSerial.h>

MPU6050 mpu1(0x68);  // Default I2C address
MPU6050 mpu2(0x69);  // Alternate I2C address

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(9600);
  SerialBT.begin("SlouchDetector"); // Bluetooth name

  Wire.begin();
  
  // Initialize MPU6050 sensors
  mpu1.initialize();
  mpu2.initialize();

  // Inform user that Bluetooth is ready
  Serial.println("Bluetooth device is ready to pair");
}
int slouch =0;
void loop() {
  // Wait for a connection
  if (SerialBT.hasClient()) {
    // Variables to store raw accelerometer data
    int16_t ax1, ay1, az1;
    int16_t ax2, ay2, az2;
    
    // Read MPU6050 sensor data
    mpu1.getAcceleration(&ax1, &ay1, &az1);
    mpu2.getAcceleration(&ax2, &ay2, &az2);
    
    // Calculate angles (pitch)
    float angle1 = atan2(ax1, az1) * 180 / M_PI;
    float angle2 = atan2(ax2, az2) * 180 / M_PI;

    // Print sensor data and angles to Serial Monitor and Bluetooth
    String data1 = "MPU1 Acceleration: X: " + String(ax1) + " Y: " + String(ay1) + " Z: " + String(az1) + " | Angle: " + String(angle1);
    String data2 = "MPU2 Acceleration: X: " + String(ax2) + " Y: " + String(ay2) + " Z: " + String(az2) + " | Angle: " + String(angle2);

    Serial.println(data1);
    Serial.println(data2);
    
//    SerialBT.println(data1);
//    SerialBT.println(data2);

    // Calculate bend in spine and send it
    float spineBend = 360 - ((360 - 2 * (angle1 + angle2)) / 2);
    Serial.print("Bend in spine: ");
    Serial.println(spineBend);
    
//    SerialBT.print("Bend in spine: ");
    SerialBT.print(spineBend);
    SerialBT.print(";");

    // Check if slouched and send alert
    if (angle1 < 90 || angle2 < -90) {
      slouch=1;
      Serial.println("Slouch detected!");
    } else {
//      SerialBT.println("Good");
      Serial.println("Good");
      slouch=0;
    }
    SerialBT.print(slouch);
    SerialBT.print(";");
    delay(500);
  } 
}