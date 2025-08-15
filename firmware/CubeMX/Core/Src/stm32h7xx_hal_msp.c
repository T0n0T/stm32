/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32h7xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
extern DMA_HandleTypeDef hdma_dcmi;

extern DMA_HandleTypeDef hdma_spi6_tx;

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
  * @brief DCMI MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hdcmi: DCMI handle pointer
  * @retval None
  */
void HAL_DCMI_MspInit(DCMI_HandleTypeDef* hdcmi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hdcmi->Instance==DCMI)
  {
    /* USER CODE BEGIN DCMI_MspInit 0 */

    /* USER CODE END DCMI_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_DCMI_CLK_ENABLE();

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    /**DCMI GPIO Configuration
    PE4     ------> DCMI_D4
    PE5     ------> DCMI_D6
    PE6     ------> DCMI_D7
    PA4     ------> DCMI_HSYNC
    PA6     ------> DCMI_PIXCLK
    PC6     ------> DCMI_D0
    PC7     ------> DCMI_D1
    PD3     ------> DCMI_D5
    PG9     ------> DCMI_VSYNC
    PG10     ------> DCMI_D2
    PG11     ------> DCMI_D3
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* DCMI DMA Init */
    /* DCMI Init */
    hdma_dcmi.Instance = DMA1_Stream0;
    hdma_dcmi.Init.Request = DMA_REQUEST_DCMI_PSSI;
    hdma_dcmi.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_dcmi.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dcmi.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dcmi.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_dcmi.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_dcmi.Init.Mode = DMA_CIRCULAR;
    hdma_dcmi.Init.Priority = DMA_PRIORITY_LOW;
    hdma_dcmi.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_dcmi.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_dcmi.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_dcmi.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hdma_dcmi) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hdcmi,DMA_Handle,hdma_dcmi);

    /* DCMI interrupt Init */
    HAL_NVIC_SetPriority(DCMI_PSSI_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DCMI_PSSI_IRQn);
    /* USER CODE BEGIN DCMI_MspInit 1 */

    /* USER CODE END DCMI_MspInit 1 */

  }

}

/**
  * @brief DCMI MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hdcmi: DCMI handle pointer
  * @retval None
  */
void HAL_DCMI_MspDeInit(DCMI_HandleTypeDef* hdcmi)
{
  if(hdcmi->Instance==DCMI)
  {
    /* USER CODE BEGIN DCMI_MspDeInit 0 */

    /* USER CODE END DCMI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DCMI_CLK_DISABLE();

    /**DCMI GPIO Configuration
    PE4     ------> DCMI_D4
    PE5     ------> DCMI_D6
    PE6     ------> DCMI_D7
    PA4     ------> DCMI_HSYNC
    PA6     ------> DCMI_PIXCLK
    PC6     ------> DCMI_D0
    PC7     ------> DCMI_D1
    PD3     ------> DCMI_D5
    PG9     ------> DCMI_VSYNC
    PG10     ------> DCMI_D2
    PG11     ------> DCMI_D3
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_3);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11);

    /* DCMI DMA DeInit */
    HAL_DMA_DeInit(hdcmi->DMA_Handle);

    /* DCMI interrupt DeInit */
    HAL_NVIC_DisableIRQ(DCMI_PSSI_IRQn);
    /* USER CODE BEGIN DCMI_MspDeInit 1 */

    /* USER CODE END DCMI_MspDeInit 1 */
  }

}

/**
  * @brief LPTIM MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hlptim: LPTIM handle pointer
  * @retval None
  */
void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef* hlptim)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hlptim->Instance==LPTIM1)
  {
    /* USER CODE BEGIN LPTIM1_MspInit 0 */

    /* USER CODE END LPTIM1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
    PeriphClkInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_LPTIM1_CLK_ENABLE();
    /* LPTIM1 interrupt Init */
    HAL_NVIC_SetPriority(LPTIM1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(LPTIM1_IRQn);
    /* USER CODE BEGIN LPTIM1_MspInit 1 */

    /* USER CODE END LPTIM1_MspInit 1 */

  }

}

/**
  * @brief LPTIM MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hlptim: LPTIM handle pointer
  * @retval None
  */
void HAL_LPTIM_MspDeInit(LPTIM_HandleTypeDef* hlptim)
{
  if(hlptim->Instance==LPTIM1)
  {
    /* USER CODE BEGIN LPTIM1_MspDeInit 0 */

    /* USER CODE END LPTIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LPTIM1_CLK_DISABLE();

    /* LPTIM1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(LPTIM1_IRQn);
    /* USER CODE BEGIN LPTIM1_MspDeInit 1 */

    /* USER CODE END LPTIM1_MspDeInit 1 */
  }

}

/**
  * @brief SPI MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hspi->Instance==SPI6)
  {
    /* USER CODE BEGIN SPI6_MspInit 0 */

    /* USER CODE END SPI6_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI6;
    PeriphClkInitStruct.Spi6ClockSelection = RCC_SPI6CLKSOURCE_D3PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_SPI6_CLK_ENABLE();

    __HAL_RCC_GPIOG_CLK_ENABLE();
    /**SPI6 GPIO Configuration
    PG8     ------> SPI6_NSS
    PG13     ------> SPI6_SCK
    PG14     ------> SPI6_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_13|GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI6;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* SPI6 DMA Init */
    /* SPI6_TX Init */
    hdma_spi6_tx.Instance = BDMA_Channel0;
    hdma_spi6_tx.Init.Request = BDMA_REQUEST_SPI6_TX;
    hdma_spi6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi6_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi6_tx.Init.Mode = DMA_NORMAL;
    hdma_spi6_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_spi6_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hspi,hdmatx,hdma_spi6_tx);

    /* SPI6 interrupt Init */
    HAL_NVIC_SetPriority(SPI6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI6_IRQn);
    /* USER CODE BEGIN SPI6_MspInit 1 */

    /* USER CODE END SPI6_MspInit 1 */

  }

}

/**
  * @brief SPI MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
  if(hspi->Instance==SPI6)
  {
    /* USER CODE BEGIN SPI6_MspDeInit 0 */

    /* USER CODE END SPI6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI6_CLK_DISABLE();

    /**SPI6 GPIO Configuration
    PG8     ------> SPI6_NSS
    PG13     ------> SPI6_SCK
    PG14     ------> SPI6_MOSI
    */
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_8|GPIO_PIN_13|GPIO_PIN_14);

    /* SPI6 DMA DeInit */
    HAL_DMA_DeInit(hspi->hdmatx);

    /* SPI6 interrupt DeInit */
    HAL_NVIC_DisableIRQ(SPI6_IRQn);
    /* USER CODE BEGIN SPI6_MspDeInit 1 */

    /* USER CODE END SPI6_MspDeInit 1 */
  }

}

/**
  * @brief UART MSP Initialization
  * This function configures the hardware resources used in this example
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(huart->Instance==USART1)
  {
    /* USER CODE BEGIN USART1_MspInit 0 */

    /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16910CLKSOURCE_D2PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN USART1_MspInit 1 */

    /* USER CODE END USART1_MspInit 1 */

  }

}

/**
  * @brief UART MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART1)
  {
    /* USER CODE BEGIN USART1_MspDeInit 0 */

    /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USER CODE BEGIN USART1_MspDeInit 1 */

    /* USER CODE END USART1_MspDeInit 1 */
  }

}

/**
  * @brief PCD MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hpcd: PCD handle pointer
  * @retval None
  */
void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hpcd->Instance==USB_OTG_HS)
  {
    /* USER CODE BEGIN USB_OTG_HS_MspInit 0 */

    /* USER CODE END USB_OTG_HS_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.PLL3.PLL3M = 5;
    PeriphClkInitStruct.PLL3.PLL3N = 48;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 5;
    PeriphClkInitStruct.PLL3.PLL3R = 2;
    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

  /** Enable USB Voltage detector
  */
    HAL_PWREx_EnableUSBVoltageDetector();

    /* Peripheral clock enable */
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    /* USER CODE BEGIN USB_OTG_HS_MspInit 1 */

    /* USER CODE END USB_OTG_HS_MspInit 1 */

  }

}

/**
  * @brief PCD MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hpcd: PCD handle pointer
  * @retval None
  */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)
{
  if(hpcd->Instance==USB_OTG_HS)
  {
    /* USER CODE BEGIN USB_OTG_HS_MspDeInit 0 */

    /* USER CODE END USB_OTG_HS_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
    /* USER CODE BEGIN USB_OTG_HS_MspDeInit 1 */

    /* USER CODE END USB_OTG_HS_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
