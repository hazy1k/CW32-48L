/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2022-7-12
 *
 * @copyright Copyright (c) 2021
 *
 */
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
#include "..\inc\main.h"

void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

#define  I2C1_SCL_GPIO_PORT       CW_GPIOB
#define  I2C1_SCL_GPIO_PIN        GPIO_PIN_6      //如果改动口线则GPIO初始化代码需要做同步修改
#define  I2C1_SDA_GPIO_PORT       CW_GPIOB
#define  I2C1_SDA_GPIO_PIN        GPIO_PIN_7      //如果改动口线则GPIO初始化代码需要做同步修改

uint8_t u8Recdata[30]= {0x00};    //存放接收到的数据
uint32_t u32SendLen=0;            //存放接收数据的长度
uint32_t u32RecvLen=0;            //存放发送数据的长度
uint8_t u8State = 0;              //I2C模块状态

/**
 * @brief
 *
 * @return int32_t
 */
int32_t main(void)
{
    I2C_InitTypeDef I2C_InitStruct = {0};

    //时钟初始化
    RCC_Configuration();

    //IO口初始化
    GPIO_Configuration();

    //I2C初始化
    I2C_InitStruct.I2C_OwnSlaveAddr0 = OwnSlaveAddress0;  //TEST ADDRESS0
    I2C_InitStruct.I2C_OwnSlaveAddr1 = OwnSlaveAddress1;  //TEST ADDRESS1
    I2C_InitStruct.I2C_OwnSlaveAddr2 = OwnSlaveAddress2;  //TEST ADDRESS2

    I2C_InitStruct.I2C_OwnGc = DISABLE;
    I2C_InitStruct.I2C_FLT = DISABLE;
    I2C_InitStruct.I2C_AA = DISABLE;

    I2C1_DeInit();
    I2C_Slave_Init(CW_I2C1,&I2C_InitStruct);//初始化模块
    I2C_Cmd(CW_I2C1, ENABLE);//模块使能
    I2C_AcknowledgeConfig(CW_I2C1,ENABLE);//ACK打开


    while(1)
    {
        I2C_SlaveRecDataFromMaster(CW_I2C1,u8Recdata,&u32RecvLen);//等主机写数据
        I2C_SlaveSendDataToMaster(CW_I2C1,u8Recdata,&u32SendLen);//等主机读数据
    }
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    CW_SYSCTRL->APBEN1_f.I2C1 = 1U;    //
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    CW_SYSCTRL->AHBEN_f.GPIOA  = 1;
    CW_SYSCTRL->AHBEN_f.GPIOB  = 1;
    CW_SYSCTRL->AHBEN_f.GPIOC  = 1;    
    CW_SYSCTRL->AHBEN_f.GPIOF  = 1;

    PB06_AFx_I2C1SCL();
    PB07_AFx_I2C1SDA();
    GPIO_InitStructure.Pins = I2C1_SCL_GPIO_PIN | I2C1_SDA_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init(I2C1_SCL_GPIO_PORT, &GPIO_InitStructure);
}



/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
 /**
   * @brief  Reports the name of the source file and the source line number
   *         where the assert_param error has occurred.
   * @param  file: pointer to the source file name
   * @param  line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
       /* USER CODE END 6 */
}
#endif

