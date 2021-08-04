//Author: Dylan Ixcayau 18486

#define _XTAL_FREQ 4000000
#include <stdint.h>
#include <pic16f887.h>
#include "ADC.h"
int pot1 = 0;
int pot2 = 0;
void recibir_valoresADC(void)
{
    if (PIR1bits.ADIF == 1)         //Revisamos la bandera del ADC
    {
        if(ADCON0bits.CHS == 0)     //Revisamos si estamos en el canal 0
        {
            pot1 = ADRESH;          //Pot_1 recibe el valor en el ADC
        }
        else                        //Revisamos si el canal es 0
        {
            pot2 = ADRESH;          //Pot2 recibe el valor en el ADC
        }
    }
    PIR1bits.ADIF = 0;              //Bajamos la bandera de interrupcion
}
void inicio_conversionADC(void)
{
    if(ADCON0bits.GO == 0)           //Si GO es 0
    {
        if(ADCON0bits.CHS == 0)      //Si el canal es el 0
        {
            ADCON0bits.CHS = 1;      //Se cambia el canal a 1
        }
        else
        {
            ADCON0bits.CHS = 0;     //Si el canal es diferente de 0
                                        //Se pasa a uno
        }
    }
    __delay_us(50);
    ADCON0bits.GO = 1;               //Se deja GO en 1
}
