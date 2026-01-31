#include "LED.h"
#include "main.h"
void Alarm1_Play(void)
{
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin); // 切换LED状态
    HAL_GPIO_TogglePin(BUZ_GPIO_Port, BUZ_Pin); // 切换蜂鸣器状态
 
}

void Alarm2_Play(void)
{
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin); // 切换LED状态
    HAL_GPIO_TogglePin(BUZ_GPIO_Port, BUZ_Pin); // 切换蜂鸣器状态
}
