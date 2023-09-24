// ------------------DMX-----------------------
#include <Arduino.h>
#include <DMXSerial.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

byte maxBrightness     = 200;    // brightness range [off..on] = [0..255], keep dim for less current draw
byte brightness,
     effect,
     effectspeed,
     redLevel_1,  // store the received channel level control data
     grnLevel_1,
     bluLevel_1,
     relaisLevel_1,
     relaisLevel_2,
     relaisLevel_3,
     relaisLevel_4,
     relaisLevel_5,
     relaisLevel_6;

byte brightnessCh      =   0,   // DMX channel offsets from base channel
     strobe            =   1,
     effectCh          =   2,
     effectspeedCh     =   3,
     redCh_1           =   4,
     grnCh_1           =   5,
     bluCh_1           =   6,
     relaisCh_1        =   7,
     relaisCh_2        =   8,
     relaisCh_3        =   9,
     relaisCh_4        =   10,
     relaisCh_5        =   11,
     relaisCh_6        =   12;

#define PIN 13 // Hier wird angegeben, an welchem digitalen Pin die WS2812 LEDs bzw. NeoPixel angeschlossen sind
#define NUMPIXELS 60 // Hier wird die AndmxBaseCh der angeschlossenen WS2812 LEDs bzw. NeoPixel angegeben
int Pixel_0 = 0,
    Abstand_Pixel = 1;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


// ------------------TM1637Display-----------------------
#include <TM1637Display.h>
// Module connection pins (Digital Pins)
TM1637Display display(11, 10); // CLK, DIO

const uint8_t d[] = {SEG_B | SEG_C | SEG_D | SEG_E | SEG_G};
const uint8_t A[] = {SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G};
const uint8_t C[] = {SEG_A | SEG_D | SEG_E | SEG_F };
const uint8_t Sound[] = {SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,
                         SEG_C | SEG_D | SEG_E | SEG_G,
                         SEG_C | SEG_D | SEG_E,
                         SEG_B | SEG_C | SEG_D | SEG_E | SEG_G};
const uint8_t Bindestrich[] = {SEG_G,
                               SEG_G,
                               SEG_G,
                               SEG_G};

// ------------------EEPROM-----------------------
#include <EEPROM.h>
int Adresse_0 = 0,    // Speicher DMX-Adresse
    Adresse_1 = 1,
    Adresse_2 = 2,
    Adresse_3 = 3,
    Adresse_4 = 4,
    Adresse_5 = 5,
    Adresse_6 = 6,
    Adresse_7 = 7,
    Adresse_8 = 8,
    Adresse_9 = 9,    // Speicher DMX-Adresse
    Adresse_10 = 10,    // Speicher Hauptmenü DMX-Modus oder Autoprogramm
    Adresse_11 = 11;    // Speicher Autoprogramm

// ------------------Taster-----------------------
int TasteUP = 4,    // Pin vom Taster
    StatusTasteUP,    // Zustand vom Taster
    TasteDOWN = 5,
    StatusTasteDOWN,
    TasteMODE = 6,
    StatusTasteMODE,
    TasteENTER = 7,
    StatusTasteENTER;


// ------------------Sound-----------------------
int sound_digital = 12,
    sound_analog = A0;

// ------------------Relais-----------------------
int relais_1 = 8,
    relais_2 = 9,
    relais_3 = 2,
    relais_4 = 3,
    relais_5 = A2,
    relais_6 = A3;
    

// Wert Relais schalten
int WertRelaisHigh = 200,
    WertRelaisLow = 10;

int RelaisAnAus1 = 0,
    RelaisAnAus2 = 1;

// ------------------Variabeln-----------------------
int dmxBaseCh,    // DMX-Adresse
    stelle,
    MODE = 0,
    ModeMenueZaehler,    // EEprom letzter Zustand
    Hauptmenuezaehler,    // EEprom letzter Zustand
    Hauptmenue;    // Wert kommt aus/wird berechnet aus dem EEPROM

// Zwischenspeicher Hauptmenü Funktion (in welcher Funktion er ist)
int DMX_Modus_Nix,
    DMX_Adresse_einstellen,
    Autoprogramme;


// -------------------CH07------------------------------------------------------------------

void ansteuern(int r, int g, int b){
  for(int x=Pixel_0; x<=NUMPIXELS; x++){
    pixels.setPixelColor(x, pixels.Color(r,g,b));
    pixels.show(); // Durchführen der Pixel-Ansteuerung
  }
}

// -------------------------------------------------------------------------------------

void AnsteuernRelais(){
  if(relaisLevel_1 > WertRelaisHigh){
    digitalWrite(relais_1, RelaisAnAus1);
  }
  if(relaisLevel_1 < WertRelaisLow){
    digitalWrite(relais_1, RelaisAnAus2);
  }


  if(relaisLevel_2 > WertRelaisHigh){
    digitalWrite(relais_2, RelaisAnAus1);
  }
  if(relaisLevel_2 < WertRelaisLow){
    digitalWrite(relais_2, RelaisAnAus2);
  }


  if(relaisLevel_3 > WertRelaisHigh){
    digitalWrite(relais_3, RelaisAnAus1);
  }
  if(relaisLevel_3 < WertRelaisLow){
    digitalWrite(relais_3, RelaisAnAus2);
  }


  if(relaisLevel_4 > WertRelaisHigh){
    digitalWrite(relais_4, RelaisAnAus1);
  }
  if(relaisLevel_4 < WertRelaisLow){
    digitalWrite(relais_4, RelaisAnAus2);
  }


  if(relaisLevel_5 > WertRelaisHigh){
    digitalWrite(relais_5, RelaisAnAus1);
  }
  if(relaisLevel_5 < WertRelaisLow){
    digitalWrite(relais_5, RelaisAnAus2);
  }


  if(relaisLevel_6 > WertRelaisHigh){
    digitalWrite(relais_6, RelaisAnAus1);
  }
  if(relaisLevel_6 < WertRelaisLow){
    digitalWrite(relais_6, RelaisAnAus2);
  }
}

// -------------------------------------------------------------------------------------

void DMXauslesen(){
  effect = DMXSerial.read(dmxBaseCh + effectCh);     // Dmx Effect Wert auslesen
  effectspeed = DMXSerial.read(dmxBaseCh + effectspeedCh);     // Dmx Effect Speed Wert auslesen
  
  redLevel_1 = DMXSerial.read(dmxBaseCh + redCh_1);     // Dmx Rot Wert auslesen
  grnLevel_1 = DMXSerial.read(dmxBaseCh + grnCh_1);     // Dmx Grün Wert auslesen
  bluLevel_1 = DMXSerial.read(dmxBaseCh + bluCh_1);     // Dmx Blau Wert auslesen

  relaisLevel_1 = DMXSerial.read(dmxBaseCh + relaisCh_1);     // Dmx Relais Wert auslesen
  relaisLevel_2 = DMXSerial.read(dmxBaseCh + relaisCh_2);     // Dmx Relais Wert auslesen
  relaisLevel_3 = DMXSerial.read(dmxBaseCh + relaisCh_3);     // Dmx Relais Wert auslesen
  relaisLevel_4 = DMXSerial.read(dmxBaseCh + relaisCh_4);     // Dmx Relais Wert auslesen
  relaisLevel_5 = DMXSerial.read(dmxBaseCh + relaisCh_5);     // Dmx Relais Wert auslesen
  relaisLevel_6 = DMXSerial.read(dmxBaseCh + relaisCh_6);     // Dmx Relais Wert auslesen

  brightness = DMXSerial.read(dmxBaseCh + brightnessCh);     // Dmx Dimmer Wert auslesen
  brightness = map(brightness, 1, 255, 0, maxBrightness);   // Dimmer Wert runter rechnen

  redLevel_1 = float(redLevel_1) * (float(brightness) / float(maxBrightness));    // Dimmer auf Farbe anwenden
  grnLevel_1 = float(grnLevel_1) * (float(brightness) / float(maxBrightness));    // Dimmer auf Farbe anwenden
  bluLevel_1 = float(bluLevel_1) * (float(brightness) / float(maxBrightness));    // Dimmer auf Farbe anwenden
}

// -------------------------------------------------------------------------------------

void DMXsteuern(){
  int wait = map(effectspeed, 0, 255, 0, 75); // Wert, untere Grenze, obere Grenze, untere Grenze Ziel, obere Grenze Ziel

  if (effect < 9) {
    ansteuern(redLevel_1, grnLevel_1, bluLevel_1);    // RGB Farben ansteuern
    AnsteuernRelais();
  }
  else if (effect < 30 && effect > 10){
    rainbow(wait);
  }
  else if (effect < 50 && effect > 31){
    Lauflicht_2(wait);
  }
  else if (effect < 255 && effect > 51){
    Lauflicht_4_S();
  }
}

// -------------------------------------------------------------------------------------

void zaehlen(){     // DMX Adresse bei Wert Überschreitung (Hoch / Runter zählen) verändern 
  dmxwertzeigen();
  if(dmxBaseCh < 1){
    dmxBaseCh = 512;
  }
  if(dmxBaseCh >= 513){
    dmxBaseCh = 1;
  }
}

// -------------------------------------------------------------------------------------

void dmxwertzeigen(){     // DMX Wert anzeigen d001
  display.showNumberDec(dmxBaseCh, false, 4, 0);
  display.setSegments(d, 1, 0);
}

// -------------------------------------------------------------------------------------

void ModeMenue(){     // Autoprogramm Wert zeigen A001
  display.showNumberDec(ModeMenueZaehler, false, 4, 0);
  display.setSegments(A, 1, 0);
}

// -------------------------------------------------------------------------------------

void eepromauslesen(){
  dmxBaseCh = (EEPROM.read(Adresse_0) * 1) + (EEPROM.read(Adresse_1) * 2) + (EEPROM.read(Adresse_2) * 4) + (EEPROM.read(Adresse_3) * 8) + (EEPROM.read(Adresse_4) * 16) + (EEPROM.read(Adresse_5) * 32) + (EEPROM.read(Adresse_6) * 64) + (EEPROM.read(Adresse_7) * 128) + (EEPROM.read(Adresse_8) * 256) + (EEPROM.read(Adresse_9) * 512);
}

// -------------------------------------------------------------------------------------

void eepromschreiben(){
  if(StatusTasteENTER == HIGH){
    int speicher = dmxBaseCh / 2;
    int zs = speicher *2;
    int neu = dmxBaseCh - zs;    // Speichern in bit
    if(speicher == 0 && zs == 0){
      stelle = 0;
      goto bailout;
    }
    
    int speicher1;
    speicher1 = speicher / 2;
    int zs1 = speicher1 *2;
    int neu1 = speicher - zs1;    // Speichern in bit
    if(speicher1 == 0 && zs1 == 0){
      stelle = 1;
      goto bailout;
    }

    int speicher2;
    speicher2 = speicher1 / 2;
    int zs2 = speicher2 *2;
    int neu2 = speicher1 - zs2;    // Speichern in bit
    if(speicher2 == 0 && zs2 == 0){
      stelle = 2;
      goto bailout;
    }

    int speicher3;
    speicher3 = speicher2 / 2;
    int zs3 = speicher3 *2;
    int neu3 = speicher2 - zs3;    // Speichern in bit
    if(speicher3 == 0 && zs3 == 0){
      stelle = 3;
      goto bailout;
    }

    int speicher4;
    speicher4 = speicher3 / 2;
    int zs4 = speicher4 *2;
    int neu4 = speicher3 - zs4;    // Speichern in bit
    if(speicher4 == 0 && zs4 == 0){
    stelle = 4;
      goto bailout;
    }

    int speicher5;
    speicher5 = speicher4 / 2;
    int zs5 = speicher5 *2;       // Wenn beide hier null dann abbruch
    int neu5 = speicher4 - zs5;    // Speichern in bit
    if(speicher5 == 0 && zs5 == 0){
      stelle = 5;
      goto bailout;
    }

    int speicher6;
    speicher6 = speicher5 / 2;
    int zs6 = speicher6 *2;       // Wenn beide hier null dann abbruch
    int neu6 = speicher5 - zs6;    // Speichern in bit
    if(speicher6 == 0 && zs6 == 0){
      stelle = 6;
      goto bailout;
    }

    int speicher7;
    speicher7 = speicher6 / 2;
    int zs7 = speicher7 *2;       // Wenn beide hier null dann abbruch
    int neu7 = speicher6 - zs7;    // Speichern in bit
    if(speicher7 == 0 && zs7 == 0){
      stelle = 7;
      goto bailout;
    }

    int speicher8;
    speicher8 = speicher7 / 2;
    int zs8 = speicher8 *2;       // Wenn beide hier null dann abbruch
    int neu8 = speicher7 - zs8;    // Speichern in bit
    if(speicher8 == 0 && zs8 == 0){
      stelle = 8;
      goto bailout;
    }

    int speicher9;
    speicher9 = speicher8 / 2;
    int zs9 = speicher9 *2;       // Wenn beide hier null dann abbruch
    int neu9 = speicher8 - zs9;    // Speichern in bit
    if(speicher9 == 0 && zs9 == 0){
      stelle = 9;
      goto bailout;
    }

    bailout:
    if(stelle == 9){
      EEPROM.write(Adresse_9, neu9);EEPROM.write(Adresse_8, neu8);EEPROM.write(Adresse_7, neu7);EEPROM.write(Adresse_6, neu6);EEPROM.write(Adresse_5, neu5);EEPROM.write(Adresse_4, neu4);EEPROM.write(Adresse_3, neu3);EEPROM.write(Adresse_2, neu2);EEPROM.write(Adresse_1, neu1);EEPROM.write(Adresse_0, neu);
    }
    if(stelle == 8){
      EEPROM.write(Adresse_9, 0);EEPROM.write(Adresse_8, neu8);EEPROM.write(Adresse_7, neu7);EEPROM.write(Adresse_6, neu6);EEPROM.write(Adresse_5, neu5);EEPROM.write(Adresse_4, neu4);EEPROM.write(Adresse_3, neu3);EEPROM.write(Adresse_2, neu2);EEPROM.write(Adresse_1, neu1);EEPROM.write(Adresse_0, neu);
    }
    if(stelle == 7){
      EEPROM.write(Adresse_9, 0);EEPROM.write(Adresse_8, 0);EEPROM.write(Adresse_7, neu7);EEPROM.write(Adresse_6, neu6);EEPROM.write(Adresse_5, neu5);EEPROM.write(Adresse_4, neu4);EEPROM.write(Adresse_3, neu3);EEPROM.write(Adresse_2, neu2);EEPROM.write(Adresse_1, neu1);EEPROM.write(Adresse_0, neu);
    }
    if(stelle == 6){
      EEPROM.write(Adresse_9, 0);EEPROM.write(Adresse_8, 0);EEPROM.write(Adresse_7, 0);EEPROM.write(Adresse_6, neu6);EEPROM.write(Adresse_5, neu5);EEPROM.write(Adresse_4, neu4);EEPROM.write(Adresse_3, neu3);EEPROM.write(Adresse_2, neu2);EEPROM.write(Adresse_1, neu1);EEPROM.write(Adresse_0, neu);
    }
    if(stelle == 5){
      EEPROM.write(Adresse_9, 0);EEPROM.write(Adresse_8, 0);EEPROM.write(Adresse_7, 0);EEPROM.write(Adresse_6, 0);EEPROM.write(Adresse_5, neu5);EEPROM.write(Adresse_4, neu4);EEPROM.write(Adresse_3, neu3);EEPROM.write(Adresse_2, neu2);EEPROM.write(Adresse_1, neu1);EEPROM.write(Adresse_0, neu);
    }
    if(stelle == 4){
      EEPROM.write(Adresse_9, 0);EEPROM.write(Adresse_8, 0);EEPROM.write(Adresse_7, 0);EEPROM.write(Adresse_6, 0);EEPROM.write(Adresse_5, 0);EEPROM.write(Adresse_4, neu4);EEPROM.write(Adresse_3, neu3);EEPROM.write(Adresse_2, neu2);EEPROM.write(Adresse_1, neu1);EEPROM.write(Adresse_0, neu);
    }
    if(stelle == 3){
      EEPROM.write(Adresse_9, 0);EEPROM.write(Adresse_8, 0);EEPROM.write(Adresse_7, 0);EEPROM.write(Adresse_6, 0);EEPROM.write(Adresse_5, 0);EEPROM.write(Adresse_4, 0);EEPROM.write(Adresse_3, neu3);EEPROM.write(Adresse_2, neu2);EEPROM.write(Adresse_1, neu1);EEPROM.write(Adresse_0, neu);
    }
    if(stelle == 2){
      EEPROM.write(Adresse_9, 0);EEPROM.write(Adresse_8, 0);EEPROM.write(Adresse_7, 0);EEPROM.write(Adresse_6, 0);EEPROM.write(Adresse_5, 0);EEPROM.write(Adresse_4, 0);EEPROM.write(Adresse_3, 0);EEPROM.write(Adresse_2, neu2);EEPROM.write(Adresse_1, neu1);EEPROM.write(Adresse_0, neu);
    }
    if(stelle == 1){
      EEPROM.write(Adresse_9, 0);EEPROM.write(Adresse_8, 0);EEPROM.write(Adresse_7, 0);EEPROM.write(Adresse_6, 0);EEPROM.write(Adresse_5, 0);EEPROM.write(Adresse_4, 0);EEPROM.write(Adresse_3, 0);EEPROM.write(Adresse_2, 0);EEPROM.write(Adresse_1, neu1);EEPROM.write(Adresse_0, neu);
    }
    if(stelle == 0){
      EEPROM.write(Adresse_9, 0);EEPROM.write(Adresse_8, 0);EEPROM.write(Adresse_7, 0);EEPROM.write(Adresse_6, 0);EEPROM.write(Adresse_5, 0);EEPROM.write(Adresse_4, 0);EEPROM.write(Adresse_3, 0);EEPROM.write(Adresse_2, 0);EEPROM.write(Adresse_1, 0);EEPROM.write(Adresse_0, neu);   // Wert in den EEPROM schreiben
    }
  }
}

// -------------------------------------------------------------------------------------

void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<pixels.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
    pixels.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// -------------------------------------------------------------------------------------

void AlleRot(){
  for(int x=Pixel_0; x<=NUMPIXELS; x++){
    pixels.setPixelColor(x, pixels.Color(255,0,0)); // Pixel leuchtet in der Farbe Rot
    pixels.show(); // Durchführen der Pixel-Ansteuerung
  }
}

// -------------------------------------------------------------------------------------

void AlleGruen(){
  for(int x=Pixel_0; x<=NUMPIXELS; x++){
    pixels.setPixelColor(x, pixels.Color(0,255,0)); // Pixel leuchtet in der Farbe Rot
    pixels.show(); // Durchführen der Pixel-Ansteuerung
  }
}

// -------------------------------------------------------------------------------------

void AlleBlau(){
  for(int x=Pixel_0; x<=NUMPIXELS; x++){
    pixels.setPixelColor(x, pixels.Color(0,0,255)); // Pixel leuchtet in der Farbe Rot
    pixels.show(); // Durchführen der Pixel-Ansteuerung
  }
}

// -------------------------------------------------------------------------------------

void AlleGelb(){
  for(int x=Pixel_0; x<=NUMPIXELS; x++){
    pixels.setPixelColor(x, pixels.Color(255,255,0)); // Pixel leuchtet in der Farbe Rot
    pixels.show(); // Durchführen der Pixel-Ansteuerung
  }
}

// -------------------------------------------------------------------------------------

void AlleTuerkis(){
  for(int x=Pixel_0; x<=NUMPIXELS; x++){
    pixels.setPixelColor(x, pixels.Color(0,255,255)); // Pixel leuchtet in der Farbe Rot
    pixels.show(); // Durchführen der Pixel-Ansteuerung
  }
}

// -------------------------------------------------------------------------------------

void AlleLila(){
  for(int x=Pixel_0; x<=NUMPIXELS; x++){
    pixels.setPixelColor(x, pixels.Color(255,0,255)); // Pixel leuchtet in der Farbe Rot
    pixels.show(); // Durchführen der Pixel-Ansteuerung
  }
}

// -------------------------------------------------------------------------------------

void AlleWeiss(){
  for(int x=Pixel_0; x<=NUMPIXELS; x++){
    pixels.setPixelColor(x, pixels.Color(255,255,255)); // Pixel leuchtet in der Farbe Rot
    pixels.show(); // Durchführen der Pixel-Ansteuerung
  }
}

// -------------------------------------------------------------------------------------

void AlleRot_Reverse(){
  for(int x=Pixel_0; x<=NUMPIXELS; x++){
    pixels.setPixelColor(NUMPIXELS -x, pixels.Color(255,0,0)); // Pixel leuchtet in der Farbe Rot
    pixels.show(); // Durchführen der Pixel-Ansteuerung
  }
}

// -------------------------------------------------------------------------------------

void AlleWeiss_Reverse(){
  for(int x=Pixel_0; x<=NUMPIXELS; x++){
    pixels.setPixelColor(NUMPIXELS -x, pixels.Color(255,255,255)); // Pixel leuchtet in der Farbe Rot
    pixels.show(); // Durchführen der Pixel-Ansteuerung
  }
}

// -------------------------------------------------------------------------------------

void AlleAus(){
  for(int x=Pixel_0; x<=NUMPIXELS; x++){
    pixels.setPixelColor(x, pixels.Color(0,0,0)); // Pixel leuchtet in der Farbe Rot
    pixels.show(); // Durchführen der Pixel-Ansteuerung
  }
}

// -------------------------------------------------------------------------------------

void Lauflicht_1(int wait_1_1, int wait_1_2){
  int nummer = random(1, 8);
  int wait_1_3 = random(wait_1_1, wait_1_2);
  switch(nummer){
    case 1:
      AlleRot();  delay(wait_1_3);
      break;
    case 2:
      AlleGruen();  delay(wait_1_3);
      break;
    case 3:
      AlleBlau();  delay(wait_1_3);
      break;
    case 4:
      AlleGelb();  delay(wait_1_3);
      break;
    case 5:
      AlleTuerkis();  delay(wait_1_3);
      break;
    case 6:
      AlleLila();  delay(wait_1_3);
      break;
    case 7:
      AlleWeiss();  delay(wait_1_3);
      break;
  }
}

// -------------------------------------------------------------------------------------

void Lauflicht_2(int wait_2_1){
  int nummer = random(1, 8);
  switch(nummer){
    case 1:
      AlleRot();  delay(wait_2_1);
      break;
    case 2:
      AlleGruen();  delay(wait_2_1);
      break;
    case 3:
      AlleBlau();  delay(wait_2_1);
      break;
    case 4:
      AlleGelb();  delay(wait_2_1);
      break;
    case 5:
      AlleTuerkis();  delay(wait_2_1);
      break;
    case 6:
      AlleLila();  delay(wait_2_1);
      break;
    case 7:
      AlleWeiss();  delay(wait_2_1);
      break;
  }
}

// -------------------------------------------------------------------------------------

void Lauflicht_3(int wait_3_1, int wait_3_2){
  int nummer = random(1, 8);
  int wait_3_3 = random(wait_3_1, wait_3_2);
  switch(nummer){
    case 1:
      AlleRot();  delay(wait_3_3);
      break;
    case 2:
      AlleGruen();  delay(wait_3_3);
      break;
    case 3:
      AlleBlau();  delay(wait_3_3);
      break;
    case 4:
      AlleGelb();  delay(wait_3_3);
      break;
    case 5:
      AlleTuerkis();  delay(wait_3_3);
      break;
    case 6:
      AlleLila();  delay(wait_3_3);
      break;
    case 7:
      AlleWeiss();  delay(wait_3_3);
      break;
  }
}

// -------------------------------------------------------------------------------------

void Lauflicht_4_S(){
  int val_digital = digitalRead(sound_digital);
  int nummer;
  if (val_digital == HIGH){
    nummer = random(1, 8);
  }
  switch(nummer){
    case 1:
      AlleRot();
      break;
    case 2:
      AlleGruen();
      break;
    case 3:
      AlleBlau();
      break;
    case 4:
      AlleGelb();
      break;
    case 5:
      AlleTuerkis();
      break;
    case 6:
      AlleLila();
      break;
    case 7:
      AlleWeiss();
      break;
  }
}

// -------------------------------------------------------------------------------------

void Lauflicht_5(int wait_5){
  int nummer = random(1, 8);
  switch(nummer){
    case 1:
      AlleRot_Reverse();  delay(wait_5);
      break;
    case 2:
      AlleGruen();  delay(wait_5);
      break;
    case 3:
      AlleBlau();  delay(wait_5);
      break;
    case 4:
      AlleGelb();  delay(wait_5);
      break;
    case 5:
      AlleTuerkis();  delay(wait_5);
      break;
    case 6:
      AlleLila();  delay(wait_5);
      break;
    case 7:
      AlleWeiss_Reverse();  delay(wait_5);
      break;
  }
}

// ------------Hauptmenü-------------------------------------------------------------------------

void HauptMenu(){
  TasterAuslesen();
  
  if(StatusTasteMODE == HIGH){
    delay(100);
    Hauptmenue = 1;
    Hauptmenuezaehler++;
    if(Hauptmenuezaehler >= 5){
      Hauptmenuezaehler = 1;
    }
  }

  switch(Hauptmenuezaehler){
    case 1:
      DMX_Modus_Nix = 1;
      while(DMX_Modus_Nix == 1){
        display.setSegments(Bindestrich, 4, 0);
        DMXauslesen();
        DMXsteuern();
        TasterAuslesenDMXModus();
        if(StatusTasteENTER == HIGH){   // Wenn Enter gedrückt, dann
          EEPROM.write(Adresse_10, Hauptmenuezaehler);   // Sperichere Wert im EEPROM
          delay(100);
        }
        if(StatusTasteMODE == HIGH){
          delay(100);
          DMX_Modus_Nix = 0;
          Hauptmenuezaehler++;
          if(Hauptmenuezaehler >= 5){
            Hauptmenuezaehler = 1;
          }
          AlleAus();
          return;
        }
      }
      break;
    case 2:
      DMX_Adresse_einstellen = 1;
      dmxwertzeigen();
      while(DMX_Adresse_einstellen == 1){
        dmxBaseChberechnen();
        eepromschreiben();
        TasterAuslesen();
        if(StatusTasteMODE == HIGH){
          delay(100);
          DMX_Adresse_einstellen = 0;
          Hauptmenuezaehler++;
          if(Hauptmenuezaehler >= 5){
            Hauptmenuezaehler = 1;
          }
          return;
        }
      }
      break;
    case 3:
      Autoprogramme = 1;
      ModeMenue();
      while(Autoprogramme == 1){
        ModeFunktion();
        TasterAuslesen();
        if(StatusTasteMODE == HIGH){
          delay(100);
          Autoprogramme = 0;
          Hauptmenuezaehler++;
          if(Hauptmenuezaehler >= 5){
            Hauptmenuezaehler = 1;
          }
          return;
        }
      }
      break;
  }
}

// ------------Autoprogramme-------------------------------------------------------------------------

void ModeFunktion(){
  TasterAuslesen();
  if(StatusTasteUP == HIGH){
    ModeMenueZaehler++;
    pruefenModeMenu();
    ModeMenue();
  }
  if(StatusTasteDOWN == HIGH){
    ModeMenueZaehler--;
    pruefenModeMenu();
    ModeMenue();
  }
  
  if(StatusTasteENTER == HIGH){   // Wenn Enter gedrückt, dann
    EEPROM.write(Adresse_10, Hauptmenuezaehler);   // Speichere Wert Hauptmenü im EEPROM
    EEPROM.write(Adresse_11, ModeMenueZaehler);   // Speichere Wert Autoprogramm im EEPROM
    delay(100);
  }

  switch(ModeMenueZaehler){
    case 1:
      AlleRot();
      break;
    case 2:
      AlleGruen();
      break;
    case 3:
      AlleBlau();
      break;
    case 4:
      AlleGelb();
      break;
    case 5:
      AlleTuerkis();
      break;
    case 6:
      AlleLila();
      break;
    case 7:
      AlleWeiss();
      break;
    case 8:
      Lauflicht_4_S();
      break;
    case 9:
      Lauflicht_1(100, 1000);
      break;
    case 10:
      Lauflicht_2(1000);
      break;
    case 11:
      Lauflicht_1(5000, 10000);
      break;
    case 13:
      rainbow(75);
      break;
    case 12:
      Lauflicht_5(1000);
      break;
  }
}

// -------------------------------------------------------------------------------------
void TasterAuslesen(){
  StatusTasteUP = digitalRead(TasteUP);
  StatusTasteDOWN = digitalRead(TasteDOWN);
  StatusTasteMODE = digitalRead(TasteMODE);
  StatusTasteENTER = digitalRead(TasteENTER);
  delay(100);
}

void TasterAuslesenDMXModus(){
  StatusTasteMODE = digitalRead(TasteMODE);
  StatusTasteENTER = digitalRead(TasteENTER);
  delay(50);
}

// -------------------------------------------------------------------------------------

void dmxBaseChberechnen(){
  if(StatusTasteUP == HIGH){
    dmxBaseCh++;
    zaehlen();
  }
  if(StatusTasteDOWN == HIGH){
    dmxBaseCh--;
    zaehlen();
  }
}

// -------------------------------------------------------------------------------------

void pruefenModeMenu(){
  if(ModeMenueZaehler < 1){
    ModeMenueZaehler = 20;
  }
  if(ModeMenueZaehler >= 21){
    ModeMenueZaehler = 1;
  }
}

// ------------------------------------Setup-----------------------------------------
void setup() {
  // ------------------DMX-----------------------
  pixels.begin(); // Initialisierung der NeoPixel
  DMXSerial.init(DMXReceiver);  // Pin 2

  // ------------------Taster-----------------------
  pinMode(TasteUP, INPUT_PULLUP);
  pinMode(TasteDOWN, INPUT_PULLUP);
  pinMode(TasteMODE, INPUT_PULLUP);
  pinMode(TasteENTER, INPUT_PULLUP);

// ------------------Relais-----------------------
  pinMode(relais_1, OUTPUT);
  pinMode(relais_2, OUTPUT);
  pinMode(relais_3, OUTPUT);
  pinMode(relais_4, OUTPUT);
  pinMode(relais_5, OUTPUT);
  pinMode(relais_6, OUTPUT);

  digitalWrite(relais_1, RelaisAnAus2);
  digitalWrite(relais_2, RelaisAnAus2);
  digitalWrite(relais_3, RelaisAnAus2);
  digitalWrite(relais_4, RelaisAnAus2);
  digitalWrite(relais_5, RelaisAnAus2);
  digitalWrite(relais_6, RelaisAnAus2);

// ------------------Sound-----------------------
  pinMode(sound_digital, INPUT);

  // ------------------Display-----------------------
  display.setBrightness(0x0f);

  // ------------------EEPROM-----------------------
  eepromauslesen();

  // Werte aus dem EEPROM lesen
  Hauptmenuezaehler = EEPROM.read(Adresse_10);
  ModeMenueZaehler = EEPROM.read(Adresse_11);
}

// ------------------------------------Loop-----------------------------------------
void loop() {
  HauptMenu();
}
