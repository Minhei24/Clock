#include "main.h"
#include "breathe.h"
#include "tim.h"

void PWM_LED_On()
{
    static uint8_t flag = 1;//CCR值变化趋势 1增加 0减少
    int pulse = __HAL_TIM_GET_COMPARE(&htim2,TIM_CHANNEL_1);
    if (pulse >= __HAL_TIM_GetAutoreload(&htim2))
    {
        flag = 0;//CCR值大于ARR时，CCR递减
    }
    if (pulse <= 0)
    {
        flag = 1;//CCR值等于0时，CCR递增
    }
    pulse = flag ? pulse +1 :pulse -1;
    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,pulse);//更新CCR值
    HAL_Delay(1);
}


void PWM_LED_Off()
{
    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,__HAL_TIM_GetAutoreload(&htim2));
}