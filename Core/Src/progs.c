#include "main.h"
#include "gpio.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_hsem.h"
#include "user_syscalls.h"
#include "process.h"
#include <stdint.h>
#include <stdio.h>

/*
 * Blink the green LED (LD1) at 2Hz
 */
int process1(void)
{
	//static uint8_t i = 0;
	uint8_t i = 0;
	while (1) {
		//while (HAL_HSEM_Take(0, current->pid) == HAL_ERROR) {}
		printf("Current: %d | The current value of i is: %u\r\n", current->pid, i++);
		//HAL_HSEM_Release(0, current->pid);
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		//microsleep(500000);
	}
	return 0;
}

/*
 * Blink the amber LED (LD2) at 1Hz
 */
void process2(void)
{
	while (1) {
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		microsleep(1000000);
	}
}
