// -------------------CH07------------------------------------------------------------------

void ansteuern(int r, int g, int b){
  if(Speicher_Channel == 7){
    for(int x=Pixel_0; x<=NUMPIXELS; x++){
      pixels.setPixelColor(x, pixels.Color(r,g,b));
      pixels.show(); // Durchführen der Pixel-Ansteuerung
    }
  }
}


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
