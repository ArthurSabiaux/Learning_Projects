/////////////////// IMPLEMENTATION DES BIBLIOTHEQUES ////////////////////////

#include <Servo.h>

/////////////////// DECLARATION DES VARIABLES ////////////////////////

///////Variables gestion LED

const int PINSIGNALLUMINEUX1 = 4;
const int PINSIGNALLUMINEUX2 = 5; 
bool etatlampe = 0;
const int TEMPSALLUMAGESL = 800;
unsigned long tempsLampeini = 0;
unsigned long tlampe;

///////Variables gestion son

const byte PLAY_E=3;
const int TEMPSALLUMAGESON = 3000;
unsigned long tempssonini = 0;
unsigned long tson;
bool etatson=false;

///////Variables gestion barrières

Servo monServomoteur1; //création objet monServomoteur1 pour son contrôle
Servo monServomoteur2; //création objet monServomoteur2 pour son contrôle
const int PINSERVO2=8;
const int PINSERVO1=9;
int POSITIONHAUTE=90; //constante pour les barrières en position haute
int indexpos=0; //variable pour la position des barrières
unsigned long tempsbarriereini=0; //initialisation du temps de descente des barrières
unsigned long tbarriere; //variable pour le temps pris par les barrières à se mettre en position haute ou basse
const int TEMPSBARRIERE=50; //constante pour le temps souhaité de descente/montée
const int LISTEPOSFERMER[]= {90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10};
//liste des positions prises par les barrières pour se fermer 
const int LISTEPOSOUVRIR[]= {90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10};
//liste des positions prises par les barrières pour s'ouvrir

///////Variables gestion détection
//Déclaration des détecteur de présence infraraouges
const byte DETECTEUR1 = 10;          //Affecte la constante "DETECTEUR1" de type byte (1 octet de 8 bits : 0 à 255) au nombre 10
const byte DETECTEUR2 = 11;          //Affecte la constante "DETECTEUR2" de type byte au nombre 11
const byte DETECTEUR3 = 12;          //Affecte la constante "DETECTEUR3" de type byte au nombre 12
const byte DETECTEUR4 = 13;          //Affecte la constante "DETECTEUR4" de type byte au nombre 13

//Declaration des variables
boolean detection = false;
boolean premiere_detection = 0;
unsigned int sens_train = 0; //Entier positif qui est initialise a 0, mais est egal a 1 quand le train vient de la gauche, et egale a 2 quand le train vient de la droite

/////////////////// SETUP DES CONDITIONS INITIALES ////////////////////////

void setup() {
  Serial.begin(9600);
  // Ouvrir Barrière

  monServomoteur1.attach(PINSERVO1); //affectation de l'objet monServomoteur1 à la broche 9 
  monServomoteur2.attach(PINSERVO2); //affectation de l'objet monServomoteur2 à la broche 8
  monServomoteur1.write(POSITIONHAUTE);
  monServomoteur2.write(POSITIONHAUTE);
  
  // Eteindre Lampe
  
  pinMode(PINSIGNALLUMINEUX1, OUTPUT);
  pinMode(PINSIGNALLUMINEUX2, OUTPUT);
  digitalWrite(PINSIGNALLUMINEUX1, etatlampe);
  digitalWrite(PINSIGNALLUMINEUX2, etatlampe);
  
  // Couper signal sonore
  
  pinMode(PLAY_E,OUTPUT);// PLAY_E est une sortie
  digitalWrite(PLAY_E, LOW); 

  //Setup de la détection

  pinMode(DETECTEUR1, INPUT);        //constante DETECTEUR1 (donc pin 10) configuré en entrée
  pinMode(DETECTEUR2, INPUT);        //constante DETECTEUR2 (donc pin 11) configuré en entrée
  pinMode(DETECTEUR3, INPUT);        //constante DETECTEUR3 (donc pin 12) configuré en entrée
  pinMode(DETECTEUR4, INPUT);        //constante DETECTEUR4 (donc pin 13) configuré en entrée

}

/////////////////// FONCTIONNEMENT LAMBDA ////////////////////////

void loop() {
  
/////////////////// Recherche d'un train et ouverture barrière////////////////////////
  while (!detection) 
  {  
    // ouverture barrière
    
    tbarriere=millis()-tempsbarriereini;
    if ((tbarriere>TEMPSBARRIERE) && (indexpos>=1))
    {
      indexpos-=1;
      monServomoteur2.write(LISTEPOSOUVRIR[indexpos]); //modifie l'angle du bras du servomoteur2
      monServomoteur1.write(LISTEPOSOUVRIR[indexpos]); //modifie l'angle du bras du servomoteur1
      tempsbarriereini=millis();
    }
    
    //détection train
    
    if ( digitalRead(DETECTEUR1) == HIGH)
    {
      Serial.println("Train rentré par la gauche");
      sens_train = 1; //On dit que le train vient de la gauche
      detection = true;
    }

    if ( digitalRead(DETECTEUR4) == HIGH)
    {
      Serial.println("Train rentré par la droite");
      sens_train = 2; //On dit que le train vient de la droite
      detection = true;
    }

    //extinction lampe
    
    etatlampe=false;
    digitalWrite(PINSIGNALLUMINEUX1, etatlampe);
    digitalWrite(PINSIGNALLUMINEUX2, etatlampe);

    // extinction signal sonore
    
    digitalWrite(PLAY_E, LOW); 
  }
  
/////////////////////////////// Train Détecter => Fermeture barrière et activations des signaux lumieux et sonore ///////////////////////////////

  while (detection) {
    
    //Signal Sonore
    tson = millis()-tempssonini;
    if (tlampe>10 && etatson) {
      etatson =!etatson;
      digitalWrite(PLAY_E, etatson);
    }
    if (tson>TEMPSALLUMAGESON)
      tempssonini=millis();{
      etatson=!etatson;
      digitalWrite(PLAY_E, etatson);
    }
    //Signal Lumineux
    
    tlampe = millis()-tempsLampeini;
    if (tlampe>TEMPSALLUMAGESL) {
      etatlampe = !etatlampe;
      digitalWrite(PINSIGNALLUMINEUX1, etatlampe);
      digitalWrite(PINSIGNALLUMINEUX2, etatlampe);
      tempsLampeini = millis();
    }
    
    //Fermer barrière
    
    tbarriere=millis()-tempsbarriereini;
      if ((tbarriere>TEMPSBARRIERE) && (indexpos<=109)){
        indexpos+=1;
        monServomoteur2.write(LISTEPOSFERMER[indexpos]); //modifie l'angle du bras du servomoteur2
        monServomoteur1.write(LISTEPOSFERMER[indexpos]); //modifie l'angle du bras du servomoteur1
        tempsbarriereini=millis();
    }


    //Tester la fin du train
    
    if (digitalRead(DETECTEUR4) == HIGH && sens_train==1)
    {
      premiere_detection = 1;
    }
    
    if (digitalRead(DETECTEUR1) == HIGH && sens_train==2)
    {
      premiere_detection = 1;
    }
    
    if (premiere_detection == 1 && digitalRead(DETECTEUR4) == LOW && digitalRead(DETECTEUR3) == LOW && sens_train == 1)
    {
      premiere_detection = 0;
      sens_train = 0;
      Serial.println("Train sortie a droite");
      detection = false;
    }
    if (premiere_detection == 1 && digitalRead(DETECTEUR1) == LOW && digitalRead(DETECTEUR2) == LOW && sens_train == 2)
    {
      premiere_detection = 0;
      sens_train = 0;
      Serial.println("Train sortie a gauche");
      detection = false;
    }
  }
  
  if (indexpos>104) { 
    indexpos=indexpos-30;
  }
  else if (indexpos>99) { 
    indexpos=89+109-indexpos;
  }
  else if (indexpos>89) {
    indexpos=indexpos-20;
  }
  else if (indexpos>79) {
    indexpos=2*89-indexpos;
  }
}
