#include "../Re_keypadDrv.h"
#include "Re_all_config.h"
#include "drv\Re_nativeInterface.h"
#if mcu_stm32f1_hal
#if service_input_keypad
#define ROW0_Pin GPIO_PIN_8
#define ROW0_GPIO_Port GPIOB

#define ROW1_Pin GPIO_PIN_9
#define ROW1_GPIO_Port GPIOB

#define ROW2_Pin GPIO_PIN_10
#define ROW2_GPIO_Port GPIOB

#define ROW3_Pin GPIO_PIN_11
#define ROW3_GPIO_Port GPIOB

#define COL0_Pin GPIO_PIN_12
#define COL0_GPIO_Port GPIOB

#define COL1_Pin GPIO_PIN_13
#define COL1_GPIO_Port GPIOB

#define COL2_Pin GPIO_PIN_14
#define COL2_GPIO_Port GPIOB

#define COL3_Pin GPIO_PIN_15
#define COL3_GPIO_Port GPIOB

void keypad_init_hardware(Pin_input_mode index)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = ROW0_Pin | ROW1_Pin | ROW2_Pin | ROW3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    if (index == Pullup)
    {
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    }
    else
    {
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    }
    HAL_GPIO_Init(ROW0_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = COL0_Pin | COL1_Pin | COL2_Pin | COL3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(COL0_GPIO_Port, &GPIO_InitStruct);
}

char keypad_readrow(char index)
{
    switch (index)
    {
    case 0:
        return HAL_GPIO_ReadPin(ROW0_GPIO_Port, ROW0_Pin);
        break;
    case 1:
        return HAL_GPIO_ReadPin(ROW1_GPIO_Port, ROW1_Pin);
        break;
    case 2:
        return HAL_GPIO_ReadPin(ROW2_GPIO_Port, ROW2_Pin);
        break;
    case 3:
        return HAL_GPIO_ReadPin(ROW3_GPIO_Port, ROW3_Pin);
        break;
    }
}
void keypad_writecol(char index, char state)
{
    switch (index)
    {
    case 0:
        HAL_GPIO_WritePin(COL0_GPIO_Port,COL0_Pin,state);
        break;
    case 1:
        HAL_GPIO_WritePin(COL1_GPIO_Port,COL1_Pin,state);
        break;
    case 2:
        HAL_GPIO_WritePin(COL2_GPIO_Port,COL2_Pin,state);
        break;
    case 3:
        HAL_GPIO_WritePin(COL3_GPIO_Port,COL3_Pin,state);
        break;
    }
}
#endif
#endif
