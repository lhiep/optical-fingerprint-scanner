#include <Adafruit_Fingerprint.h>

#define LED_PIN 5

#define FINGERPRINT_SCANNER_INPUT 2      // GREEN WIRE
#define FINGERPRINT_SCANNER_OUTPUT 3     // WHITE WIRE

// Define input/output pins on fingerprint scanner for software serial communication
SoftwareSerial fpScanner(FINGERPRINT_SCANNER_INPUT, FINGERPRINT_SCANNER_OUTPUT);

// Initialize fingerprint scanner
Adafruit_Fingerprint fps = Adafruit_Fingerprint(&fpScanner);

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);
  Serial.println("Welcome!");

  // Set up fingerprint scanner with data rate of 57600 Hz
  fps.begin(57600);

  if (fps.verifyPassword()) {
    Serial.println("Fingerprint scanner found. Selection an option below:");
  } else {
    Serial.println("Could not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  // Set up LED pins
  pinMode(LED_PIN, OUTPUT);

}

void loop() {

  displayOptions();

  int input = readInput();

  if(input==1) { // enrolling new fingerprint
    Serial.println("-------------------------------------------------------------");
    Serial.println("Please select an address to save this fingerprint in. (1-127)");
    uint8_t id = readInput();
    Serial.println("-------------------------------------------------------------");
    Serial.print("Enrolling new fingerprint in address #");
    Serial.println(id);
    enrollFingerPrint(id);
  } else if(input==2) {
    Serial.println("-------------------------------------------------------------");
    Serial.println("Please select an address to delete. (1-127)");
    uint8_t id = readInput();
    Serial.println("-------------------------------------------------------------");
    Serial.print("Deleting fingerprint at address #");
    Serial.println(id);
    deleteFingerPrint(id);
  } else if(input==3) {
    Serial.println("-------------------------------------------------------------");
    Serial.println("Looking up fingerprints ...");
    Serial.print(viewStat()); Serial.println(" fingerprints found!");
    Serial.println("-------------------------------------------------------------");
  } else if(input==4) {
    Serial.println("-------------------------------------------------------------");
    Serial.println("Locking device ...");
    lock();
  }

}

// view status
int viewStat() {
  int counter = 0;
  for (int finger = 1; finger < 10; finger++) {
    if(downloadFingerprint(finger) > 0) {
      counter = counter + 1;
    }
  }
  return counter;
}

// download fingerprints from scanner
int downloadFingerprint(uint16_t id)
{
  uint8_t p = fps.loadModel(id);
  if(p==FINGERPRINT_OK) {
      return 1;
  }

  return -1;
}

// lock and unlock
void lock() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("-------------------------------------------------------------");
  Serial.println("Device is locked. Unlock with your finger");
  fps.getTemplateCount();
  Serial.print("Scanner contains "); Serial.print(fps.templateCount); Serial.println(" fingerprints.");
  Serial.println("Waiting for valid fingerprint...");

  int fingerPrint = -1;

  while(fingerPrint < 0) {
    fingerPrint = getFingerPrint();
    delay(100);
  }

  digitalWrite(LED_PIN, LOW);
  Serial.println("Valid fingerprint found. Device is unlocked");
}

// get fingerprint id
int getFingerPrint() {
  uint8_t p = fps.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = fps.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = fps.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(fps.fingerID); 
  Serial.print(" with confidence of "); Serial.println(fps.confidence);
  return fps.fingerID; 
}

// delete fingerprint
void deleteFingerPrint(uint8_t id) {
  uint8_t p = -1;
  
  p = fps.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }
  Serial.println("-------------------------------------------------------------");
}

// enrolling new fingerprint
void enrollFingerPrint(uint8_t id) {

  // waiting for finger

  Serial.println("Please put your finger on the scanner");
  int p = -1;
  while (p != FINGERPRINT_OK) {
    p = fps.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // finger detected
  
  p = fps.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // finger scanned

  Serial.println("Please remove finger from scanner");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = fps.getImage();
  }

  p = -1;
  Serial.println("Please place the same finger on the scanner again");
  while (p != FINGERPRINT_OK) {
    p = fps.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // finger detected
  p = fps.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // finger scanned
  
  Serial.print("Creating model");
  
  p = fps.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // fingerprints matched, ready to enroll
  
  p = fps.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.println("-------------------------------------------------------------");
  
}

// display options
void displayOptions() {
  Serial.println("[1] Enroll new fingerprint");
  Serial.println("[2] Delete fingerprint");
  Serial.println("[3] View status");
  Serial.println("[4] Lock");
}

// read input from user
uint8_t readInput() {
  uint8_t input = 0;
  
  while (input < 1 || input > 127) {
    while (! Serial.available());
    input = Serial.parseInt();
  }
  return input;
}
