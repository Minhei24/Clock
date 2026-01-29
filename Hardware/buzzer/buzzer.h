#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "main.h"

// 完整音阶频率定义
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_0   0

// 导出的函数接口
void Buzzer_Set_Pitch(uint16_t pitch);
void Buzzer_Play_Star(void); // 明确命名为“完整版”

#endif