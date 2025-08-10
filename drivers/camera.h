#ifndef CAMERA_H
#define CAMERA_H

#include "i2c_soft.h"
#include "board.h"
#include <stdint.h>

void camera_init(void);
void camera_start(void);
void camera_stop(void);


uint16_t camera_read_id(void);

//曝光时间
void camera_set_exposure_time(uint16_t time);
//增益
void camera_set_gain(uint16_t gain);
//图像格式
void camera_set_format(uint8_t format);
//白平衡
void camera_set_white_balance(uint8_t mode);
//镜像
void camera_set_mirror(uint8_t mode);
//翻转
void camera_set_flip(uint8_t mode);
//饱和度
void camera_set_saturation(uint8_t level);
//对比度
void camera_set_contrast(uint8_t level);
//锐度
void camera_set_sharpness(uint8_t level);
//帧率
void camera_set_frame_rate(uint8_t rate);
//分辨率
void camera_set_resolution(uint8_t resolution);


#endif /* CAMERA_H */