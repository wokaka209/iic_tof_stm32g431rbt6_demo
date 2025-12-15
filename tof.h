/*
 * @Author: wokaka209 1325536985@qq.com
 * @Date: 2025-12-08 20:31:04
 * @LastEditors: wokaka209 1325536985@qq.com
 * @LastEditTime: 2025-12-14 23:11:45
 * @FilePath: \eided:\vscodeied\stm32g431RBT6\tof_test\my_mian\tof.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: wokaka209 1325536985@qq.com
 * @Date: 2025-12-08 20:31:04
 * @LastEditors: wokaka209 1325536985@qq.com
 * @LastEditTime: 2025-12-14 22:58:14
 * @FilePath: \my_mian\tof.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __TOF_H__
#define __TOF_H__

#include "stm32g4xx_hal.h"

#define VL6180X_ADDR 0x29 // 7bit 地址
#define SOFT_I2C_SCL_GPIO      GPIOA
#define SOFT_I2C_SDA_GPIO      GPIOA
#define SOFT_I2C_SCL_PIN       GPIO_PIN_6
#define SOFT_I2C_SDA_PIN       GPIO_PIN_7

/* 将 SDA 设置为输入 */
#define SDA_IN()   { SOFT_I2C_SDA_GPIO->MODER &= ~(3<<(7*2)); SOFT_I2C_SDA_GPIO->MODER |= (0<<(7*2)); }
/* 将 SDA 设置为输出 */
#define SDA_OUT()  { SOFT_I2C_SDA_GPIO->MODER &= ~(3<<(7*2)); SOFT_I2C_SDA_GPIO->MODER |= (1<<(7*2)); }
/* SCL 固定输出模式 */
#define SCL_OUT()  { SOFT_I2C_SCL_GPIO->MODER &= ~(3<<(6*2)); SOFT_I2C_SCL_GPIO->MODER |= (1<<(6*2)); }

/* IO 操作 */
#define SDA_H() HAL_GPIO_WritePin(SOFT_I2C_SDA_GPIO,SOFT_I2C_SDA_PIN,GPIO_PIN_SET)
#define SDA_L() HAL_GPIO_WritePin(SOFT_I2C_SDA_GPIO,SOFT_I2C_SDA_PIN,GPIO_PIN_RESET)
#define SCL_H() HAL_GPIO_WritePin(SOFT_I2C_SCL_GPIO,SOFT_I2C_SCL_PIN,GPIO_PIN_SET)
#define SCL_L() HAL_GPIO_WritePin(SOFT_I2C_SCL_GPIO,SOFT_I2C_SCL_PIN,GPIO_PIN_RESET)
#define SDA_READ() HAL_GPIO_ReadPin(SOFT_I2C_SDA_GPIO,SOFT_I2C_SDA_PIN)

void Soft_I2C_Init(void);
void Soft_I2C_Start(void);
void Soft_I2C_Stop(void);
uint8_t Soft_I2C_WriteByte(uint8_t dat);
uint8_t Soft_I2C_ReadByte(uint8_t ack);

uint8_t VL6180X_Init(void);
uint8_t VL6180X_ReadRange(uint8_t *range_mm);

uint8_t VL6180X_WriteReg8(uint16_t reg, uint8_t val);
uint8_t VL6180X_ReadReg8(uint16_t reg, uint8_t *val);

uint8_t Soft_I2C_Scan(uint8_t *found_addr);

#endif
