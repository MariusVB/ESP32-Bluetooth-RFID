#include <BleKeyboard.h>
#include <Arduino.h>
#include <MFRC522_I2C.h>

// Declare variables
#define RST 14
#define SDA 23
#define SCL 22
#define DEVICE_NAME "RFID Scanner"
#define READ_TIMEOUT_MS 3000

// Declare scanner
MFRC522 mfrc522(0x28, RST);
// Declare keyboard with Bluetooth device name
BleKeyboard bleKeyboard("RFID Scanner");

// Forward function declaration
String getRFIDString(MFRC522::Uid);

void setup() {
    Serial.begin(115200);
    bleKeyboard.begin();

    // Begin SDA and SCL Wire
    Wire.begin(SDA,SCL);
    // Init Scanner
    mfrc522.PCD_Init();
}

int lastRead = 0;
void loop() {

    bool cardRead = mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial();
    bool timeoutOver = (lastRead + READ_TIMEOUT_MS) < millis();
    
    if (bleKeyboard.isConnected() && cardRead && timeoutOver ) {
        String result = getRFIDString(mfrc522.uid);        
        Serial.println("RFID: " + result);
        bleKeyboard.println(result);
        lastRead = millis();
    }
    
    delay(50);
}

String getRFIDString(MFRC522::Uid uid) {
    String result = "";
    for (byte i = 0; i < uid.size; i++) {
        if (i != 0) {
            result += " ";
        }
        if (uid.uidByte[i] < 0x10) {
            result += "0";
        }
        result += String(mfrc522.uid.uidByte[i], HEX);
    }
    result.toUpperCase();
    return result;
}