/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Encoding/Inc/encode.h
  * @author  MCD Application Team
  * @brief   header of encode.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ENCODE_H
#define __ENCODE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx.h"
#include "jpeglib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void jpeg_encode(uint8_t* inbuf, uint32_t insize,
                 uint8_t* outbuf, uint32_t *outsize,
                 uint32_t width, uint32_t height,
                 uint32_t image_quality,
                 uint8_t* buff);

#endif /* __ENCODE_H */

