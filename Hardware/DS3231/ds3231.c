#include "ds3231.h"
#include "oled.h"
// BCD码转十进制
uint8_t BCD2DEC(uint8_t bcd) 
{
    return ((bcd / 16) * 10) + (bcd % 16);
}

// 十进制转BCD码
uint8_t DEC2BCD(uint8_t dec) 
{
    return ((dec / 10) << 4) + (dec % 10);
}

// 设置DS3231时间
HAL_StatusTypeDef DS3231_SetTime(uint8_t year, uint8_t month, uint8_t day, uint8_t week, uint8_t hour, uint8_t min, uint8_t sec) 
{
    uint8_t buf[7];
    // 按寄存器顺序填充：秒、分、时、星期、日、月、年
    buf[0] = DEC2BCD(sec);
    buf[1] = DEC2BCD(min);
    buf[2] = DEC2BCD(hour);
    buf[3] = DEC2BCD(week);
    buf[4] = DEC2BCD(day);
    buf[5] = DEC2BCD(month);
    buf[6] = DEC2BCD(year);
    
    // 从秒寄存器开始，连续写入7个字节
    return HAL_I2C_Mem_Write(&hi2c2, DS3231_ADDR, DS3231_SEC, I2C_MEMADD_SIZE_8BIT, buf, 7, 500);
}

// 读取DS3231时间
HAL_StatusTypeDef DS3231_GetTime(DS3231_TimeTypeDef *time) 
{
    uint8_t buf[7];
    // 从秒寄存器开始，连续读取7个字节
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(&hi2c2, DS3231_ADDR, DS3231_SEC, I2C_MEMADD_SIZE_8BIT, buf, 7, 500);
    if (ret == HAL_OK) {
        // BCD转十进制，赋值给时间结构体
        time->sec = BCD2DEC(buf[0]);
        time->min = BCD2DEC(buf[1]);
        time->hour = BCD2DEC(buf[2]);
        time->week = BCD2DEC(buf[3]);
        time->day = BCD2DEC(buf[4]);
        time->month = BCD2DEC(buf[5]);
        time->year = BCD2DEC(buf[6]);
    }
    
    return ret;
}

// 设置DS3231Alarm1（每天在指定时间触发）
void DS3231_SetAlarm1(uint8_t hour, uint8_t min, uint8_t sec) {
    uint8_t buf[4];
    // 配置Alarm1的秒/分/时/日（bit7=0：参与比较；日bit7=0：按日期比较）
    buf[0] = DEC2BCD(sec) & 0x7F;    // 秒（00），bit7=0
    buf[1] = DEC2BCD(min) & 0x7F;    // 分（00），bit7=0
    buf[2] = DEC2BCD(hour) & 0x3F;   // 时（08），bit7=0（24小时制）
    buf[3] = 0x80;                   // 日：bit7=1（忽略日，每天触发）
    
    // 写入 Alarm1 寄存器（0x07..0x0A）
    HAL_I2C_Mem_Write(&hi2c2, DS3231_ADDR, ALARM1_SEC,
                            I2C_MEMADD_SIZE_8BIT, buf, sizeof(buf), 1000);

    // 读 STATUS 寄存器并清除 A1F（bit0），以避免中断被立即置低
    uint8_t status;
    HAL_I2C_Mem_Read(&hi2c2, DS3231_ADDR, STATUS_REG,
                           I2C_MEMADD_SIZE_8BIT, &status, 1, 500);

    status &= ~(1 << 0); // 清 A1F (bit0) 为 0
    HAL_I2C_Mem_Write(&hi2c2, DS3231_ADDR, STATUS_REG,
                            I2C_MEMADD_SIZE_8BIT, &status, 1, 500);

    // 读 Control 寄存器，设置 INTCN (bit2) 和 A1IE (bit0)，保持其他位不变
    uint8_t ctrl;
    HAL_I2C_Mem_Read(&hi2c2, DS3231_ADDR, CONTROL_REG,
                           I2C_MEMADD_SIZE_8BIT, &ctrl, 1, 500);
    ctrl |= (1 << 2); // INTCN = 1 (使能中断输出，禁用方波)
    ctrl |= (1 << 0); // A1IE  = 1 (启用 Alarm1 中断)
    HAL_I2C_Mem_Write(&hi2c2, DS3231_ADDR, CONTROL_REG,
                            I2C_MEMADD_SIZE_8BIT, &ctrl, 1, 500);
}

// 禁用DS3231Alarm1
void DS3231_DisableAlarm1(void) 
{
    uint8_t reg;

    // 读取当前控制寄存器值，仅禁用Alarm1中断使能（bit0）
    HAL_I2C_Mem_Read(&hi2c2, DS3231_ADDR, CONTROL_REG,
                           I2C_MEMADD_SIZE_8BIT, &reg, 1, 500);
    reg &= ~0x01;  // 只把bit0(A1IE)置0，其他位保持不变
    HAL_I2C_Mem_Write(&hi2c2, DS3231_ADDR, CONTROL_REG, 
                           I2C_MEMADD_SIZE_8BIT, &reg, 1, 500);

    // 清除Alarm1的触发标志（bit0）
    HAL_I2C_Mem_Read(&hi2c2, DS3231_ADDR, STATUS_REG,
                           I2C_MEMADD_SIZE_8BIT, &reg, 1, 500);
    reg &= ~0x01;  // 只清空A1F位，保留其他状态
    HAL_I2C_Mem_Write(&hi2c2, DS3231_ADDR, STATUS_REG,
                           I2C_MEMADD_SIZE_8BIT, &reg, 1, 500);

}

// 仅关闭本次Alarm1（下次仍会触发）
void DS3231_CloseAlarm(void) 
{
    uint8_t status;

    // 读取状态寄存器
    HAL_I2C_Mem_Read(&hi2c2, DS3231_ADDR, STATUS_REG,
                            I2C_MEMADD_SIZE_8BIT, &status, 1, 500);
    
    // 仅清除Alarm1触发标志（A1F，bit0），其他位保持不变
    status &= ~(1 << 0); // 清 A1F (bit0) 为 0

    // 写回状态寄存器，关闭本次中断
    HAL_I2C_Mem_Write(&hi2c2, DS3231_ADDR, STATUS_REG,
                            I2C_MEMADD_SIZE_8BIT, &status, 1, 500);
    
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET); // 熄灭LED表示闹钟关闭

}

// 外部中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == Alarm_NVIC_Pin)
    {
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET); // 点亮LED表示闹钟触发
    }
}