/**
 * *****************************************************************************
 * @file    delay.h
 * @author  kabudac (kabudac@outlook.com)
 * @version V1.0
 * @date    2024-08-27
 * @brief   
 * *****************************************************************************
 */
#ifndef __DELAY_H__
#define __DELAY_H__

#include "main.h"

void delay_init(uint16_t sysclk);       /* 初始化延迟函数 */
void delay_ms(uint16_t nms);            /* 延时nms */
void delay_us(uint32_t nus);            /* 延时nus */


#endif



























