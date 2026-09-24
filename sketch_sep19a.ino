#include <IRremote.hpp>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

using namespace std;

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int IR_PIN = 2;
const int SERVO_PIN = 12;
Servo bar_gate;
const int SPEAKER_PIN = 8;
const int LDR_PIN = A0;
uint16_t id;
uint16_t verified_ids[] = {
        0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9
};

bool match = 0;


void setup() {
    Serial.begin(115200);

    bar_gate.attach(SERVO_PIN);
    bar_gate.write(0);

    pinMode(SPEAKER_PIN, OUTPUT);
    
    Wire.begin();
    lcd.clear();
    lcd.init();
    lcd.backlight();

    // Start listening for infrared signals
    IrReceiver.begin(IR_PIN);
}

void loop() {

    // Did we receive an IR signal?
    if (IrReceiver.decode()) {

        Serial.println("IR Received");
        IrReceiver.printIRResultShort(&Serial);


        if (IrReceiver.decodedIRData.protocol != UNKNOWN) {
          id = IrReceiver.decodedIRData.command;
        } else {
          id = 999;
        }

        IrReceiver.resume();

        match = 0;

        for (int i = 0; i < (sizeof(verified_ids) / sizeof(verified_ids[0])); i++ ) {
          if (verified_ids[i] == id) {
            match = 1;
            break;
            
          } 
        }

        switch (match) {

          case 0:
            
           
            break;

          case 1:
            bar_gate.write(90);

            lcd.setCursor(0, 0);
            lcd.print("ID: ");
            lcd.print(id, HEX);
            lcd.setCursor(0, 1);
            lcd.print("Access granted");

            for (int i = 0; i < 6; i++) {
              tone(SPEAKER_PIN, 440);
              delay(150);
              noTone(SPEAKER_PIN);
              delay(150);
            }

            IrReceiver.stop();

            IrReceiver.begin(IR_PIN);

            delay(1000);
            
            while (analogRead(LDR_PIN) >= 100) {
              delay(100);
            }

            bar_gate.write(0);
              
            lcd.clear();
            match = false;
            break;

        }

    }aaa

}
