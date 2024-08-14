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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CW32_EVAL_SPI_FLASH_H
#define __CW32_EVAL_SPI_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cw32l031.h"
#include "cw32l031_rcc.h"
#include "cw32l031_gpio.h"
#include "cw32l031_spi.h"

/******************************************************************************
 * type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * pre-processor symbols/macros ('#define')
 ******************************************************************************/
//Manufacturer and Device Identification (W25Q64)
#define sDeviceID                         0x16
#define sManufactDeviceID                 0xEF16
#define sJedecID                          0xEF4017

/* SPI FLASH supported commands */
#define FLASH_CMD_WriteEnable        0x06
#define FLASH_CMD_WriteDisable        0x04
#define FLASH_CMD_ReadStatusReg1    0x05
#define FLASH_CMD_ReadStatusReg2    0x35
#define FLASH_CMD_WriteStatusReg1    0x01
#define FLASH_CMD_WriteStatusReg2    0x31
#define FLASH_CMD_ReadData         0x03
#define FLASH_CMD_FastReadData        0x0B
#define FLASH_CMD_FastReadDual        0x3B
#define FLASH_CMD_PageProgram        0x02
#define FLASH_CMD_SectorErase        0x20
#define FLASH_CMD_BlockErase     0xD8
#define FLASH_CMD_ChipErase         0xC7
#define FLASH_CMD_PowerDown         0xB9
#define FLASH_CMD_ReleasePowerDown       0xAB
#define FLASH_CMD_DeviceID         0xAB
#define FLASH_CMD_ManufactDeviceID      0x90
#define FLASH_CMD_JedecID            0x9F
#define FLASH_CMD_UniqueID            0x4B

/* Write In Progress (WIP) flag */
#define FLASH_WIP_FLAG                    0x01

#define FLASH_DUMMY_BYTE                  0xFF

#define SPI_FLASH_PageSize                256
#define SPI_FLASH_PerWritePageSize        256

//SPIx
#define FLASH_SPIx                        CW_SPI1
#define FLASH_SPI_CLK                     RCC_APB2_PERIPH_SPI1
#define FLASH_SPI_APBClkENx               RCC_APBPeriphClk_Enable2

//SPIx GPIO
#define FLASH_SPI_SCK_GPIO_CLK            RCC_AHB_PERIPH_GPIOA
#define FLASH_SPI_SCK_GPIO_PORT           CW_GPIOA
#define FLASH_SPI_SCK_GPIO_PIN            GPIO_PIN_10

#define FLASH_SPI_MISO_GPIO_CLK           RCC_AHB_PERIPH_GPIOA
#define FLASH_SPI_MISO_GPIO_PORT          CW_GPIOA
#define FLASH_SPI_MISO_GPIO_PIN           GPIO_PIN_11

#define FLASH_SPI_MOSI_GPIO_CLK           RCC_AHB_PERIPH_GPIOA
#define FLASH_SPI_MOSI_GPIO_PORT          CW_GPIOA
#define FLASH_SPI_MOSI_GPIO_PIN           GPIO_PIN_12

#define FLASH_SPI_CS_GPIO_CLK             RCC_AHB_PERIPH_GPIOA
#define FLASH_SPI_CS_GPIO_PORT            CW_GPIOA
#define FLASH_SPI_CS_GPIO_PIN             GPIO_PIN_15

//GPIO AF
#define FLASH_SPI_AF_SCK                  PA10_AFx_SPI1SCK()
#define FLASH_SPI_AF_MISO                 PA11_AFx_SPI1MISO()
#define FLASH_SPI_AF_MOSI                 PA12_AFx_SPI1MOSI()

//CS LOW or HIGH
#define FLASH_SPI_CS_LOW()             PA15_SETLOW()
#define FLASH_SPI_CS_HIGH()            PA15_SETHIGH()

/******************************************************************************
* Global variable definitions (declared in header file with 'extern')
******************************************************************************/


/******************************************************************************
* Global function prototypes
******************************************************************************/

/* SPI FLASH 初始化*/
void SPI_FLASH_Init(void);

/* Low layer functions */
uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

/* 读写状态寄存器 */
uint8_t SPI_FLASH_ReadStatusReg1(void);
uint8_t SPI_FLASH_ReadStatusReg2(void);
void SPI_FLASH_WriteStatusReg1(uint8_t status);
void SPI_FLASH_WriteStatusReg2(uint8_t status);

/* High layer functions */
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BlockErase(uint32_t BlockAddr);
void SPI_FLASH_ChipErase(void);

void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);

uint8_t SPI_FLASH_DeviceID(void);
uint16_t SPI_FLASH_ManufactDeviceID(void);
uint32_t SPI_FLASH_JedecID(void);
void SPI_FLASH_UniqueID(uint8_t* pBuffer);
void SPI_FLASH_PowerDown(void);
void SPI_FLASH_WAKEUP(void);

#ifdef __cplusplus
}
#endif

#endif /* __CW32_EVAL_SPI_FLASH_H */

