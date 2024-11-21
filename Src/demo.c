/*
 * demo.c
 *
 *  Created on: Feb 14, 2021
 *      Author: larry kiser
 *  Update to starter code for non-blocking assignment.
 *
 *  Updated on: Sept 20, 2021
 *      Author: Mitesh Parikh
 *
 *  Updated on: March 20, 2022
 *  	Author: Saurav Raut
 *  	(LED Project Part 1)
 */

#include <stdio.h>
#include <string.h>

#include "LED.h"
#include "UART.h"
#include "demo.h"
#include "gpio.h"
#include "stm32l4xx.h"
#include "main.h"
//#include "stm32l4xx_hal_gpio.h"


static volatile uint8_t one_second_elapsed = 0;
static volatile uint8_t two_second_elapsed = 0;
static uint32_t Counter = 0;
static volatile uint8_t red_flasher = 0;   // used as a boolean to flash red led
static volatile uint8_t green_flasher = 0; // used as a boolean to flash green led
//static uint32_t CounterTwo = 0;
static volatile uint8_t pinState = 0;
static volatile uint8_t localMode = 0;

char *promt = ">>";
char *remote = "\r\n***REMOTE MODE ACTIVE***\r\n";
char *localMessage = "\r\n***MANUAL OVERRIDE MODE ACTIVE***\r\n";

static void initialize_help(){
	// Print out the available commands of the LED system
	char *helps = "\r\n***REMOTE LED CONTROL MENU***\r\n"
			"Available User Commands\r\n"
			"HELP - See all commands\r\n"
			"RON - Turn on RED LED\r\n"
			"ROFF - Turn off RED LED\r\n"
			"GON - Turn on GREEN LED\r\n"
			"GOFF - Turn off GREEN LED\r\n"
			"RFLASH - Start flashing RED LED\r\n"
			"GFLASH - Start flashing GREEN LED\r\n"
			"ALLOFF - TURNOFF LEDs\r\n";
	USART_Write(USART2, (uint8_t*)helps, strlen(helps));

}

static void process_commands(char input[]){
	char *invalid = "\r\nInvalid command\r\n";

	uint8_t line_feed = '\n';
	if (strcmp(input, "HELP") == 0){	// help command has been chosen
		initialize_help();
	}
	else if (strcmp(input,"RON") == 0){ 	// RED led on
		red_flasher = 0;
		LED_On_RED();
		USART_Write(USART2, &line_feed, 1);
	}
	else if (strcmp(input,"ROFF") == 0){ 	// RED led off
		red_flasher = 0;
		LED_Off_RED();
		USART_Write(USART2, &line_feed, 1);
	}
	else if (strcmp(input,"GON") == 0){	// GREEN led on
		green_flasher = 0;
		LED_On_GREEN();
		USART_Write(USART2, &line_feed, 1);
	}
	else if (strcmp(input,"GOFF") == 0){ 	// GREEN led off
		green_flasher = 0;
		LED_Off_GREEN();
		USART_Write(USART2, &line_feed, 1);
	}
	else if (strcmp(input, "ALLOFF") == 0){ // ALL OFF command has been chosen
		red_flasher = 0;
		green_flasher = 0;
		LED_ALL_OFF();
		USART_Write(USART2, &line_feed, 1);
	}
	else if (strcmp(input, "RFLASH") == 0){ // RED LED FLASHING
		red_flasher = 1;
		USART_Write(USART2, &line_feed, 1);

	}
	else if (strcmp(input, "GFLASH") == 0){ // GREEN LED FLASHING
		green_flasher = 1;
		USART_Write(USART2, &line_feed, 1);

	}

	else{
		USART_Write(USART2,(uint8_t*)invalid, strlen(invalid));// if an invalid command is chosen
	}

}

static void init_systick()
{

	// Disable SysTick by clearing the CTRL (CSR) register.
	SysTick->CTRL = 0x0;
	
	// Set the LOAD (RVR) to 80,000 to give us a 1 millisecond timer.
	uint32_t reload_value = 0x1387F;
	SysTick->LOAD = reload_value;
	
	// Set the clock source bit in the CTRL (CSR) to the internal clock.
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	// Set the enable bit in the CTRL (CSR) to start the timer.
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;


}



void SysTick_Handler(){
	if (Counter == 1000){
		one_second_elapsed = 1;
		Counter = 0;

	}
	else{
		Counter++;
	}
}

//void Led_BothFlash(){
//	while(1){
//		if (two_second_elapsed == 2){
//			LED_Flash_RED();
//			LED_Flash_GREEN();
//			two_second_elapsed = 0;
//		}
//	}
//}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{


	if(GPIO_Pin == B1_Pin)
	{
		if (pinState == 0){
			LED_On_External();
			USART_Write(USART2, (uint8_t *)localMessage,strlen(localMessage));
			pinState = 1;
			localMode = 1;
//			if (CounterTwo < 2001){
//				CounterTwo++;
//			}
//			while(CounterTwo == 2000){
//				LED_Flash_RED();
//				LED_Flash_GREEN();
//				CounterTwo = 0;
//			}

		}
		else{
			LED_ALL_OFF();
			LED_Off_External();
			USART_Write(USART2, (uint8_t*)remote, strlen(remote));
			USART_Write(USART2, (uint8_t*)promt, 2);
			pinState = 0;
			localMode = 0;
		}

	}
}

void run_demo()
{
	uint8_t current_char = 0 ;
	char user_command[15] ;
	int index = 0;
	uint8_t backspace[] = "\b \b";


	USART_Write(USART2, (uint8_t*)remote, strlen(remote));

	initialize_help();
	init_systick() ;

	USART_Write(USART2, (uint8_t*)promt, 2);

	while (1)
	{

		if (localMode == 1){
			current_char = USART_READ_Nonblocking( USART2 ) ; // read a character but ignore it.
			continue;
		}

		current_char = USART_READ_Nonblocking( USART2 ) ; // read a character
		// user presses 'enter'
		if (current_char == '\r'){
			user_command[index] = '\0'; // terminate the string
			process_commands(user_command); // process that command
			USART_Write( USART2, &current_char, 1 ) ;
			USART_Write(USART2, (uint8_t*)promt, 2);
			index = 0;
			memset(&user_command[0],0,sizeof(user_command)); //reset the buffer
		}
		if (current_char != '\r' && current_char != '\0' && current_char != 127){
			USART_Write( USART2, &current_char, 1 ) ;
			user_command[index] = (char)current_char;
			index++;

		}


		if (user_command[9] != '\0'){
					continue;
		}

		// if the user doesn't press 'enter', store characters until they do.

		if (current_char == 127){ // if a user presses backspace
			USART_Write( USART2, &backspace[0], sizeof (backspace) ) ; //delete the preceding characters

			if (index > 0){
				index--;
				user_command[index] = '\0';
			}
		}

		// Flash LEDS
		if (one_second_elapsed == 1){
				LED_Toggle();
				if (red_flasher == 1){
					LED_Flash_RED();
				}
				if (green_flasher == 1){
					LED_Flash_GREEN();
				}
				one_second_elapsed = 0;
		}

	}

}

