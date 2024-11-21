// main.c with demo
// Updated and refactored Larry Kiser copyright 2021


// include project specific header files
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "demo.h"
#include "stm32l4xx_hal.h"
#include "gpio.h"


	
int main(void){

	// initialization code
	System_Clock_Init(); // set System Clock = 80 MHz
	LED_Init();
	UART2_Init();
	HAL_Init();
	GPIO_Init();

	
	// application run function
	run_demo() ;
}

