 /******************************************************************************
 *
 * Module: application
 *
 * File Name: App.h
 *
 * Description: Header file for Application Tasks.
 *
 * Author: team 8 
 ******************************************************************************/

#ifndef APP_H_
#define APP_H_

#include "Std_Types.h"


#define button_released 1
#define button_pressed  0



/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/
/*enum for states of sender ECU*/
enum states {normal , button1_pressed , button2_pressed , both_pressed};



/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/* Description: Task executes once to initialize all the Modules */
void Init_Task(void);

/*function to send can message for ecu2&3 every 500ms*/
void transmit_task(void);

/*function responsiple for detecting switches and change the states*/
void t_state_machine (void);


#endif /* APP_H_ */
