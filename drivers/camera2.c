/***
 * @version V1.0
 * @date 2023-4-11
 * @author ���ͿƼ�
 * @brief  摄像头驱动实现文件
 *******************************************************************************************************************
 * @description
 * 	实现方案:	STM32H723ZGT6芯片（型号FK723M1-ZGT6）+ OV5640模块(OV5640M1-500W)
 * 	官网地址:	https://shop212360197.taobao.com
 * 	QQ交流群	536665479
 *
 * 	参考资料:	Arduino/ArduCAM 以及 OpenMV 的资料
 *
 * 	说明:	1. 本工程配置默认为 OV5640 为 4:3(1280*960) ,43帧/秒 ,JPEG模式2帧 ,JPEG帧率最高约7帧
 * 	 	    2. 本方案采用DMA驱动模式 ,若采用中断模式 ,需要自行对应增加中断响应 ,请参考例程代码
 * **********FANke*********************************************
 ***/

#include "camera.h"

extern DCMI_HandleTypeDef hdcmi;           // DCMI实例
DMA_HandleTypeDef  DMA_Handle_dcmi; // DMA实例

volatile uint8_t OV5640_FrameState = 0; // DCMI状态标志位 , 捕获到一帧后 HAL_DCMI_FrameCallback() 函数触发设置为 1
volatile uint8_t OV5640_FPS;            // 帧率

/**
 *@brief DCMI 硬件外设初始化
 */
void MX_DCMI_Init(void)
{
    hdcmi.Instance              = DCMI;
    hdcmi.Init.SynchroMode      = DCMI_SYNCHRO_HARDWARE;   // 硬件同步方式 ,采用外部输入的VS/HS脉冲同步
    hdcmi.Init.PCKPolarity      = DCMI_PCKPOLARITY_RISING; // 像素时钟上升沿有效
    hdcmi.Init.VSPolarity       = DCMI_VSPOLARITY_LOW;     // VS极性低有效
    hdcmi.Init.HSPolarity       = DCMI_HSPOLARITY_LOW;     // HS极性低有效
    hdcmi.Init.CaptureRate      = DCMI_CR_ALL_FRAME;       // 所有帧都捕获模式
    hdcmi.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;     // 8位数据模式
    hdcmi.Init.JPEGMode         = DCMI_JPEG_DISABLE;       // 不使用DCMI的JPEG模式
    hdcmi.Init.ByteSelectMode   = DCMI_BSM_ALL;            // DCMI接口全字节模式
    hdcmi.Init.ByteSelectStart  = DCMI_OEBS_ODD;           // 字节选取起始 ,奇帧/偶行 开始读取数据的首字节
    hdcmi.Init.LineSelectMode   = DCMI_LSM_ALL;            // 行选取模式
    hdcmi.Init.LineSelectStart  = DCMI_OELS_ODD;           // 起始 ,奇帧开始或偶行开始
    HAL_DCMI_Init(&hdcmi);

    HAL_NVIC_SetPriority(DCMI_IRQn, 0, 5); // 设置中断优先级
    HAL_NVIC_EnableIRQ(DCMI_IRQn);         // 使能DCMI中断
                                           // 在JPEG模式下 ,还需要使能FRAME中断
                                           //__HAL_DCMI_ENABLE_IT (&hdcmi, DCMI_IT_FRAME);
}

#define OV2640_DEVICE_ADDRESS 0x60 // OV2640地址
#define OV5640_DEVICE_ADDRESS 0X78 // OV5640地址

/*----------------------------------------- IIIC 引脚配置宏 -----------------------------------------------*/

#define SCCB_SCL_CLK_ENABLE   __HAL_RCC_GPIOF_CLK_ENABLE() // SCL 引脚时钟
#define SCCB_SCL_PORT         GPIOF                        // SCL 引脚端口
#define SCCB_SCL_PIN          GPIO_PIN_14                  // SCL 引脚

#define SCCB_SDA_CLK_ENABLE   __HAL_RCC_GPIOF_CLK_ENABLE() // SDA 引脚时钟
#define SCCB_SDA_PORT         GPIOF                        // SDA 引脚端口
#define SCCB_SDA_PIN          GPIO_PIN_15                  // SDA 引脚

/*------------------------------------------ IIC相关定义 -------------------------------------------------*/

#define ACK_OK                1 // 响应正常
#define ACK_ERR               0 // 响应错误

// IIC通信延时，Touch_IIC_Delay()函数使用，
#define SCCB_DelayVaule       15

/*-------------------------------------------- IO口操作 ---------------------------------------------------*/

#define SCCB_SCL(a)                                                   \
    if (a)                                                            \
        HAL_GPIO_WritePin(SCCB_SCL_PORT, SCCB_SCL_PIN, GPIO_PIN_SET); \
    else                                                              \
        HAL_GPIO_WritePin(SCCB_SCL_PORT, SCCB_SCL_PIN, GPIO_PIN_RESET)

#define SCCB_SDA(a)                                                   \
    if (a)                                                            \
        HAL_GPIO_WritePin(SCCB_SDA_PORT, SCCB_SDA_PIN, GPIO_PIN_SET); \
    else                                                              \
        HAL_GPIO_WritePin(SCCB_SDA_PORT, SCCB_SDA_PIN, GPIO_PIN_RESET)

/*--------------------------------------------- 函数声明 --------------------------------------------------*/

/*****************************************************************************************
 *	函 数 名: SCCB_GPIO_Config
 *	入口参数: 无
 *	返 回 值: 无
 *	函数功能: 初始化IIC的GPIO口,推挽输出
 *	说    明: 由于IIC通信速度不高，这里的IO口速度配置为2M即可
 ******************************************************************************************/

static void SCCB_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    SCCB_SCL_CLK_ENABLE; // 初始化IO口时钟
    SCCB_SDA_CLK_ENABLE;

    GPIO_InitStruct.Pin   = SCCB_SCL_PIN;        // SCL引脚
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD; // 开漏输出
    GPIO_InitStruct.Pull  = GPIO_NOPULL;         // 不带上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 速度等级
    HAL_GPIO_Init(SCCB_SCL_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SCCB_SDA_PIN; // SDA引脚
    HAL_GPIO_Init(SCCB_SDA_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出
    GPIO_InitStruct.Pull = GPIO_PULLUP;         // 上拉

    HAL_GPIO_WritePin(SCCB_SCL_PORT, SCCB_SCL_PIN, GPIO_PIN_SET); // SCL输出高电平
    HAL_GPIO_WritePin(SCCB_SDA_PORT, SCCB_SDA_PIN, GPIO_PIN_SET); // SDA输出高电平
}

/*****************************************************************************************
 *	函 数 名: SCCB_Delay
 *	入口参数: a - 延时时间
 *	返 回 值: 无
 *	函数功能: 简单延时函数
 *	说    明: 为了移植的简便性且对延时精度要求不高，所以不需要使用定时器做延时
 ******************************************************************************************/

static void SCCB_Delay(uint32_t a)
{
    volatile uint16_t i;
    while (a--) {
        for (i = 0; i < 3; i++);
    }
}

/*****************************************************************************************
 *	函 数 名: SCCB_Start
 *	入口参数: 无
 *	返 回 值: 无
 *	函数功能: IIC起始信号
 *	说    明: 在SCL处于高电平期间，SDA由高到低跳变为起始信号
 ******************************************************************************************/

static void SCCB_Start(void)
{
    SCCB_SDA(1);
    SCCB_SCL(1);
    SCCB_Delay(SCCB_DelayVaule);

    SCCB_SDA(0);
    SCCB_Delay(SCCB_DelayVaule);
    SCCB_SCL(0);
    SCCB_Delay(SCCB_DelayVaule);
}

/*****************************************************************************************
 *	函 数 名: SCCB_Stop
 *	入口参数: 无
 *	返 回 值: 无
 *	函数功能: IIC停止信号
 *	说    明: 在SCL处于高电平期间，SDA由低到高跳变为起始信号
 ******************************************************************************************/

static void SCCB_Stop(void)
{
    SCCB_SCL(0);
    SCCB_Delay(SCCB_DelayVaule);
    SCCB_SDA(0);
    SCCB_Delay(SCCB_DelayVaule);

    SCCB_SCL(1);
    SCCB_Delay(SCCB_DelayVaule);
    SCCB_SDA(1);
    SCCB_Delay(SCCB_DelayVaule);
}

/*****************************************************************************************
 *	函 数 名: SCCB_ACK
 *	入口参数: 无
 *	返 回 值: 无
 *	函数功能: IIC应答信号
 *	说    明: 在SCL为高电平期间，SDA引脚输出为低电平，产生应答信号
 ******************************************************************************************/

static void SCCB_ACK(void)
{
    SCCB_SCL(0);
    SCCB_Delay(SCCB_DelayVaule);
    SCCB_SDA(0);
    SCCB_Delay(SCCB_DelayVaule);
    SCCB_SCL(1);
    SCCB_Delay(SCCB_DelayVaule);

    SCCB_SCL(0); // SCL输出低时，SDA应立即拉高，释放总线
    SCCB_SDA(1);

    SCCB_Delay(SCCB_DelayVaule);
}

/*****************************************************************************************
 *	函 数 名: SCCB_NoACK
 *	入口参数: 无
 *	返 回 值: 无
 *	函数功能: IIC非应答信号
 *	说    明: 在SCL为高电平期间，若SDA引脚为高电平，产生非应答信号
 ******************************************************************************************/

static void SCCB_NoACK(void)
{
    SCCB_SCL(0);
    SCCB_Delay(SCCB_DelayVaule);
    SCCB_SDA(1);
    SCCB_Delay(SCCB_DelayVaule);
    SCCB_SCL(1);
    SCCB_Delay(SCCB_DelayVaule);

    SCCB_SCL(0);
    SCCB_Delay(SCCB_DelayVaule);
}

/*****************************************************************************************
 *	函 数 名: SCCB_WaitACK
 *	入口参数: 无
 *	返 回 值: 无
 *	函数功能: 等待接收设备发出应答信号
 *	说    明: 在SCL为高电平期间，若检测到SDA引脚为低电平，则接收设备响应正常
 ******************************************************************************************/

static uint8_t SCCB_WaitACK(void)
{
    SCCB_SDA(1);
    SCCB_Delay(SCCB_DelayVaule);
    SCCB_SCL(1);
    SCCB_Delay(SCCB_DelayVaule);

    if (HAL_GPIO_ReadPin(SCCB_SDA_PORT, SCCB_SDA_PIN) != 0) // 判断设备是否有做出响应
    {
        SCCB_SCL(0);
        SCCB_Delay(SCCB_DelayVaule);
        return ACK_ERR; // 无应答
    } else {
        SCCB_SCL(0);
        SCCB_Delay(SCCB_DelayVaule);
        return ACK_OK; // 应答正常
    }
}

/*****************************************************************************************
 *	函 数 名:	SCCB_WriteByte
 *	入口参数:	IIC_Data - 要写入的8位数据
 *	返 回 值:	ACK_OK  - 设备响应正常
 *          	   ACK_ERR - 设备响应错误
 *	函数功能:	写一字节数据
 *	说    明:   高位在前
 ******************************************************************************************/

static uint8_t SCCB_WriteByte(uint8_t IIC_Data)
{
    uint8_t i;

    for (i = 0; i < 8; i++) {
        SCCB_SDA(IIC_Data & 0x80);

        SCCB_Delay(SCCB_DelayVaule);
        SCCB_SCL(1);
        SCCB_Delay(SCCB_DelayVaule);
        SCCB_SCL(0);
        if (i == 7) {
            SCCB_SDA(1);
        }
        IIC_Data <<= 1;
    }

    return SCCB_WaitACK(); // 等待设备响应
}

/*****************************************************************************************
 *	函 数 名:	SCCB_ReadByte
 *	入口参数:	ACK_Mode - 响应模式，输入1则发出应答信号，输入0发出非应答信号
 *	返 回 值:	ACK_OK  - 设备响应正常
 *          	   ACK_ERR - 设备响应错误
 *	函数功能:   读一字节数据
 *	说    明:   1.高位在前
 *				   2.应在主机接收最后一字节数据时发送非应答信号
 ******************************************************************************************/

static uint8_t SCCB_ReadByte(uint8_t ACK_Mode)
{
    uint8_t IIC_Data = 0;
    uint8_t i        = 0;

    for (i = 0; i < 8; i++) {
        IIC_Data <<= 1;

        SCCB_SCL(1);
        SCCB_Delay(SCCB_DelayVaule);
        IIC_Data |= (HAL_GPIO_ReadPin(SCCB_SDA_PORT, SCCB_SDA_PIN) & 0x01);
        SCCB_SCL(0);
        SCCB_Delay(SCCB_DelayVaule);
    }

    if (ACK_Mode == 1) //	应答信号
        SCCB_ACK();
    else
        SCCB_NoACK(); // 非应答信号

    return IIC_Data;
}

/*************************************************************************************************************************************
 *	函 数 名:	SCCB_WriteHandle
 *
 *	入口参数:	addr - 要进行操作的寄存器(8位地址)
 *
 *	返 回 值:	SUCCESS - 操作成功，ERROR	  - 操作失败
 *
 *	函数功能:	对指定的寄存器(8位地址)执行写操作，OV2640用到
 ************************************************************************************************************************************/

static uint8_t SCCB_WriteHandle(uint8_t addr)
{
    uint8_t status; // 状态标志位

    SCCB_Start();                                        // 启动IIC通信
    if (SCCB_WriteByte(OV2640_DEVICE_ADDRESS) == ACK_OK) // 写数据指令
    {
        if (SCCB_WriteByte((uint8_t)(addr)) != ACK_OK) {
            status = ERROR; // 操作失败
        }
    }
    status = SUCCESS; // 操作成功
    return status;
}

/*************************************************************************************************************************************
 *	函 数 名:	SCCB_WriteReg
 *
 *	入口参数:	addr - 要写入的寄存器(8位地址)，value - 要写入的数据
 *
 *	返 回 值:	SUCCESS - 操作成功， ERROR	  - 操作失败
 *
 *	函数功能:	对指定的寄存器(8位地址)写一字节数据，OV2640用到
 ************************************************************************************************************************************/

static uint8_t SCCB_WriteReg(uint8_t addr, uint8_t value)
{
    uint8_t status;

    SCCB_Start(); // 启动IIC通讯

    if (SCCB_WriteHandle(addr) == SUCCESS) // 写入要操作的寄存器
    {
        if (SCCB_WriteByte(value) != ACK_OK) // 写数据
        {
            status = ERROR;
        }
    }
    SCCB_Stop(); // 停止通讯

    status = SUCCESS; // 写入成功
    return status;
}
/*************************************************************************************************************************************
 *	函 数 名:	SCCB_ReadReg
 *
 *	入口参数:	addr - 要读取的寄存器(8位地址)
 *
 *	返 回 值:	读到的数据
 *
 *	函数功能:	对指定的寄存器(8位地址)读取一字节数据，OV2640用到
 ************************************************************************************************************************************/

static uint8_t SCCB_ReadReg(uint8_t addr)
{
    uint8_t value = 0;

    SCCB_Start(); // 启动IIC通信

    if (SCCB_WriteHandle(addr) == SUCCESS) // 写入要操作的寄存器
    {
        SCCB_Stop();  // 停止IIC通信
        SCCB_Start(); // 重新启动IIC通讯

        if (SCCB_WriteByte(OV2640_DEVICE_ADDRESS | 0X01) == ACK_OK) // 发送读命令
        {
            value = SCCB_ReadByte(0); // 读到最后一个数据时发送 非应答信号
        }
        SCCB_Stop(); // 停止IIC通信
    }

    return value;
}

/*************************************************************************************************************************************
 *	函 数 名:	SCCB_WriteHandle_16Bit
 *
 *	入口参数:	addr - 要进行操作的寄存器(16位地址)
 *
 *	返 回 值:	SUCCESS - 操作成功，ERROR - 操作失败
 *
 *	函数功能:	对指定的寄存器(16位地址)执行写操作，OV5640用到
 ************************************************************************************************************************************/

static uint8_t SCCB_WriteHandle_16Bit(uint16_t addr)
{
    uint8_t status; // 状态标志位

    SCCB_Start();                                        // 启动IIC通信
    if (SCCB_WriteByte(OV5640_DEVICE_ADDRESS) == ACK_OK) // 写数据指令
    {
        if (SCCB_WriteByte((uint8_t)(addr >> 8)) == ACK_OK) // 写入16位地址
        {
            if (SCCB_WriteByte((uint8_t)(addr)) != ACK_OK) {
                status = ERROR; // 操作失败
            }
        }
    }
    status = SUCCESS; // 操作成功
    return status;
}

/*************************************************************************************************************************************
 *	函 数 名:	SCCB_WriteReg_16Bit
 *
 *	入口参数:	addr - 要写入的寄存器(16位地址)  value - 要写入的数据
 *
 *	返 回 值:	SUCCESS - 操作成功，ERROR	  - 操作失败
 *
 *	函数功能:	对指定的寄存器(16位地址)写一字节数据，OV5640用到
 ************************************************************************************************************************************/

static uint8_t SCCB_WriteReg_16Bit(uint16_t addr, uint8_t value)
{
    uint8_t status;

    SCCB_Start(); // 启动IIC通讯

    if (SCCB_WriteHandle_16Bit(addr) == SUCCESS) // 写入要操作的寄存器
    {
        if (SCCB_WriteByte(value) != ACK_OK) // 写数据
        {
            status = ERROR;
        }
    }
    SCCB_Stop(); // 停止通讯

    status = SUCCESS; // 写入成功
    return status;
}

/*************************************************************************************************************************************
 *	函 数 名:	SCCB_ReadReg_16Bit
 *
 *	入口参数:	addr - 要读取的寄存器(16位地址)
 *
 *	返 回 值:	读到的数据
 *
 *	函数功能:	对指定的寄存器(16位地址)读取一字节数据，OV5640用到
 ************************************************************************************************************************************/

static uint8_t SCCB_ReadReg_16Bit(uint16_t addr)
{
    uint8_t value = 0;

    SCCB_Start(); // 启动IIC通信

    if (SCCB_WriteHandle_16Bit(addr) == SUCCESS) // 写入要操作的寄存器
    {
        SCCB_Stop();  // 停止IIC通信
        SCCB_Start(); // 重新启动IIC通讯

        if (SCCB_WriteByte(OV5640_DEVICE_ADDRESS | 0X01) == ACK_OK) // 发送读命令
        {
            value = SCCB_ReadByte(0); // 读到最后一个数据时发送 非应答信号
        }
        SCCB_Stop(); // 停止IIC通信
    }

    return value;
}

/*************************************************************************************************************************************
 *	函 数 名:	SCCB_WriteBuffer_16Bit
 *
 *	入口参数:	addr - 要写入的寄存器(16位地址)  *pData - 数据区   size - 要传输数据的大小
 *
 *	返 回 值:	SUCCESS - 操作成功，ERROR	  - 操作失败
 *
 *	函数功能:	对指定的寄存器(16位地址)批量写数据，OV5640 写入自动对焦固件时用到
 ************************************************************************************************************************************/
static uint8_t SCCB_WriteBuffer_16Bit(uint16_t addr, uint8_t* pData, uint32_t size)
{
    uint8_t  status;
    uint32_t i;

    SCCB_Start(); // 启动IIC通讯

    if (SCCB_WriteHandle_16Bit(addr) == SUCCESS) // 写入要操作的寄存器
    {
        for (i = 0; i < size; i++) {
            SCCB_WriteByte(*pData); // 写数据
            pData++;
        }
    }
    SCCB_Stop(); // 停止通讯

    status = SUCCESS; // 写入成功
    return status;
}
/********************************************************************************************/

/**
 *@brief DMA初始化配置
 */
void OV5640_DMA_Init(void)
{
    __HAL_RCC_DMA2_CLK_ENABLE(); // 使能DMA2时钟

    DMA_Handle_dcmi.Instance                 = DMA2_Stream7;            // DMA2数据流7
    DMA_Handle_dcmi.Init.Request             = DMA_REQUEST_DCMI;        // DMA请求来自DCMI
    DMA_Handle_dcmi.Init.Direction           = DMA_PERIPH_TO_MEMORY;    // 外设到存储器模式
    DMA_Handle_dcmi.Init.PeriphInc           = DMA_PINC_DISABLE;        // 外设地址禁止自增
    DMA_Handle_dcmi.Init.MemInc              = DMA_MINC_ENABLE;         // 存储器地址自增
    DMA_Handle_dcmi.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;     // DCMI数据位宽，32位
    DMA_Handle_dcmi.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;     // 存储器数据位宽，32位
    DMA_Handle_dcmi.Init.Mode                = DMA_CIRCULAR;            // 循环模式
    DMA_Handle_dcmi.Init.Priority            = DMA_PRIORITY_LOW;        // 优先级低
    DMA_Handle_dcmi.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;     // 使能fifo
    DMA_Handle_dcmi.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL; // 全fifo模式，4*32bit大小
    DMA_Handle_dcmi.Init.MemBurst            = DMA_MBURST_SINGLE;       // 单次传输
    DMA_Handle_dcmi.Init.PeriphBurst         = DMA_PBURST_SINGLE;       // 单次传输

    HAL_DMA_Init(&DMA_Handle_dcmi);                     // 配置DMA
    __HAL_LINKDMA(&hdcmi, DMA_Handle, DMA_Handle_dcmi); // 关联DCMI句柄

    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0); // 设置中断优先级
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);         // 使能中断
}
/**
 *@brief 延时函数
 */
void OV5640_Delay(uint32_t Delay)
{
    volatile uint16_t i;

    while (Delay--) {
        for (i = 0; i < 20000; i++);
    }
}
/**
 *@brief 摄像头模块初始化函数
 */
int8_t DCMI_OV5640_Init(void)
{
    uint16_t Device_ID;          // 存储设备ID
    SCCB_GPIO_Config();          // SCCB IO初始化
    MX_DCMI_Init();              // 初始化DCMI配置
    OV5640_DMA_Init();           // 初始化DMA
    OV5640_Reset();              // 执行复位操作
    Device_ID = OV5640_ReadID(); // 读取设备ID
    if (Device_ID == 0x5640)     // 比较匹配
    {
        printf("OV5640 OK,ID:0x%X\r\n", Device_ID);                                   // 匹配成功
        OV5640_Config();                                                              // 配置摄像头参数
        OV5640_Set_Framesize(OV5640_Width, OV5640_Height);                            // 	设置OV5640图像大小
        OV5640_DCMI_Crop(Display_Width, Display_Height, OV5640_Width, OV5640_Height); // 设置图像预览对应屏幕的大小 ,JPEG模式下要裁剪
        return OV5640_Success;                                                        // 返回初始化成功的标志
    } else {
        printf("OV5640 ERROR!!!!!  ID:%X\r\n", Device_ID); // 读取ID失败
        return OV5640_Error;                               // 返回初始化失败的标志
    }
}
/**
 *@brief 使用DMA循环传输模式获取连续图像数据
 */
void OV5640_DMA_Transmit_Continuous(uint32_t DMA_Buffer, uint32_t DMA_BufferSize)
{
    DMA_Handle_dcmi.Init.Mode = DMA_CIRCULAR; // 循环模式
    HAL_DMA_Init(&DMA_Handle_dcmi);           // 初始化DMA
    HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)DMA_Buffer, DMA_BufferSize);
}
/**
 *@brief 使用DMA正常模式抓拍一张图像数据
 */
void OV5640_DMA_Transmit_Snapshot(uint32_t DMA_Buffer, uint32_t DMA_BufferSize)
{
    DMA_Handle_dcmi.Init.Mode = DMA_NORMAL; // 正常模式
    HAL_DMA_Init(&DMA_Handle_dcmi);         // 初始化DMA
    HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t)DMA_Buffer, DMA_BufferSize);
}
/**
 *@brief 暂停DCMI数据采集 ,仅停止DMA
 */
void OV5640_DCMI_Suspend(void)
{
    HAL_DCMI_Suspend(&hdcmi);
}
/**
 *@brief 继续DCMI图像获取 ,恢复DMA
 */
void OV5640_DCMI_Resume(void)
{
	(&hdcmi)->State = HAL_DCMI_STATE_BUSY;	   // 变更DCMI标志
	(&hdcmi)->Instance->CR |= DCMI_CR_CAPTURE; // 开启DCMI捕获
}
/**
 *@brief 停止DCMI采集数据 ,关闭DMA和DCMI外设
 */
void OV5640_DCMI_Stop(void)
{
    HAL_DCMI_Stop(&hdcmi);
}
/**
 *@brief 设置摄像头的图像预览区域裁剪
 */
int8_t OV5640_DCMI_Crop(uint16_t Displey_XSize, uint16_t Displey_YSize, uint16_t Sensor_XSize, uint16_t Sensor_YSize)
{
    uint16_t DCMI_X_Offset, DCMI_Y_Offset; // 水平起始偏移量 ,图像宽度和传感器图像宽度之差
    uint16_t DCMI_CAPCNT;                  // 水平有效像素数
    uint16_t DCMI_VLINE;                   // 垂直有效像素数
    if (Displey_XSize >= Sensor_XSize || Displey_YSize >= Sensor_YSize) {
        return OV5640_Error; // 如果实际显示宽度大于或者等于图像的宽度
    }
    DCMI_X_Offset = Sensor_XSize - Displey_XSize;
    DCMI_Y_Offset = (Sensor_YSize - Displey_YSize) / 2 - 1;
    DCMI_CAPCNT   = Displey_XSize * 2 - 1; // RGB565格式,每像素占2个字节
    DCMI_VLINE    = Displey_YSize - 1;

    HAL_DCMI_ConfigCrop(&hdcmi, DCMI_X_Offset, DCMI_Y_Offset, DCMI_CAPCNT, DCMI_VLINE); // 配置裁剪参数
    HAL_DCMI_EnableCrop(&hdcmi);
    return OV5640_Success;
}
/**
 *@brief 执行复位摄像头的操作
 */
void OV5640_Reset(void)
{
    OV5640_Delay(30);
    OV5640_PWDN_OFF;
    OV5640_Delay(5);
    OV5640_Delay(20);
    SCCB_WriteReg_16Bit(0x3103, 0x11); // 设置像素寄存器内部寄存器 ,复位期间要保持在待机模式下
    SCCB_WriteReg_16Bit(0x3008, 0x82); // 执行一次复位
    OV5640_Delay(5);
}
/**
 *@brief 读取摄像头的设备ID
 */
uint16_t OV5640_ReadID(void)
{
    uint8_t PID_H, PID_L; // 存储ID

    PID_H = SCCB_ReadReg_16Bit(OV5640_ChipID_H); // 读取ID高字节
    PID_L = SCCB_ReadReg_16Bit(OV5640_ChipID_L); // 读取ID低字节

    return (PID_H << 8) | PID_L;
}
/**
 *@brief 配置摄像头参数
 */
void OV5640_Config(void)
{
    uint32_t i;
    for (i = 0; i < (sizeof(OV5640_INIT_Config) / 4); i++) {
        SCCB_WriteReg_16Bit(OV5640_INIT_Config[i][0], OV5640_INIT_Config[i][1]); // 写设置寄存器
        OV5640_Delay(1);
    }
}
/**
 *@brief 设置摄像头图像像素格式
 */
void OV5640_Set_Pixformat(uint8_t pixformat)
{
    uint8_t OV5640_Reg;
    if (pixformat == Pixformat_JPEG) {
        SCCB_WriteReg_16Bit(OV5640_FORMAT_CONTROL, 0x30);     // 设置数据输出接口的数据格式
        SCCB_WriteReg_16Bit(OV5640_FORMAT_CONTROL_MUX, 0x00); // ISP的内部格式
        SCCB_WriteReg_16Bit(OV5640_JPEG_MODE_SELECT, 0x02);   // 设置JPEG模式2
        SCCB_WriteReg_16Bit(OV5640_JPEG_VFIFO_CTRL00, 0xA0);
        SCCB_WriteReg_16Bit(OV5640_JPEG_VFIFO_HSIZE_H, OV5640_Width >> 8);      // 设置JPEG图像水平边长 字节
        SCCB_WriteReg_16Bit(OV5640_JPEG_VFIFO_HSIZE_L, (uint8_t)OV5640_Width);  // 设置JPEG图像水平边长 字节
        SCCB_WriteReg_16Bit(OV5640_JPEG_VFIFO_VSIZE_H, OV5640_Height >> 8);     // 设置JPEG图像垂直边长 字节
        SCCB_WriteReg_16Bit(OV5640_JPEG_VFIFO_VSIZE_L, (uint8_t)OV5640_Height); // 设置JPEG图像垂直边长 字节
    } else if (pixformat == Pixformat_GRAY) {
        SCCB_WriteReg_16Bit(OV5640_FORMAT_CONTROL, 0x10);     // 设置数据输出接口的数据格式
        SCCB_WriteReg_16Bit(OV5640_FORMAT_CONTROL_MUX, 0x00); // ISP的内部格式
    } else                                                    // RGB565
    {
        SCCB_WriteReg_16Bit(OV5640_FORMAT_CONTROL, 0x6F);     // 设置为RGB565格式 ,即G[2:0]B[4:0], R[4:0]G[5:3]
        SCCB_WriteReg_16Bit(OV5640_FORMAT_CONTROL_MUX, 0x01); // ISP的内部格式
    }
    OV5640_Reg = SCCB_ReadReg_16Bit(0x3821); // 读取寄存器值 ,Bit[5]=1 用于判断是否使用JPEG模式
    SCCB_WriteReg_16Bit(0x3821, (OV5640_Reg & 0xDF) | ((pixformat == Pixformat_JPEG) ? 0x20 : 0x00));
    OV5640_Reg = SCCB_ReadReg_16Bit(0x3002); // 读取寄存器值 ,Bit[7]、Bit[4]、Bit[2] 设置ISP中是否使用VFIFO、JFIFO、JPG
    SCCB_WriteReg_16Bit(0x3002, (OV5640_Reg & 0xE3) | ((pixformat == Pixformat_JPEG) ? 0x00 : 0x1C));
    OV5640_Reg = SCCB_ReadReg_16Bit(0x3006); // 读取寄存器值 ,Bit[5]、Bit[3] 设置JPEG时是否使用JPG
    SCCB_WriteReg_16Bit(0x3006, (OV5640_Reg & 0xD7) | ((pixformat == Pixformat_JPEG) ? 0x28 : 0x00));
}
/**
 *@brief 设置JPEG图像压缩质量
 */
void OV5640_Set_JPEG_QuantizationScale(uint8_t scale)
{
    SCCB_WriteReg_16Bit(0x4407, scale);
}
/**
 *@brief 设置摄像头图像大小
 */
int8_t OV5640_Set_Framesize(uint16_t width, uint16_t height)
{ // OV5640的像素配置需要按照一定的组别操作
    SCCB_WriteReg_16Bit(OV5640_GroupAccess, 0X03);
    SCCB_WriteReg_16Bit(OV5640_TIMING_DVPHO_H, width >> 8); // 水平像素高位
    SCCB_WriteReg_16Bit(OV5640_TIMING_DVPHO_L, width & 0xff);
    SCCB_WriteReg_16Bit(OV5640_TIMING_DVPVO_H, height >> 8); // 垂直像素高位
    SCCB_WriteReg_16Bit(OV5640_TIMING_DVPVO_L, height & 0xff);
    SCCB_WriteReg_16Bit(OV5640_GroupAccess, 0X13);
    SCCB_WriteReg_16Bit(OV5640_GroupAccess, 0Xa3);
    return OV5640_Success;
}
/**
 *@brief 设置图像水平镜像模式
 */
int8_t OV5640_Set_Horizontal_Mirror(int8_t ConfigState)
{
    uint8_t OV5640_Reg;
    OV5640_Reg = SCCB_ReadReg_16Bit(OV5640_TIMING_MIRROR);
    if (ConfigState == OV5640_Enable) { // 启用水平镜像
        OV5640_Reg |= 0X06;
    } else { // 禁用水平镜像
        OV5640_Reg &= 0xF9;
    }
    return SCCB_WriteReg_16Bit(OV5640_TIMING_MIRROR, OV5640_Reg);
}
/**
 *@brief 设置图像垂直翻转模式
 */
int8_t OV5640_Set_Vertical_Flip(int8_t ConfigState)
{
    uint8_t OV5640_Reg;
    OV5640_Reg = SCCB_ReadReg_16Bit(OV5640_TIMING_FLIP);
    if (ConfigState == OV5640_Enable) { // 启用垂直翻转
        OV5640_Reg |= 0X06;
    } else { // 禁用垂直翻转
        OV5640_Reg &= 0xF9;
    }
    return SCCB_WriteReg_16Bit(OV5640_TIMING_FLIP, OV5640_Reg);
}
/**
 *@brief 设置图像亮度
 */
void OV5640_Set_Brightness(int8_t Brightness)
{
    Brightness = Brightness + 4; // 增加亮度范围
    SCCB_WriteReg_16Bit(OV5640_GroupAccess, 0X03);
    SCCB_WriteReg_16Bit(0x5587, OV5640_Brightness_Config[Brightness][0]);
    SCCB_WriteReg_16Bit(0x5588, OV5640_Brightness_Config[Brightness][1]);
    SCCB_WriteReg_16Bit(OV5640_GroupAccess, 0X13);
    SCCB_WriteReg_16Bit(OV5640_GroupAccess, 0Xa3);
}
/**
 *@brief 设置图像对比度
 */
void OV5640_Set_Contrast(int8_t Contrast)
{
    Contrast = Contrast + 3; // 设置对比度范围
    SCCB_WriteReg_16Bit(OV5640_GroupAccess, 0X03);
    SCCB_WriteReg_16Bit(0x5586, OV5640_Contrast_Config[Contrast][0]);
    SCCB_WriteReg_16Bit(0x5585, OV5640_Contrast_Config[Contrast][1]);
    SCCB_WriteReg_16Bit(OV5640_GroupAccess, 0X13);
    SCCB_WriteReg_16Bit(OV5640_GroupAccess, 0Xa3);
}
/**
 *@brief 设置图像效果模式
 */
void OV5640_Set_Effect(uint8_t effect_Mode)
{
    SCCB_WriteReg_16Bit(OV5640_GroupAccess, 0X03);
    SCCB_WriteReg_16Bit(0x5580, OV5640_Effect_Config[effect_Mode][0]);
    SCCB_WriteReg_16Bit(0x5583, OV5640_Effect_Config[effect_Mode][1]);
    SCCB_WriteReg_16Bit(0x5584, OV5640_Effect_Config[effect_Mode][2]);
    SCCB_WriteReg_16Bit(0x5003, OV5640_Effect_Config[effect_Mode][3]);
    SCCB_WriteReg_16Bit(OV5640_GroupAccess, 0X13);
    SCCB_WriteReg_16Bit(OV5640_GroupAccess, 0Xa3);
}
/**
 *@brief 下载自动聚焦固件到摄像头
 */
int8_t OV5640_AF_Download_Firmware(void)
{
    uint8_t AF_Status=0;			   // 状态
	uint16_t i=0;					   // 计数器
	uint16_t OV5640_MCU_Addr=0x8000; // OV5640 MCU 内存存储起始地址为 0x8000 总共4KB

	SCCB_WriteReg_16Bit(0x3000, 0x20); // Bit[5] 设置之前下载 固件前需要执行复位操作
	SCCB_WriteBuffer_16Bit(OV5640_MCU_Addr,(uint8_t*)OV5640_AF_Firmware,sizeof(OV5640_AF_Firmware));
	SCCB_WriteReg_16Bit(0x3000, 0x00); // Bit[1] =0 使能MCU
	for(i=0;i<100;i++){	// 在下载固件之后 ,再查询100次的状态 ,每次1毫秒
		AF_Status = SCCB_ReadReg_16Bit(OV5640_AF_FW_STATUS); // 读取固件的状态
		if(AF_Status == 0x7E){
			printf("AF固件启动....\r\n");
		}
		if(AF_Status == 0x70){ // 当自动聚焦开始时 ,状态为0x70 ,表示自动聚焦固件成功被加载
			printf("AF固件下载成功.\r\n");
			return OV5640_Success;
		}
	}
	printf("自动聚焦固件下载失败...错误......\r\n");
	return OV5640_Error;
}
/**
 *@brief 获取自动聚焦状态
 */
int8_t OV5640_AF_QueryStatus(void)
{
	uint8_t AF_Status=0; // 状态
	AF_Status = SCCB_ReadReg_16Bit(OV5640_AF_FW_STATUS);// 读取固件的状态
	if(AF_Status == 0x10 || AF_Status == 0x20){ // 检查固件的聚焦模式
		return OV5640_AF_End;// 释放标志
	}
	else{
		return OV5640_AF_Focusing;// 第二聚焦标志
	}
}
/**
 *@brief  执行持续自动聚焦 ,一般不需要手动触发
 */
void OV5640_AF_Trigger_Constant(void)
{
	SCCB_WriteReg_16Bit(0x3022, 0x04);		
}
/**
 *@brief 单次自动聚焦
 */
void OV5640_AF_Trigger_Single(void)
{
	SCCB_WriteReg_16Bit(OV5640_AF_CMD_MAIN, 0x03);
}
/**
 *@brief 释放自动聚焦焦点 ,将主镜头复位到默认的远点
 */
void OV5640_AF_Release(void)
{
	SCCB_WriteReg_16Bit(OV5640_AF_CMD_MAIN, 0x08);
}

/**
 *@brief DCMI接收帧结束中断回调
 */
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	static uint32_t DCMI_Tick = 0;		 // 记录当前时间的数值
	static uint8_t DCMI_Frame_Count = 0; // 帧计数器
	if(HAL_GetTick()-DCMI_Tick >= 1000){// 每1秒查询一次帧
		DCMI_Tick = HAL_GetTick();	// 记录最新的时间数值
		OV5640_FPS = DCMI_Frame_Count;// 设置刷新率
		DCMI_Frame_Count = 0;// 清零
	}	
	DCMI_Frame_Count ++;// 每接收一次帧中断 , 帧计数器自动+1
	OV5640_FrameState = 1;// 图像采集标志位置1
}
