#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Wi-Fi credentials
const char* ssid = "a"; // Replace with your Wi-Fi SSID
const char* password = "12345678"; // Replace with your Wi-Fi password

// ThingSpeak settings
unsigned long channelID = 2776558; // Replace with your ThingSpeak channel ID
const char* writeAPIKey = "X0N7OOHIJMA3RBEZ"; // Replace with your ThingSpeak write API key

WiFiClient client;

#define enA D7        // Enable1 L298 Pin enA (PWM)
#define enB D8        // Enable2 L298 Pin enB (PWM)

#define in1 D3        // Motor1 L298 Pin in1
#define in2 D4        // Motor1 L298 Pin in2
#define in3 D5        // Motor2 L298 Pin in3
#define in4 D6        // Motor2 L298 Pin in4

#define ir_R D2       // Right Fire Sensor Pin
#define ir_F D1       // Front Fire Sensor Pin
#define ir_L D0       // Left Fire Sensor Pin

#define relayPin D9   // Relay module connected to D9 (Modify pin if needed)

// Flame sensor values
int flameDR;        // Right sensor value
int flameDF;        // Front sensor value
int flameDL;        // Left sensor value

void setup() {
  Serial.begin(9600);  // Start serial communication for debugging

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Motor pin modes
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Flame sensor pin modes
  pinMode(ir_R, INPUT);
  pinMode(ir_F, INPUT);
  pinMode(ir_L, INPUT);

  // Motor speed control pins (PWM)
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  // Relay pin mode
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Ensure the relay is off initially
}

void loop() {
  // Read flame sensor values
  flameDR = digitalRead(ir_R);
  flameDF = digitalRead(ir_F);
  flameDL = digitalRead(ir_L);

  // Print flame sensor values for debugging
  Serial.print("flameDR: ");
  Serial.println(flameDR);
  Serial.print("flameDF: ");
  Serial.println(flameDF);
  Serial.print("flameDL: ");
  Serial.println(flameDL);
  Serial.println("-------------");

  // Motor control based on sensor values
  if (flameDF == 0) {
    // Front Fire Detected - Moving Forward
    Serial.println("Front Fire Detected - Moving Forward");
    analogWrite(enA, 180);
    analogWrite(enB, 180);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else if (flameDR == 0) {
    // Right Fire Detected - Turning Right
    Serial.println("Right Fire Detected - Turning Right");
    analogWrite(enA, 150);
    analogWrite(enB, 180);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else if (flameDL == 0) {
    // Left Fire Detected - Turning Left
    Serial.println("Left Fire Detected - Turning Left");
    analogWrite(enA, 180);
    analogWrite(enB, 150);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else {
    // No Fire Detected - Stopping Motors
    Serial.println("No Fire Detected - Stopping Motors");
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  }

  // If the car reaches the fire location, spray water
  if (flameDF == 0 || flameDR == 0 || flameDL == 0) {
    Serial.println("Fire detected, activating water pump...");
    digitalWrite(relayPin, HIGH);  // Activate relay to turn on the pump
    delay(5000); // Run the pump for 5 seconds (adjust this duration as needed)
    digitalWrite(relayPin, LOW);   // Deactivate relay to turn off the pump
  }

  // Send sensor data to ThingSpeak every 3 seconds
  static unsigned long lastUpdateTime = 0;
  if (millis() - lastUpdateTime >= 3000) {
    lastUpdateTime = millis();

    // Write to ThingSpeak fields
    ThingSpeak.setField(1, flameDL); // Left sensor data
    ThingSpeak.setField(2, flameDF); // Front sensor data
    ThingSpeak.setField(3, flameDR); // Right sensor data

    // Execute ThingSpeak update
    int responseCode = ThingSpeak.writeFields(channelID, writeAPIKey);
    if (responseCode == 200) {
      Serial.println("ThingSpeak update successful");
    } else {
      Serial.print("ThingSpeak update failed, error: ");
      Serial.println(responseCode);
    }
  }

  delay(100); // Small delay to avoid bouncing
}