
#include "BCM.h"


static volatile uint8 BufferKey = UNLOCKED;
static volatile EnumBCMState_t CurrentState = Idle;
static StrBCMInternalBuffer_t Buffer;


static void BCM_TX_ISR_Callback(void)
{
	CurrentState = SendingByteComplete;
}



void BCM_Init(void)
{
	UART_SetterTransmit(BCM_TX_ISR_Callback);
	UART_Init();
}


EnumBCMError_t BCM_Send(uint8 *PtrArr , uint16 Buffer_Size)
{
	EnumBCMError_t StateVal = BCM_ok;
	if (BufferKey == LOCKED)
	{
		StateVal = SendInProcess;
	}
	else
	{
		BufferKey = LOCKED;
		Buffer.Module_ID = BCM_ID;
		Buffer.DataSize = Buffer_Size;
		Buffer.PtrData = PtrArr;
		Buffer.CheckSum = 0;
		
		
	
	}
	return StateVal;
}

EnumBCMError_t BCM_DispatcherTx(void)
{
	EnumBCMError_t state_val = BCM_ok;
	static uint16 BytesCounter = 0;
	static uint16 Local_iterator = 0;
	
	switch(CurrentState)
	{
		case Idle:
			if (BufferKey == LOCKED)
			{
				UART_Transmit(Buffer.Module_ID);
				BytesCounter = 1;
				Buffer.CheckSum += Buffer.Module_ID;
				CurrentState = SendingByte;
			}
			else
			{
				// Idle sleeping
			}
		
		break;
		
		case SendingByte:
		
		
		break;
		
		case SendingByteComplete:
		
		// check which byte is supposed to sent
			if (BytesCounter == Buffer.DataSize + 4)
			{
				CurrentState = SendingFrameComplete;
			}
			else if (BytesCounter == 1)
			{
				// transmit low byte of the size variable
				UART_Transmit((uint8) Buffer.DataSize);
				// change state
				CurrentState = SendingByte;
				// counter will be increased
				BytesCounter++;
				// increase the check sum
				Buffer.CheckSum += ( (uint8) Buffer.DataSize);
			}
			else if (BytesCounter == 2)
			{
				// transmit high byte of the size variable
				UART_Transmit((uint8)((Buffer.DataSize)>>8));
				CurrentState = SendingByte;
				BytesCounter++;
				Buffer.CheckSum += ((uint8)((Buffer.DataSize)>>8));
			}
			else if (BytesCounter <= (Buffer.DataSize)+2 )
			{
				// start to transmit the data 
				UART_Transmit(Buffer.PtrData[Local_iterator]);
				Buffer.CheckSum += Buffer.PtrData[Local_iterator];
				Local_iterator++;
				CurrentState = SendingByte;
				BytesCounter++;
			}
			else 
			{
				UART_Transmit(Buffer.CheckSum);
				CurrentState = SendingByte;
				BytesCounter++;
			}	
		break;
		
		case SendingFrameComplete:
			// consumer
			PORTA |= 1;
			// unlock the buffer
			BufferKey = UNLOCKED;
			// clear the counters
			BytesCounter = 0;
			Local_iterator = 0;
			// idle state
			CurrentState = Idle;
		break;
		
		default:
		break;
	
	}
	
	
}