// 以DB2接入六个ADAM模块，以DB1接显示屏
#include "ControlRequest.h"
#include <iostream>
#include <chrono> // 用于时间戳
using namespace std;
ControlRequest::ControlRequest( ADAM4068& adam_1, ADAM4068& adam_2, ADAM4068& adam_3, ADAM4051& adam_4, ADAM4051& adam_5, ADAM4168& adam_6)
    : adam_1(adam_1), adam_2(adam_2), adam_3(adam_3), 
    adam_4(adam_4), adam_5(adam_5), adam_6(adam_6) {}
ControlRequest::~ControlRequest() {}

// 控制警示灯
int ControlRequest::controlWarningLight(bool state) {
    adam_1.write_coil(WARNING_LIGHT, state);
    return 0;
}

/**
 * @brief 控制状态灯
 * 
 * @param color 可选 STATUS_LIGHT_YELLOW, STATUS_LIGHT_BLUE, STATUS_LIGHT_RED, STATUS_LIGHT_WHITE, STATUS_LIGHT_GREEN
 * @return int 
 */
int ControlRequest::controlStatusLight(int color) {
    // stopStatusLight();
    adam_1.write_coil(color, true);
    return 0;
}

// 关闭状态灯
int ControlRequest::stopStatusLight() {
    // 多线圈写入
    vector<uint8_t> coils(5, 0);
    adam_1.write_coils(STATUS_LIGHT_YELLOW, 5, coils.data());
    return 0;
}

// 控制舱内灯
int ControlRequest::controlCabinLight(bool state) {
    adam_1.write_coil(CABIN_LIGHT, state);
    return 0;
}

/**
 * @brief 启动辊筒朝左送(不包括升起阻挡)
 * 
 * @param whichRoller 前辊筒0，后辊筒1
 * @return int 
 */
int ControlRequest::RollingToLeft(bool whichRoller){
    if (whichRoller == FRONT) {
        // 前辊筒左送 1 0
        adam_2.write_coil(FRONT_ROLLER_LEFT, true);
        
    } else {
        // 后辊筒左送
        adam_2.write_coil(REAR_ROLLER_LEFT, true);
    }
    return 0;
}
/**
 * @brief 启动辊筒朝右送(不包括升起阻挡)
 * 
 * @param whichRoller 前辊筒0，后辊筒1
 * @return int 
 */
int ControlRequest::RollingToRight(bool whichRoller){
    if (whichRoller == FRONT) {
        // 前辊筒右送 0 1
        adam_2.write_coil(FRONT_ROLLER_RIGHT, true);
        
    } else {
        // 后辊筒右送
        adam_2.write_coil(REAR_ROLLER_RIGHT, true);
    }
    return 0;
}
/**
 * @brief 控制货物往左输送(包括左侧阻挡升起)
 * 
 * @param whichRoller 前辊筒0，后辊筒1
 * @return int 
 */
int ControlRequest::controlDeliverToLeft(bool whichRoller) {
    // 左方阻挡升起 0 1
    // adam_2.write_coil(LEFT_BLOCKER_DOWN, false);
    // adam_2.write_coil(LEFT_BLOCKER_UP, true);
    vector<uint8_t> coils = {0, 1}; // {LEFT_BLOCKER_DOWN, LEFT_BLOCKER_UP}
    adam_2.write_coils(LEFT_BLOCKER_DOWN, 2, coils.data());

    // 根据阻挡到位关DO
    if(waitForPositionSensorState(LEFT_BLOCKER_UP_POSITION, 1) != -1) {
        sleep(1);
        // 左方阻挡升起到位
        adam_2.write_coil(LEFT_BLOCKER_UP, false); // 已到位，关闭输出
        ControlRequest::RollingToLeft(whichRoller);
        return 0;
    }
    else { return -1; }
}

int ControlRequest::waitForPositionSensorState(int PositionSensor, bool state_shouldBe) {
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::seconds(4); // 设置等待时间为4秒

    while (std::chrono::steady_clock::now() < end) {
        bool state = ControlRequest::readPositionSensor()[PositionSensor];
        if (state == state_shouldBe) {
            return 0;
        }
        sleep(0.5); // 每500毫秒检查一次
    }
    return -1; // 超时未达到期望状态
}

/**
 * @brief 控制货物往右输送(包括右侧阻挡升起)
 * 
 * @param whichRoller  前辊筒0，后辊筒1
 * @return int 
 */
int ControlRequest::controlDeliverToRight(bool whichRoller) {
    // 右方阻挡升起 0 1
    // adam_2.write_coil(RIGHT_BLOCKER_DOWN, false);
    // adam_2.write_coil(RIGHT_BLOCKER_UP, true);
    vector<uint8_t> coils = {0, 1}; // {RIGHT_BLOCKER_DOWN, RIGHT_BLOCKER_UP}
    adam_2.write_coils(RIGHT_BLOCKER_DOWN, 2, coils.data());

    if(waitForPositionSensorState(RIGHT_BLOCKER_UP_POSITION, 1) != -1) {
        // 右方阻挡升起到位
        sleep(1);
        adam_2.write_coil(RIGHT_BLOCKER_UP, false); // 已到位，关闭输出
        ControlRequest::RollingToRight(whichRoller);
        return 0;
    }
    else {
        return -1;
    }
}

// 停止辊筒输送(不包括下放阻挡)
int ControlRequest::stopRolling() {
    vector<uint8_t> coils(4, 0); // {FRONT_ROLLER_LEFT, FRONT_ROLLER_RIGHT, REAR_ROLLER_LEFT, REAR_ROLLER_RIGHT}
    adam_2.write_coils(FRONT_ROLLER_LEFT, 4, coils.data());
    return 0;
}

// 下放阻挡
int ControlRequest::blockerDown() {
    vector<uint8_t> coils(4, 0); // {LEFT_BLOCKER_DOWN, RIGHT_BLOCKER_UP, RIGHT_BLOCKER_DOWN, RIGHT_BLOCKER_UP}
    coils[LEFT_BLOCKER_DOWN - LEFT_BLOCKER_DOWN] = 1;
    coils[RIGHT_BLOCKER_DOWN - LEFT_BLOCKER_DOWN] = 1;
    adam_2.write_coils(LEFT_BLOCKER_DOWN, 4, coils.data());
    // 左阻挡落下到位
    if(waitForPositionSensorState(LEFT_BLOCKER_DOWN_POSITION, 1) != -1) {
        sleep(1);
        adam_2.write_coil(LEFT_BLOCKER_DOWN, false);
    }
    else {return -1;}
    // 右阻挡落下到位
    if(waitForPositionSensorState(RIGHT_BLOCKER_DOWN_POSITION, 1) != -1) {
        sleep(1);
        adam_2.write_coil(RIGHT_BLOCKER_DOWN, false);
    }
    else {return -1;}

    return 0;
}



// 停止货物输送(包括停止辊筒、下放阻挡)
int ControlRequest::stopDeliver() {
    ControlRequest::stopRolling();
    sleep(1);
    ControlRequest::blockerDown();
    return 0;
}

// 升起左侧卷帘门
int ControlRequest::riseLeftRollingDoor() {
    adam_3.write_coil(LEFT_ROLLING_DOOR_DOWN, false);
    adam_3.write_coil(LEFT_ROLLING_DOOR_UP, true);
    // TODO 用到位传感器来停下IO输出
    return 0;
}

// 升起右侧卷帘门
int ControlRequest::riseRightRollingDoor() {
    adam_3.write_coil(RIGHT_ROLLING_DOOR_DOWN, false);
    adam_3.write_coil(RIGHT_ROLLING_DOOR_UP, true);
    // TODO 用到位传感器来停下IO输出
    return 0;
}

// 下放左侧卷帘门
int ControlRequest::downLeftRollingDoor() {
    adam_3.write_coil(LEFT_ROLLING_DOOR_UP, false);
    adam_3.write_coil(LEFT_ROLLING_DOOR_DOWN, true);
    // TODO 用到位传感器来停下IO输出
    return 0;
}

// 下放右侧卷帘门
int ControlRequest::downRightRollingDoor() {
    adam_3.write_coil(RIGHT_ROLLING_DOOR_UP, false);
    adam_3.write_coil(RIGHT_ROLLING_DOOR_DOWN, true);
    // TODO 用到位传感器来停下IO输出
    return 0;
}

// 停止卷帘门运动
int ControlRequest::stopRollingDoor() {
    adam_3.write_coil(LEFT_ROLLING_DOOR_UP, false);
    adam_3.write_coil(LEFT_ROLLING_DOOR_DOWN, false);
    adam_3.write_coil(RIGHT_ROLLING_DOOR_UP, false);
    adam_3.write_coil(RIGHT_ROLLING_DOOR_DOWN, false);
    return 0;
}

/**
 * @brief 打开喇叭
 * 
 * @param horn 通道数可选HORN_1~4
 * @return int 
 */
int ControlRequest::controlHorn(int horn) {
    // stopHorn();
    // adam_3.write_coil(horn, true);
    vector<uint8_t> coils(4, 0);
    coils[horn - HORN_1] = 1;
    adam_3.write_coils(HORN_1, 4, coils.data());
    return 0;
}
// 关闭喇叭
int ControlRequest::stopHorn() {
    vector<uint8_t> coils(4, 0);
    adam_3.write_coils(HORN_1, 4, coils.data());
    return 0;
}

// 读取并打印到位传感器状态
vector<bool> ControlRequest::readPositionSensor() {
    vector<bool> positionSensor;
    adam_4.read_coils();
    // cout << "Position Sensor : ";
    // 用vector的方法复制
    positionSensor.assign(adam_4.state_coils.begin(), adam_4.state_coils.end());

    // for (int i = 0; i < 16; i++) {
    //     // cout << positionSensor[i] << " "; // 可用于检查输出
    // }
    // cout << endl;
    return positionSensor;
}


/**
 * @brief 打开行车灯
 * 
 * @param state 0 关闭，1 打开
 */
int ControlRequest::controlDrivingLight(bool state) {
    adam_6.SetDO(RUNNING_LIGHT, state); 
    return 0;
}

/**
 * @brief 打开转向灯，闪烁三次
 * 
 * @param direction 0 左转向灯, 1 右转向灯
 */
int ControlRequest::controlTurningLight(bool direction) {
    vector<int> PulseChannel;
    if (direction) {
        PulseChannel = {TURNING_RIGHT_LIGHT};
    } else {
        PulseChannel = {TURNING_LEFT_LIGHT};
    }
    adam_6.SetMode(PulseChannel); // 打开转向
    // adam_6.StartPulse(PulseChannel, BLINK); // 闪烁三次
    return 0;
}

// 关闭转向灯
int ControlRequest::turnOffTurningLight() {
    adam_6.SetMode({}); // 关闭脉冲模式
    vector<uint8_t> DOs = {0, 0}; // {TURNING_LEFT_LIGHT, TURNING_RIGHT_LIGHT}
    adam_6.SetDOs(TURNING_LEFT_LIGHT, 2, DOs.data());
    return 0;
}

// 打开近光灯,会自动关闭远光灯
int ControlRequest::controlNearLight(bool state) {
    vector<uint8_t> DOs = {uint8_t(state), 0}; // TODO 优化形参 bool->uint8_t
    adam_6.SetDOs(NEAR_LIGHT, 2, DOs.data()); // {NEAR_LIGHT, FAR_LIGHT}
    return 0;
}
// 打开远光灯,会自动关闭近光灯
int ControlRequest::controlFarLight(bool state) {
    vector<uint8_t> DOs = {0, uint8_t(state)};
    adam_6.SetDOs(NEAR_LIGHT, 2, DOs.data()); // {NEAR_LIGHT, FAR_LIGHT}
    return 0;
}

// 打开倒车灯
int ControlRequest::controlReversingLight(bool state) {
    adam_6.SetDO(REVERSING_LIGHT, state); // 打开倒车灯
    return 0;
}
// 打开刹车灯
int ControlRequest::controlBrakeLight(bool state) {
    adam_6.SetDO(BRAKE_LIGHT, state); // 打开刹车灯
    return 0;
}

