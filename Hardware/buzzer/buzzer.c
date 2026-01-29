#include "buzzer.h"
#include "stdint.h"
#include "delay.h"
extern TIM_HandleTypeDef htim2; // 引用你在 CubeMX 中配置的 TIM2

// 完整版乐谱：42 个音符，一拍不落
static uint16_t star_melody_full[] = {
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, // 一闪一闪亮晶晶
    NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4, // 满天都是小星星
    NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, // 挂在天空放光明
    NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, // 好像许多小眼睛
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, // 一闪一闪亮晶晶
    NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4  // 满天都是小星星
};

void Buzzer_Set_Pitch(uint16_t pitch) {
    if (pitch == 0) {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0); 
    } else {
        uint16_t new_arr = 1000000 / pitch - 1; // 频率换算公式
        __HAL_TIM_SET_AUTORELOAD(&htim2, new_arr);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, new_arr / 2);
    }
}

void Buzzer_Play_Star(void) {
    int length = sizeof(star_melody_full) / sizeof(star_melody_full[0]);
    
    for (int i = 0; i < length; i++) {
        Buzzer_Set_Pitch(star_melody_full[i]);
        delay_ms(400);
        Buzzer_Set_Pitch(0); 
        delay_ms(50);    // 必要的停顿感
    }
}
