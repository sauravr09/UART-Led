/*
 * gpio.c
 *
 *  Created on: Nov 4, 2021
 *      Author: Mitesh Parikh
 */


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "LED.h"
#include "UART.h"
#include "demo.h"
#include <stdbool.h>
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal.h"
#include <string.h>

// External Global Variables that we will need access to

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/* EXTI15_10_IRQn interrupt init*/
	// Note you will have to add EXTI15_10_IRQn Interrupt handler function as well
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}





