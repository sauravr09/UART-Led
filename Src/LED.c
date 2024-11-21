//#include "LED.h"
//
//
////******************************************************************************************
//// User LED = LD2 Green LED = PA.5
////******************************************************************************************
//#define LED_PIN 5
//
//void LED_Init(void){
//
//	// Enable the peripheral clock of GPIO Port
//	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
//
//	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
//	GPIOA->MODER &= ~(3U<<(2*LED_PIN));
//	GPIOA->MODER |= 1U<<(2*LED_PIN);      //  Output(01)
//
//	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
//	GPIOA->OSPEEDR &= ~(3U<<(2*LED_PIN));
//	GPIOA->OSPEEDR |=   3U<<(2*LED_PIN);  // High speed
//
//	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
//	GPIOA->OTYPER &= ~(1U<<LED_PIN);       // Push-pull
//
//	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
//	GPIOA->PUPDR   &= ~(3U<<(2*LED_PIN));  // No pull-up, no pull-down
//
//}
//
////******************************************************************************************
//// Turn LED On
////******************************************************************************************
//void LED_On(void){
//	GPIOA->ODR |= (1UL<<LED_PIN);
//}
//
////******************************************************************************************
//// Turn LED Off
////******************************************************************************************
//void LED_Off(void){
//	GPIOA->ODR &= ~(1UL<<LED_PIN);
//}
//
////******************************************************************************************
//// Toggle LED
////******************************************************************************************
//void LED_Toggle(void){
//	GPIOA->ODR ^= (1UL<<LED_PIN);
//}
#include "LED.h"


//******************************************************************************************
// Green LED on Nucleo board is LD2 Green. This is hard wired to GPIO pin PA5.
//******************************************************************************************
#define NUCLEO_GREEN_LED_PIN (5)	// GPIO PA5

// project specific manually connected LEDs on breadboard shield.
#define ARDUINO_D12 (6)				// GPIO PA6 -- hard wired to Arduino D12
#define ARDUINO_D11 (7)				// GPIO PA7 -- hard wired to Arduino D11
#define ARDUINO_D8  (9)

// GPIO Output initialization for GPIO banks A, B, C, and D only
// Expects GPIO to be GPIOA, GPIOB, GPIOC, or GPIOD.
// pin is expected to be 0 through 15
// Note -- if the clock is not enabled the 16 GPIO pins are non-functional!
void GPIO_Output_Init( GPIO_TypeDef *GPIO, uint32_t pin )
{
	// Enable the peripheral clocks of for GPIO
	if ( GPIO == GPIOA )
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN ;
	else if ( GPIO == GPIOB )
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN ;
	else if ( GPIO == GPIOC )
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN ;
	else if ( GPIO == GPIOD )
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN ;
	else
		return ;

	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIO->MODER &= ~( (uint32_t)3 << ( 2 * pin ) ) ;  // clear to input mode (00)
	GPIO->MODER |= (uint32_t)1 << ( 2 * pin ) ;      //  Output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIO->OSPEEDR &= ~( (uint32_t)3 << ( 2 * pin ) ) ;	// set to cleared which is low speed
	GPIO->OSPEEDR |=   (uint32_t)3 << ( 2 * pin ) ;  // High speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIO->OTYPER &= ~( (uint32_t)1 << pin ) ;       // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIO->PUPDR   &= ~( (uint32_t)3 << ( 2 * pin ) ) ;  // No pull-up, no pull-down
}

// set up PA5, PA6, and PA7 to drive LEDs
void LED_Init(void){
	GPIO_Output_Init( GPIOA, NUCLEO_GREEN_LED_PIN ) ;	// hard wired PA5 to on-board LD2 LED
	GPIO_Output_Init( GPIOA, ARDUINO_D12 ) ;		// init PA6 as output (D12)
	GPIO_Output_Init( GPIOA, ARDUINO_D11 ) ;		// init PA7 as output (D11)
	GPIO_Output_Init( GPIOA, ARDUINO_D8  ) ;
}

//******************************************************************************************
// Turn Green LED On and GPIOA 6 and 7 to ON
//******************************************************************************************
void LED_On_GREEN(void){

	GPIOA->ODR |= (uint32_t)1 << ARDUINO_D12 ;	// PA6

}

//******************************************************************************************
// Turn Green LED Off and GPIOA 6 and 7 to OFF
//******************************************************************************************
void LED_Off_GREEN(void){

	GPIOA->ODR &= ~( (uint32_t)1 << ARDUINO_D12 ) ;	// PA6

}

//******************************************************************************************
// Set Green LD2 Nucleo LED and GPIOA 6 and 7 to opposite states
//******************************************************************************************
void LED_Toggle(void){
	GPIOA->ODR ^= (uint32_t)1 << NUCLEO_GREEN_LED_PIN ;
	//GPIOA->ODR ^= (uint32_t)1 << ARDUINO_D12 ;	// PA6
	//GPIOA->ODR ^= (uint32_t)1 << ARDUINO_D11 ;	// PA7
}

void LED_On_RED(void){
	GPIOA->ODR |= (uint32_t)1 << ARDUINO_D11 ;	// PA7
}

void LED_Off_RED(void){
	GPIOA->ODR &= ~( (uint32_t)1 << ARDUINO_D11 ) ;	// PA7
}

void LED_ALL_OFF(void){
	 GPIOA->ODR &= ~( (uint32_t)1 << NUCLEO_GREEN_LED_PIN );
	 GPIOA->ODR &= ~( (uint32_t)1 << ARDUINO_D12 ) ;	// PA6
	 GPIOA->ODR &= ~( (uint32_t)1 << ARDUINO_D11 ) ;	// PA7
	// GPIOA->ODR &= ~( (uint32_t)1 << ARDUINO_D8 ) ;
}

void LED_Flash_RED(void){
	GPIOA->ODR ^= (uint32_t)1 << ARDUINO_D11 ;
}

void LED_Flash_GREEN(void){
	GPIOA->ODR ^= (uint32_t)1 << ARDUINO_D12 ;
}

void LED_On_External(void){
	GPIOA->ODR |= (uint32_t)1 << ARDUINO_D8 ;
}

void LED_Off_External(void){
	GPIOA->ODR &= ~( (uint32_t)1 << ARDUINO_D8 ) ;
}


