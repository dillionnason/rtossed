#include "main.h"
#include "gpio.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include "user_syscalls.h"

/**
	* Blink the green LED (LD1) at 2Hz
	*/
int process1(void)
{
	while (1) {
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		microsleep(500000);
	}
	return 0;
}

/**
	* Blink the amber LED (LD2) at 1Hz
	*/
void process2(void)
{
	while (1) {
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		microsleep(1000000);
	}
}
