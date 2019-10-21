/*
 * BCM.c
 *
 * Created: 10/19/2019 6:35:32 PM
 * Author : minam
 */ 


#include "BCM.h"

// this function will be called when the transmition completed.
void Tx_Consumer(void)
{
	// toggle led without using the DIO driver.
	DDRA |= 1;
	PORTA |= 1;
}

int main(void)
{
	
	BCM_Init();
	BCM_TXCCallback(Tx_Consumer);
	uint8 Data_Arr[5] = "Hello";
	BCM_Send(Data_Arr , 5);
	
    while (1) 
    {
		BCM_DispatcherTx();
    }
}

