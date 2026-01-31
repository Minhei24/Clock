#ifndef __ALARM_H__
#define __ALARM_H__
#include "stdint.h"
#include "ds3231.h"
#include "LED.h"

// 基础标志
#define ALARM1_INTERVAL 250  // Alarm1间隔（500ms切换一次）
#define ALARM2_INTERVAL 1000 // Alarm2间隔（1000ms切换一次）
#define ALARM_DURATION 10000 // 闹铃持续时长（10秒）


// 【关键】每个闹钟独立状态，互不干扰
typedef struct
{
    uint8_t trigger_flag;      // 1=已触发，0=未触发
    uint32_t last_action_time; // 上次动作时间
    uint32_t start_time;       // 触发起始时间
} Alarm_State;



void Alarm_Function(void);
void Alarm_Close(void);
#endif //__ALARM_H__