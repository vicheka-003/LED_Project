// Define LED pins
const int LED1 = 8; // Connect LED 1 to pin 8
const int LED2 = 9; // Connect LED 2 to pin 9
const int LED3 = 10; // Connect LED 3 to pin 10

// Variables to store LED states
bool led1State = false;
bool led2State = false;
bool led3State = false;

// Variable to store incoming command
String command = "";

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Configure LED pins as outputs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  // Initially turn all LEDs off
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);

  // Send ready message
  Serial.println("Arduino 3-LED controller ready");
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the incoming command until newline
    command = Serial.readStringUntil('\n');
    // Remove whitespace
    command.trim();

    // Process commands for LED 1
    if (command == "on1") {
      digitalWrite(LED1, HIGH);
      led1State = true;
      Serial.println("LED 1 turned ON");
    }
    else if (command == "off1") {
      digitalWrite(LED1, LOW);
      led1State = false;
      Serial.println("LED 1 turned OFF");
    }

    // Process commands for LED 2
    else if (command == "on2") {
      digitalWrite(LED2, HIGH);
      led2State = true;
      Serial.println("LED 2 turned ON");
    }
    else if (command == "off2") {
      digitalWrite(LED2, LOW);
      led2State = false;
      Serial.println("LED 2 turned OFF");
    }

    // Process commands for LED 3
    else if (command == "on3") {
      digitalWrite(LED3, HIGH);
      led3State = true;
      Serial.println("LED 3 turned ON");
    }
    else if (command == "off3") {
      digitalWrite(LED3, LOW);
      led3State = false;
      Serial.println("LED 3 turned OFF");
    }

    // Command to get the status of all LEDs
    else if (command == "status") {
      sendStatus();
    }

    // Unknown command
    else {
      Serial.println("Unknown command: ");
      Serial.println(command);
    }
  }
}

// Function to send the status of all LEDs
void sendStatus() {
  Serial.print("STATUS: ");
  Serial.print("LED1=");
  Serial.print(led1State ? "ON" : "OFF");
  Serial.print(", LED2=");
  Serial.print(led2State ? "ON" : "OFF");
  Serial.print(", LED3=");
  Serial.println(led3State ? "ON" : "OFF");
}