/*****************************************************************************************
 Logiciel qui rel�ve les positions du joystick avec les entr�es analogiques AN0 et AN1. 
 Il scrute aussi l'appui sur les boutons poussoirs par interruption CCP1 et CCP2
 La position et l'�tat des boutons sont envoy�s en permanence via XBEE.
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
   CCP1_isr() est un sous-programme d'interruption, il est ex�cut� lorsque l'utilisateur appuie
   sur le bouton poussoir BP1. Dans ce cas il met � jour la variable d'�tat � BP1=0xFF pour
   indiquer au programme principal un appui. Il allume la DEL pour informer de la bonne prise
   en compte de l'appui.
***********************************************************************************************/

#INT_CCP1               
void  CCP1_isr(void) 
{
	BP1=0xFF;		// Mise � jour de la variable d'�tat "BP1"
	Allumer_DEL;
}

/**********************************************************************************************
   CCP2_isr() est un sous-programme d'interruption, il est ex�cut� lorsque l'utilisateur appuie
   sur le bouton poussoir BP2. Dans ce cas il met � jour la variable d'�tat � BP2=0xFF pour
   indiquer au programme principal un appui. Il allume la DEL pour informer de la bonne prise
   en compte de l'appui.
***********************************************************************************************/

#INT_CCP2               
void  CCP2_isr(void) 
{
	BP2=0xFF;		// Mise � jour de la variable d'�tat "BP2"
	Allumer_DEL;
	delay_ms(50);
}

// Programme principal //

void main()
{
    init();					// Configuration du microcontr�leur

	while(1)				// R�p�ter en boucle la suite du programme
    {
		Eteindre_DEL;
		mesures_pos();		// Mesures des tensions pr�sentent sur les axes du joytick
		emission_UART();	// Transmission des infos au robot via le module XBEE
		BP1=0;				// R�initialisation de l'�tat du bonton poussoir 1 pour autoriser un nouvel appui
		BP2=0;				// Idem pour le bouton poussoir 2
		delay_ms(100);		// Temporisation de 100ms
			
	}
}

/**********************************************************************************************
   mesures_pos() est un sous-programme permettant de r�cup�rer l'image des tensions pr�sentent 
   sur chaque axe du joystick, les variables "mesure_H" et "mesure_V" sont respectivement le 
   r�sultat de la convertion analogique/num�rique des axes horizontal et vertical.   
***********************************************************************************************/
   
void mesures_pos()
{
	set_adc_channel(1);		// S�lection de l'entr�e AN1 (axe vertical)
	delay_us(1);			// Attente �tablissement de la tension
	mesure_V=read_adc();	// convertion de la tension en un nombre (0V -> Nb=0 , 5V -> Nb=255)
	
	set_adc_channel(0);		// Idem mais avec s�lection de l'entr�e AN0 (axe horizontal)
	delay_us(1);
	mesure_H=read_adc();
}

/**********************************************************************************************
   emission_UART() est un sous-programme permettant de communiquer au transmetteur HF (le module 
   XBEE) les octets image de la position du joystick ainsi que l'�tat des boutons poussoir. Cette
   communication se fait via le bus UART r�gl� � 9600 Bauds, 8 bits de donn�e, 1 bits de start 
   et 1 bit de stop sans contr�le de flux.
***********************************************************************************************/

void emission_UART()
{
	putc(0xAA);				// Emmission octet de d�but de trame
	putc(mesure_V);			// Emmission de l'octet image de la position verticale du joystick
	putc(mesure_H);			// Emmission de l'octet image de la position horizontale du joystick
	putc(BP1);				// Emmission de l'octet image de l'�tat du bouton poussoir 1
	putc(BP2);				// Emmission de l'octet image de l'�tat du bouton poussoir 2

	
}

/**********************************************************************************************
   init() est un sous-programme contenant l'ensemble des initialisations du microcontr�leur.
   il permet de configurer le convertisseur analogique/num�rique, les timers et les interruptions.
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