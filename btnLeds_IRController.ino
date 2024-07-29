#include <IRremote.hpp>
#include <TinyIRSender.hpp>

// how many Buttons with LED
#define numBtns 3

// Variables will change:
int btnState[numBtns];
int lastBtnState[numBtns];
int btnPin[numBtns] = {2, 5, 8};
int ledPin[numBtns] = {3, 6, 9};
int gndPin[numBtns] = {4, 7, 10};
int reading[numBtns];
int pinNum;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime[numBtns];  // the last time the output pin was toggled
unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers
void setup() {
  Serial.begin(9600);

   //Setup Buttons and LEDS
  for (int i = 0; i < numBtns; i++) {
    pinMode(btnPin[i], INPUT_PULLUP);
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], HIGH);
    pinMode(gndPin[i], OUTPUT);
    digitalWrite(gndPin[i], LOW);
    lastBtnState[i] = HIGH;
  }
  
  uint8_t tSendPin = A3;
  IrSender.begin(tSendPin, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN); // Specify send pin and enable feedback LED at default feedback LED pin
  // You can change send pin later with IrSender.setSendPin();
  
  Serial.println("Ready");
}

String valA, inString, strpin, stradr, strcom, strsum1, strsum2, strsum3, strsum4, strsum5, strsum6, strsum7, strsum8;

void loop() {
  //Loop Button
  for (int i = 0; i < numBtns; i++) {
    reading[i] = digitalRead(btnPin[i]);
    if (reading[i] != lastBtnState[i]) {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading[i] != btnState[i]) {
        btnState[i] = reading[i];
        if (btnState[i] == LOW) {
          Serial.println(i);
        }
      }
    }
    lastBtnState[i] = reading[i];
  }  
    
  if (Serial.available() > 0) {
    valA = Serial.readString();
    inString = String(valA);

    if(valA.charAt(0) == '0'){
      digitalWrite(ledPin[0], valA.charAt(1));
    } else  if(valA.charAt(0) == 's' || valA.charAt(0) == 'S'){
      Serial.println("Samsung");
      //converter digitos 2 e 3 para o pin
      strsum1 =  valA.charAt(1);
      strsum2 = valA.charAt(2);
      strpin = strsum1 + strsum2;
      int pin = strpin.toInt();
      Serial.print("PIN: ");
      Serial.println(pin);  
      IrSender.setSendPin(pin);//seleciona a porta para envio do sinal
      //converter digitos 4 5 e 6 para o address
      strsum3 = valA.charAt(3);
      strsum4 = valA.charAt(4);
      strsum5 = valA.charAt(5);
      stradr = strsum3 + strsum4 + strsum5;
      uint16_t long adr = strtol(stradr.c_str(), NULL, 16);
      Serial.print("ADDRESS: 0x");
      Serial.println(adr,16);
      //converte digitos 7 8 e 9 para o commmand
      strsum6 = valA.charAt(6);
      strsum7 = valA.charAt(7);
      strsum8 = valA.charAt(8);
      strcom = strsum6 + strsum7 + strsum8;
      uint16_t long com = strtol(strcom.c_str(), NULL, 16);
      Serial.print("COMMAND: 0x");
      Serial.println(com, 16);
      //envia o sinal Samsung
      IrSender.sendSamsung(adr, com, 1);
    }else
    if(valA.charAt(0) == 'n' || valA.charAt(0) == 'N'){
      Serial.println("NEC");
      //converter digitos 2 e 3 para o pin
      strsum1 =  valA.charAt(1);
      strsum2 = valA.charAt(2);
      strpin = strsum1 + strsum2;
      int pin = strpin.toInt();
      Serial.print("PIN: ");
      Serial.println(pin);
      //converter digitos 4 5 e 6 para o address
      strsum3 = valA.charAt(3);
      strsum4 = valA.charAt(4);
      strsum5 = valA.charAt(5);
      stradr = strsum3 + strsum4 + strsum5;
      uint16_t long adr = strtol(stradr.c_str(), NULL, 16);
      Serial.print("ADDRESS: 0x");
      Serial.println(adr, 16);
      //converte digitos 7 8 e 9 para o commmand
      strsum6 = valA.charAt(6);
      strsum7 = valA.charAt(7);
      strsum8 = valA.charAt(8);
      strcom = strsum6 + strsum7 + strsum8;
      uint16_t long com = strtol(strcom.c_str(), NULL, 16);
      Serial.print("COMMAND: 0x");
      Serial.println(com,16);
      //envia o sinal NEC
      sendNEC(pin, adr, com, 1);
    }
  }
}
