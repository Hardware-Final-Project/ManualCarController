#include <xc.h>
    //setting TX/RX

char mystring[20];
int lenStr = 0;

char enterFlag = 0;


int getLenStr() {
    return lenStr;
}


void setEnterFlag(const char flag) {
    enterFlag = flag;
}


char getEnterFlag() {
    return enterFlag;
}


void UART_Initialize(const char fileno) {
           
    /*       TODObasic   
           Serial Setting      
        1.   Setting Baud rate
        2.   choose sync/async mode 
        3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
        3.5  enable Tx, Rx Interrupt(optional)
        4.   Enable Tx & RX
    */      
    TRISCbits.TRISC6 = 1;            
    TRISCbits.TRISC7 = 1;            
    
    //  Setting baud rate 9600
    TXSTAbits.SYNC = 0;           
    BAUDCONbits.BRG16 = 1;          
    TXSTAbits.BRGH = 1;
    SPBRG = 16;      
    
   //   Serial enable
    RCSTAbits.SPEN = 1;              
    PIR1bits.TXIF = 0;
    PIR1bits.RCIF = 0;
    TXSTAbits.TXEN = 1;           
    RCSTAbits.CREN = 1;             
    PIE1bits.TXIE = 0;       
    IPR1bits.TXIP = 0;             
    PIE1bits.RCIE = 1;    
    IPR1bits.RCIP = 1;           
}

void UART_Write(unsigned char data)  // Output on Terminal
{
    while(!TXSTAbits.TRMT);
    TXREG = data;              //write to TXREG will send data 
}


void UART_Write_Text(char* text) { // Output on Terminal, limit:10 chars
    for(int i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}

void ClearBuffer(){
    for(int i = 0; i < 10 ; i++)
        mystring[i] = '\0';
    lenStr = 0;
    setEnterFlag(0);
}

void MyusartRead()
{
    /* Read data from UART */
    char data = RCREG;

    // Check for end of command markers
    if (data == '\r' || data == '\n' || data == '@') {
        enterFlag = 1;
        mystring[lenStr] = '\0';
    } else {
        // Store received character and echo back
        mystring[lenStr++] = data;
        UART_Write(data);
    }

    return;
}

char *GetString(){
    return mystring;
}

