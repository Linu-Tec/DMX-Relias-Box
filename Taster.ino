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

// -------------------------------------------------------------------------------------

void pruefenChannel(){
  if(chanel_nummer >= 6){
    chanel_nummer = 1;
  }
  if(chanel_nummer < 1){
    chanel_nummer = 5;
  }
}

// -------------------------------------------------------------------------------------

void pruefenChannelNumber(){
  if(Speicher_Channel == 7){
    chanel_nummer = 1;
  }
  if(Speicher_Channel == 10){
    chanel_nummer = 2;
  }
  if(Speicher_Channel == 16){
    chanel_nummer = 3;
  }
  if(Speicher_Channel == 22){
    chanel_nummer = 4;
  }
  if(Speicher_Channel == 28){
    chanel_nummer = 5;
  }
}
