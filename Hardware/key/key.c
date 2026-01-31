#include "key.h"

void Key_Scan(DS3231_AlarmTypeDef *alarm)
{
    // 当Mid按下时自动检测当前触发的闹钟，并暂停
    if (HAL_GPIO_ReadPin(KEY_Mid_GPIO_Port, KEY_Mid_Pin) == GPIO_PIN_RESET)
    {
        Alarm_Close();
    }
    // 当Rigth或Left按下时切换菜单界面
    if (HAL_GPIO_ReadPin(KEY_Left_GPIO_Port, KEY_Left_Pin) == GPIO_PIN_RESET ||
        HAL_GPIO_ReadPin(KEY_Right_GPIO_Port, KEY_Right_Pin) == GPIO_PIN_RESET)
    {
        delay_ms(50); // 消抖
        if (HAL_GPIO_ReadPin(KEY_Left_GPIO_Port, KEY_Left_Pin) == GPIO_PIN_RESET)
        {
            menu_state--;
            if (menu_state < 1)
            {
                menu_state = 2;
            }
            while (HAL_GPIO_ReadPin(KEY_Left_GPIO_Port, KEY_Left_Pin) == GPIO_PIN_RESET)
                ;
        }
        else if (HAL_GPIO_ReadPin(KEY_Right_GPIO_Port, KEY_Right_Pin) == GPIO_PIN_RESET)
        {
            menu_state++;
            if (menu_state > 2)
            {
                menu_state = 1;
            }
            while (HAL_GPIO_ReadPin(KEY_Right_GPIO_Port, KEY_Right_Pin) == GPIO_PIN_RESET)
                ;
        }
    }
    // Delay按键按下时闹钟延后
    if (HAL_GPIO_ReadPin(KEY_Delay_GPIO_Port, KEY_Delay_Pin) == GPIO_PIN_RESET)
    {
        uint8_t nmin;  // 修改后的min
        uint8_t nhour; // 修改后的hour

        delay_ms(50); // 消抖
        if (HAL_GPIO_ReadPin(KEY_Delay_GPIO_Port, KEY_Delay_Pin) == GPIO_PIN_RESET)
        {   Alarm_Close();
            DS3231_ReadAlarm1Time(alarm);
            nmin = alarm->amin + 1;
            if (nmin >= 60)
                nhour = alarm->ahour + 1;
            else
                nhour = alarm->ahour;
            DS3231_SetAlarm1(nhour % 24, nmin % 60, alarm->asec);
            while (HAL_GPIO_ReadPin(KEY_Delay_GPIO_Port, KEY_Delay_Pin) == GPIO_PIN_RESET)
                ;
        }
    }
}