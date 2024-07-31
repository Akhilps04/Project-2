#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN 8  // Define the LED pin

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Define RFID tag UID and corresponding names
const char* tagUIDs[] = {
    "D16F4D24",
    "E0D74A1F", 
    "0536C52B16B0C1",
    "05346DCF08B0C1" // Replace with the actual UID of your RFID tag
  // Add more tag UIDs and corresponding names as needed
};

const char* tagNames[] = {
  "Bus1",
  "Bus2",
  "Bus3",
  "Bus4"
  // Add more names corresponding to the UIDs above
};

const int numTags = sizeof(tagUIDs) / sizeof(tagUIDs[0]);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED_PIN, OUTPUT);  // Initialize the LED pin as an output
  Serial.println("Place your card:");
}

void loop() {
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String tagUID = getTagUID();
    // Serial.println("Detected UID: " + tagUID);

    // Check if the tag is in the list
    int tagIndex = findTagIndex(tagUID);
    if (tagIndex != -1) {
      const char* tagName = tagNames[tagIndex];
      Serial.println("Access granted for: " + String(tagName));
      digitalWrite(LED_PIN, HIGH);  // Turn on the LED
      delay(1000);
      digitalWrite(LED_PIN, LOW);  // Turn off the LED// Add your code for granting access here
    } else {
      Serial.println("Access denied for unknown tag");
      // Add your code for denying access here
    }

    delay(1000); // Add a delay to prevent continuous reads for the same card
  }
}

String getTagUID() {
  String tagUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagUID += (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    tagUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  tagUID.toUpperCase();
  return tagUID;
}

int findTagIndex(const String& tagUID) {
  // Check if the received tag UID is in the list of allowed tags
  for (int i = 0; i < numTags; i++) {
    if (tagUID.equals(tagUIDs[i])) {
      return i; // Return the index of the found UID
    }
  }
  return -1; // Return -1 if the UID is not found
}
