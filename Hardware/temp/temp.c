#include "temp.h"
#include <math.h> // 必须包含数学库

extern ADC_HandleTypeDef hadc1; // 声明 CubeMX 生成的 ADC 句柄

// 根据你买的传感器参数修改（通常规格书会写）
#define B_VALUE 3950.0f      // B 常数
#define R25     010000.f     // 25℃ 时的电阻值 (10K)
#define T25     298.15f      // 25℃ 对应的开氏温度 (273.15 + 25)
#define R_FIXED 10000.0f     // 模块内部的分压电阻 (通常也是 10K)

float Get_Temperature(void) {
    uint32_t adc_val = 0;
    float Rt, temp_k, temp_c;

    // 1. 启动 ADC 并读取原始值
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        adc_val = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    // 2. 计算热敏电阻阻值 (假设 NTC 在分压电路下桥臂)
    if (adc_val == 0 || adc_val >= 4095) return 0; 
    Rt = R_FIXED * ( (float)adc_val / (4095.0f - (float)adc_val) );

    // 3. B 值公式换算开氏温度
    temp_k = 1.0f / (1.0f / T25 + log(Rt / R25) / B_VALUE);
    
    // 4. 换算成摄氏度
    temp_c = temp_k - 273.15f;

    return temp_c;
}