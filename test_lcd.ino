/*
 * test_lcd.ino
 * Utiliser la bibliothèque LiquidCrystal pour faire afficher
 * des messages/données sur l'afficheur LCD.
 * 
 * L'afficheur LCD pourrait être intégré dans le projet IoT
 * de ce cours.
 * 
 * GPA788 - ETS
 * T. Wong
 * 08-2019
 * 08-2020
 * 
 * Ce programme est inspiré de l'exemple "Hello World!" de la page:
 * https://www.arduino.cc/en/Tutorial/HelloWorld. La classe LiquidCrystal()
 * et ses fonctions membres sont listées sur la page: 
 * https://www.arduino.cc/en/Reference/LiquidCrystal
 * Le branchement électrique est présenté d'une façon plus évidente dans le
 * protocole de laboratoire du cours. Finalement, la technique de lecture
 * de la température interne de l'ATmega328P est donnée dans les notes
 * de cours.
 */

// Classe pour afficher des caractères à l'aide du LCD
#include <LiquidCrystal.h>

// Classe pour mesurer la température avec le capteur dht11
#include "dhtlib_gpa788.h"


/* ---------------------------------------------------------------
   Créer un objet (global) de type LiquidCrystal.
   Le constructeur utilisé est:
     LiquidCrystal(rs, enable, d4, d5, d6, d7)
     rs (register select): broche 12
     enable: broche 11
     d4: broche 5, d5: broche 4, d6: broche 3, d7: broche 2
   ---------------------------------------------------------------    
   Note: Les numéros de broches sont ceux du côté d'Arduino.   
         Brancher ces broches sur l'afficheur LCD selon le
         diagramme électrique donné sur le protocole de laboratoire.
   --------------------------------------------------------------- */
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*---------------------------------------------*/
/* Constantes et variables globales */
/*---------------------------------------------*/
// Relier le capteur à la broche #7
const uint8_t DHT11_PIN{7};

// Taux de transmission du port série
const int BAUD_RATE{19200};

// Créer un objet de type dht
dhtlib_gpa788 DHT(DHT11_PIN); 

/* ---------------------------------------------------------------
   Fonction d'initialisation obligatoire (exécutée 1 seule fois).
   --------------------------------------------------------------- */
void setup() {

  // Pour afficher la température sur le terminal série
  Serial.begin(BAUD_RATE);

  // Sur le VS Code, l'ouverture du port série prend du temps et on
  // peut perdre des caractères. Ce problème n'existe pas sur la carte
  // de l'Arduino.
  waitUntil(2000);

  // Pour l'afficheur LCD
  // 0) Il s'agir d'un afficheur 16 x 2
  lcd.begin(16, 2);
  // 1) Effacer l'affichage
  lcd.clear();

}

void loop() {

  //Prendre une mesure à l'aide du capteur DHT11
  DHTLIB_ErrorCode chk = DHT.read11();

  if (chk == DHTLIB_ErrorCode::DHTLIB_OK) {

    showTemp(DHT, lcd);

  } else {

    showError(lcd, chk);

  }

  lcd.display();

  delay(2000);
  
  lcd.noDisplay();

  waitUntil(1000);  
}

/* ---------------------------------------------------------------
   Fonction pour créer un delai de w millisecondes
   
   La fonction delay() est utilisée dans bien des tutoriel pour
   créer un delai temporel. On peut aussi créer notre propre délai
   et utiliser une unité de temps à notre guise.
   --------------------------------------------------------------- */
void waitUntil(uint32_t w) {
  uint32_t t{millis()};
  // Attendre w millisecondes
  while (millis() < t + w) {}
}


/* ---------------------------------------------------------------
   Fonction pour afficher la température et l'humidité mesuré.
   --------------------------------------------------------------- */
void showTemp(dhtlib_gpa788 &d, LiquidCrystal &l) {

  // D'abord le terminal série
  Serial.print(F("Temperature = "));
  Serial.println(d.getTemperature(), 0); 
  Serial.print(F("Humidité = "));
  Serial.println(d.getHumidity(), 0); 
 
  l.clear();

  // Ensuite l'afficheur LCD
  l.setCursor(0,0); 
  l.print("Temp. = ");
  l.print(d.getTemperature(), 0);
  l.print((char)223); l.print("C");

  l.setCursor(0,1); 
  l.print("Humidite = ");
  l.print(d.getHumidity(),0);
  l.print("%");
  
}

/* ---------------------------------------------------------------
   Fonction pour afficher un code d'erreur lorsque la lecture n'a pas pu être réalisé.
   --------------------------------------------------------------- */
void showError(LiquidCrystal &l, DHTLIB_ErrorCode chk){

  Serial.println(F("DHT11: Erreur"));

  l.clear();
  l.setCursor(0,0); 
  l.print("DHT11: Erreur");

  switch (chk)
  {
  case DHTLIB_ErrorCode::DHTLIB_ERROR_CHECKSUM:
    // D'abord le terminal série
    
    Serial.println(F("DHT11: code -1"));

    l.setCursor(0,1); 
    l.print("DHT11: code -1");
    break;
  
  case DHTLIB_ErrorCode::DHTLIB_ERROR_TIMEOUT:

    // D'abord le terminal série
    Serial.println(F("DHT11: code -2"));

    l.setCursor(0,1); 
    l.print("DHT11: code -2");
    break;

  } 

}