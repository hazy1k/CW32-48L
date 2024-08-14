/*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/
#include "cw32l031cxtx_startkit.h"

KEY_ClassTypeDef KEY1, KEY2;
LED_InitTypeDef LED1 = { 0 }, LED2 = { 0 };

static FlagStatus KeyRead(void *self)
{
    KEY_ClassTypeDef *_self = {0};
    _self = (KEY_ClassTypeDef *)self;
    
    return (_self->Port->IDR & ~_self->Pin) ? SET : RESET;
}

static void KeyOpen(KEY_ClassTypeDef *Key)
{
    KEY_ClassTypeDef *self = {0};
    self = Key;
    switch ((uint32_t)self->Port)
    {
        case (uint32_t)CW_GPIOA:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOA_Msk;
            break;
        case (uint32_t)CW_GPIOB:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOB_Msk;
            break;
        case (uint32_t)CW_GPIOC:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOC_Msk;
            break;
        case (uint32_t)CW_GPIOF:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOF_Msk;
            break;
        default:
            break;
    }
    self->Port->ANALOG &= ~(self->Pin);
    self->Port->DIR |= self->Pin;

    self->Read = KeyRead;    
}



static void LED_On(void* LED)
{
    LED_InitTypeDef *_LED = {0};
    _LED = (LED_InitTypeDef *)LED;

    _LED->LED_Port->BSRR = _LED->LED_Pin;
}

static void LED_Off(void *LED)
{
    LED_InitTypeDef *_LED = {0};
    _LED = (LED_InitTypeDef *)LED;
    _LED->LED_Port->BRR = _LED->LED_Pin;
}

static void LED_Toggle(void *LED)
{
    LED_InitTypeDef *_LED = {0};
    _LED = (LED_InitTypeDef *)LED;
    _LED->LED_Port->TOG = _LED->LED_Pin;
}

static void LED_Open(LED_InitTypeDef* LED)
{
    LED_InitTypeDef *_LED = {0};
    _LED = (LED_InitTypeDef *)LED;

    switch ((uint32_t)_LED->LED_Port)
    {
        case (uint32_t)CW_GPIOA:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOA_Msk;
            break;
        case (uint32_t)CW_GPIOB:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOB_Msk;
            break;
        case (uint32_t)CW_GPIOC:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOC_Msk;
            break;
        case (uint32_t)CW_GPIOF:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOF_Msk;
            break;
        default:
            break;
    }

    _LED->LED_Port->ANALOG &= ~(_LED->LED_Pin);
    _LED->LED_Port->DIR &= ~(_LED->LED_Pin);

    _LED->LED_Off = LED_Off;
    _LED->LED_On = LED_On;
    _LED->LED_Toggle = LED_Toggle;
}

void LED_Init(void)
{   

    /* LED 初始化 */
    LED1.LED_Port = LED1_PORT;
    LED1.LED_Pin = LED1_PIN;
    LED_Open(&LED1);    

    LED2.LED_Port = LED2_PORT;
    LED2.LED_Pin = LED2_PIN;
    LED_Open(&LED2);   
}


void KEY_Init(void)
{
    KEY1.Pin = KEY1_PIN;
    KEY1.Port = KEY1_PORT;
    
    KeyOpen(&KEY1);
    
    KEY2.Pin = KEY2_PIN;
    KEY2.Port  = KEY2_PORT;
    KeyOpen(&KEY2);    
}
