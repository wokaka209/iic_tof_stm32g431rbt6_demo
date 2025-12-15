/*
 * @Author: wokaka209 1325536985@qq.com
 * @Date: 2025-12-08 20:30:44
 * @LastEditors: wokaka209 1325536985@qq.com
 * @LastEditTime: 2025-12-14 23:06:30
 * @FilePath: \eided:\vscodeied\stm32g431RBT6\tof_test\my_mian\my_main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "my_main.h"

char str[] = "State:0 , Range Valid\nd: 113 mm";

/******lcd 变量***** */
char str_lcd[20];

/*tof 变量*/
uint8_t dist = 0;

/***************uart******************/
char rxbuff[100] = {0};
char txbuff[100] = {0};
#define rxlen 100
char rxdata[rxlen] = {0};
extern DMA_HandleTypeDef hdma_usart2_rx;

void setup()
{
    LCD_Init();
    LCD_SetBackColor(Blue);
    LCD_SetTextColor(White);
    LCD_Clear(Blue);
    
    // 初始化软件I2C
    Soft_I2C_Init();
    HAL_Delay(10);
    
    // I2C扫描调试
    uint8_t found_addr[10];
    uint8_t found = Soft_I2C_Scan((uint8_t *)found_addr);
    
    sprintf(str_lcd,"I2C Found: %d", found);
    LCD_DisplayStringLine(Line1,(u8 *)str_lcd);
    memset(str_lcd,0,20);
    HAL_Delay(1000);
    
    // 检查VL6180X地址(0x29)是否被找到
    uint8_t vl6180x_found = 0;
    for(uint8_t i = 0; i < found; i++)
    {
        if(found_addr[i] == 0x29)
        {
            vl6180x_found = 1;
            break;
        }
    }
    
    if(!vl6180x_found)
    {
        sprintf(str_lcd,"VL6180X Not Found");
        LCD_DisplayStringLine(Line2,(u8 *)str_lcd);
        memset(str_lcd,0,20);
        while(1);
    }
    
    sprintf(str_lcd,"VL6180X Found");
    LCD_DisplayStringLine(Line2,(u8 *)str_lcd);
    memset(str_lcd,0,20);
    HAL_Delay(1000);
    
    // 初始化VL6180X
    if(!VL6180X_Init())
    {
        sprintf(str_lcd,"VL6180X Init Failed");
        LCD_DisplayStringLine(Line3,(u8 *)str_lcd);
        memset(str_lcd,0,20);
        while(1);
    }
    
    sprintf(str_lcd,"VL6180X Init OK");
    LCD_DisplayStringLine(Line3,(u8 *)str_lcd);
    memset(str_lcd,0,20);
    HAL_Delay(1000);
    
    // 使用DMA以非阻塞方式接收UART数据直到空闲
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxbuff, sizeof(rxbuff));
    // 禁用DMA半传输中断
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
}

void loop()
{
    // sprintf(txbuff, "761212");
    // HAL_Delay(100);
    // HAL_UART_Transmit_DMA(&huart2, (uint8_t *)txbuff, strlen(txbuff));
    // memset(txbuff, 0, 100);

    uint8_t range_result = 0;
    if (VL6180X_ReadRange(&dist))
    {
        sprintf(str_lcd, "Distance: %d mm", dist);
        LCD_DisplayStringLine(Line3, (u8 *)str_lcd);
        memset(str_lcd, 0, 20);
        
        // 通过串口发送距离数据用于调试
        sprintf(txbuff, "Range: %d mm\r\n", dist);
        HAL_UART_Transmit_DMA(&huart2, (uint8_t *)txbuff, strlen(txbuff));
        memset(txbuff, 0, 100);
    }
    else
    {
        sprintf(str_lcd, "TOF Error: %d", range_result);
        LCD_DisplayStringLine(Line4, (u8 *)str_lcd);
        memset(str_lcd, 0, 20);
        
        // 通过串口发送错误信息用于调试
        sprintf(txbuff, "TOF Error: %d\r\n", range_result);
        HAL_UART_Transmit_DMA(&huart2, (uint8_t *)txbuff, strlen(txbuff));
        memset(txbuff, 0, 100);
    }
    
    HAL_Delay(500); // 每500ms读取一次
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    UNUSED(Size);
    // 处理USART1的接收数据
    if (huart->Instance == USART2)
    {
        // 检查接收到的数据大小是否等于预期长度
        if (Size > 0)
        {
            memcpy(rxdata, rxbuff, Size);
            rxdata[Size] = '\0'; // 添加字符串结束符
            uart_serv(rxdata);
            memset(rxbuff, 0, sizeof(rxbuff));
        }
        // 准备接收下一个数据包
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxbuff, sizeof(rxbuff));
        // 禁用DMA中断
        __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);
    }
}

// rxdata
void uart_serv(char *str)
{
    int state = -1;
    int distance = -1;

    // 解析 State 行
    char *s = strstr(str, "State:");
    if (s != NULL)
    {
        sscanf(s, "State:%d", &state);
    }

    // 解析 d: 行
    char *p = strstr(str, "d:");
    if (p != NULL)
    {
        sscanf(p, "d: %d", &distance);
    }

    // 判断有效
    dist = (uint16_t)distance;
}
