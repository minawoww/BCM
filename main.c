/*
 * BCM.c
 *
 * Created: 10/19/2019 6:35:32 PM
 * Author : minam
 */ 

#include "BCM.h"


int main(void)
{
	BCM_Init();
	uint8 Arr[3] = {1, 2, 3};
	BCM_Send(Arr, 3);
	
	
    while (1) 
    {
		BCM_DispatcherTx();
    }
}

