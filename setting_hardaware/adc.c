#include <xc.h>


int adc_value = 0;

void ADC_Initialize(void) {
    TRISA = 0xff;		// Set as input port 
    // ADCON2 = 0x92;  	
    ADFM = 1 ;          // Right Justifie
    
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.PCFG = 0b1110; // AN0 as analog input, the others are digital
    
    ADCON0bits.CHS = 0b0000;  // AN0 as analog input
    ADCON0bits.ADON = 1;
    
    ADCON2bits.ADCS = 0b100; // max 5.71 MHz => Fosc / 4 = 1000 kHz
    ADCON2bits.ACQT = 0b010; // 4 Tad = 4 us
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    IPR1bits.ADIP = 0;
    
    ADRESH=0;  			// Flush ADC output Register
    ADRESL=0;  
}

int ADC_Read(int channel)
{
    int digital;
    
    ADCON0bits.GO = 1;
    ADCON0bits.ADON = 1;
    
    while(ADCON0bits.GO_nDONE==1);

    digital = (ADRESH*256) | (ADRESL);
    return(digital);
}


int ADC_GetValue() {
    return adc_value;
}
