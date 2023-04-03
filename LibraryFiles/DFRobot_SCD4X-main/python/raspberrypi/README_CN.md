# DFRobot_SCD4X
* [English Version](./README.md)

SCD4x(SCD40/SCD41)是传感公司的下一代微型二氧化碳传感器。该传感器基于光声传感原理和Sensirion的专利PAsens®和CMOSens®技术以无与伦比的价格和最小的外形系数提供高精度。贴片组装使传感器的集成成本和空间有效，结合最大的自由设计。内置SHT4x温湿度传感器实现片上信号补偿。

二氧化碳是室内空气质量的一个关键指标，因为高水平的二氧化碳会损害人类的认知能力和福祉。SCD4x智能通风系统能够以最节能和最人性化的方式调节通风。此外，室内空气质量监测仪等连接设备基于
SCD4x可以帮助在健康、高效的环境中保持低二氧化碳浓度。

![产品实物图](../../resources/images/SCD4X.png)


## 产品链接 (https://www.dfrobot.com.cn/search.php?keywords=SCD4X)
    SKU: SEN0536


## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性)
* [历史](#历史)
* [创作者](#创作者)


## 概述

* 光声传感器技术PASens®
* 大输出范围:0 ppm - 40 ' 000 ppm
* 大电源电压范围:2.4 - 5.5 V
* 高精度:±(40ppm + 5%)
* 数字I2C接口
* 集成温湿度传感器
* 低功率运行到< 0.4 mA平均@ 5 V, 1 meas。/ 5分钟


## 库安装

1. 下载库至树莓派，要使用这个库，首先要将库下载到Raspberry Pi，命令下载方法如下:<br>

```python
sudo git clone https://github.com/DFRobot/DFRobot_SCD4X
```

2. 打开并运行例程，要执行一个例程demo_x.py，请在命令行中输入python demo_x.py。例如，要执行 period_measure.py例程，你需要输入:<br>

```python
python period_measure.py
或 
python2 period_measure.py
或 
python3 period_measure.py
```


## 方法

```python

    ''''''''''''''''''''''''''' Init and reset '''''''''''''''''''''''''''

    '''!
      @brief 初始化函数
      @return  返回初始化状态
      @retval True 表示初始化成功
      @retval False 表示初始化成失败
    '''
    @property
    def begin(self):

    '''!
      @brief 设置传感器休眠或唤醒(SCD41 only)
      @param mode - 休眠唤醒模式:
      @n       SCD4X_POWER_DOWN : 将传感器从闲置状态切换到休眠状态，以减少电流消耗
      @n       SCD4X_WAKE_UP : 唤醒传感器从休眠模式到空闲模式
      @note 注意， SCD4x不会答复wake_up命令。 命令执行时间 : 20毫秒
      @n 执行此功能时，传感器不能处于周期测量模式
    '''
    def set_sleep_mode(self, mode):

    '''!
      @brief 执行自我测试
      @details perform_self_test特性可以用作行尾测试，检查传感器功能和客户对传感器的供电。
      @return 模块状态:
      @n        0 : 没有发现故障
      @n        other : 检测到故障
      @note 命令执行时间 : 10000 ms
      @n 执行此功能时，传感器不能处于周期测量模式
    '''
    @property
    def perform_self_test(self):

    '''!
      @brief 模块 重初始化
      @details  reinit命令通过从EEPROM重新加载用户设置来重新初始化传感器。
      @note 如果reinit命令没有触发所需的重新初始化，则应该对SCD4x应用电源循环。
      @n 命令执行时间 : 20 ms
      @n 执行此功能时，传感器不能处于周期测量模式
    '''
    @property
    def module_reinit(self):

    '''!
      @brief 执行工厂复位
      @details perform_factory_reset命令重置存储在EEPROM中的所有配置设置，并擦除FRC和ASC算法的历史记录。
      @note 命令执行时间 : 1200 ms
      @n 执行此功能时，传感器不能处于周期测量模式
    '''
    @property
    def perform_factory_reset(self):

  ''''''''''''''''''''''''''' 测量函数 '''''''''''''''''''''''''''

    '''!
      @brief 单发测量(只适用于SCD41)
      @details  按需测量二氧化碳浓度，相对湿度和温度。
      @n  依然通过是 readMeasurement(sSensorMeasurement_t data) 接口获取测量数据
      @param mode - 单次测量数据模式 :
      @n       SCD4X_MEASURE_SINGLE_SHOT : 按需测量二氧化碳浓度，相对湿度和温度。 最大命令持续时间 5000 [ms].
      @n       SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY : 按需测量相对湿度和温度。最大命令持续时间 50 [ms].
      @note 在 SCD4X_MEASURE_SINGLE_SHOT_RHT_ONLY 模式下, 二氧化碳测量值为 0 ppm。
      @n 执行此功能时，传感器不能处于周期测量模式
    '''
    def measure_single_shot(self, mode):

    '''!
      @brief 设置周期测量模式
      @param mode - 定期测量模式:
      @n       SCD4X_START_PERIODIC_MEASURE : 开始周期性测量，信号更新间隔为5秒
      @n       SCD4X_STOP_PERIODIC_MEASURE : 停止周期测量命令
      @n       SCD4X_START_LOW_POWER_MEASURE : 启动低功耗周期测量，信号更新间隔约为30秒
      @note 在对传感器进行设置的时候, 必须停止测量模式； 且在发出stop_periodic_measurement命令后, 传感器等待500ms后才响应其他命令。
    '''
    def enable_period_measure(self, mode):

    '''!
      @brief 读取测量数据
      @return 传感器测量值
      @retval CO2ppm - CO2浓度(ppm)
      @retval temp - 温度(C)
      @retval humidity - 湿度(RH)
      @note CO2测量范围: 0~40000 ppm; 温度测量范围: -10~60 C; 湿度测量范围: 0~100 %RH.
    '''
    @property
    def read_measurement(self):

    '''!
      @brief 获取数据准备状态
      @return 数据就绪状态:
      @n        True : 数据就绪
      @n        False : 数据未就绪
    '''
    @property
    def get_data_ready_status(self):

  ''''''''''''''''''''''''''' 补偿和校准 '''''''''''''''''''''''''''

    '''!
      @brief 设定温度补偿值
      @details T(offset_actual) = T(SCD4X) - T(reference) + T(offset_previous)
      @n T(offset_actual) : 计算出的实际所需温度偏移量
      @n T(SCD4X) : 传感器测量出来的温度（需等待一段时间稳定后的读数）
      @n T(reference) : 当前环境的温度标准参考值
      @n T(offset_previous) : 之前设置的温度偏移量
      @n 例如 : 32(传感器测量出来的温度) - 30(当前环境的温度标准参考值) + 2(之前设置的温度偏移量) = 4(计算出的实际所需温度偏移量)
      @param temp_comp - 温度偏移值，单位C
      @note 执行此功能时，传感器不能处于周期测量模式
    '''
    def set_temp_comp(self, temp_comp):

    '''!
      @brief 获取温度补偿值
      @return 当前设置的温度补偿值, 单位C
      @note 执行此功能时，传感器不能处于周期测量模式
    '''
    @property
    def get_temp_comp(self):

    '''!
      @brief 设置传感器当前所处环境的海拔值
      @param altitude - 传感器当前所处环境的海拔, 单位m
      @note 执行此功能时，传感器不能处于周期测量模式
    '''
    def set_sensor_altitude(self, altitude):

    '''!
      @brief 获取当前设置的环境海拔
      @return 当前设置的环境海拔, 单位m
      @note 执行此功能时，传感器不能处于周期测量模式
    '''
    @property
    def get_sensor_altitude(self):

    '''!
      @brief 设置传感器当前所处环境的大气压强
      @param ambient_pressure - 传感器当前所处环境的大气压强, 单位Pa
    '''
    def set_ambient_pressure(self, ambient_pressure):

    '''!
      @brief 执行强制校准
      @param CO2ppm - 目标二氧化碳浓度，单位ppm
      @return 校准的幅度, 校准失败返回(int16_t)0x7fff
      @note 命令执行时间 : 400 ms
      @n 执行此功能时，传感器不能处于周期测量模式
    '''
    def perform_forced_recalibration(self, CO2ppm):

    '''!
      @brief 设置自动自校准模式
      @param mode - 自动校准模式:
      @n       True : 开启自动校准
      @n       False : 关闭自动校准
      @note 执行此功能时，传感器不能处于周期测量模式
    '''
    def set_auto_calib_mode(self, mode):

    '''!
      @brief 获取自动自校准模式
      @return 自动校准模式:
      @n        True : 自动校准开启
      @n        False : 自动校准关闭
      @note 执行此功能时，传感器不能处于周期测量模式
    '''
    @property
    def get_auto_calib_mode(self):

    '''!
      @brief persist settings
      @brief 将配置参数保存在EEPROM中，默认是在RAM
      @details 如温度偏移、传感器高度和ASC使能/禁用参数等配置设置默认只存储在易失性内存(RAM)中，在上电周期后将丢失。
      @note 为了避免EEPROM的不必要损耗，只有在需要持久化并且对配置进行了实际更改时才应该发送persist_settings命令。
      @n EEPROM保证在发生故障前至少承受2000个写周期。
      @n 命令执行时间 : 800 ms
      @n 执行此功能时，传感器不能处于周期测量模式
    '''
    @property
    def persist_settings(self):

```


## 兼容性

* RaspberryPi 版本

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | ------- |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python 版本

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |


## 历史

- 2022/06/06 - 1.0.0 版本
- 2022/12/13 - 1.0.1 版本


## 创作者

Written by qsjhyy(yihuan.huang@dfrobot.com), 2022. (Welcome to our [website](https://www.dfrobot.com/))

