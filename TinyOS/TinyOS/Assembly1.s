 // Lab3P1.s
 //
 // Created: 1/30/2018 4:15:16 AM
 // Author : Eugene Rockey
 // Copyright 2018, All Rights Reserved


		   .section   ".data"			//Defines the data section of the program
		   .equ    DDRB,0x04			//Data direction Register B, addressed as I/O register
		   .equ	   DDRD,0x0A			//Data direction Register D, addressed as I/O register
		   .equ	   PORTB,0x05			//Port B data register, addressed as I/O register
		   .equ	   PORTD,0x0B			//Port D data register, addressed as I/O register
		   .equ	   U2X0,1				//Double the USART Transmission Speed Toggle. addressed as 1. Doubles asynch baud rate.
		   .global UBRR0L
		   .equ	   UBRR0L,0xC4			//Baud Rate Low Register addressed to 0xC4. This is the lower 8 bits of the USART baud rate.
		   .global UBRR0H
		   .equ	   UBRR0H,0xC5			//Baud Rate High Register addressed to 0xC5. This is the highest 4 bits of the USART baud rate.
										//Together the two UBRR0L/H make up the value of the baud rate.
		   .equ	   UCSR0A,0xC0			//USART Control and Status Register 0 A. Addressed to 0xC0
		   .global UCSR0C
		   .equ	   UCSR0B,0xC1			//USART Control and Status Register 0 B. Addressed to 0xC1
		   .global UCSR0C
		   .equ	   UCSR0C,0xC2			//USART Control and Status Register 0 C. Addressed to 0xC2
										//Above provides basic utilities such as baud rate, parity bit, character size, Rx/Tx enables, etc
		   .equ	   UDR0,0xC6			//USART Transmit/Receive Data Buffer. Addressed to 0xc6
		   .equ	   RXC0,0x07			//USART Receive Complete. High when there is unread data in the recieve buffer. Addressed to 0x07
		   .equ	   UDRE0,0x05			//USART Data Register Empty. High when there the buffer is empty. Addressed to 0x05
		   .equ	   ADCSRA,0x7A			//ADC(analog to digital converter) Control and Status Register A. Addressed to 0x7A
		   .equ	   ADMUX,0x7C			//ADC multiplexer selection register. Addressed to 0x7C
		   .equ	   ADCSRB,0x7B			//ADC Control and Status Register B. Addressed to 0x7B
		   .equ	   DIDR0,0x7E			//Digital Input Disable Register 0. Used to turn off digital input buffers on corresponding ADC pin. Addressed to 0x7E
		   .equ	   DIDR1,0x7F			//Digital Input Disable Register 1. Used to disable the AIN1/0 pin. Addressed to 0x7F
		   .equ	   ADSC,6				//ADC Start Conversation. Starts an analog digital conversion. High when conversion in progress, low when done. Addressed to 6.
		   .equ	   ADIF,4				//ADC Interrupt Flag. Set high when an ADC conversion completes and the Data Registers are updated. Addressed to 4
		   .equ	   ADCL,0x78			//ADC Data Register Low. The 8 least significant bits of the ADC Data Register. Addressed to 0x78
		   .equ	   ADCH,0x79			//ADC Data Register High. The 2 most significatn bits of the ADC Data Register. Addressed to 0x79
		   .equ	   EECR,0x1F			//EEPROM Control Register. Addressed to 0x1F
		   .equ	   EEDR,0x20			//EEPROM Data Register. Addressed to 0x20
		   .equ	   EEARL,0x21			//EEPROM Address Register Low, The 8 LSbits of the EEPROM Data Register. Addressed to 0x21
		   .equ	   EEARH,0x22			//EEPROM Address Register High, The 2 MSbits of the EEPROM Data Register. Addressed to 0x22
		   .equ	   EERE,0				//EEPROM Read Enable. Flag that allows EEPROM to be read when high. Addressed to 0
		   .equ	   EEPE,1				//EEPROM Write Enable. Flag that allowa EEPROM to be written to when high. Addressed to 1
		   .equ	   EEMPE,2				//EEPROM Master Write enable. Flag that allows EEMPE to function when high. Addressed to 2
		   .equ	   EERIE,3				//EEPROM Ready to Interrupt. Flag that allows EEPROM Ready Interrupt to function when high. Addressed in 3.

		   .global    HADC				//High register for ADC
		   .global    LADC				//Low Register for ADC
		   .global    ASCII				//Character transfered and received by USART
		   .global    DATA				//Defie global variable ***

		   .set	      temp,0			//Adress temporary value to 0

		   .section   ".text"			//Defines a text section of the program.
		   .global    Mega328P_Init
Mega328P_Init:
		   ldi    r16,0x07				//PB0(R*W),PB1(RS),PB2(E) as fixed outputs
		   out    DDRB,r16				//Set pins PB0-PB2 as outputs
		   ldi    r16,0					//Clear r16
		   out    PORTB,r16				//Clear the Port B data register
		   out    U2X0,r16				//initialize UART, 8bits, no parity, 1 stop, 9600
		   ldi    r17,0x0				//clear r17
		   ldi    r16,0x67				//set r16 to 0x67, 01100111
		   sts	  UBRR0H,r17			//Clear buad rate high register
		   sts	  UBRR0L,r16			//Set baud rate low register to 0x67
										//Together baud rate is set to 0x67
		   ldi	  r16,24				//load 0x18,00011000, into r16
		   sts	  UCSR0B,r16			//Set bit 3 and 4 of the USART Control and Status Register 0 B. 
										//Allows for Receiving and Transmitting
		   ldi	  r16,6					//set r16 to 0x06, 00000110
		   sts    UCSR0C,r16			//Set bit 1 and 2 of the USART Control and Status Register 0 C.
										//Exstablishes 8-bit character size for receiving/transmission
		   ldi    r16,0x87				//initialize ADC, 10000111
		   sts	  ADCSRA,r16			//Sets bit 0,1,2, and 7 of the ADC Control and Status Register A.
										//bits 2-0 set the division factor for the system clock and the input clock to 128
										//bit 7 enables ADC
		   ldi    r16,0x40				//set r16 to 0x40, 01000000
		   sts    ADMUX,r16				//Sets bit 6 of the ADC multiplier selection register.
										//Sets the voltage reference of the ADC to Vcc with external capacitor with Aref pin
		   ldi    r16,0					//clear r16 
		   sts    ADCSRB,r16			//clear ADC Control and Status Register B
		   ldi    r16,0xFE				//set r16 to 0xFE, 11111110
		   sts    DIDR0,r16				//set bits 1-7 of the Digital Input Disable Register 0.
										//Effectively turns off digital input buffers from pins ADC7-ADC1. Prevents digital input
		   ldi    r16,0xFF				//set r16 to 0xFF, 11111111
		   sts    DIDR1,r16				//set bits 0-1 of the Digital Input Disable Register 1.
										//Effectively turns off digital input on pins AIN1 and AIN0. 
										//This is a two bit register so there are only two bits that can be set
		   ret							//Return to main
	
.global LCD_Write_Command
LCD_Write_Command:
		   call	  UART_Off				//stop receiving/transmitting
		   ldi    r16,0xFF				//PD0 - PD7 as outputs
		   out    DDRD,r16				//Set PD0-PD7 as outputs
		   lds    r16,DATA				//load Data to be sent from global variable
		   out    PORTD,r16				//load data to Port D data register
		   ldi    r16,4					//load 0x04 into r16 ***
		   out    PORTB,r16				//load 0x04 to Port B data register ***
		   call	  LCD_Delay				//Delay the LCD with a double loop ***
		   ldi    r16,0					//clear r16 ***
		   out    PORTB,r16				//clear port B data register ***
		   call	  LCD_Delay				//Delay the LCD with a double loop. ***
		   call	  UART_On				//resume receiving/transmitting
		   ret							//return from where called

LCD_Delay:
		   ldi    r16,0xFA				//set outer loop iterator
D0:	       ldi    r17,0xFF				//set inner loop iterator
D1:	       dec    r17					//decrement inner iterator
		   brne	  D1					//if D1 != 0 stay in loop
		   dec    r16					//decrement out loop
		   brne	  D0					//id D0 != 0 stay in loop
										//loop runs for 0xFA * 0xFF times.
										//just consumes clock cycles
		   ret							//return from where called

.global LCD_Write_Data
LCD_Write_Data:
		   call	  UART_Off				//Stop receiving/transmitting
		   ldi    r16,0xFF				//PD0 - PD7 as outputs
		   out    DDRD,r16				//Set PD0-PD7 as outputs
		   lds    r16,DATA				//load Data to be sent from global variable
		   out    PORTD,r16				//load data to Port D data register
		   ldi    r16,6					//load 0x06 into r16 ***
		   out    PORTB,r16				//load 0x06 into the Port B data register ***
		   call   LCD_Delay				//Delay the LCD with double loop ***
		   ldi    r16,0					//clear r16 ***
		   out    PORTB,r16				//clear port B data register ***
		   call	  LCD_Delay				//Delay the LCD with double loop ***
		   call	  UART_On				//Resume receiving/transmitting
		   ret							//return from where called

.global LCD_Read_Data
LCD_Read_Data:
		   call	  UART_Off				//Stop receiving/transmitting
		   ldi    r16,0x00				//clear r16
		   out    DDRD,r16				//Set PD0-PD7 to input pins
		   out    PORTB,4				//Set port B data register to 0x04 ***
		   in     r16,PORTD				//store data in port D data register into r16
		   sts    DATA,r16				//Store the data in r16 in global variable
		   out    PORTB,0				//clear port B data register ***
		   call	  UART_On				//Resume receiving/transmitting
		   ret							//return from where called.

.global UART_On
UART_On:
		   ldi    r16,2					//set r16 to 2, 00000010
		   out    DDRD,r16				//Set PD2 as an output
		   ldi    r16,24				//load 0x18,00011000, into r16
		   sts	  UCSR0B,r16			//Set bit 3 and 4 of the USART Control and Status Register 0 B. 
										//Allows for Receiving and Transmitting
		   ret							//return from where called.

.global UART_Off
UART_Off:
		   ldi    r16,0					//Set r16 to 0
		   sts    UCSR0B,r16			//clear UCSR0B, effectively shuts down any receiving/transmitting
		   ret							//return from where called.

.global UART_Clear
UART_Clear:
		   lds    r16,UCSR0A			//set r16 to the UCSR0A
		   sbrs	  r16,RXC0				//If bit 7 is high skip.
										//Skips if there is data in the USART Receive Buffer
		   ret							//If RXC0 is 0 than their is no data in the recieve buffer
										//No clear is needed. Return to where called
		   lds    r16,UDR0				//Set r16 to the USART data register. ***
		   rjmp   UART_Clear			//Continue into loop until the recieve buffer is empty. 

.global UART_Get
UART_Get:
		   lds    r16,UCSR0A			//Set r16 to UCSR0A
		   sbrs   r16,RXC0				//If bit 7 is high skip.
										//Skips if there is data in the USART Receive Buffer
		   rjmp	  UART_Get				//Remain in loop until there is data in the USART Receive Buffer
		   lds    r16,UDR0				//Set r16 to the USART data register. ***
		   sts    ASCII,r16				//load character in the USART data register into ASCII
		   ret							//Return from where called

.global UART_Put
UART_Put:
		   lds    r17,UCSR0A			//Set r16 to UCSR0A
		   sbrs	  r17,UDRE0				//If bit 5 is high skip.
										//Skips if the transmit buffer is ready to receive new data
		   rjmp	  UART_Put				//Remain in loop until transmit buffer is ready
		   lds    r16,ASCII				//Load character in ASCII to r16
		   sts    UDR0,r16				//Store r16 into the USART data register
		   ret							//return from where called.

.global ADC_Get
ADC_Get:
		   ldi    r16,0xC7				//set r16 to 0xC7, 11000111
		   sts    ADCSRA,r16			//Sets bits 7,6,2,1,0 of the ADCSRA
										//Bits 7 and 6 enable and start the ADC conversion 
										//Bits 2-0 Set the division factor to 128
A2V1:	   lds    r16,ADCSRA			//Load ADCSRA to r16
		   sbrc	  r16,ADSC				//if bit 6 is low skip
										//Bit 6 or the ADSC is high so long as a conversion is being performed
		   rjmp   A2V1					//Stays in loop so long as conversion is being performed
										//Below stores the result in global variables
		   lds    r16,ADCL				//stores the ADC Register Low into r16
		   sts    LADC,r16				//stores ADCL in LADC global variable
		   lds    r16,ADCH				//stores  the ADC Register High into r16
		   sts    HADC,r16				//stores ADCH into HADC global variable
		   ret							//return from where called

.global EEPROM_Write
EEPROM_Write:      
		   sbic   EECR,EEPE
		   rjmp   EEPROM_Write			; Wait for completion of previous write
		   ldi    r18,0x00				; Set up address (r18:r17) in address register
		   ldi    r17,0x05 
		   ldi    r16,'F'				; Set up data in r16    
		   out    EEARH, r18      
		   out    EEARL, r17			      
		   out    EEDR,r16				; Write data (r16) to Data Register  
		   sbi    EECR,EEMPE			; Write logical one to EEMPE
		   sbi    EECR,EEPE				; Start eeprom write by setting EEPE
		   ret 

.global EEPROM_Read
EEPROM_Read:					    
		   sbic   EECR,EEPE    
		   rjmp   EEPROM_Read			; Wait for completion of previous write
		   ldi    r18,0x00				; Set up address (r18:r17) in EEPROM address register
		   ldi    r17,0x05
		   ldi    r16,0x00   
		   out    EEARH,r18   
		   out    EEARL,r17		   
		   sbi    EECR,EERE				; Start eeprom read by writing EERE
		   in     r16,EEDR				; Read data from Data Register
		   sts    ASCII,r16  
		   ret

		   .end

