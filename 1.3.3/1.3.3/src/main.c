/* Name: main.c
 * Author: WENXI WEI
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */


#include "MEAM_general.h"
#include "m_usb.h"
#define ADC0D 0
#define ADC1D 1
#define ADC4D 4
#define ADC5D 5
#define ADC6D 6
#define ADC7D 7
#define ADC8D 8
#define ADC9D 9
#define ADC10D 10
#define ADC11D 11
#define ADC12D 12
#define ADC13D 13

//set up ADC
void ADC_setup(int adcNumber){
    //set the system clock
    _clockdivide(0);

    //step 1: set the voltage reference to Vcc
    set(ADMUX, REFS0);
    clear(ADMUX, REFS1);

    //step 2: set the ADC clock prescaler to 128. 16M/128=125kHz
    ADCSRA = (ADPS2 << 1) | (ADPS1 << 1) | (ADPS0 << 1);

    //step 3: disable digital 
    //skip step 4: interrupt
    //step 5: and select the analog input pin
    switch (adcNumber) {  //run step3 and step4 according to different adcNumber
        case 0:
            DIDR0 |= (1 << adcNumber);

            ADCSRB &= ~(1 << MUX5);
            ADMUX &= ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0));
            break;
        case 1:
            DIDR0 |= (1 << adcNumber);

            ADCSRB &= ~(1 << MUX5);
            ADMUX &= ~((1 << MUX2) | (1 << MUX1));
            ADMUX |= (1 << MUX0);
            break;
        case 4:
            DIDR0 |= (1 << adcNumber);

            ADCSRB &= ~(1 << MUX5);
            ADMUX |= (1<<MUX2);
            ADMUX &= ~((MUX1 << 1) | (MUX0 << 1));
            break;
        case 5:
            DIDR0 |= (1 << adcNumber);

            ADCSRB &= ~(1 << MUX5);
            ADMUX |= ((1<<MUX2) | (MUX0 << 1));
            ADMUX &= ~(MUX1 << 1);
            break;
        case 6:
            DIDR0 |= (1 << adcNumber);

            ADCSRB &= ~(1 << MUX5);
            ADMUX |= ((1<<MUX2) | (MUX1 << 1));
            ADMUX &= ~(MUX0 << 1);
            break;
        case 7:
            DIDR0 |= (1 << adcNumber);

            ADCSRB &= ~(1 << MUX5);
            ADMUX |= ((1<<MUX2) | (MUX1 << 1) | (MUX0 << 1));
            break;
        case 8:
            DIDR2 |= (1 << adcNumber); //DIDR2 for ADC8D---ADC13D

            ADCSRB |= (1 << MUX5);
            ADMUX &= ~((1<<MUX2) | (MUX1 << 1) | (MUX0 << 1));
            break;
        case 9:
            DIDR2 |= (1 << adcNumber); 

            ADCSRB |= (1 << MUX5);
            ADMUX &= ~((1<<MUX2) | (MUX1 << 1));
            ADMUX |= (MUX0 << 1);
            break;
        case 10:
            DIDR2 |= (1 << adcNumber); 

            ADCSRB |= (1 << MUX5);
            ADMUX &= ~((1<<MUX2) | (MUX0 << 1));
            ADMUX |= (MUX1 << 1);
            break;
        case 11:
            DIDR2 |= (1 << adcNumber); 

            ADCSRB |= (1 << MUX5);
            ADMUX &= ~(1<<MUX2);
            ADMUX |= ( (MUX1 << 1) | (MUX0 << 1));
            break;
        case 12:
            DIDR2 |= (1 << adcNumber); 

            ADCSRB |= (1 << MUX5);
            ADMUX &= ~((1<<MUX1) | (MUX0 << 1));
            ADMUX |= (MUX2 << 1);
            break;
        case 13:
            DIDR2 |= (1 << adcNumber); 

            ADCSRB |= (1 << MUX5);
            ADMUX &= ~(1<<MUX1);
            ADMUX |= ( (MUX2 << 1) | (MUX0 << 1));
            break;
        default:
            break;
    }
}

//read ADC
void ADC_reading() {
    //step 6: enable conversions
    ADCSRA |= (1 << ADEN); //enable ADC

    //step 7: begin conversion
    ADCSRA |= (1 << ADSC); //for the first time conversion

    //step 8: wait for the conversion to finish 
    //step 9: then we can read
    //step 10: clear the conversion flag
    while (1) {
        if (bit_is_set(ADCSRA,ADIF)){ // if ADC conversion is complete
            set(ADCSRA, ADIF);  // clear the flag

            m_usb_tx_string(" ADC =");
            m_usb_tx_uint(ADC);  //print ADC values
            m_usb_tx_string("\n");

            ADCSRA |= (1 << ADSC); // start conversion again
            break;
        }
    }
}


int main() {
    m_usb_init(); // Initialize USB transmission
    
    while(1){
        ADC_setup(ADC0D);
        ADC_reading();
        _delay_ms(100);
        ADC_setup(ADC1D);
        ADC_reading();
        _delay_ms(100);
    }
    


    return 0;
}


