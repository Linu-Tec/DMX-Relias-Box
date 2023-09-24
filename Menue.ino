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
      //Serial.println("DMX-Modus-Nix");
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
      //Serial.println("DMX-Adresse");
      DMX_Adresse_einstellen = 1;
      dmxwertzeigen();
      while(DMX_Adresse_einstellen == 1){
        TasterAuslesen();
        
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
      //Serial.println("Channel");
      Channel_einstellen = 1;
      display.setSegments(C, 1, 0);
      while(Channel_einstellen == 1){
        Channel();
        
        TasterAuslesen();
        delay(100);
        
        if(StatusTasteMODE == HIGH){
          delay(100);
          Channel_einstellen = 0;
          Hauptmenuezaehler++;
          if(Hauptmenuezaehler >= 5){
            Hauptmenuezaehler = 1;
          }
          return;
        }
      }
      
      break;
    case 4:
      //Serial.println("Autoprogramme");
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
    // -
  }
}
