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
#include <string.h>
#include "SPI.h"
//-------------------------------variables--------------------------------------
int cont=0;
uint8_t contador = 0;
int c = 0;
float potenciometro1 = 0;
float potenciometro2 = 0;
char s[20];
int pot1 = 0;
int pot2 = 0;
int val3 = 0;

//-----------------------------funciones----------------------------------------
void setup(void); 
void Envio(char bt);
void cadena(char st[]);
void sensores(void);

void main(void) {
    setup();
//******************************************************************************
    // Loop infinito
//*****************************************************************************
    while(1){
        
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       spiWrite(c);
       if (cont == 0){          //Contador para recibir un dato a la vez
            pot1 = spiRead();   //Lee lo que recibe y lo pasa a pot1
            cont = 1;           
       } else {
            pot2 = spiRead();   //Lee lo que recibe y lo pasa a pot2
            cont = 0;
       }
       
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
       sensores();   
       if(PIR1bits.RCIF){                      //Bandera de UART
            char escritura = RCREG;             //variable para escribir
            if(escritura == 43){                //si la variable es igual a + en ASCCI
                contador++;                     //Aumenta la varibale contador
            }
            if(escritura == 45){                //si la variable es igual a - en ASCCI
                contador--;                     //Decrementa la variable contador     
            }
        }
        
        potenciometro1 = (pot1*0.0196);   //Conversion a voltaje de 0 a 5
        potenciometro2 = (pot2*0.0196);
        
        PORTB = contador;
        
    }
}

void setup (void){
    ANSEL = 0x00;           //Salidas
    ANSELH = 0x00;          //Salidas
    
    TRISB = 0x00;
    TRISD = 0x00;
    TRISC2 = 0; 
    PORTB = 0x00;
    PORTD = 0x00;
    PORTCbits.RC2 = 1;      //Recibe datos de ese pin
    
//----------------------------configuracion_oscilador---------------------------
    OSCCONbits.IRCF2 = 1; 
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;  //4Mhz
    OSCCONbits.SCS = 1;

//------------------------------------configuracion_UART------------------------
    
    TXSTAbits.SYNC = 0;             //Modo asíncrono
    TXSTAbits.BRGH = 1;             //BAUD RATE
    BAUDCTLbits.BRG16 = 0; 
    SPBRG = 25;                     //Registros para BAUD RATE
    SPBRGH = 0; 
    RCSTAbits.SPEN = 1;         //Habilitar puerto asíncrono
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;         //Habilitar recepción de datos 
    TXSTAbits.TXEN = 1;         //Habilitar transmision
    
//-----------------------------------Configuracion_de_interrupciones-----------
    INTCONbits.GIE = 1;        //Globales
    INTCONbits.PEIE = 1;        //Perifericas
//---------------------------------------Configuraciones del SPI----------------    
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}

//---------------------------------Envio_de_caracter----------------------------
void Envio(char caracter)
{
    while(!TXIF);               //Espera que envie dato TXIF = 1 constantemente
    TXREG = caracter;           //Carga el caracter a TXREG y envía 
    return; 
}

//-------------------------------funcion_de_cadena------------------------------
void cadena(char st[])
{
    int i = 0; 
    while (st[i] !=0)       //revisar la posicion de valor de i 
    {
        Envio(st[i]);       //enviar caracter de esa posicion 
        i++;                //incrementar variable para pasar a otra posicion 
    }                       //de cadena 
    return;
}

//--------------------------Funcion_String_para_UART----------------------------
void sensores(void)
{
    sprintf(s, "\r S1=%.2f \n", potenciometro1);   //String_de_terminal
    cadena(s);
    
    sprintf(s, "\n S2=%.2f", potenciometro2);      
    cadena(s);
    
    sprintf(s, "\n S3=%d", contador);
    cadena(s);
}
