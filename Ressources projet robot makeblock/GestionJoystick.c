/*****************************************************************************************
 Logiciel qui relève les positions du joystick avec les entrées analogiques AN0 et AN1. 
 Il scrute aussi l'appui sur les boutons poussoirs par interruption CCP1 et CCP2
 La position et l'état des boutons sont envoyés en permanence via XBEE.
 Protocole de communication:
 transmission position: 0xAA suivi des deux octets de position (vertical en premier) 
 + 0xFF ou 0x00 pour BP1 + 0xFF ou 0x00 pour BP2
 *****************************************************************************************/


#include <robot.h>
#define Eteindre_DEL output_low(PIN_A5)
#define Allumer_DEL output_high(PIN_A5)

void init(void);
void mesures_pos(void);
void emission_UART(void);

int mesure_V,mesure_H;
int BP1, BP2;


/**********************************************************************************************
   CCP1_isr() est un sous-programme d'interruption, il est exécuté lorsque l'utilisateur appuie
   sur le bouton poussoir BP1. Dans ce cas il met à jour la variable d'état à BP1=0xFF pour
   indiquer au programme principal un appui. Il allume la DEL pour informer de la bonne prise
   en compte de l'appui.
***********************************************************************************************/

#INT_CCP1               
void  CCP1_isr(void) 
{
	BP1=0xFF;		// Mise à jour de la variable d'état "BP1"
	Allumer_DEL;
}

/**********************************************************************************************
   CCP2_isr() est un sous-programme d'interruption, il est exécuté lorsque l'utilisateur appuie
   sur le bouton poussoir BP2. Dans ce cas il met à jour la variable d'état à BP2=0xFF pour
   indiquer au programme principal un appui. Il allume la DEL pour informer de la bonne prise
   en compte de l'appui.
***********************************************************************************************/

#INT_CCP2               
void  CCP2_isr(void) 
{
	BP2=0xFF;		// Mise à jour de la variable d'état "BP2"
	Allumer_DEL;
	delay_ms(50);
}

// Programme principal //

void main()
{
    init();					// Configuration du microcontrôleur

	while(1)				// Répéter en boucle la suite du programme
    {
		Eteindre_DEL;
		mesures_pos();		// Mesures des tensions présentent sur les axes du joytick
		emission_UART();	// Transmission des infos au robot via le module XBEE
		BP1=0;				// Réinitialisation de l'état du bonton poussoir 1 pour autoriser un nouvel appui
		BP2=0;				// Idem pour le bouton poussoir 2
		delay_ms(100);		// Temporisation de 100ms
			
	}
}

/**********************************************************************************************
   mesures_pos() est un sous-programme permettant de récupérer l'image des tensions présentent 
   sur chaque axe du joystick, les variables "mesure_H" et "mesure_V" sont respectivement le 
   résultat de la convertion analogique/numérique des axes horizontal et vertical.   
***********************************************************************************************/
   
void mesures_pos()
{
	set_adc_channel(1);		// Sélection de l'entrée AN1 (axe vertical)
	delay_us(1);			// Attente établissement de la tension
	mesure_V=read_adc();	// convertion de la tension en un nombre (0V -> Nb=0 , 5V -> Nb=255)
	
	set_adc_channel(0);		// Idem mais avec sélection de l'entrée AN0 (axe horizontal)
	delay_us(1);
	mesure_H=read_adc();
}

/**********************************************************************************************
   emission_UART() est un sous-programme permettant de communiquer au transmetteur HF (le module 
   XBEE) les octets image de la position du joystick ainsi que l'état des boutons poussoir. Cette
   communication se fait via le bus UART réglé à 9600 Bauds, 8 bits de donnée, 1 bits de start 
   et 1 bit de stop sans contrôle de flux.
***********************************************************************************************/

void emission_UART()
{
	putc(0xAA);				// Emmission octet de début de trame
	putc(mesure_V);			// Emmission de l'octet image de la position verticale du joystick
	putc(mesure_H);			// Emmission de l'octet image de la position horizontale du joystick
	putc(BP1);				// Emmission de l'octet image de l'état du bouton poussoir 1
	putc(BP2);				// Emmission de l'octet image de l'état du bouton poussoir 2

	
}

/**********************************************************************************************
   init() est un sous-programme contenant l'ensemble des initialisations du microcontrôleur.
   il permet de configurer le convertisseur analogique/numérique, les timers et les interruptions.
***********************************************************************************************/

void init()
{
   setup_adc(ALL_ANALOG);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(0);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_timer_2(T2_DISABLED,0,1);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);		//209 ms overflow
   setup_ccp1(CCP_CAPTURE_FE);                  // RE pour front montant
   setup_ccp2(CCP_CAPTURE_FE);
   enable_interrupts(INT_CCP1);
   enable_interrupts(INT_CCP2);
   enable_interrupts(GLOBAL);
}