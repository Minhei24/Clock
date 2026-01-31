#include "alarm.h"
// 初始化两个闹钟的状态
Alarm_State alarm1 = {0, 0, 0};
Alarm_State alarm2 = {0, 0, 0};

// 计时变量（每个闹钟独立）
uint32_t alarm1_last_time = 0;  // Alarm1上次动作时间
uint32_t alarm2_last_time = 0;  // Alarm2上次动作时间
uint32_t alarm_start_time = 0;  // 闹钟触发起始时间
uint8_t current_alarm_type = 0; // 0=无,1=Alarm1,2=Alarm2,3=两者
uint8_t has_alarm_triggered = 0; // 标记是否有闹钟在触发

void Alarm_Function()
{
    // 第一步：检测闹钟触发，记录触发类型（仅执行一次）
    if (alarm_signal)
    {
        alarm_signal = 0; // 清除触发标志，避免重复检测
        current_alarm_type = DS3231_CheckAlarms();
        // 触发Alarm1：独立置位状态，复位计时
        if (current_alarm_type & 0x01)
        {
            alarm1.trigger_flag = 1;
            alarm1.start_time = HAL_GetTick();
            alarm1.last_action_time = HAL_GetTick();
        }

        // 触发Alarm2：独立置位状态，复位计时（不影响Alarm1）
        if (current_alarm_type & 0x02)
        {
            alarm2.trigger_flag = 1;
            alarm2.start_time = HAL_GetTick();
            alarm2.last_action_time = HAL_GetTick();
        }
    }
    // 第二步：执行Alarm1逻辑（完全独立）
    if (alarm1.trigger_flag)
    {
        // 按间隔执行动作
        if (HAL_GetTick() - alarm1.last_action_time >= ALARM1_INTERVAL)
        {
            Alarm1_Play();
            alarm1.last_action_time = HAL_GetTick();
        }
        // 超时停止Alarm1（不影响Alarm2）
        if (HAL_GetTick() - alarm1.start_time > ALARM_DURATION)
        {
            alarm1.trigger_flag = 0;
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(BUZ_GPIO_Port, BUZ_Pin, GPIO_PIN_SET);
            DS3231_CloseAlarm1();
        }
    }

    // 第三步：执行Alarm2逻辑（完全独立）
    if (alarm2.trigger_flag)
    {
        // 按间隔执行动作
        if (HAL_GetTick() - alarm2.last_action_time >= ALARM2_INTERVAL)
        {
            Alarm2_Play();
            alarm2.last_action_time = HAL_GetTick();
        }
        // 超时停止Alarm2（不影响Alarm1）
        if (HAL_GetTick() - alarm2.start_time > ALARM_DURATION)
        {
            alarm2.trigger_flag = 0;
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(BUZ_GPIO_Port, BUZ_Pin, GPIO_PIN_SET);
            DS3231_CloseAlarm2();
        }
    }
}

void Alarm_Close(void)
{
    if (alarm1.trigger_flag)
    {
        alarm1.start_time = HAL_GetTick() - ALARM_DURATION - 1;
        has_alarm_triggered = 1;
    }

    if (alarm2.trigger_flag)
    {
        alarm2.start_time = HAL_GetTick() - ALARM_DURATION - 1;
        has_alarm_triggered = 1;
    }
}