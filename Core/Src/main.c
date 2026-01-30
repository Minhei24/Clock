/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "i2c.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LED.h"
#include "delay.h"
#include "display.h"
#include "ds3231.h"
#include "u8g2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
DS3231_TimeTypeDef Time;    // DS3231时间结构体实例
DS3231_AlarmTypeDef Alarm1; // DS3231闹钟结构体实例
u8g2_t u8g2;                // U8g2结构体实例
// 基础标志
#define ALARM1_INTERVAL 250   // Alarm1间隔（500ms切换一次）
#define ALARM2_INTERVAL 1000  // Alarm2间隔（1000ms切换一次）
#define ALARM_DURATION 10000  // 闹铃持续时长（10秒）

// 计时变量（每个闹钟独立）
uint32_t alarm1_last_time = 0; // Alarm1上次动作时间
uint32_t alarm2_last_time = 0; // Alarm2上次动作时间
uint32_t alarm_start_time = 0; // 闹钟触发起始时间
uint8_t current_alarm_type = 0; // 0=无,1=Alarm1,2=Alarm2,3=两者

// 【关键】每个闹钟独立状态，互不干扰
typedef struct {
    uint8_t trigger_flag;      // 1=已触发，0=未触发
    uint32_t last_action_time; // 上次动作时间
    uint32_t start_time;       // 触发起始时间
} Alarm_State;

// 初始化两个闹钟的状态
Alarm_State alarm1 = {0, 0, 0};
Alarm_State alarm2 = {0, 0, 0};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C2_Init();
    MX_I2C1_Init();
    /* USER CODE BEGIN 2 */
    Display_init(&u8g2);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    DS3231_SetTime(26, 1, 29, 4, 15, 59, 55); // 设置时间 （只需运行一次，设置完成后可注释掉）
    DS3231_SetAlarm1(16, 01, 00);             // 设置闹钟时间
    DS3231_SetAlarm2(0);                      // 设置整点报时

    while (1)
    {
        Display_ShowTime(&u8g2, &Time, &Alarm1);
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
     if (HAL_GPIO_ReadPin(KEY_Mid_GPIO_Port,KEY_Mid_Pin)==GPIO_PIN_RESET)
      {
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET); // 熄灭LED表示闹钟未触发
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BUZ_GPIO_Port, BUZ_Pin, GPIO_PIN_SET); // 关闭蜂鸣器
        
      }

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    /* USER CODE END 3 */
  }
/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
