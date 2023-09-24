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

void Channel(){     // Channel einstellen
  display.setSegments(C, 1, 0);
  TasterAuslesen();
  if(StatusTasteUP == HIGH){   // Wenn UP gedrückt, dann
    chanel_nummer++;
    pruefenChannel();
  }
  if(StatusTasteDOWN == HIGH){   // Wenn DOWN gedrückt, dann
    chanel_nummer--;
    pruefenChannel();
  }
  if(StatusTasteENTER == HIGH){   // Wenn Enter gedrückt, dann
    EEPROM.write(Adresse_12, Speicher_Channel);   // Sperichere Wert im EEPROM
    delay(100);
  }
  
  switch(chanel_nummer){
    case 1:   // CH07
      display.showNumberDec(7, false, 4, 0);
      Speicher_Channel = 7;
      break;
    case 2:   // CH10
      display.showNumberDec(10, false, 4, 0);
      Speicher_Channel = 10;
      break;
    case 3:   // CH16
      display.showNumberDec(16, false, 4, 0);
      Speicher_Channel = 16;
      break;
    case 4:   // CH22
      display.showNumberDec(22, false, 4, 0);
      Speicher_Channel = 22;
      break;
    case 5:   // CH28
      display.showNumberDec(28, false, 4, 0);
      Speicher_Channel = 28;
      break;
  }
}
