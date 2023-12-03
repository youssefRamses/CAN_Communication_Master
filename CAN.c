#include "CAN.h"

volatile uint32 g_ui32MsgCount_rx = 0;
volatile uint32 g_ui32MsgCount_tx = 0;

volatile bool g_bRXFlag1 = 0;
volatile bool g_bRXFlag2 = 0;
volatile bool g_bErrFlag = 0;

typedef uint32 unsignedint;
uint8 r_data=0;

uint8_t pui8MsgData[6];

tCANMsgObject sCANMessage_tx1;
tCANMsgObject sCANMessage_tx2;
tCANMsgObject sCANMessage_rx;

void CANIntHandler(void)
{
    uint32_t ui32Status;
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);
    
    if(ui32Status == CAN_INT_INTID_STATUS)
    {
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
        g_bErrFlag = 1;
    }
    else if(ui32Status == 1)
    {
        CANIntClear(CAN0_BASE, 1);
        g_ui32MsgCount_rx++;
        g_bRXFlag1 = 1;
        g_bErrFlag = 0;
    }

    else if(ui32Status == 2)
    {
        CANIntClear(CAN0_BASE, 2);
        g_ui32MsgCount_rx++;
        g_bRXFlag2 = 1;
        g_bErrFlag = 0;
    }    else if(ui32Status == 1)
    {
        CANIntClear(CAN0_BASE, 1);
        g_ui32MsgCount_rx++;
        g_bRXFlag1 = 1;
        g_bErrFlag = 0;
    }

    else if(ui32Status == 2)
    {
        CANIntClear(CAN0_BASE, 2);
        g_ui32MsgCount_rx++;g_bRXFlag2 = 1;
        g_bErrFlag = 0;
    }

    else
    {
        
    }
}
void CAN_INIT(void)
{
   SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    CANInit(CAN0_BASE);
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
    defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
    CANBitRateSet(CAN0_BASE, ui32SysClock, 500000);
#else
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
#endif
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
    IntEnable(INT_CAN0);
    CANEnable(CAN0_BASE);
    sCANMessage_rx.ui32MsgID =0x1001; sCANMessage_tx1.ui32MsgID=0x1001; sCANMessage_tx2.ui32MsgID=0x2001;
    sCANMessage_rx.ui32MsgIDMask = 0xfffff;  sCANMessage_tx1.ui32MsgIDMask = 0; sCANMessage_tx1.ui32MsgIDMask = 0; 
    sCANMessage_rx.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    sCANMessage_tx1.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    sCANMessage_rx.ui32MsgLen = 8; sCANMessage_tx1.ui32MsgLen = 8; sCANMessage_tx2.ui32MsgLen = 8;
    sCANMessage_tx1.pui8MsgData = pui8MsgData;
    sCANMessage_tx2.pui8MsgData = pui8MsgData;
    CANMessageSet(CAN0_BASE, 1, &sCANMessage_rx, MSG_OBJ_TYPE_RX);
    CANMessageSet(CAN0_BASE, 2, &sCANMessage_rx, MSG_OBJ_TYPE_RX);
}
void CAN_Send1(uint8 data){
  pui8MsgData[0]=data;
  CANMessageSet(CAN0_BASE, 1, &sCANMessage_tx1, MSG_OBJ_TYPE_TX);
}
void CAN_Send2(uint8 data){
  pui8MsgData[0]=data;
  CANMessageSet(CAN0_BASE, 2, &sCANMessage_tx2, MSG_OBJ_TYPE_TX);
}
uint8 CAN_Receiver(void){
        if(g_bRXFlag1==1)
        {
            sCANMessage_rx.pui8MsgData = pui8MsgData;
            CANMessageGet(CAN0_BASE, 1, &sCANMessage_rx, 0);
            
            r_data=pui8MsgData[0];
            
              
            
            g_bRXFlag1 = 0;
        }
        
         if(g_bRXFlag2==1)
        {
            sCANMessage_rx.pui8MsgData = pui8MsgData;
            CANMessageGet(CAN0_BASE, 2, &sCANMessage_rx, 0);
            r_data=pui8MsgData[0];
            
            
            g_bRXFlag2 = 0;
        }
        return r_data ;
        
}