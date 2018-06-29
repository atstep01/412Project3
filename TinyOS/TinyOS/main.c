// Lab3P1.c
//
// Created: 1/30/2018 4:04:52 AM
// Author : Eugene Rockey
// Copyright 2018, All Rights Reserved
 
//no includes, no ASF, no libraries
#include <math.h>
#include <string.h>
 
const char MS1[] = "\r\nECE-412 ATMega328P Tiny OS";
const char MS2[] = "\r\nby Eugene Rockey Copyright 2018, All Rights Reserved";
const char MS3[] = "\r\nMenu: (L)CD, (A)DC, (E)EPROM, (U)SART Parameters\r\n";
const char MS4[] = "\r\nReady: ";
const char MS5[] = "\r\nInvalid Command Try Again...";
const char MS6[] = " Degrees Fahrenheit\r\n";
const char MS7[] = "\r\nWhat Parameter Should Be Adjusted?:\r\n(B)aud rate, (D)ata bits, (P)arity. (S)top bits\r\n";
 
 

void LCD_Init(void);			//external Assembly functions
void LCD_Delay(void);
void UART_Init(void);
void UART_Clear(void);
void UART_Get(void);
void UART_Get2(void);
void UART_Put(void);
void UART_Get_NoInterrupt(void);
void LCD_Write_Data(void);
void LCD_Write_Command(void);
void LCD_Read_Data(void);
void Mega328P_Init(void);
void ADC_Get(void);
void EEPROM_Read(void);
void EEPROM_Write(void);

unsigned char ASCII;			//shared I/O variable with Assembly
unsigned char DATA;				//shared internal variable with Assembly
char HADC;						//shared ADC variable with Assembly
char LADC;						//shared ADC variable with Assembly
char EEPROMH;					//shared most significant byte of the EEPROM address
char EEPROML;					//shared least significant byte of the EEPROM address
char EEPROMV;					//shared value to be written to EEPROM

unsigned int UBRRValue = 0;				//shared variable for setting baud rate
extern unsigned char UBRR0L asm("UBRR0L");
extern unsigned char UBRR0H asm("UBRR0H");
extern unsigned char UCSR0C asm("UCSR0C");
extern unsigned char UCSR0B asm("UCSR0B");
extern unsigned char ADCSRB asm("ADCSRB");

//int test;
char test;
char temp[7];					//string buffer for ADC output
char lastTemp[7];
int Acc;						//Accumulator for ADC use

void UART_Puts(const char *str)	//Display a string in the PC Terminal Program
{
	while (*str)                //While the current character is not the null character
	{
		ASCII = *str++;         
		UART_Put();
	}
}

void LCD_Puts(const char *str)	//Display a string on the LCD Module
{
	while (*str)
	{
		DATA = *str++;
		LCD_Write_Data();
	}
}


void Banner(void)				//Display Tiny OS Banner on Terminal
{
	UART_Puts(MS1);
	UART_Puts(MS2);
	UART_Puts(MS4);
}

void HELP(void)						//Display available Tiny OS Commands on Terminal
{
	UART_Puts(MS3);
}

void LCD(void)						//Lite LCD demo
{
	DATA = 0x01;
	LCD_Write_Command();
	DATA = 0x34;					//Student Comment Here
	LCD_Write_Command();
	DATA = 0x08;					//Student Comment Here
	LCD_Write_Command();
	DATA = 0x02;					//Student Comment Here
	LCD_Write_Command();
	DATA = 0x06;					//Student Comment Here
	LCD_Write_Command();
	DATA = 0x0f;					//Student Comment Here
	LCD_Write_Command();
	//LCD_Puts("TEAM 9");
	
	test == '\0';
	char Team_Name[16] = "TEAM 9"; 
	while (1)				//not entering loop here for some reason
	{
		LCD_Puts(Team_Name);
		for(int i = 0; i < 16; i++)
		{
			DATA = 0x1C;
			LCD_Write_Command();
			LCD_Delay();
			LCD_Delay();
			LCD_Delay();
			LCD_Delay();
			LCD_Delay();
			LCD_Delay();
			LCD_Delay();
			LCD_Delay();
			
			UART_Get2();
			if(test != '\0')
			{
				HELP();
				return;
			}
				
			
		}
		
	}
	/*
	Re-engineer this subroutine to have the LCD endlessly scroll a marquee sign of 
	your Team's name either vertically or horizontally. Any key press should stop
	the scrolling and return execution to the command line in Terminal. User must
	always be able to return to command line.
	*/
}

void ADC(void)                        //Lite Demo of the Analog to Digital Converter
{
    //test = 1;
    
    ADC_Get();
    
    double celsius = 0;
    double kelvin = 0;
    int fahrenheit = 0;
	const double t0 = 295.37;
	const double B = 3950.0;
    
    Acc = (((int)HADC) * 0x100 + (int)(LADC));
    double r = (10000.0 * ((double)Acc))/(1024.0 - ((double)Acc));
    double r0 = ((double)10000 * (double)512)/((double)1024 - (double)512);
    r = r/r0;
    
    kelvin = (B * t0)/(t0 * log(r) + B);

    celsius = kelvin - 273.15;
    
    fahrenheit = 10 * (celsius*(9.0/5.0) + 1);
    
    char f0 = floor((fahrenheit/1000)) + '0';
    fahrenheit = fahrenheit % 1000;
    char f1 = floor((fahrenheit/100)) + '0';
    fahrenheit = fahrenheit % 100;
    char f2 = floor(fahrenheit/10) + '0';
    fahrenheit = fahrenheit % 10;
    char f3 = floor(fahrenheit) + '0';
    
    if(f0 == '0'){
        f0 = ' ';
    }
    
    temp[0] = f0;
    temp[1] = f1;
    temp[2] = f2;
    temp[3] = 46;
    temp[4] = f3;
    temp[5] = 167;
    temp[6] = 0;
    
    if(strcmp(lastTemp, temp) != 0){
        
        UART_Puts("          \r");
        UART_Puts(temp);
        UART_Puts(" F\r");
        
        strcpy(lastTemp, temp);
    }
    
    LCD_Delay();
    LCD_Delay();
    LCD_Delay();
    LCD_Delay();
    
    UART_Get_NoInterrupt();
    
    if((ASCII == 'x')){ 
        //test = 0;
        strcpy(lastTemp, "     ");
        strcpy(temp, "     ");
        
    }
    
    /*
        Re-engineer this subroutine to display temperature in degrees Fahrenheit on the Terminal.
        The potentiometer simulates a thermistor, its varying resistance simulates the
        varying resistance of a thermistor as it is heated and cooled. See the thermistor
        equations in the lab 3 folder. User must always be able to return to command line.
    */
    return;
}


void writeEEPROM(){
	UART_Puts("\r\nEnter the most significant bit of the address to be written to\r\n");
	ASCII = '\0';
	while(ASCII == '\0'){
		
		UART_Get();
	}
	EEPROMH = ASCII;
	UART_Puts("\r\nEnter the least significant bit of the address to be written to\r\n");
	ASCII = '\0';
	while(ASCII == '\0'){
		
		UART_Get();
	}
	EEPROML = ASCII;
	UART_Puts("\r\nEnter an 8 bit value to store\r\n");
	ASCII = '\0';
	while(ASCII == '\0'){
		
		UART_Get();
	}
	EEPROMV = ASCII;
	EEPROM_Write();
	UART_Puts("\r\nThe data has been stored in EEPROM");
	
}

void readEEPROM(){
	UART_Puts("\r\nEnter the most significant bit of the address to be read\r\n");
	ASCII = '\0';
	while(ASCII == '\0'){
		
		UART_Get();
	}
	EEPROMH = ASCII;
	UART_Puts("\r\nEnter the least significant bit of the address to be read\r\n");
	ASCII = '\0';
	while(ASCII == '\0'){
		
		UART_Get();
	}
	EEPROML = ASCII;
	EEPROM_Read();
	UART_Put();
	UART_Puts("\r\nThe data has been stored in EEPROM");
}

void EEPROM(void)
{
	UART_Puts("\r\nEEPROM (W)rite or (R)Read.\r\n");
	ASCII = '\0';
	while(ASCII == '\0'){
		
		UART_Get();
	}
	switch(ASCII){
		
		case 'W' | 'w': writeEEPROM();
		break;
		case 'R' | 'r': readEEPROM();
		break;
		default:
		UART_Puts(MS5);
		HELP();
		break;
	}
	
	
	/*
	Re-engineer this subroutine so that a byte of data can be written to any address in EEPROM
	during run-time via the command line and the same byte of data can be read back and verified after the power to
	the Xplained Mini board has been cycled. Ask the user to enter a valid EEPROM address and an
	8-bit data value. Utilize the following two given Assembly based drivers to communicate with the EEPROM. You
	may modify the EEPROM drivers as needed. User must be able to always return to command line.
	*/
}



void setBaud(int i){         //Helper function that changes the baud rate
	UBRRValue = i;			 //Set UBBRBalue to preferred baud rate
	UBRR0L = (unsigned char) UBRRValue;		   //The rest of the baud rate goes here.
	UBRR0H = (unsigned char) (UBRRValue >> 8); //Upper four bits of the baud rate go here.
	
	
}

void baudRouter(){
	UART_Puts("\r\nWhat baud rate is preferable?\r\n1: 9600, 2: 14400, 3: 19200, 4: 28800, 5: 38400\r\n");
	ASCII = '\0';
	while(ASCII == '\0'){
		
		UART_Get();
	}
	switch(ASCII){
		
		case '1': setBaud(103);
		UART_Puts("\r\nBaud rate = 9600.\r\n");
		break;
		case '2': setBaud(68);
		UART_Puts("\r\nBaud rate = 14400\r\n");
		break;
		case '3': setBaud(51);
		UART_Puts("\r\nBaud rate = 19200\r\n");
		break;
		case '4': setBaud(34);
		UART_Puts("\r\nBaud rate = 28800\r\n");
		break;
		case '5': setBaud(25);
		UART_Puts("\r\nBaud rate = 38400\r\n");
		break;
		default:
		UART_Puts(MS5);
		HELP();
		break;
	}
}

void setDataBitNum(){
	UART_Puts("\r\nHow many data bits are preferable?\r\n5, 6, 7, 8 , or 9 bits\r\n");
	ASCII = '\0';
	while(ASCII == '\0'){
		
		UART_Get();
	}
	switch(ASCII){
		
		case '5':
		UCSR0C |= (0<<2)&&(0<<1); //if the bits 2-0 of UCSR0C == 000
		UCSR0B |= (0<<2);
		UART_Puts("\r\n# of Data Bits = 5\r\n");
		break;
		case '6':
		UCSR0C |= (0<<2)&&(1<<1); //if bits 2-0 of UCSR0C == 001
		UCSR0B |= (0<<2);
		UART_Puts("\r\n# of Data Bits = 6\r\n");
		break;
		case '7':
		UCSR0C |= (1<<2)&&(0<<1); //if bits 2-0 of UCSr0C == 010
		UCSR0B |= (0<<2);
		UART_Puts("\r\n# of Data Bits = 7\r\n");
		break;
		case '8':
		UCSR0C |= (1<<2)&&(1<<1); //if bits 2-0 of USCR0C == 011
		UCSR0B |= (0<<2);
		UART_Puts("\r\n# of Data Bits = 8\r\n");
		break;
		case '9':
		UCSR0C |= (1<<2)&&(1<<1); //if bits 2-0 of USCR0C == 111
		UCSR0B |= (1<<2);
		UART_Puts("\r\n# of Data Bits = 9\r\n");
		break;
		default:
		UART_Puts(MS5);
		HELP();
		break;
	}
}

void setParity(){
	UART_Puts("\r\nWhat sort of parity is preferable?\r\n(N)o Parity, (O)dd parity, (E)ven Parity\r\n");
	ASCII = '\0';
	while(ASCII == '\0'){
		
		UART_Get();
	}
	switch(ASCII){
		
		case 'N' | 'n':
		UCSR0C |= (0<<5)&&(0<<4);
		UART_Puts("\r\nParity = No Parity\r\n");
		break;
		case 'O' | 'o':
		UCSR0C |= (1<<5)&&(1<<4);
		UART_Puts("\r\nParity = Odd Parity\r\n");
		break;
		case 'E' | 'e':
		UCSR0C |= (1<<5)&&(0<<4);
		UART_Puts("\r\nParity = Even Parity\r\n");
		break;
		default:
		UART_Puts(MS5);
		HELP();
		break;
	}
}

void setStopBitNum(){
	UART_Puts("\r\nHow many stops bits is preferable?\r\n(1) bit or (2) bits\r\n");
	ASCII = '\0';
	while(ASCII == '\0'){
		
		UART_Get();
	}
	switch(ASCII){
		
		case '1':
		UCSR0C |= (0<<3);
		UART_Puts("\r\n# of Stop Bits = 1\r\n");
		break;
		case '2':
		UCSR0C |= (1<<3);
		UART_Puts("\r\n# of Stop Bits = 2\r\n");
		break;
		default:
		UART_Puts(MS5);
		HELP();
		break;
	}
}

void Params(void)					//Router for  USART parameter changes
{
	UART_Puts(MS7);
	ASCII = '\0';
	while(ASCII == '\0'){
		
		UART_Get();
	}
	switch(ASCII){                  //For each parameter change send to another router which handles setting the parameter.
		
		case 'B' | 'b': baudRouter();
		break;
		case 'D' | 'd': setDataBitNum();
		break;
		case 'P' | 'p': setParity();
		break;
		case 'S' | 's': setStopBitNum();
		break;
		default:
		UART_Puts(MS5);
		HELP();
		break;
	}
}

void Command(void)					//command interpreter
{
	UART_Puts(MS3);
	ASCII = '\0';						
	while (ASCII == '\0')
	{
		UART_Get();
	}
	switch (ASCII)
	{
		case 'L' | 'l': LCD();
		break;
		case 'A' | 'a': ADC();
		break;
		case 'E' | 'e': EEPROM();
		break;
		case 'U' | 'u': Params();   //Send USART parameter changes to another router   
		break;
		default:
		UART_Puts(MS5);
		HELP();
		break;  					//Add a 'USART' command and subroutine to allow the user to reconfigure the 						
									//serial port parameters during runtime. Modify baud rate, # of data bits, parity, 							
									//# of stop bits.
	}
}

int main(void)
{
	Mega328P_Init();
	Banner();
	while (1)
	{
		Command();				//infinite command loop
	}
}

