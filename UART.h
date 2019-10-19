#ifndef UART_H_
#define UART_H_

#include "UART_conf.h"
#include "STD_Types.h"
#include "avr_reg.h"

// max number of UART in the target
#define UART_MAX_MODULE     1

#define  NULL  ((void*)0)

STD_Fun_t UART_Init(void);
void UART_Transmit(uint8 Data);
void UART_Receive(uint8 * Data);

void UART_SetterTransmit(void (*Ptr) (void));
void UART_SetterReceive(void (*Ptr) (void));


#endif /* UART_H_ */
