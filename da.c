#include <mega164a.h>
#include <delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <alcd_twi.h>
#include <i2c.h>

int ohm;

#define F_CPU 8000000UL
#define PCF8574_I2C_ADDRESS 0x27

// Function to initialize ADC
void init_ADC() {
    ADMUX |= (1 << REFS0); // Reference voltage is AVCC with external capacitor at AREF pin
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC and set prescaler to 128
}

// Function to read ADC value from specified channel
int ADC_Read(char channel) {
    int Ain, AinLow;
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Set input channel to read
    ADCSRA |= (1 << ADSC); // Start conversion
    while((ADCSRA & (1 << ADIF)) == 0); // Wait for conversion to finish
    AinLow = (int)ADCL; // Read lower byte
    Ain = (int)ADCH * 256; // Read higher 2 bits and multiply by weight
    Ain += AinLow;
    return Ain; // Return digital value
}

void main() {   
    twi_master_init(100);  
    #asm("sei")    
    lcd_twi_init(PCF8574_I2C_ADDRESS,16);
    init_ADC();
    DDRA &= ~(1 << DDA0);
    DDRB |= (1 << DDB0);
    lcd_clear();
    while(1) {
        int adc_rd = ADC_Read(0);
        
        if (adc_rd >= 0 && adc_rd <= 2) {
            lcd_printfxy(0,0,"Insert Resistor");
            delay_ms(500); // Add a delay to reduce blinking effect
        } else {
            ohm = ((1000 * 1024) / adc_rd) - 1000; // Calculating the unknown resistance

            if (ohm > 1000000) {
                lcd_clear();
                lcd_printfxy(1,1,"OHM-METER");
                lcd_printfxy(0,0,"Res:%3d", ohm / 1000000); // Print the resistance value in Megaohms
                lcd_printfxy(0,4,".%03d", (ohm % 1000000) / 1000);
                lcd_printfxy(0,8,"M");
                lcd_printfxy(0,10,"ohm");
                delay_ms(500);
            } else if (ohm > 1000) {
                lcd_clear();
                lcd_printfxy(1,1,"OHM-METER");
                lcd_printfxy(0,0,"Res:%3d", ohm / 1000); // Print the resistance value in Kiloohms
                lcd_printfxy(0,4,".%03d", ohm % 1000);
                lcd_printfxy(0,8,"k");
                lcd_printfxy(0,10,"ohm");
                delay_ms(500);
            } else {
                lcd_clear();
                lcd_printfxy(1,1,"OHM-METER");
                lcd_printfxy(0,0,"Res:%4d", ohm); // Print the resistance value in Ohms
                lcd_printfxy(0,5,"ohm");
                delay_ms(500);
            }
        }
    }
}
