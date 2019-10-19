#include"STD_Types.h"
#include"avr_reg.h"
#include"UART.h"


void (*Transmiter_Ptr) (void) = NULL;
void (*UDR_Ptr) (void) = NULL;
void (*Receiver_Ptr) (void) = NULL;


//ISR DEFINITION AND ATTRIBUTE
#define ISR(vector, ...)            \
void vector (void) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__; \
void vector (void)



//VECTOR NAMING
#define _VECTOR(N) __vector_ ## N

#define USART_UDRE_vect			_VECTOR(14)
#define USART_RXC_vect			_VECTOR(13)
#define USART_TXC_vect			_VECTOR(15)




STD_Fun_t UART_Init(void){
	uint8 Local_Loop;
	STD_Fun_t StateVariable = OK;

	// check if UART Modules do not exceed the MAX of UART module
	if (Num_UART > UART_MAX_MODULE){
		StateVariable = NOK;
	}
	else {
		// loop on the modules
		for(Local_Loop=0; Local_Loop<Num_UART; Local_Loop++){
			// switch on modes
			switch (UART_Array_Conf[Local_Loop].TranRecMode)
			{
				case TranRecMode_Transmitter:
				UCSRB |= (1<<TXEN);
				break;
				case TranRecMode_Receiver:
				UCSRB |= (1<<RXEN);
				break;
				case TranRecMode_Tran_Rec:
				UCSRB |= (1<<TXEN);
				UCSRB |= (1<<RXEN);
				break;
				default:
				StateVariable = NOK;
				break;
			}
			
			// switch on frame size
			switch (UART_Array_Conf[Local_Loop].FrameSize)
			{
				case FrameSize_8:
				UCSRC |=(1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
				break;
				default:
				StateVariable = NOK;
				break;
			}
			switch (UART_Array_Conf[Local_Loop].BaudRate)
			{
				case BaudRate_9600:
				UBRRH = 0x00;
				UBRRL = 0x33;
				break;
				default:
				StateVariable = NOK;
				break;
			}
			// switch on interrupt
			switch (UART_Array_Conf[Local_Loop].InterruptMode)
			{
			case InterruptMode_Off:
			break;
			case InterruptMode_On:
			if (UART_Array_Conf[Local_Loop].TranRecMode == TranRecMode_Transmitter){
			UCSRB |= (1<<TXCIE);
			SREG |= (1<<7);
			}
			else if (UART_Array_Conf[Local_Loop].TranRecMode == TranRecMode_Receiver){
			UCSRB |= (1<<RXCIE);
			SREG |= (1<<7);
			}
			else if (UART_Array_Conf[Local_Loop].TranRecMode == TranRecMode_Tran_Rec){
			UCSRB |= (1<<TXCIE) | (1<<RXCIE);
			SREG |= (1<<7);
			}
			break;
			default:
			StateVariable = NOK;
			break;
			}
		}
	}
	return StateVariable;

}


void UART_Transmit(uint8 Data){
	UDR = Data;
}


void UART_Receive(uint8 * Data){
	*Data = UDR;
}


void UART_SetterTransmit(void (*Ptr_1) (void)){
	Transmiter_Ptr = Ptr_1;
}

void UART_SetterUDREmpty(void (*Ptr_1) (void)){
	UDR_Ptr = Ptr_1;
}

void UART_SetterReceive(void (*Ptr_2) (void)){
	Receiver_Ptr = Ptr_2;
}

ISR(USART_TXC_vect){
	
	if ( Transmiter_Ptr != NULL){
		Transmiter_Ptr();
	}
}

ISR(USART_UDRE_vect){
	
	if ( UDR_Ptr != NULL){
		UDR_Ptr();
	}
	
}

ISR(USART_RXC_vect){
	
	if ( Receiver_Ptr != NULL){
			Receiver_Ptr();
	}
}
