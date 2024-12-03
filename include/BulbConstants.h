#ifndef BULBCONSTANTS_H
#define BULBCONSTANTS_H

// slave_1 io定义
const int WARNING_LIGHT = 0; // 警示灯
const int STATUS_LIGHT_YELLOW = 2; // 状态灯（黄）
const int STATUS_LIGHT_BLUE = 3; // 状态灯（蓝）
const int STATUS_LIGHT_RED = 4; // 状态灯（红）
const int STATUS_LIGHT_WHITE = 5; // 状态灯（白）
const int STATUS_LIGHT_GREEN = 6; // 状态灯（绿）
const int CABIN_LIGHT = 7; // 舱内灯

// slave_2 io定义
const int FRONT_ROLLER_LEFT = 0; // 前辊筒左送
const int FRONT_ROLLER_RIGHT = 1; // 前辊筒右送
const int REAR_ROLLER_LEFT = 2; // 后辊筒左送
const int REAR_ROLLER_RIGHT = 3; // 后辊筒右送
const int LEFT_BLOCKER_DOWN = 4; // 左侧阻挡器落下
const int LEFT_BLOCKER_UP = 5; // 左侧阻挡器升起
const int RIGHT_BLOCKER_DOWN = 6; // 右侧阻挡器落下
const int RIGHT_BLOCKER_UP = 7; // 右侧阻挡器升起

// slave_3 io定义
// 0-1为左侧卷帘门升起、下降，2-3为右侧卷帘门升起、下降，4-7为喇叭输出通道1~4
const int LEFT_ROLLING_DOOR_UP = 0; // 左侧卷帘门升起
const int LEFT_ROLLING_DOOR_DOWN = 1; // 左侧卷帘门下降
const int RIGHT_ROLLING_DOOR_UP = 2; // 右侧卷帘门升起
const int RIGHT_ROLLING_DOOR_DOWN = 3; // 右侧卷帘门下降
const int HORN_1 = 4; // 喇叭输出通道1
const int HORN_2 = 5; // 喇叭输出通道2
const int HORN_3 = 6; // 喇叭输出通道3
const int HORN_4 = 7; // 喇叭输出通道4

// slave_4 io定义
//0 1 0 0 1 0 0 1 0 0 1 0 1 1 0 0 全下
//1 0 0 0 0 0 0 1 0 0 1 0 1 1 0 0 左上右下
//0 1 0 0 1 0 1 0 0 1 0 0 1 1 0 0 左下右上
const int LEFT_BLOCKER_UP_POSITION = 0; // 左阻挡升起到位
const int LEFT_BLOCKER_DOWN_POSITION = 1; // 左阻挡下落到位
const int LEFT_FRONT_SENSOR = 2; // 左前到位传感器
const int LEFT_REAR_SENSOR = 5; // 左后到位传感器
const int RIGHT_BLOCKER_UP_POSITION  = 6; // 右阻挡升起到位
const int RIGHT_BLOCKER_DOWN_POSITION = 7; // 右阻挡下落到位
const int RIGHT_FRONT_SENSOR = 8; // 右前到位传感器
const int RIGHT_REAR_SENSOR = 11; // 右后到位传感器

// slave_6 io定义
const int RUNNING_LIGHT = 0; // 行车灯
const int NEAR_LIGHT = 1; // 近光灯
const int FAR_LIGHT = 2; // 远光灯
const int TURNING_LEFT_LIGHT = 3; // 左转向灯
const int TURNING_RIGHT_LIGHT = 4; // 右转向灯
const int REVERSING_LIGHT = 5; // 倒车灯
const int BRAKE_LIGHT = 6; // 刹车灯

// 其他数量与逻辑号定义
const int BLINK = 3; // 转向灯闪烁次数
const bool FRONT = 0; // 前方辊筒
const bool REAR = 1; // 后方辊筒
const bool LEFT = 0; // 左转向
const bool RIGHT = 1; // 右转向



#endif // BULBCONSTANTS_H