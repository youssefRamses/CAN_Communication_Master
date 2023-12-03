 /******************************************************************************
 *
 * Module: Application
 *
 * File Name: App.c
 *
 * Description: Source file for Application Tasks.
 *
 * Author: team 8 
 ******************************************************************************/

#include "App.h"
#include "Dio.h"
#include "Port.h"
#include "CAN.h"


/*enum instance for states*/
enum states s = normal ;
/*global variables for reading switches states */
static uint8 sw1=button_released; 
static uint8 sw2=button_released;



/************************************************************************************
* Function Name: Init_Task
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* inputs : None
* outputs : None
* Description: Function to Initialize Dio, CAN and PORTS
************************************************************************************/
void Init_Task(void)
{
    /* Initialize Port Driver */
    Port_Init(&Port_pinConfigurationSet); 
    /* Initialize Dio Driver */
    Dio_Init(&Dio_Configuration);
    /*initialize CAN driver */
    CAN_INIT();

}


/************************************************************************************
* Function Name: t_state_machine
* Sync/Async: Synchronous
* Reentrancy: reentrant
* inputs : None
* outputs : None
* Description: function read switche values and change the state of the sender ECU
************************************************************************************/
void t_state_machine (void)

{
  /*check the value of the switch 1 only when it is released */
if( sw1==button_released) 
    sw1 = Dio_ReadChannel(DioConf_SW1_CHANNEL_ID_INDEX);
    
/*check the value of the switch 2 only when it is released */
if (sw2==button_released)
    sw2 = Dio_ReadChannel(DioConf_SW2_CHANNEL_ID_INDEX);


/*checking if switch 1 is released then check switch 2 is released or pressed */
    if (sw1==button_released)
  {
    /*if switch 2 is released so the both switches are released so stay at normal state*/
    if (sw2==button_released) 
    {
      s = normal ;
    }
    /*if switch 2 is pressed so switch 2 is only pressed so state change to button2_pressed*/
    else if (sw2==button_pressed)
    {
      s = button2_pressed;
    }
    
  }
  /*checking if switch 1 is pressed then check switch 2 is released or pressed */
  else if (sw1 ==button_pressed)
  {
    /*if switch 2 is releaased so switch 1 is only pressed so state change to button1_pressed*/
     if (sw2==button_released) 
    {
      s = button1_pressed ;
    }
    /*if switch 2 is pressed so the both switches are pressed and state changes to both_pressed*/
    else if (sw2==button_pressed)
    {
      s = both_pressed;
    }
  }
}



/************************************************************************************
* Function Name: transmit_task
* Sync/Async: Synchronous
* Reentrancy: reentrant
* inputs : None
* outputs : None
* Description: function called every 500 ms so sending a messaage through can to both ECU 2&3 
               according to the state
************************************************************************************/
void transmit_task(void) 
{
   /*all cases will send can messages to both tx1 &tx2  
     sending 0 means nothing pressed , 1 means pressed , 2 means both pressed */
    switch (s)
    {
       /*case normal means no switches is pressed */
       case normal:
          CAN_Send1(0);
          CAN_Send2(0);
          break ;
       /*case button1_pressed means only button 1 is pressed */
       case button1_pressed: 
         CAN_Send1(1);
         CAN_Send2(0);
         break;
        /*case button2_pressed means only button 2 is pressed */ 
       case button2_pressed: 
         CAN_Send1(0);
         CAN_Send2(1);
         break;
         /*case both_pressed means both switches pressed */
       case both_pressed: 
         CAN_Send1(2);
         CAN_Send2(2);
         break;
    }
    
    /*after sending can messages return switch 1&2 global variables as released */
    sw1= button_released ;
    sw2= button_released;
}
