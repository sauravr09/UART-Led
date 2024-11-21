#ifndef __NUCLEO476_LED_H
#define __NUCLEO476_LED_H

#include "stm32l476xx.h"

void LED_Init(void);

void LED_Off_GREEN(void);
void LED_On_GREEN(void);
void LED_Toggle(void);
void LED_On_RED(void);
void LED_Off_RED(void);
void LED_ALL_OFF(void);
void LED_Flash_RED(void);
void LED_Flash_GREEN(void);
void LED_On_External(void);
void LED_Off_External(void);

#endif /* __NUCLEO476_LED_H */
