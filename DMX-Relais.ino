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
    Adresse_11 = 11,    // Speicher Autoprogramm
    Adresse_12 = 12;    // Speicher Channel Auswahl

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
    Hauptmenue,
    chanel_nummer,    // Wert kommt aus/wird berechnet aus dem EEPROM
    Speicher_Channel;

// Zwischenspeicher Hauptmenü Funktion (in welcher Funktion er ist)
int DMX_Modus_Nix,
    DMX_Adresse_einstellen,
    Channel_einstellen,
    Autoprogramme;


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
  Speicher_Channel = EEPROM.read(Adresse_12);

  pruefenChannelNumber();
}

// ------------------------------------Loop-----------------------------------------
void loop() {
  HauptMenu();
}
