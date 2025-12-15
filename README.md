
### 1. iic_tof_stm32g431rbt6_demo

- __项目名称__: STM32G4 VL6180X TOF Distance Measurement
- __简短描述__: 基于STM32G431RBT6微控制器和VL6180X飞行时间传感器的嵌入式距离测量系统，具有LCD实时显示和串口数据输出功能。

### 2. 项目概述

- 详细说明项目功能：通过软件I2C驱动VL6180X TOF传感器，测量距离（0-200mm），结果在240x320 LCD上实时显示，并通过USART2串口输出调试信息。
- 采用Arduino风格的`setup()`和`loop()`主循环结构，便于理解和扩展。
- 支持UART DMA接收和空闲中断，可处理外部指令（如解析“State: X, d: Y mm”格式数据）。

### 3. 硬件要求

- __微控制器__: STM32G431RBT6（或其他STM32G4系列）

- __TOF传感器__: VL6180X（I2C接口，地址0x29）

- __LCD显示屏__: 240x320像素，控制器ILI932X或uC8230（与CT117E竞赛板兼容）

- __连接__:

  - VL6180X: SDA→PA7, SCL→PA6
  - LCD: 使用GPIOB和GPIOC控制（具体引脚见lcd.c）
  - USART2: TX→PA2, RX→PA3（用于串口调试）

- __电源__: 3.3V供电

### 4. 软件依赖和开发环境

- __IDE__: Visual Studio Code + EIDE（嵌入式集成开发环境）插件
- __工具链__: ARM GCC（通过EIDE自动管理）
- __框架__: STM32Cube HAL库（已集成到项目中）
- __构建系统__: EIDE（项目根目录包含.eide/和.vscode/配置）

### 5. 构建和烧录步骤

1. __克隆仓库__: `git clone <repo-url>`
2. __打开项目__: 在VS Code中打开项目根目录（`d:/vscodeied/stm32g431RBT6/tof_test/eide`）
3. __安装EIDE插件__: 在VS Code扩展商店搜索“EIDE”并安装
4. __构建项目__: 按`Ctrl+Shift+B`选择“build”任务，或使用终端命令`eide project build`
5. __烧录到设备__: 使用“flash”任务，或通过ST-Link等调试器烧录生成的.hex/.bin文件

### 6. 使用方法

1. 按硬件要求连接传感器和LCD。
2. 烧录程序后复位开发板。
3. LCD将显示蓝色背景，初始化信息（I2C扫描结果、VL6180X初始化状态）。
4. 正常运行后，LCD第三行显示实时距离（例如“Distance: 113 mm”）。
5. 串口（波特率默认115200）输出距离数据和错误信息。
6. 可通过串口发送格式为“State:X , Range Valid\nd: Y mm”的字符串来模拟外部数据输入（会被`uart_serv`解析并更新距离变量）。

### 7. 项目文件结构

```javascript
my_mian/
├── fonts.h          # 16x24 ASCII字体点阵数据
├── lcd.c/lcd.h     # LCD驱动（初始化、绘制文本图形、颜色设置）
├── my_main.c       # 主程序（setup/loop、UART回调）
├── my_main.h       # 主头文件（函数声明、包含）
└── tof.c/tof.h     # VL6180X驱动（软件I2C、传感器读写）
```

### 8. 关键代码说明

- __软件I2C__: 通过GPIO模拟，在`tof.c`中实现Start/Stop/读写字节。
- __LCD驱动__: 支持两种控制器，提供字符显示、图形绘制、窗口模式等功能。
- __UART DMA__: 使用`HAL_UARTEx_ReceiveToIdle_DMA`实现非阻塞接收，在回调中处理数据。
- __TOF读取__: `VL6180X_ReadRange`启动单次测距并等待结果，返回毫米值。



### 10. 贡献和联系方式

- 提供问题反馈和功能建议的途径。
- “wokaka209”。

