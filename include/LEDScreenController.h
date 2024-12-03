#ifndef LEDSCREENCONTROLLER_H
#define LEDSCREENCONTROLLER_H
#include <vector>
#include "modbus.h"
#include <string>
#include <iostream>
#include <functional>

using namespace std;

std::vector<uint16_t> gbk_hex(const std::string& gbk_str);

/**
 * @brief 控制LED显示屏
 * 
 */
class LEDScreenController {
    public:
        LEDScreenController(const char *ip, int port, bool setDebug);
        ~LEDScreenController();
        int set_non_blocking(int fd);
        int connect();
        void disconnect();

        bool turnOn();
        bool turnOff();

        // 编写M4002~7用于设置亮度和指定显示页面
        bool setScreen(uint16_t save_state, uint16_t brightness, uint16_t page); 
        // 编写M4018~N用于写入文本
        bool writeText(const string& text, uint16_t save_state);
        int initLED();

        int retry_operation(const function<int()>& operation, int retry_count=5, float delay=0.5);

    private:
        const char *ip;
        int port;
        modbus_t* ctx;
        bool setDebug;
        // param
        uint16_t save_state;    // 0: 不保存状态，1: 保存状态
        uint16_t brightness; // 亮度
        uint16_t page;      // 操作的页面
        uint16_t zone;     // 操作的分区
        uint16_t align;    // 0: 左对齐，1: 居中，2: 右对齐
        uint16_t size;     // 字体大小
        uint16_t speaker; // 0为不发声，1为发声
        uint16_t color = 71; // 字体，71为红色
};

#endif // LEDSCREENCONTROLLER_H