/***************************************************************************************************************************
  Ce programme gére le robot "pince", la direction est assurée par le joystick et le monte-charge par les boutons poussoir
 **************************************************************************************************************************/


#include "MeOrion.h"
#include <SoftwareSerial.h>


void reception_donnee_XBEE(void);
void Gestion_moteurs(void);
void Gestion_boutons_poussoir(void);

unsigned char vitesseG, vitesseD;
unsigned char Position_joystick_H, Position_joystick_V;
unsigned char BP1, BP2;
unsigned char debut_trame;
int Donnee_XBEE = 0;

SoftwareSerial softuart(13, 12);  // Initialise la liaison UART pour la communication avec le module XBEE

// Choix des ports sur le lequel sont connecté les moteurs
MeDCMotor motor1(PORT_1); // Position bras
MeDCMotor motor2(PORT_2); // Pince
MeDCMotor motor3(M1);     // moteur droit
MeDCMotor motor4(M2);     // moteur gauche

/**********************************************************************************************
   setup() est un sous-programme permettant d'initialiser la carte ORION (compatible Arduino).
   Il n'est exécuter qu'une fois lors de la mise sous tension du robot.
***********************************************************************************************/
int port_v = A0; // joystick mouvement vertical
int port_h = A1; // joystick mouvement horizontal    
int valeur_v = 0;
int valeur_h = 0;

void setup()
{
  softuart.begin(9600);
}

// Programme principal //

void loop()
{

  if (softuart.available())       // Une donnée est-elle disponible sur le module XBEE?
  {
    reception_donnee_XBEE();	  // Réception de toutes les données (position et boutons poussoir)
    Gestion_boutons_poussoir();   // Gestion du lève charges
    Gestion_moteurs();            // Calcul des vitesses et commande des moteurs
  }
}

void reception_donnee_XBEE()
{
  while ((Donnee_XBEE = softuart.read()) != 0xAA) {}	// Attente de la réception de l'octet de début de trame 0xAA
  debut_trame = Donnee_XBEE;
  delay(1);
  // Réception des données images de la position du joystick
	Position_joystick_V = softuart.read();
  delay(1);
  Position_joystick_H = softuart.read();
  delay(1);
  // Réception des données images des états des boutons poussoir
	BP1 = softuart.read();
  delay(1);
  BP2 = softuart.read();
  delay(1);

}

/* La position du joystick se situe 9 zones:
  1- Avance et tourne à gauche
  2- Avance et tourne à droite
  3- Avance tout droit
  4- Arrêt
  5- Recule et tourne à gauche
  6- Recule et tourne à droite
  7- Recule tout droit
  8- Tourne sur place à gauche
  9- Tourne sur place à droite
*/

void Gestion_moteurs()
{
  if ((Position_joystick_V >= 147) && (Position_joystick_H <= 107))     // Le joystick est en zone 1
  {
    vitesseD = (Position_joystick_V - 147) * 2;
    vitesseG = vitesseD - (107 - Position_joystick_H) * 2;
    motor3.run(vitesseD);
    motor4.run(vitesseG);
  }
  if ((Position_joystick_V >= 147) && (Position_joystick_H >= 147))     // Le joystick est en zone 2
  {
    vitesseG = (Position_joystick_V - 147) * 2;
    vitesseD = vitesseG - (Position_joystick_H - 147) * 2;
    motor3.run(vitesseD);
    motor4.run(vitesseG);
  }
  if ((Position_joystick_V >= 107) && (Position_joystick_H > 107) && (Position_joystick_H < 147))   // Le joystick est en zone 3
  {
    vitesseG = (Position_joystick_V - 147) * 2;
    motor3.run(vitesseG);
    motor4.run(vitesseG);
    
  }
  if ((Position_joystick_V > 107) && (Position_joystick_V < 147) && (Position_joystick_H > 107) && (Position_joystick_H < 147)) // Le joystick est en zone 4
  {
    motor3.stop();
    motor4.stop();
  }
  if ((Position_joystick_V <= 107) && (Position_joystick_H <= 107))     // Le joystick est en zone 5
  {
    vitesseD = (107 - Position_joystick_V) * 2;
    vitesseG = vitesseD - (107 - Position_joystick_H) * 2;
    motor3.run(-vitesseD);
    motor4.run(-vitesseG);
  }
  if ((Position_joystick_V <= 107) && (Position_joystick_H >= 147))     // Le joystick est en zone 6
  {
    vitesseG = (107 - Position_joystick_V) * 2;
    vitesseD = vitesseG - (Position_joystick_H - 147) * 2;
    motor3.run(-vitesseD);
    motor4.run(-vitesseG);
  }
  if ((Position_joystick_V <= 107) && (Position_joystick_H > 107) && (Position_joystick_H < 147))   // Le joystick est en zone 7
  {
    vitesseG = (107 - Position_joystick_V) * 2;
    motor3.run(-vitesseG);
    motor4.run(-vitesseG);
  }
  if ((Position_joystick_V > 107) && (Position_joystick_V < 147) && (Position_joystick_H <= 107))   // Le joystick est en zone 8
  {
    vitesseG = (107 - Position_joystick_H) * 2;
    motor3.run(vitesseG);
    motor4.run(-vitesseG);
  }
  if ((Position_joystick_V > 107) && (Position_joystick_V < 147) && (Position_joystick_H >= 147))   // Le joystick est en zone 9
  {
    vitesseG = (Position_joystick_H - 147) * 2;
    motor3.run(-vitesseG);
    motor4.run(vitesseG);
  }
}


void Gestion_boutons_poussoir()
{
  if (BP1 >= 100)
  {
   motor2.run(100);
   delay(400);
   motor1.run(100);
   delay(200);
  }
  if (BP2 >= 100)
  {
    motor2.run(-100);
    delay(300);
    motor1.run(-100);
    delay(300);
  }
  if ((BP1 == 0) && (BP2 == 0))
  {
    motor2.stop();
    motor1.stop();
  }
}
