#ifndef UART_conf_H_
#define UART_conf_H_

#include "avr_reg.h"
#include "STD_Types.h"

typedef enum {module1, Num_UART} UART_t ;


// UCSRA
// 7 	6 	5 	 4   3   2  1   0
// RXC TXC UDRE FE 	DOR PE U2X MPCM

#define RXC		7   // receive complete
#define	TXC		6	// transmit complete
#define	UDRE	5	// data reg empty
#define	FE		4	// flag error
#define	DOR		3	// data overrun
#define	PE		2	// parity error
#define	U2X		1	// double speed mode
#define	MPCM	0	// multi processor mode


// UCSRB
// 	 7 	   6 	 5 	  4 	3 	 2 	   1    0
// RXCIE TXCIE UDRIE RXEN TXEN UCSZ2 RXB8 TXB8

#define	RXCIE	7	// receive complete interrupt enable
#define	TXCIE	6	// transmit ..  ..
#define	UDRIE	5	// data reg empty interrupt enable
#define	RXEN	4	// receive enable
#define	TXEN	3	// transmit enable
#define	UCSZ2	2	// character size
#define	RXB8	1	// 8 bit receive
#define	TXB8	0	// 8 bit transmit


// UCSRC
//	7 	   6 	5 	  4    3 	2 	  1 	0
// URSEL UMSEL UPM1 UPM0 USBS UCSZ1 UCSZ0 UCPOL

#define	URSEL	7	// usart register select
#define	UMSEL	6	// usart mode select
#define	UPM1	5	// usart parity mode
#define	UPM0	4	// usart .. ..
#define	USBS	3	// usart stop bit select
#define	UCSZ1	2	// usart charater size
#define	UCSZ0	1	// usart character size
#define	UCPOL	0	// usart polarity


#define TranRecMode_Transmitter     0
#define TranRecMode_Receiver        1
#define TranRecMode_Tran_Rec        2



#define BaudRate_9600        0


#define FrameSize_8          0


#define ParityMode_None     0
#define ParityMode_Even     1
#define ParityMode_Odd      2


#define StopMode_1Bit       1
#define StopMode_2Bit       2


#define InterruptMode_Off   0
#define InterruptMode_On    1



typedef struct {

    uint8 TranRecMode;
    uint8 BaudRate;
    uint8 FrameSize;
    uint8 InterruptMode;

} UART_Conf_t;



extern UART_Conf_t UART_Array_Conf[Num_UART] ;


#endif /* UART_conf_H_ */

