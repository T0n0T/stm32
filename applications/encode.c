/**
 ******************************************************************************
 * @file    LibJPEG/LibJPEG_Encoding/Src/encode.c
 * @author  MCD Application Team
 * @brief   This file contain the compress method.
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

/* Includes ------------------------------------------------------------------*/
#include "encode.h"

/* Private typedef -----------------------------------------------------------*/
/* This struct contains the JPEG compression parameters */
static struct jpeg_compress_struct cinfo;
/* This struct represents a JPEG error handler */
static struct jpeg_error_mgr jerr;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Jpeg Encode
  * @param  inbuf:         pointer to the input buffer
  * @param  insize:        input buffer size
  * @param  outbuf:        pointer to the output buffer
  * @param  outsize:       pointer to the output buffer size
  * @param  width:         image width
  * @param  height:        image height
  * @param  image_quality: image quality
  * @param  buff:          pointer to the temporary buffer for line processing ( > width * 3)
  * @retval None
  */
void jpeg_encode(uint8_t* inbuf, uint32_t insize,
                 uint8_t* outbuf, uint32_t* outsize,
                 uint32_t width, uint32_t height,
                 uint32_t image_quality,
                 uint8_t* buff)
{
    /* Encode BMP Image to JPEG */
    JSAMPROW row_pointer; /* Pointer to a single row */
    uint32_t index;

    /* Step 1: allocate and initialize JPEG compression object */
    /* Set up the error handler */
    cinfo.err = jpeg_std_error(&jerr);

    /* Initialize the JPEG compression object */
    jpeg_create_compress(&cinfo);

    /* Step 2: specify data destination */
    jpeg_mem_dest(&cinfo, &outbuf, (unsigned long*)outsize);

    /* Step 3: set parameters for compression */
    cinfo.image_width      = width;
    cinfo.image_height     = height;
    cinfo.input_components = 3;
    cinfo.in_color_space   = JCS_RGB;

    /* Set default compression parameters */
    jpeg_set_defaults(&cinfo);

    cinfo.dct_method = JDCT_FLOAT;

    jpeg_set_quality(&cinfo, image_quality, TRUE);

    /* Step 4: start compressor */
    jpeg_start_compress(&cinfo, TRUE);

    /* Process image data */
    /* Convert RGB565 to RGB888 and compress line by line */
    /* JPEG library expects RGB888 format (3 bytes per pixel) */
    /* RGB565 format: 5 bits red, 6 bits green, 5 bits blue (2 bytes per pixel) */
    for (index = 0; index < height; index++) {
        /* For JPEG, we process from top to bottom */
        uint8_t *rgb888_line = buff;  /* Use buff as temporary buffer for RGB888 data */
        uint16_t *rgb565_line = (uint16_t *)(inbuf + (index * width * 2));  /* 2 bytes per pixel */
        
        /* Convert RGB565 to RGB888 */
        for (uint32_t i = 0; i < width; i++) {
            uint16_t rgb565_pixel = rgb565_line[i];
            /* Extract RGB565 components */
            uint8_t r = (rgb565_pixel >> 11) & 0x1F;
            uint8_t g = (rgb565_pixel >> 5) & 0x3F;
            uint8_t b = rgb565_pixel & 0x1F;
            
            /* Convert 5-bit/6-bit to 8-bit */
            rgb888_line[i * 3] = (r << 3) | (r >> 2);     /* Red */
            rgb888_line[i * 3 + 1] = (g << 2) | (g >> 4); /* Green */
            rgb888_line[i * 3 + 2] = (b << 3) | (b >> 2); /* Blue */
        }
        
        row_pointer = (JSAMPROW)rgb888_line;
        jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    }
    /* Step 5: finish compression */
    jpeg_finish_compress(&cinfo);

    /* Step 6: release JPEG compression object */
    jpeg_destroy_compress(&cinfo);
}
