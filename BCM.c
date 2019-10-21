
#include "BCM.h"


static volatile uint8 BufferKey = UNLOCKED;
static volatile uint8 RxBufferKey = UNLOCKED;

static volatile EnumTxStateBCM_t CurrentState = Idle;
// state variable for the BCM receiver
static volatile EnumRxStateBCM_t Rx_CurrentState = Idle; 

// create an object of the buffer for the transmitter.
static StrBCMInternalBuffer_t Buffer;
// create an object of the buffer for the receiver. 
static StrBCMInternalBuffer_t RxBuffer;


// the size of the array in the application
static volatile uint16 ApplicationArraySize = 0;
// variable for counting the bytes received.
static volatile uint16 RxBytesCounter = 0;
// flag for size of the data 
static volatile uint8 RxInsufficientSize = 0;
// check sum global for checking
static volatile uint8 RxReceivingCheckSum = 0;


// prototype for the static function 
static EnumBCMError_t BCM_SetupBuffer(uint8 Array[], uint16 ArrSize );

static Ptr_VFunctionV PtrBCM_ConsumerTx = NULL;
static Ptr_VFunctionV PtrBCM_ConsumerRxSuccessful = NULL;
static Ptr_VFunctionV PtrBCM_ConsumerRxFailed = NULL;


static void BCM_TX_ISR_Callback(void)
{
	CurrentState = SendingByteComplete;
}

static void BCM_RxISRCallBack(void)
{	
	volatile uint8 ReceivedData;
	static uint16 Local_Iterator = 0;
	
	UART_Receive(&ReceivedData);
	RxBytesCounter++;

	 // if the buffer is ready to receive.
	 if (RxBufferKey == LOCKED)
	 {
		 if (RxBytesCounter == 1)
		 {
			 // save the ID
			 RxBuffer.Module_ID = ReceivedData;
			 // check if the received ID is correct
			 if (RxBuffer.Module_ID == BCM_ID)
			 {
				  RxReceivingCheckSum += ReceivedData;
			 }
			 // received ID doesn't match.
			 else 
			 {			 
				 RxBytesCounter = 0;
			 }
		 }
		 else if (RxBytesCounter == 2)
		 {
			 RxBuffer.DataSize = (uint8)ReceivedData;
			 RxReceivingCheckSum += ReceivedData;
		 }
		 else if (RxBytesCounter == 3)
		 {

			PORTA^= 1;	
			RxBuffer.DataSize &= 0x00ff; 
			RxBuffer.DataSize |= (uint8)(ReceivedData<<8);
			RxReceivingCheckSum += ReceivedData;
			if (ApplicationArraySize < RxBuffer.DataSize )
			{
				// flag for not having a sufficient size
				RxInsufficientSize = 1;
			}	
		 }
		 else if ( (RxInsufficientSize == 0) && (RxBytesCounter <= RxBuffer.DataSize+3) )
		 {
			 RxBuffer.PtrData[Local_Iterator] = ReceivedData;
			 Local_Iterator++;
			 RxReceivingCheckSum += ReceivedData;
		 }
		 else if( RxBytesCounter == RxBuffer.DataSize + 4)
		 {
			 RxBuffer.CheckSum = ReceivedData;
			 Rx_CurrentState = ReceivingFrameComplete;
		 }
	 }
	 else
	 {
		// App didn't request to receive data 
	 }
}




void BCM_Init(void)
{
	UART_SetterTransmit(BCM_TX_ISR_Callback);
	UART_SetterReceive(BCM_RxISRCallBack);
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
			if (PtrBCM_ConsumerTx != NULL)
			{
				PtrBCM_ConsumerTx();
			}
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


EnumBCMError_t BCM_Receive(uint8 Array[], uint16 ArrSize)
{	
	EnumBCMError_t StateVal = BCM_ok;
	if (RxBufferKey == UNLOCKED)
	{
		RxBufferKey = LOCKED;
		// call function to construct the buffer
		BCM_SetupBuffer(Array, ArrSize);
		// state change to byte receiving state
		Rx_CurrentState = ReceivingByte;
	}
	else 
	{
		StateVal = ReceiveInProcess;
	}
	return StateVal;
}

static EnumBCMError_t BCM_SetupBuffer(uint8 Array[], uint16 ArrSize )
{
	EnumBCMError_t StateVal = BCM_ok;
	if (Array == NULL)
	{
		StateVal = NullPtr;
	}
	else 
	{
			RxBuffer.CheckSum = 0;
			RxBuffer.PtrData = Array;
			ApplicationArraySize = ArrSize;
	}
	return StateVal;
}



EnumBCMError_t BCM_DispatcherRx (void)
{
	EnumBCMError_t StateVal = BCM_ok;
	
	switch(Rx_CurrentState)
	{
		case Rx_Idle:
		
		break;
		
		case ReceivingByte:
		
		break;
		
		case ReceivingByteComplete:
			
		break;
		
		case ReceivingFrameComplete:
		// check on the check sum to make sure that data was correct
			if (RxBuffer.CheckSum == RxReceivingCheckSum)
			{
				// call the successful consumer in the application
				if (PtrBCM_ConsumerRxSuccessful != NULL)
				{
					PtrBCM_ConsumerRxSuccessful();
				}

			}
			else
			 {
				 // call the failed consumer in the application
				if (PtrBCM_ConsumerRxFailed != NULL)
				{
					PtrBCM_ConsumerRxFailed();
				}
			 }
			 
			Rx_CurrentState = Rx_Idle;
			RxBytesCounter = 0;
			RxBuffer.CheckSum = 0;
			RxBuffer.DataSize = 0;
		break;
	}
	
}

void BCM_SuccessfulRxCallBack (Ptr_VFunctionV PtrFunction)
{
	PtrBCM_ConsumerRxSuccessful = PtrFunction;
}

void BCM_FailedRxCallBack (Ptr_VFunctionV PtrFunction)
{
	PtrBCM_ConsumerRxFailed = PtrFunction;
}

void BCM_TXCCallback(Ptr_VFunctionV ptr)
{
	PtrBCM_ConsumerTx = ptr;
}