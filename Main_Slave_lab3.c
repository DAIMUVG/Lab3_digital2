// Dispositivo:    PIC 16F887
// Hadward:        potenciometro, leds, arduino 
// Autor:          Dylan Ixcayau 18486
// Fecha Creada:   29 de julio del 2021
// Ultima Actualizacion:    

// ----------------- Laboratorio No.3 ----------------------------------------

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//----------------------Importancion de librerias-------------------------------
#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <pic16f887.h>
#include "ADC.h"      //Importar libreria creada para ADC
#include "SPI.h"
//-----------------------------declaracion_de_variables-------------------------       
int pot1 = 0;
int pot2 = 0;
int cont = 0;
//-----------------------------funciones----------------------------------------
void setup(void);       

void __interrupt()isr(void)
{
    recibir_valoresADC();   //Funcion de libreria_ADC
    if(SSPIF == 1){         //Bandera para el intercambio de datos
        if (cont == 0) {    //Si cont es 0
         spiWrite(pot1);    //Escribe para pot1
         cont = 1;      
        } else {
            spiWrite(pot2); //Escribe para pot2
            cont = 0;
        }
        SSPIF = 0;          //Baja la bandera de intercambio de datos
    }
}


void main(void) {
     setup();               //llamamos configuraciones
    __delay_us(50);         
    ADCON0bits.GO = 1;     //Bandera de GO levantada
    while(1) {
    inicio_conversionADC();                 //funcion de ibreria ADC 
    }
     return;
}

void setup(void){
    ANSEL = 0x03; //00100011
    ANSELH = 0x00;
    
    TRISA = 0b00100011;
    TRISB = 0x00;
    TRISD = 0x00;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTD = 0x00;
//----------------------------configuracion_oscilador---------------------------
    OSCCONbits. IRCF2 = 1; 
    OSCCONbits. IRCF1 = 1;
    OSCCONbits. IRCF0 = 0;  //4Mhz
    OSCCONbits. SCS = 1;

//----------------------------Configuracion_ADC---------------------------------
    
    ADCON1bits.ADFM = 0;        //justificado a la izquierda 
    ADCON1bits.VCFG0 = 0;       //Voltage de referencia VDD
    ADCON1bits.VCFG1 = 0;       //Voltage de referencia VSS
    ADCON0bits.ADCS = 1;        //Fosc/8
    ADCON0bits.CHS = 0;         //Empezamos con el canal 0
    __delay_us(100);
    ADCON0bits.ADON = 1;        //ADC encendido
    
//-----------------------------------Configuracion_de_interrupciones------------
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    PIE1bits.ADIE = 1;          //ADC
    PIR1bits.ADIF = 0;
//---------------------------------------Configuraciones del SPI----------------
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}
