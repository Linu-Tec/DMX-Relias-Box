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
      AlleRot();
      delay(wait_1_3);
      break;
    case 2:
      AlleGruen();
      delay(wait_1_3);
      break;
    case 3:
      AlleBlau();
      delay(wait_1_3);
      break;
    case 4:
      AlleGelb();
      delay(wait_1_3);
      break;
    case 5:
      AlleTuerkis();
      delay(wait_1_3);
      break;
    case 6:
      AlleLila();
      delay(wait_1_3);
      break;
    case 7:
      AlleWeiss();
      delay(wait_1_3);
      break;
  }
}

// -------------------------------------------------------------------------------------

void Lauflicht_2(int wait_2_1){
  int nummer = random(1, 8);
  switch(nummer){
    case 1:
      AlleRot();
      delay(wait_2_1);
      break;
    case 2:
      AlleGruen();
      delay(wait_2_1);
      break;
    case 3:
      AlleBlau();
      delay(wait_2_1);
      break;
    case 4:
      AlleGelb();
      delay(wait_2_1);
      break;
    case 5:
      AlleTuerkis();
      delay(wait_2_1);
      break;
    case 6:
      AlleLila();
      delay(wait_2_1);
      break;
    case 7:
      AlleWeiss();
      delay(wait_2_1);
      break;
  }
}

// -------------------------------------------------------------------------------------

void Lauflicht_3(int wait_3_1, int wait_3_2){
  int nummer = random(1, 8);
  int wait_3_3 = random(wait_3_1, wait_3_2);
  switch(nummer){
    case 1:
      AlleRot();
      delay(wait_3_3);
      break;
    case 2:
      AlleGruen();
      delay(wait_3_3);
      break;
    case 3:
      AlleBlau();
      delay(wait_3_3);
      break;
    case 4:
      AlleGelb();
      delay(wait_3_3);
      break;
    case 5:
      AlleTuerkis();
      delay(wait_3_3);
      break;
    case 6:
      AlleLila();
      delay(wait_3_3);
      break;
    case 7:
      AlleWeiss();
      delay(wait_3_3);
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
      AlleRot_Reverse();
      delay(wait_5);
      break;
    case 2:
      AlleGruen();
      delay(wait_5);
      break;
    case 3:
      AlleBlau();
      delay(wait_5);
      break;
    case 4:
      AlleGelb();
      delay(wait_5);
      break;
    case 5:
      AlleTuerkis();
      delay(wait_5);
      break;
    case 6:
      AlleLila();
      delay(wait_5);
      break;
    case 7:
      AlleWeiss_Reverse();
      delay(wait_5);
      break;
  }
}
