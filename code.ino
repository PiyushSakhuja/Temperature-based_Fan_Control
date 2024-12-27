#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize LCD
#define RELAY_PIN 2                 // Relay connected to pin 2

float temp;
float sum = 0;                      // Variable to store the sum of readings
const int numReadings = 100;        // Number of readings to average
const float threshold = 29.20;      // Temperature threshold for relay activation

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("Temp. Based Fan");
  
  pinMode(RELAY_PIN, OUTPUT); 
  delay(100);                       // Set relay pin as output
  digitalWrite(RELAY_PIN, HIGH);    // Ensure relay is OFF initially
  
  delay(5000);                      // Hold the initial message for 5 seconds
}

void loop() {
  sum = 0; // Reset sum for each set of readings

  // Take 100 readings and calculate the sum
  for (int i = 0; i < numReadings; i++) {
    temp = analogRead(A0);                   // Read analog value
    temp = temp * 0.48828125;                // Convert ADC value to temperature
    sum += temp;                             // Accumulate readings
    delay(10);                               // Small delay between readings
  }

  // Calculate average temperature
  float avgTemp = sum / numReadings;
  //avgTemp = avgTemp - 1;                    // Adjust temperature reading

  // Print the average temperature to the Serial Monitor
  Serial.print("Average Temperature: ");
  Serial.print(avgTemp);
  Serial.println("°C");
    
  // Display the temperature on the LCD
  lcd.setCursor(0, 0);
  lcd.print("T :");
  lcd.print(avgTemp);
  lcd.print((char)223);                     // Degree symbol
  lcd.print("C");

  // Control relay based on the temperature threshold
  if (avgTemp > threshold) {
    digitalWrite(RELAY_PIN, LOW);           // Turn relay ON
  } else {
    digitalWrite(RELAY_PIN, HIGH);          // Turn relay OFF
  }

  delay(1000);                              // Wait before the next set of readings
}
