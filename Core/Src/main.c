/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_types.h"
#include "general_config.h"
#include "PUI_public_interface.h"
#include <PCC_public_interface.h>
#include <sys_public_interface.h>
#include <AINT_public_interface.h>
#include <ATB_public_interface.h>

#include <lvgl.h>
#include "ili9341_lvgl_lcd_controller.h"
#include <TD_SPI_public_interface.h>
#include "ui.h"
#include <stdio.h>

#include "PCC_private_interface.h"
//#include "demos/benchmark/lv_demo_benchmark.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LVGL_EN 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi3_tx;

/* USER CODE BEGIN PV */
u32 blink_tick_u32                  = 0;
u32 pcc_handler_task__ticks__u32    = (u32)0;
u32 lvgl_task_tick_u32              = (u32)0;
PCC_Params_struct* active_params_ps;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI3_Init(void);
/* USER CODE BEGIN PFP */
void GPIO_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI3_Init();
  /* USER CODE BEGIN 2 */
  GPIO_Init();
  PUI_Init();

  TD_SPI_Init_v();
#if LVGL_EN
  lv_init();
  lv_port_disp_init();
#endif

  ATB_Init_v();
  SYS_IWDG_Init_v();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
#if 1
      if(ATB_CheckIfPeriodHasElapsed_b(&pcc_handler_task__ticks__u32, ATB__ms__TO__ticks__du32(1)))
      {
          PCC_Handler_v();
      }
#endif

#if LVGL_EN
	  if(ATB_CheckIfPeriodHasElapsed_b(&lvgl_task_tick_u32, ATB__ms__TO__ticks__du32(5)))
	  {
	      lv_timer_handler();
	      ui_tick();
	  }

#endif
      PUI_Handler();

#if 1
	  if(ATB_CheckIfPeriodHasElapsed_b(&blink_tick_u32, ATB__ms__TO__ticks__du32(250)))
	  {
		  LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_2);
	  }
#endif
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      SYS_IWDG_FeedTheDog_v();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4)
  {
  }
  LL_PWR_EnableRange1BoostMode();
  LL_RCC_HSE_Enable();
   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {
  }

  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_6, 85, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_EnableDomain_SYS();
  LL_RCC_PLL_Enable();
   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Insure 1us transition state at intermediate medium speed clock*/
  for (__IO uint32_t i = (170 >> 1); i !=0; i--);

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_SetSystemCoreClock(170000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 7;
  hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(CS_GPIO_Port, CS_Pin);

  /**/
  LL_GPIO_ResetOutputPin(DC_GPIO_Port, DC_Pin);

  /**/
  LL_GPIO_ResetOutputPin(RESET_GPIO_Port, RESET_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_2);

  /**/
  GPIO_InitStruct.Pin = CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = DC_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DC_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = RESET_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(RESET_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void GPIO_Init(void)
{
    /*************************************************************************************************
    * Enable all GPIO port clocks.
    *************************************************************************************************/
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN  |
				   RCC_AHB2ENR_GPIOBEN 	|
				   RCC_AHB2ENR_GPIOCEN  |
				   RCC_AHB2ENR_GPIODEN  |
				   RCC_AHB2ENR_GPIOEEN  |
				   RCC_AHB2ENR_GPIOFEN  |
				   RCC_AHB2ENR_GPIOGEN;

	/*************************************************************************************************
	* Onboard LED.
	*************************************************************************************************/
	GPIOD->MODER					&= ~GPIO_MODER_MODE2_Msk;
	GPIOD->MODER					|= 1UL << GPIO_MODER_MODE2_Pos;

   	/*************************************************************************************************
   	* BOOT0.
   	*************************************************************************************************/
	GPIOB->MODER					&= ~GPIO_MODER_MODE8_Msk;
	GPIOB->MODER					|= 1UL << GPIO_MODER_MODE8_Pos;

	/*************************************************************************************************
	* Gate driver enable pins.
	*************************************************************************************************/
	/* PC0 - PC3 <-> GD_en1 - GD_en4 */
	GPIOC->ODR						&= ~(0xF);
	GPIOC->MODER					&= 	~(GPIO_MODER_MODE0_Msk 	|
										 GPIO_MODER_MODE1_Msk 	|
										 GPIO_MODER_MODE2_Msk 	|
										 GPIO_MODER_MODE3_Msk	);
	GPIOC->MODER					|=	(1UL << GPIO_MODER_MODE0_Pos) |
										(1UL << GPIO_MODER_MODE1_Pos) |
										(1UL << GPIO_MODER_MODE2_Pos) |
										(1UL << GPIO_MODER_MODE3_Pos);

	/* PB2 <-> GD_en5 */
	GPIOB->BSRR						|= GPIO_BSRR_BR2_Msk;
	GPIOB->MODER					&= ~GPIO_MODER_MODE2_Msk;
	GPIOB->MODER					|= 1UL << GPIO_MODER_MODE2_Pos;

	/* PA5 <-> GD_en6 */
	GPIOA->BSRR						|= GPIO_BSRR_BR5_Msk;
	GPIOA->MODER					&= ~GPIO_MODER_MODE5_Msk;
	GPIOA->MODER					|= 1UL << GPIO_MODER_MODE5_Pos;

    /*************************************************************************************************
    * Digital inputs
    *************************************************************************************************/
#if (CONFIG_ENABLE_DIGITAL_INPUTS_d == ENABLE)
	DI0_GPIO_PORT_dps->MODER     &= ~(0x3U << (0x2U * DI0_GPIO_PIN_d));
	DI0_GPIO_PORT_dps->PUPDR     &= ~(0x3U << (0x2U * DI0_GPIO_PIN_d));
	DI0_GPIO_PORT_dps->PUPDR     |= 0x2U << (0x2U * DI0_GPIO_PIN_d);

	DI1_GPIO_PORT_dps->MODER     &= ~(0x3U << (0x2U * DI1_GPIO_PIN_d));
	DI1_GPIO_PORT_dps->PUPDR     &= ~(0x3U << (0x2U * DI1_GPIO_PIN_d));
	DI1_GPIO_PORT_dps->PUPDR     |= 0x2U << (0x2U * DI1_GPIO_PIN_d);
#endif

    /*************************************************************************************************
    * Configuring the PB14 pin as zero crossing detection circuit output -> input to the timer module.
    *************************************************************************************************/
	GPIOB->MODER                 &= ~GPIO_MODER_MODE14_Msk;                                          /* Reset mode for PB14. */
	GPIOB->MODER                 |= 2UL << GPIO_MODER_MODE14_Pos;                                    /* Set GPIO mode as Alternate function. */
	GPIOB->AFR[1]                &= ~GPIO_AFRH_AFSEL14_Msk;                                          /* Reset Alternate function on PB14. */
	GPIOB->AFR[1]                |= 1UL << GPIO_AFRH_AFSEL14_Pos;                                    /* Set Alternate function AF1 on PB14. */

    /*************************************************************************************************
    * Configuring the PB15 pin as zero crossing detection circuit enable pin.
    *************************************************************************************************/
	GPIOB->BSRR                  |= GPIO_BSRR_BR15;                                                  /* Set pin to default low. */
	GPIOB->MODER                 &= ~(GPIO_MODER_MODE15_Msk);                                        /* Reset mode for PB15. */
	GPIOB->MODER                 |= 1UL << GPIO_MODER_MODE15_Pos;                                    /* Set GPIO mode as General purpose output mode. */
	GPIOB->OTYPER                &= ~(GPIO_OTYPER_OT15);                                             /* Set output as Push-pull. */
	GPIOB->PUPDR                 &= ~(GPIO_PUPDR_PUPD15_Msk);

    /*************************************************************************************************
    * Display + touch
    *************************************************************************************************/
	MODIFY_REG(GPIOC->MODER,
	           GPIO_MODER_MODE8_Msk |
	           GPIO_MODER_MODE9_Msk |
	           GPIO_MODER_MODE10_Msk |
	           GPIO_MODER_MODE11_Msk |
	           GPIO_MODER_MODE12_Msk |
	           GPIO_MODER_MODE13_Msk |
	           GPIO_MODER_MODE14_Msk |
	           GPIO_MODER_MODE15_Msk,                                                               /* PC15 - TOUCH IRQ [External interrupt] */
	           (1UL << GPIO_MODER_MODE8_Pos) |                                                      /* PC8 - LCD DC [Output] */
	           (1UL << GPIO_MODER_MODE9_Pos) |                                                      /* PC9 - LCD RST [Output] */
	           (2UL << GPIO_MODER_MODE10_Pos) |                                                     /* PC10 - SPI3 SCK [AF] */
	           (2UL << GPIO_MODER_MODE11_Pos) |                                                     /* PC11 - SPI3 MISO [AF] */
	           (2UL << GPIO_MODER_MODE12_Pos) |                                                     /* PC12 - SPI3 MOSI [AF] */
	           (1UL << GPIO_MODER_MODE13_Pos) |                                                     /* PC13 - LCD CS [Output] */
	           (1UL << GPIO_MODER_MODE14_Pos));

	MODIFY_REG(GPIOC->AFR[1],
	           GPIO_AFRH_AFSEL10_Msk |
	           GPIO_AFRH_AFSEL11_Msk |
	           GPIO_AFRH_AFSEL12_Msk,
	           (6UL << GPIO_AFRH_AFSEL10_Pos) |                                                     /* PC10 - AF6 */
	           (6UL << GPIO_AFRH_AFSEL11_Pos) |                                                     /* PC10 - AF6 */
	           (6UL << GPIO_AFRH_AFSEL12_Pos));                                                     /* PC10 - AF6 */


	/* Touch interrupt request external line. */
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
	MODIFY_REG(SYSCFG->EXTICR[3], SYSCFG_EXTICR4_EXTI15_Msk, SYSCFG_EXTICR4_EXTI15_PC);
	SET_BIT(SYSCFG->EXTICR[3], SYSCFG_EXTICR4_EXTI15_PC);
	SET_BIT(EXTI->IMR1, EXTI_IMR1_IM15);                                                            /* Unmask interrupt from PC15. */
	SET_BIT(EXTI->FTSR1, EXTI_FTSR1_FT15);                                                          /* Set falling edge as interrupt trigger. */
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    NVIC_SetPriority(EXTI15_10_IRQn, 0);                                            /* Set highest priority to this interrupt. Rationale: Button is used also as on/off. */
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

int _write(int file, char *ptr, int len)
{
  /* Implement your write code here, this is used by puts and printf for example */
  int i=0;
  for(i=0 ; i<len ; i++)
    ITM_SendChar((*ptr++));
  return len;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	printf("Fault. Core stopped.\r\n");
  __disable_irq();

  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	printf("Assertion failed: file %s on line %ld\r\n", file, line);
	Error_Handler();
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
