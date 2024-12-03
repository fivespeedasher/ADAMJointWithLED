#ifndef ADAM_H
#define ADAM_H
#include <modbus.h>
#include <vector>
#include <string>
#include <functional>

using namespace std;

class ADAM
{
public:
    ADAM(string device, int baud, char check, int data_bit, int stop_bit, bool setDebug);
    ~ADAM();
    string device;
    int baud;
    int check;
    int data_bit;
    int stop_bit;
    bool setDebug;
    modbus_t *ctx;
    int connect();
    int disconnect();
    int retry_operation(const function<int()>& operation, int retry_count = 5, float delay = 1);
private:
    int slave_id = 1;
    int total_coils;
};

class ADAM4051 : public ADAM
{
public:
    ADAM4051(const ADAM& adam, int slave_id, int total_coils = 8);
    ~ADAM4051();
    vector<uint8_t> state_coils;
    int read_coils();
private:
    int slave_id;
    int total_coils;
    modbus_t *ctx;
    using ADAM::connect; // 防止派生类外部访问，using在派生类中用于改变基类成员的访问权限，不需写形参
    using ADAM::disconnect; 
};

class ADAM4168 : public ADAM
{
public:
    ADAM4168(const ADAM& adam, int slave_id, int total_channels, float duty_cycles);
    ~ADAM4168();
    int InitPulse(float duty_cycles);
    int SetMode(const vector<int>& PulseChannel);
    int StartPulse(const vector<int>& channels, uint16_t pulse_times);
    int SetDO(int channels, bool value);
    int SetDOs(int start, int nb, const uint8_t *src);
private:
    int slave_id;
    int total_channels;
    modbus_t *ctx;
    using ADAM::connect; // 防止派生类外部访问，using在派生类中用于改变基类成员的访问权限，不需写形参
    using ADAM::disconnect; 
};

class ADAM4068 : public ADAM
{
public:
    ADAM4068(const ADAM& adam, int slave_id, int total_coils);
    ~ADAM4068();
    vector<uint8_t> state_coils;
    int write_coil(int ch, bool flag);
    int write_coils(int start, int nb, const uint8_t *src);
private:
    int slave_id;
    int total_coils;
    modbus_t *ctx;
    using ADAM::connect; // 防止派生类外部访问，using在派生类中用于改变基类成员的访问权限，不需写形参
    using ADAM::disconnect; 
};

#endif