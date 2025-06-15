#include <SoftwareSerial.h>

// Define ultrasonic sensor pins
#define TRIG_PIN 9
#define ECHO_PIN 10

// Define buzzer and vibration motor
#define BUZZER_PIN 5
#define VIBRATION_PIN 6

// Bluetooth module (HC-05)
SoftwareSerial Bluetooth(2, 3); // RX, TX

// GPS module (NEO-6M)
SoftwareSerial GPS(6, 7); // RX, TX

// Distance threshold for obstacle detection
const int SAFE_DISTANCE = 50; // in cm (10 meters)

void setup() {
    Serial.begin(9600);
    Bluetooth.begin(9600);
    GPS.begin(9600); // Start GPS communication

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(VIBRATION_PIN, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(VIBRATION_PIN, LOW);

    Serial.println("System Initialized...");
}

void loop() {
    int distance = getDistance();
    String gpsData = getGPSLocation(); // Fetch GPS data

    if (distance <= SAFE_DISTANCE && distance > 0) {
        Serial.print("Obstacle detected at: ");
        Serial.print(distance);
        Serial.println(" cm");

        // Activate buzzer and vibration motor
        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(VIBRATION_PIN, HIGH);
        delay(500);
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(VIBRATION_PIN, LOW);

        // Send alert via Bluetooth
        Bluetooth.print("Obstacle at ");
        Bluetooth.print(distance);
        Bluetooth.println(" cm");

        // Print GPS location on Serial Monitor
        Serial.print("GPS Location: ");
        Serial.println(gpsData);
    }

    delay(1000);
}

// Function to measure distance
int getDistance() {
    long duration;
    int distance;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;

    return distance;
}

// Function to read GPS data
String getGPSLocation() {
    String location = "";
    while (GPS.available()) {
        char c = GPS.read();
        location += c;
    }
    return location.length() > 0 ? location : "No GPS data";
}
