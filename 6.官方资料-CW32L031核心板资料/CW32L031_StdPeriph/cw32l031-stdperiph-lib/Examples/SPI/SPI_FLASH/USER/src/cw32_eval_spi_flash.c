/**
 * @file cw32_eval_spi_flash.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2022-7-12
 *
 * @copyright Copyright (c) 2022
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

/* Includes ------------------------------------------------------------------*/
#include "cw32_eval_spi_flash.h"

/* Private_TypesDefinitions --------------------------------------------------*/

/* Private_Defines -----------------------------------------------------------*/

/* Private_Variables ---------------------------------------------------------*/

/* Private_FunctionPrototypes ------------------------------------------------*/

/* Private_Functions ---------------------------------------------------------*/

/* Public_Functions ----------------------------------------------------------*/

/**
 * @brief 配置SPI FLASH
 *
 */
void SPI_FLASH_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /************************GPIO Configuration***********************/
    RCC_AHBPeriphClk_Enable(FLASH_SPI_SCK_GPIO_CLK | FLASH_SPI_MISO_GPIO_CLK | FLASH_SPI_MOSI_GPIO_CLK | FLASH_SPI_CS_GPIO_CLK, ENABLE);
    FLASH_SPI_APBClkENx(FLASH_SPI_CLK, ENABLE);

    //SPI SCK MOSI MISO 复用
    FLASH_SPI_AF_SCK;
    FLASH_SPI_AF_MISO;
    FLASH_SPI_AF_MOSI;

    //CS
    GPIO_InitStructure.Pins = FLASH_SPI_CS_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(FLASH_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

    //SCK
    GPIO_InitStructure.Pins = FLASH_SPI_SCK_GPIO_PIN;
    GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    //MOSI
    GPIO_InitStructure.Pins = FLASH_SPI_MOSI_GPIO_PIN;
    GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    //MISO
    GPIO_InitStructure.Pins = FLASH_SPI_MISO_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_Init(FLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

    //拉高CS
    FLASH_SPI_CS_HIGH();

    /************************SPI Configuration***********************/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    // 双线全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                         // 主机模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     // 帧数据长度为8bit
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                           // 时钟空闲电平为高
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                          // 第二个边沿采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             // 片选信号由SSI寄存器控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;    // 波特率为PCLK的8分频
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                    // 最高有效位 MSB 收发在前
    SPI_InitStructure.SPI_Speed = SPI_Speed_Low;                          // 低速SPI

    SPI_Init(FLASH_SPIx, &SPI_InitStructure);
    SPI_Cmd(FLASH_SPIx, ENABLE);
}

/**
 * @brief 通过SPI读取一个字节的数据
 *
 * @return uint8_t :接收到的数据
 */
uint8_t SPI_FLASH_ReadByte(void)
{
    return (SPI_FLASH_SendByte(FLASH_DUMMY_BYTE));
}

/**
 * @brief 通过SPI发送一个字节的数据，并返回接收到的一个字节数据
 *
 * @param byte :待发送的数据
 * @return uint8_t :接收到的数据
 */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
    while(SPI_GetFlagStatus(FLASH_SPIx, SPI_FLAG_TXE) == RESET);

    SPI_SendData(FLASH_SPIx, byte);

    while(SPI_GetFlagStatus(FLASH_SPIx, SPI_FLAG_RXNE) == RESET);

    return SPI_ReceiveData(FLASH_SPIx);
}

/**
 * @brief 通过SPI发送两个字节的数据，并返回接收到的两个字节数据
 *
 * @param HalfWord :待发送的数据
 * @return uint16_t :接收到的数据
 */
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
    while(SPI_GetFlagStatus(FLASH_SPIx, SPI_FLAG_TXE) == RESET);

    SPI_SendData(FLASH_SPIx, HalfWord);

    while(SPI_GetFlagStatus(FLASH_SPIx, SPI_FLAG_RXNE) == RESET);

    return SPI_ReceiveData(FLASH_SPIx);
}

/**
 * @brief 向FLASH发送 写使能 指令
 *
 */
void SPI_FLASH_WriteEnable(void)
{
    FLASH_SPI_CS_LOW();

    //发送 写使能 指令
    SPI_FLASH_SendByte(FLASH_CMD_WriteEnable);

    FLASH_SPI_CS_HIGH();
}

/**
 * @brief 等待擦除或编程完成
 *
 */
void SPI_FLASH_WaitForWriteEnd(void)
{
    uint8_t flashstatus = 0;

    FLASH_SPI_CS_LOW();

    // 发送 读状态寄存器1 指令
    SPI_FLASH_SendByte(FLASH_CMD_ReadStatusReg1);

    do
    {
        //读FLASH状态寄存器
        flashstatus = SPI_FLASH_ReadByte();
    }
    while((flashstatus & FLASH_WIP_FLAG) == SET);   //正在写入，继续循环

    FLASH_SPI_CS_HIGH();
}

/**
 * @brief 读状态寄存器1
 *
 * @return uint8_t
 */
uint8_t SPI_FLASH_ReadStatusReg1(void)
{
    uint8_t flashstatus = 0;

    FLASH_SPI_CS_LOW();

    // 发送 读状态寄存器1 指令
    SPI_FLASH_SendByte(FLASH_CMD_ReadStatusReg1);

    flashstatus = SPI_FLASH_ReadByte();

    FLASH_SPI_CS_HIGH();

    return flashstatus;
}

/**
 * @brief 读状态寄存器2
 *
 * @return uint8_t
 */
uint8_t SPI_FLASH_ReadStatusReg2(void)
{
    uint8_t flashstatus = 0;

    FLASH_SPI_CS_LOW();

    // 发送 读状态寄存器2 指令
    SPI_FLASH_SendByte(FLASH_CMD_ReadStatusReg2);

    flashstatus = SPI_FLASH_ReadByte();

    FLASH_SPI_CS_HIGH();

    return flashstatus;
}

/**
 * @brief 写状态寄存器1
 *
 * @param status
 */
void SPI_FLASH_WriteStatusReg1(uint8_t status)
{
    //发送 写使能 指令
    SPI_FLASH_WriteEnable();

    FLASH_SPI_CS_LOW();

    // 发送 写状态寄存器1 指令
    SPI_FLASH_SendByte(FLASH_CMD_WriteStatusReg1);

    //发送 状态寄存器1 设定值
    SPI_FLASH_SendByte(status);

    FLASH_SPI_CS_HIGH();

    //等待写入完成
    SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief 写状态寄存器2
 *
 * @param status
 */
void SPI_FLASH_WriteStatusReg2(uint8_t status)
{
    //发送 写使能 指令
    SPI_FLASH_WriteEnable();

    FLASH_SPI_CS_LOW();

    // 发送 写状态寄存器2 指令
    SPI_FLASH_SendByte(FLASH_CMD_WriteStatusReg2);

    //发送 状态寄存器2 设定值
    SPI_FLASH_SendByte(status);

    FLASH_SPI_CS_HIGH();

    //等待写入完成
    SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief 扇区擦除 4KB
 *
 * @param SectorAddr :待擦除的扇区地址
 */
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
    //发送 写使能 指令
    SPI_FLASH_WriteEnable();
    //等待写入完成
    // SPI_FLASH_WaitForWriteEnd();

    FLASH_SPI_CS_LOW();

    //发送 扇区擦除 指令
    SPI_FLASH_SendByte(FLASH_CMD_SectorErase);

    //发送 待擦除扇区地址
    SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    SPI_FLASH_SendByte(SectorAddr & 0xFF);

    FLASH_SPI_CS_HIGH();

    //等待擦除完成
    SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief 块擦除 64KB
 *
 * @param BlockAddr :待擦除的块地址
 */
void SPI_FLASH_BlockErase(uint32_t BlockAddr)
{
    //发送 写使能 指令
    SPI_FLASH_WriteEnable();
    //等待写入完成
    // SPI_FLASH_WaitForWriteEnd();

    FLASH_SPI_CS_LOW();

    //发送 块擦除 指令
    SPI_FLASH_SendByte(FLASH_CMD_BlockErase);

    //发送 待擦除块地址
    SPI_FLASH_SendByte((BlockAddr & 0xFF0000) >> 16);
    SPI_FLASH_SendByte((BlockAddr & 0xFF00) >> 8);
    SPI_FLASH_SendByte(BlockAddr & 0xFF);

    FLASH_SPI_CS_HIGH();

    //等待擦除完成
    SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief 整片擦除
 *
 */
void SPI_FLASH_ChipErase(void)
{
    //发送 写使能 指令
    SPI_FLASH_WriteEnable();

    FLASH_SPI_CS_LOW();

    //发送 整片擦除 指令
    SPI_FLASH_SendByte(FLASH_CMD_ChipErase);

    FLASH_SPI_CS_HIGH();

    //等待擦除完成
    SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief 页写入数据
 *
 * @param pBuffer :待写入数据的指针
 * @param WriteAddr :写入地址
 * @param NumByteToWrite :写入数据长度
 * @note
 *    -需要先擦除
 *    -NumByteToWrite <= SPI_FLASH_PerWritePageSize
 */
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    //发送 写使能 指令
    SPI_FLASH_WriteEnable();

    FLASH_SPI_CS_LOW();

    //发送 页写入 指令
    SPI_FLASH_SendByte(FLASH_CMD_PageProgram);

    //发送 写地址
    SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    SPI_FLASH_SendByte(WriteAddr & 0xFF);

    //写入数据
    while(NumByteToWrite--)
    {
        SPI_FLASH_SendByte(*pBuffer);
        pBuffer++;
    }

    FLASH_SPI_CS_HIGH();

    //等待写入完成
    SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief 写入不定量数据
 *
 * @param pBuffer :待写入数据的指针
 * @param WriteAddr :写入地址
 * @param NumByteToWrite :写入数据长度
 * @note
 *    -需要先擦除
 */
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = WriteAddr % SPI_FLASH_PageSize;
    count = SPI_FLASH_PageSize - Addr;
    NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
    NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

    if(Addr == 0) /* WriteAddr 刚好按页对齐 */
    {
        if(NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
        {
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
        }
        else /* NumByteToWrite >= SPI_FLASH_PageSize */
        {
            while(NumOfPage--)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            if(NumOfSingle != 0)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
    else /* WriteAddr 与 SPI_FLASH_PageSize 不对齐  */
    {
        if(NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
        {
            if(NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
            {
                temp = NumOfSingle - count;

                //写完当前页
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
                WriteAddr +=  count;
                pBuffer += count;

                //写剩余数据
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
            }
            else
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else /* NumByteToWrite >= SPI_FLASH_PageSize */
        {
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            //先写完当前页，以后地址将对齐
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;

            //WriteAddr 刚好按页对齐
            while(NumOfPage--)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            if(NumOfSingle != 0)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

/**
 * @brief 读数据
 *
 * @param pBuffer :存储读出数据的指针
 * @param ReadAddr :读取地址
 * @param NumByteToRead :读取数据长度
 */
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    FLASH_SPI_CS_LOW();

    //发送 读数据 指令
    SPI_FLASH_SendByte(FLASH_CMD_ReadData);

    //发送 读地址
    SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    SPI_FLASH_SendByte(ReadAddr & 0xFF);

    //读数据
    while(NumByteToRead--)
    {
        *pBuffer = SPI_FLASH_ReadByte();
        pBuffer++;
    }

    FLASH_SPI_CS_HIGH();
}

/**
 * @brief 启动读取数据字节序列
 *
 * @param ReadAddr :读取地址
 */
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
    FLASH_SPI_CS_LOW();

    //发送 读数据 指令
    SPI_FLASH_SendByte(FLASH_CMD_ReadData);

    //发送 读地址
    SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

/**
 * @brief 读取Device ID
 *
 * @return uint8_t :Device ID
 */
uint8_t SPI_FLASH_DeviceID(void)
{
    uint8_t Temp = 0;

    FLASH_SPI_CS_LOW();

    //发送 读Device ID 指令
    SPI_FLASH_SendByte(FLASH_CMD_DeviceID);

    SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);
    SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);
    SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);

    //读数据
    Temp = SPI_FLASH_ReadByte();

    FLASH_SPI_CS_HIGH();

    return Temp;
}

/**
 * @brief 读取Manufacturer / Device ID
 *
 * @return uint16_t :Manufacturer / Device ID
 */
uint16_t SPI_FLASH_ManufactDeviceID(void)
{
    uint16_t Temp = 0, Temp0 = 0, Temp1 = 0;

    FLASH_SPI_CS_LOW();

    //发送 读Manufacturer / Device ID 指令
    SPI_FLASH_SendByte(FLASH_CMD_ManufactDeviceID);

    SPI_FLASH_SendByte(0x00);
    SPI_FLASH_SendByte(0x00);
    SPI_FLASH_SendByte(0x00);

    //读数据
    Temp0 = SPI_FLASH_ReadByte();
    Temp1 = SPI_FLASH_ReadByte();

    FLASH_SPI_CS_HIGH();

    Temp = (Temp0 << 8) | Temp1;

    return Temp;
}

/**
 * @brief 读取JEDEC ID
 *
 * @return uint32_t :JEDEC ID
 */
uint32_t SPI_FLASH_JedecID(void)
{
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    FLASH_SPI_CS_LOW();

    //发送 读JEDEC ID 指令
    SPI_FLASH_SendByte(FLASH_CMD_JedecID);

    //读数据
    Temp0 = SPI_FLASH_ReadByte();
    Temp1 = SPI_FLASH_ReadByte();
    Temp2 = SPI_FLASH_ReadByte();

    FLASH_SPI_CS_HIGH();

    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    return Temp;
}

/**
 * @brief 读取Unique ID (64bit)
 *
 * @param pBuffer :存放UID的Buffer
 */
void SPI_FLASH_UniqueID(uint8_t* pBuffer)
{
    uint8_t NumByteToRead = 8;

    FLASH_SPI_CS_LOW();

    //发送 读Unique ID 指令
    SPI_FLASH_SendByte(FLASH_CMD_UniqueID);

    SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);
    SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);
    SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);
    SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);

    //读数据
    while(NumByteToRead--)
    {
        *pBuffer = SPI_FLASH_ReadByte();
        pBuffer++;
    }

    FLASH_SPI_CS_HIGH();
}

/**
 * @brief 进入掉电模式
 *
 */
void SPI_FLASH_PowerDown(void)
{
    FLASH_SPI_CS_LOW();

    //发送 掉电 指令
    SPI_FLASH_SendByte(FLASH_CMD_PowerDown);

    FLASH_SPI_CS_HIGH();
}

/**
 * @brief 唤醒
 *
 */
void SPI_FLASH_WAKEUP(void)
{
    FLASH_SPI_CS_LOW();

    //发送 Release Power-down or High Performance Mode 指令
    SPI_FLASH_SendByte(FLASH_CMD_ReleasePowerDown);

    FLASH_SPI_CS_HIGH();
}







