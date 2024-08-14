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
#define  I2C1_SCL_GPIO_PIN        GPIO_PIN_6    //如果改动口线则GPIO初始化代码需要做同步修改
#define  I2C1_SDA_GPIO_PORT       CW_GPIOB
#define  I2C1_SDA_GPIO_PIN        GPIO_PIN_7    //如果改动口线则GPIO初始化代码需要做同步修改

//EEPROM内部地址
uint8_t u8Addr = 0x00;        //地址字节
#define WRITELEN   8          //写数据长度
#define READLEN   8           //读数据长度
#define WriteReadCycle  35    //写读次数,每次写入数据为n+i(n为次数，i=0~7)

uint8_t u8Senddata[8] = {0x66,0x02,0x03,0x04,0x05,0x60,0x70,0x20};
uint8_t u8Senddata2[8] = {0x55,0xAA,0xAA,0x55,0x55,0xAA,0x55,0xAA};
uint8_t u8Recdata[16]= {0x00};
uint8_t u8SendLen=0;
uint8_t u8RecvLen=0;
uint8_t SendFlg = 0,Comm_flg = 0;
uint8_t u8recvflg=0;
uint8_t u8State = 0;

uint8_t receivedflag = 0;    //读取完成标志


/**
 * @brief  I2C1 EEPROM读写中断函数
 *
 */
void I2c1EepromReadWriteInterruptFunction(void)
{
    u8State = I2C_GetState(CW_I2C1);
    switch(u8State)
    {
        case 0x08:     //发送完START信号
            I2C_GenerateSTART(CW_I2C1, DISABLE);
            I2C_Send7bitAddress(CW_I2C1, I2C_SLAVEADDRESS,0X00);
            break;
        case 0x10:     //发送完重复起始信号
            I2C_GenerateSTART(CW_I2C1, DISABLE);
            if(0 == SendFlg)
            {
                I2C_Send7bitAddress(CW_I2C1, I2C_SLAVEADDRESS,0X00);    //写命令
            }
            else
            {
                I2C_Send7bitAddress(CW_I2C1, I2C_SLAVEADDRESS,0X01);    //读命令,eeprom 随机读
            }
            break;
        case 0x18:    //发送完SLA+W/R字节
            I2C_GenerateSTART(CW_I2C1, DISABLE);
            I2C_SendData(CW_I2C1, u8Addr);   //发送访问EEPROM的目标地址字节
            break;
        case 0x20:    //发送完SLA+W后从机返回NACK
        case 0x38:    //主机在发送 SLA+W 阶段或者发送数据阶段丢失仲裁  或者  主机在发送 SLA+R 阶段或者回应 NACK 阶段丢失仲裁
        case 0x30:    //发送完一个数据字节后从机返回NACK
        case 0x48:    //发送完SLA+R后从机返回NACK
            I2C_GenerateSTOP(CW_I2C1, ENABLE);
            I2C_GenerateSTART(CW_I2C1, ENABLE);
            break;
        case 0x58:    //接收到一个数据字节，且NACK已回复
            u8Recdata[u8RecvLen++] = I2C_ReceiveData(CW_I2C1);//所有数据读取完成，NACK已发送
            receivedflag =1;
            I2C_GenerateSTOP(CW_I2C1, ENABLE);//发送停止条件
            break;
        case 0x28:     //发送完1字节数据：发送EEPROM中memory地址也会产生，发送后面的数据也会产生
            if(0 == SendFlg)
            {
                if(u8SendLen <WRITELEN)
                {
                    I2C_SendData(CW_I2C1,u8Senddata[u8SendLen++]);
                }
                else
                {
                    u8SendLen = 0;
                    Comm_flg = 1;
                    SendFlg = 1;
                    I2C_GenerateSTOP(CW_I2C1, ENABLE);//发送完数据，发送停止信号
                }
            }
            else       //SendFlg=1为读，SendFlg=0为写。读数据发送完地址字节后，重复起始条件
            {
                CW_I2C1->CR_f.STA = 1;  //set start       //发送重复START信号,START生成函数改写后，会导致0X10状态被略过，故此处不调用函数
                I2C_GenerateSTOP(CW_I2C1, DISABLE);
            }
            break;

        case 0x40:     //发送完SLA+R信号，开始接收数据
            u8RecvLen = 0;
            if(READLEN>1)
            {
                I2C_AcknowledgeConfig(CW_I2C1,ENABLE);//读取数据超过1个字节才发送ACK
            }
            break;
        case 0x50:     //接收完一字节数据，在接收最后1字节数据之前设置AA=0;
            u8Recdata[u8RecvLen++] = I2C_ReceiveData(CW_I2C1);
            if(u8RecvLen==READLEN-1)
            {
                I2C_AcknowledgeConfig(CW_I2C1,DISABLE);;
            }
            break;
    }
    I2C_ClearIrq(CW_I2C1);
}

int32_t main(void)
{
    I2C_InitTypeDef I2C_InitStruct = {0};

    uint16_t tempcnt = 0 ;

    //时钟初始化
    RCC_Configuration();

    //IO口初始化
    GPIO_Configuration();

    //I2C初始化
    I2C_InitStruct.I2C_Baud = 0x01;//500K=(8000000/(8*(1+1))
    I2C_InitStruct.I2C_BaudEn = ENABLE;
    I2C_InitStruct.I2C_FLT = DISABLE;
    I2C_InitStruct.I2C_AA =  DISABLE;

    I2C1_DeInit();
    I2C_Master_Init(CW_I2C1,&I2C_InitStruct);//初始化模块


    //中断设置
    NVIC_Configuration();

    //I2C模块开始工作

    I2C_Cmd(CW_I2C1,ENABLE);  //模块使能


    tempcnt =0;
    for(uint8_t i=0; i<8; i++)
    {
        u8Senddata[i] = i;
    }

    while(1)
    {
        I2C_GenerateSTART(CW_I2C1, ENABLE); //开始信号
        
        while (1)
        {
            //等待数据发送完成
            while(!Comm_flg) ;

            FirmwareDelay(3000);

            //启动读数据过程
            Comm_flg = 0;
            receivedflag=0;
            I2C_GenerateSTART(CW_I2C1, ENABLE); //开始信号

            //等待数据读取完成
            while(!receivedflag) ;

            //初始化下一次写数据
            receivedflag = 0;
            SendFlg = 0;
            u8RecvLen = 0;
            tempcnt++;
            for(uint8_t i=0; i<8; i++)
            {
                u8Senddata[i] =tempcnt+i;
            }
            break;
        }

        //测试次数完成，退出
        if(tempcnt >=WriteReadCycle)
        {
            break;
        }

    }

    while(1);

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

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(I2C1_IRQn);
    __enable_irq();
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

