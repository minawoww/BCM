



























/*
 * BCM.c
 *
 * Created: 10/19/2019 6:35:32 PM
 * Author : minam
 */ 

#include "BCM.h"
#include "LCD.h"
#include <avr/sleep.h>

volatile uint8 RxData_Arr[10];
// this function will be called when the transmition completed.
void Tx_Consumer(void)
{
	// toggle led without using the DIO driver.
	//DDRA |= 1;
	//PORTA |= 1;
}

void SucConsumer (void)
{
	for (uint8 i = 0 ; i < 10 ; i++)
	{
		// lcd on port a
		LCD_CHAR_DISP(0,i,RxData_Arr[i]);
	}
}


int main(void)
{
	BCM_Init();
	BCM_TXCCallback(Tx_Consumer);
	BCM_SuccessfulRxCallBack(SucConsumer); 
	
	uint8 TxData_Arr[5] = "MINA";
	BCM_Send(TxData_Arr , 4);
	BCM_Receive(RxData_Arr , 10);
	LCD_INIT_4bit();
    while (1) 
    {
		
		BCM_DispatcherTx();
		BCM_DispatcherRx();
		
		sleep_mode();
		
    }
}

