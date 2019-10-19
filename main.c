/*
 * BCM.c
 *
 * Created: 10/19/2019 6:35:32 PM
 * Author : minam
 */ 

#include "BCM.h"


int main(void)
{
	DDRA |= 1;
	BCM_Init();
	uint8 Arr[] = "REZK";
	BCM_Send(Arr, 4);
	
    while (1) 
    {
		BCM_DispatcherTx();
    }
}

