#ifndef __KEY_H__
#define __KEY_H__

#include "display.h"
#include "main.h"
#include "ds3231.h"
#include "delay.h"
#include "alarm.h"

//按键扫描函数
void Key_Scan(DS3231_AlarmTypeDef *alarm);

#endif //__KEY_H__