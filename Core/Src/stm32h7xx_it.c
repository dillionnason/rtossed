/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "process.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_uart.h"
#include "stm32h7xx_it.h"
#include <stdint.h>
#include <stdio.h>

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_OTG_HS;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern SPI_HandleTypeDef hspi1;
extern int kready;
extern HAL_UART_StateTypeDef huart3;

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
	while (1) {}
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
	while (1) {}
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
	while (1) {}
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
	while (1) {}
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
	while (1) {}
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void) {}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void) {}

/**
  * @brief This function handles Pendable request for system service.
  */
void __attribute__((naked)) PendSV_Handler(void)
{
	context_register_save();

	register struct task_struct *next = schedule();
	register int *sp asm ("sp");

	current->r.R4  = *(sp + 0);
	current->r.R5  = *(sp + 1);
	current->r.R6  = *(sp + 2);
	current->r.R7  = *(sp + 3);
	current->r.R8  = *(sp + 4);
	current->r.R9  = *(sp + 5);
	current->r.R10 = *(sp + 6);
	current->r.R11 = *(sp + 7);

	if (next == &idle_task) {
		current->r.SP = __get_PSP();
	} else if (current == &idle_task) {
		__set_PSP(next->r.SP);
	} else {
		current->r.SP = __get_PSP();
		__set_PSP(next->r.SP);
	}

	current = next;

	sp += 8;

	context_register_restore(next);
	context_switch_return(next);
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
	uwTick += (uint32_t)uwTickFreq;
	if (uwTick > 32 && kready == 1) {
		HAL_GPIO_TogglePin(SysTick_LED_GPIO_Port, SysTick_LED_Pin);
		yield();
	}
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_spi1_tx);
}

/**
  * @brief This function handles SPI1 global interrupt.
  */
void SPI1_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&hspi1);
}

/*
 *  USART3 Interrup handler
 */
void USART3_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart3);
}

/**
  * @brief This function handles USB On The Go HS global interrupt.
  */
void OTG_HS_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
}
