#include <xc.h>

void CCP1_Initialize() {
    TRISCbits.TRISC2=0;	// RC2 pin is output.
    LATC2 = 0;
    CCP1CONbits.CCP1M = 0b1100;
}


void CCP2_Initialize() {
    TRISCbits.TRISC1 = 0;
    LATC1 = 0;
    CCP2CONbits.CCP2M = 0b1100;
}

void CCP_Initialize() {
    CCP1_Initialize();
    CCP2_Initialize();
}