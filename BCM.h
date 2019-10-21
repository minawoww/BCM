

#ifndef BCM_H_
#define BCM_H_

#include "avr_reg.h"
#include "STD_Types.h"

#define LOCKED	 1
#define UNLOCKED 0

#define BCM_ID 0xA5

typedef void(*Ptr_VFunctionV)(void);

typedef enum { SendInProcess , ReceiveInProcess , NullPtr , BCM_ok }EnumBCMError_t;
typedef enum { Idle , SendingByte , SendingByteComplete , SendingFrameComplete }EnumTxStateBCM_t;
typedef enum { Rx_Idle , ReceivingByte, ReceivingByteComplete, ReceivingFrameComplete }EnumRxStateBCM_t;

	


typedef struct 
{
	uint8 Module_ID;
	uint8 *PtrData;
	uint8 CheckSum;
	uint16 DataSize;
	
}StrBCMInternalBuffer_t;

void BCM_Init(void);
EnumBCMError_t BCM_Send(uint8 *Data_Buffer , uint16 Buffer_Size);
EnumBCMError_t BCM_DispatcherTx(void);



EnumBCMError_t BCM_Receive(uint8 Array[], uint16 ArrSize);
EnumBCMError_t BCM_DispatcherRx (void);


void BCM_SuccessfulRxCallBack (Ptr_VFunctionV PtrFunction);
void BCM_FailedRxCallBack (Ptr_VFunctionV PtrFunction);
void BCM_TXCCallback(Ptr_VFunctionV ptr);

#endif