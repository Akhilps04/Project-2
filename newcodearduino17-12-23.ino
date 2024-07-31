#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
String tagUIDs[10];  // Increase size as needed.
String tagNames[10]; // Increase size as needed.
int numTags = 0;     // Number of tags initialized to zero.

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Place your card:");
  Serial.println("Send tag list:");
}

void loop() {
  // If there's data on the serial line, assume it's a UID to be added.
  if (Serial.available() > 0) {
    // Read the incoming tag UID
    String uid = Serial.readStringUntil('\n');
    uid.trim(); // Remove any whitespace
    if (uid.length() > 0) {
      tagUIDs[numTags] = uid;
      tagNames[numTags] = "Bus" + String(numTags + 1); // Assign a default name
      numTags++;
      Serial.print("Tag received and added: ");
      Serial.println(uid);
    }
  }

  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String tagUID = getTagUID();
    tagUID.toUpperCase(); // Convert UID to uppercase to match the format of the stored UIDs.
    
    // Check if the tag is in the list
    int tagIndex = findTagIndex(tagUID);
    if (tagIndex != -1) {
      const char* tagName = tagNames[tagIndex].c_str();
      Serial.println("Access granted for: " + String(tagName));
      // Add your code for granting access here
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
  return tagUID;
}

int findTagIndex(const String& tagUID) {
  // Check if the received tag UID is in the list of allowed tags
  for (int i = 0; i < numTags; i++) {
    if (tagUID.equalsIgnoreCase(tagUIDs[i])) {
      return i; // Return the index of the found UID
    }
  }
  return -1; // Return -1 if the UID is not found
}
