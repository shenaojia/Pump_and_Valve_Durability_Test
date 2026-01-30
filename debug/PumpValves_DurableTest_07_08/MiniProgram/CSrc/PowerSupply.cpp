// GEN BLOCK BEGIN Include
#define TSMP_IMPL
#include "TSMaster.h"
#include "MPLibrary.h"
#include "Database.h"
#include "TSMasterBaseInclude.h"
#include "Configuration.h"
// GEN BLOCK END Include

// CODE BLOCK BEGIN Global_Definitions 
#include <iostream>
#include <vector>
#include <cstdint>
#include <windows.h>
#include <sstream>
#include <unordered_set>

#define DEBUG_MODE
#define IDX_ERR_COMM_CREATE_FAILED -1
#define IDX_ERR_COMM_CONNECT_FAILED -2

// 电源通讯协议常量定义（基于文档协议）
const uint8_t FRAME_HEADER = 0x7B;       // 帧头{insert\_element\_0\_}
const uint8_t FRAME_TAIL = 0x7D;         // 帧尾{insert\_element\_1\_}
const uint32_t BAUDRATE = 38400;         // 默认波特率{insert\_element\_2\_}
const uint8_t DEFAULT_ADDRESS = 0x01;    // 默认从机地址{insert\_element\_3\_}

class TPowerSupply {
private:
    static int nextHandle;
    static std::unordered_set<int> usedHandles; // 记录已使用的句柄?
    int handle;
    HANDLE hComm;
    uint8_t deviceAddress;
    bool _isConnected;  // 修改变量名，避免与函数冲突
    COMSTAT status;
    DWORD errors;


    // 计算校验和（总字节数+从机地址+命令的和，取低位字节）{insert\_element\_4\_}
    uint8_t calculateChecksum(const std::vector<uint8_t>& data) {
        uint16_t sum = 0;
        for(uint8_t byte : data) {
            sum += byte;
        }
        return static_cast<uint8_t>(sum & 0xFF);
    }

    // 构造命令帧（遵循文档帧格式）{insert\_element\_5\_}
    std::vector<uint8_t> buildFrame(uint8_t cmdType, uint8_t cmdCode, const std::vector<uint8_t>& params = {}) {
        // 总字节数=帧头(1)+总字节数(2)+地址(1)+命令类型(1)+命令字(1)+参数(n)+校验和(1)+帧尾(1)
        uint16_t totalLength = 1 + 2 + 1 + 1 + 1 + params.size() + 1 + 1;
        std::vector<uint8_t> frame;

        frame.push_back(FRAME_HEADER);  // 帧头
        frame.push_back(static_cast<uint8_t>(totalLength >> 8));  // 总字节数高字节
        frame.push_back(static_cast<uint8_t>(totalLength & 0xFF)); // 总字节数低字节
        frame.push_back(deviceAddress);  // 从机地址
        frame.push_back(cmdType);        // 命令类型
        frame.push_back(cmdCode);        // 命令字
        frame.insert(frame.end(), params.begin(), params.end()); // 参数
        // 计算校验和
        std::vector<uint8_t> checksumData;
        checksumData.push_back(static_cast<uint8_t>(totalLength >> 8));
        checksumData.push_back(static_cast<uint8_t>(totalLength & 0xFF));
        checksumData.push_back(deviceAddress);
        checksumData.push_back(cmdType);
        checksumData.push_back(cmdCode);
        checksumData.insert(checksumData.end(), params.begin(), params.end());
        frame.push_back(calculateChecksum(checksumData));
        frame.push_back(FRAME_TAIL);  // 帧尾

        return frame;
    }

    // 解析返回帧
    bool parseResponse(const std::vector<uint8_t>& response, uint8_t expectedCmdType, uint8_t expectedCmdCode, std::vector<uint8_t>& params) {
        if(response.empty() || response.front() != FRAME_HEADER || response.back() != FRAME_TAIL) {
            return false;
        }
        if(response.size() < 8) return false;
        if(response[3] != deviceAddress) return false;  // 验证从机地址
        if(response[4] == 0x99) {  // 错误类命令{insert\_element\_6\_}
            test_log_nok("错误代码: 0x%02X", response[6]);
            return false;
        }
        if(response[4] != expectedCmdType || response[5] != expectedCmdCode) {
            return false;
        }
        // 提取参数
        size_t paramStart = 6;
        size_t paramEnd = response.size() - 2;  // 排除校验和和帧尾
        params.assign(response.begin() + paramStart, response.begin() + paramEnd);
        return true;
    }

public:
    u8 PowerStatus;
    double PowerVoltage; // 电源电压变量
    double PowerCurrent; // 电源电流变量
    TPowerSupply(uint8_t address = DEFAULT_ADDRESS) {
        // 处理句柄
        while(usedHandles.find(nextHandle) != usedHandles.end()) {
            nextHandle++;
        }
        handle = nextHandle;
        usedHandles.insert(handle);
        nextHandle++;
        // 初始化class成员变量
        deviceAddress = address;
        _isConnected = false;
        hComm = INVALID_HANDLE_VALUE; // 初始化为无效句柄
        // 初始化电压和电流变量
        PowerVoltage = 0.0; // 初始化电压变量
        PowerCurrent = 0.0; // 初始化电流变量
        PowerStatus = 0;

        std::string strSysVarVoltage = "PowerSupply" + std::to_string(handle) + ".Voltage";
        if(0 != app.set_system_var_double(strSysVarVoltage.c_str(), 0.0)) {
            app.create_system_var(strSysVarVoltage.c_str(), svtDouble, "0.0", "电源电压");
        }

        std::string strSysVarCurrent = "PowerSupply" + std::to_string(handle) + ".Current";
        if(0 != app.set_system_var_double(strSysVarCurrent.c_str(), 0.0)) {
            app.create_system_var(strSysVarCurrent.c_str(), svtDouble, "0.0", "电源电流");
        }

        std::string strSysVarState = "PowerSupply" + std::to_string(handle) + ".State";
        if(0 != app.set_system_var_int32(strSysVarState.c_str(), 0)) {
            app.create_system_var(strSysVarState.c_str(), svtInt32, "0", "电源状态");
        }
    }

    ~TPowerSupply() {
        usedHandles.erase(handle);
        // 如果删除的是当前最大的句柄，更新nextHandle?
        if(handle == nextHandle - 1) {
            nextHandle--;
            // 检查是否有更小的未使用句柄?
            while(nextHandle > 0 && usedHandles.find(nextHandle - 1) == usedHandles.end()) {
                nextHandle--;
            }

        }
        if(_isConnected) {
            CloseHandle(hComm);
        }
    }

    // 检查连接状态（修改函数名与变量名一致）
    bool isConnected() const {
        return _isConnected;
    }

    bool connect(const char* portName) {
        if(_isConnected) {
            test_log_nok("已连接到电源设备");
            return true;
        }

        // 打开串口
        hComm = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if(hComm == INVALID_HANDLE_VALUE) {
            test_log_nok("无法打开串口: %s, 错误码: %lu", portName, GetLastError());
            return false;
        }

        // 配置串口参数（1起始位、8数据位、1停止位、无校验）{insert\_element\_7\_}
        DCB dcb = {0};
        dcb.DCBlength = sizeof(DCB);
        if(!GetCommState(hComm, &dcb)) {
            test_log_nok("获取串口状态失败");
            return false;
        }
        dcb.BaudRate = BAUDRATE;
        dcb.ByteSize = 8;
        dcb.StopBits = ONESTOPBIT;
        dcb.Parity = NOPARITY;
        if(!SetCommState(hComm, &dcb)) {
            test_log_nok("设置串口参数失败");
            CloseHandle(hComm);
            return false;
        }

        // 设置超时
        COMMTIMEOUTS timeouts = {0};
        timeouts.ReadIntervalTimeout = 500;
        timeouts.ReadTotalTimeoutConstant = 1000;
        timeouts.ReadTotalTimeoutMultiplier = 500;
        timeouts.WriteTotalTimeoutConstant = 500;
        timeouts.WriteTotalTimeoutMultiplier = 100;
        SetCommTimeouts(hComm, &timeouts);

        _isConnected = true;
        return true;

    }

    // 开启电源输出（控制类命令0x0F, 0xFF）{insert\_element\_8\_}
    bool turnOn() {
        if(!_isConnected) return false;

        std::vector<uint8_t> cmd = buildFrame(0x0F, 0xFF);
        DWORD bytesWritten;
        if(!WriteFile(hComm, cmd.data(), cmd.size(), &bytesWritten, NULL) || bytesWritten != cmd.size()) {
            test_log_nok("发送开启命令失败");
            return false;
        }

        // 读取响应
        std::vector<uint8_t> response(100);
        DWORD bytesRead;
        if(!ReadFile(hComm, response.data(), response.size(), &bytesRead, NULL) || bytesRead == 0) {
            test_log_nok("未收到开启命令响应");
            return false;
        }
        response.resize(bytesRead);

        std::vector<uint8_t> params;
        if(parseResponse(response, 0x0F, 0xFF, params)) {
            test_log_ok("电源开启成功");
            return true;
        }
        else {
            test_log_nok("电源开启失败");
            return false;
        }
    }

    // 关闭电源输出（控制类命令0x0F, 0x00）{insert\_element\_9\_}
    bool turnOff() {
        if(!_isConnected) return false;

        std::vector<uint8_t> cmd = buildFrame(0x0F, 0x00);
        DWORD bytesWritten;
        if(!WriteFile(hComm, cmd.data(), cmd.size(), &bytesWritten, NULL) || bytesWritten != cmd.size()) {
            test_log_nok("发送关闭命令失败");
            return false;
        }

        std::vector<uint8_t> response(100);
        DWORD bytesRead;
        if(!ReadFile(hComm, response.data(), response.size(), &bytesRead, NULL) || bytesRead == 0) {
            test_log_nok("未收到关闭命令响应");
            return false;
        }
        response.resize(bytesRead);

        std::vector<uint8_t> params;
        if(parseResponse(response, 0x0F, 0x00, params)) {
            test_log_ok("电源关闭成功");
            return true;
        }
        else {
            test_log_nok("电源关闭失败");
            return false;
        }
    }

    // 设置电压（设置类命令0x5A, 0x00）{insert\_element\_10\_}
    bool setVoltage(float voltage) {
        if(!_isConnected || voltage < 0) return false;

        // 转换电压为协议格式（乘以100，转为16位十六进制，高字节在前）{insert\_element\_11\_}
        uint16_t voltageVal = static_cast<uint16_t>(voltage * 100);
        std::vector<uint8_t> params = {
            static_cast<uint8_t>(voltageVal >> 8),
            static_cast<uint8_t>(voltageVal & 0xFF)
        };

        std::vector<uint8_t> cmd = buildFrame(0x5A, 0x00, params);
        DWORD bytesWritten;
        if(!WriteFile(hComm, cmd.data(), cmd.size(), &bytesWritten, NULL) || bytesWritten != cmd.size()) {
            test_log_nok("发送设置电压命令失败");
            return false;
        }

        std::vector<uint8_t> response(100);
        DWORD bytesRead;
        if(!ReadFile(hComm, response.data(), response.size(), &bytesRead, NULL) || bytesRead == 0) {
            test_log_nok("未收到设置电压响应");
            return false;
        }
        response.resize(bytesRead);

        std::vector<uint8_t> paramsResp;
        if(parseResponse(response, 0x5A, 0x00, paramsResp)) {
            test_log_ok("电压设置成功: %fV", voltage);
            return true;
        }
        else {
            test_log_nok("电压设置失败");
            return false;
        }
    }

    // 设置电流（设置类命令0x5A, 0x01）{insert\_element\_12\_}
    bool setCurrent(float current) {
        if(!_isConnected || current < 0) return false;

        // 转换电流值为协议格式（乘以100，转为16位无符号整数，高字节在前）
        uint16_t currentVal = static_cast<uint16_t>(current * 100);
        std::vector<uint8_t> params = {
            static_cast<uint8_t>(currentVal >> 8),    // 高字节
            static_cast<uint8_t>(currentVal & 0xFF)   // 低字节
        };

        // 构造设置类命令（0x5A, 0x01）
        std::vector<uint8_t> cmd = buildFrame(0x5A, 0x01, params);

        // 发送命令
        DWORD bytesWritten;
        if(!WriteFile(hComm, cmd.data(), cmd.size(), &bytesWritten, NULL) || bytesWritten != cmd.size()) {
            test_log_nok("发送设置电流命令失败");
            return false;
        }

        // 读取响应
        std::vector<uint8_t> response(100);
        DWORD bytesRead;
        if(!ReadFile(hComm, response.data(), response.size(), &bytesRead, NULL) || bytesRead == 0) {
            test_log_nok("未收到设置电流命令响应");
            return false;
        }
        response.resize(bytesRead);

        std::vector<uint8_t> respParams;
        if(parseResponse(response, 0x5A, 0x01, respParams) && !respParams.empty() && respParams[0] == 0x00) {
            test_log_ok("电流设置成功: %fA", current);
            return true;
        }
        else {
            test_log_nok("电流设置失败");
            return false;
        }
    }
    // 获取实际电压（查询类命令0xF0, 0x10）{insert\_element\_12\_}
    bool getVoltage(float* pVoltage) {
        if(!_isConnected) return false;

        std::vector<uint8_t> cmd = buildFrame(0xF0, 0x10);
        DWORD bytesWritten;
        if(!WriteFile(hComm, cmd.data(), cmd.size(), &bytesWritten, NULL) || bytesWritten != cmd.size()) {
            test_log_nok("发送查询电压命令失败");
            return false;
        }

        std::vector<uint8_t> response(100);
        DWORD bytesRead;
        if(!ReadFile(hComm, response.data(), response.size(), &bytesRead, NULL) || bytesRead == 0) {
            test_log_nok("未收到查询电压响应");
            return false;
        }
        response.resize(bytesRead);

        std::vector<uint8_t> params;
        if(parseResponse(response, 0xF0, 0x10, params) && params.size() == 2) {
            // 解析返回的16位参数（高字节在前），转换为电压值
            uint16_t voltageVal = (static_cast<uint16_t>(params[0]) << 8) | params[1];
            *pVoltage = voltageVal / 100.0f;
#ifdef DEBUG_MODE
            test_log_ok("当前输出电压: %fV", *pVoltage);
#endif
            std::string strSysVarVoltage = "PowerSupply" + std::to_string(handle) + ".Voltage";
            app.set_system_var_double(strSysVarVoltage.c_str(), *pVoltage);
            return true;
        }
        else {
            test_log_nok("获取电压失败");
            return false;
        }
    }

    // 获取实际输出电流（单位：A）
    bool getCurrent(float* pCurrent) {
        if(!_isConnected) return false;

        // 构造查询类命令（0xF0, 0x11），用于查询电流输出值{insert\_element\_0\_}
        std::vector<uint8_t> cmd = buildFrame(0xF0, 0x11);

        // 发送命令
        DWORD bytesWritten;
        if(!WriteFile(hComm, cmd.data(), cmd.size(), &bytesWritten, NULL) || bytesWritten != cmd.size()) {
            test_log_nok("发送查询电流命令失败");
            return false;
        }

        // 读取响应，带超时处理
        std::vector<uint8_t> response(100);
        DWORD bytesRead;
        if(!ReadFile(hComm, response.data(), response.size(), &bytesRead, NULL)) {
            test_log_nok("读取电流响应超时或失败");
            return false;
        }
        if(bytesRead == 0) {
            test_log_nok("超时未收到电流数据");
            return false;
        }
        response.resize(bytesRead);

        // 解析返回帧，响应参数为2字节电流值（十六进制，去小数点数值）{insert\_element\_1\_}
        std::vector<uint8_t> params;
        if(parseResponse(response, 0xF0, 0x11, params) && params.size() == 2) {
            // 转换为电流值（单位0.01A，需除以100）
            uint16_t currentVal = (static_cast<uint16_t>(params[0]) << 8) | params[1];
            *pCurrent = currentVal / 100.0f;
            std::string strSysVarCurrent = "PowerSupply" + std::to_string(handle) + ".Current";
            app.set_system_var_double(strSysVarCurrent.c_str(), *pCurrent);
#ifdef DEBUG_MODE
            test_log_ok("当前输出电流: %fA", *pCurrent);
#endif
            return true;
        }
        else {
            test_log_nok("获取电流失败");
            return false;
        }
    }

    // 查询电源当前状态（待机/启动/报警）
    bool getPowerState(u8* pState) {
        if(!_isConnected) return false;

        // 构造查询类命令（0xF0, 0xEB），用于查询电源当前状态
        std::vector<uint8_t> cmd = buildFrame(0xF0, 0xEB);

        // 发送命令
        DWORD bytesWritten;
        if(!WriteFile(hComm, cmd.data(), cmd.size(), &bytesWritten, NULL) || bytesWritten != cmd.size()) {
            test_log_nok("发送查询电源状态命令失败");
            return false;
        }

        // 读取响应，带超时处理
        std::vector<uint8_t> response(100);
        DWORD bytesRead;
        if(!ReadFile(hComm, response.data(), response.size(), &bytesRead, NULL)) {
            test_log_nok("读取电源状态响应超时或失败");
            return false;
        }
        if(bytesRead == 0) {
            test_log_nok("超时未收到电源状态数据");
            return false;
        }
        response.resize(bytesRead);

        // 解析返回帧，响应参数为1字节状态值
        std::vector<uint8_t> params;
        if(parseResponse(response, 0xF0, 0xEB, params) && params.size() == 1) {
            *pState = params[0];
            // 状态说明：0x01待机，0x02启动，0x03报警
            PowerStatus = *pState; // 更新状态变量

            std::string strSysVarState = "PowerSupply" + std::to_string(handle) + ".State";
            app.set_system_var_int32(strSysVarState.c_str(), PowerStatus);
#ifdef DEBUG_MODE
            test_log_ok("当前电源状态代码: 0x%02X", *pState);
            switch(*pState) {
            case 0x01:
                test_log_ok("电源状态：待机状态");
                break;
            case 0x02:
                test_log_ok("电源状态：启动状态");
                break;
            case 0x03:
                test_log_ok("电源状态：报警状态");
                break;
            default:
                test_log_ok("电源状态：未知状态");
                break;
            }
#endif
            return true;
        }
        else {
            test_log_nok("获取电源状态失败");
            return false;
        }
    }
};

// 静态成员变量需要在类外初始化
inline int TPowerSupply::nextHandle = 0; // 从0开始分配句柄
std::unordered_set<int> TPowerSupply::usedHandles;
TPowerSupply* pPowerSupply_A50 = nullptr;
TPowerSupply* pPowerSupply_A51 = nullptr;

int main() {
    // TPowerSupply Power_AN50("COM2");
    // if(!Power_AN50.isConnected()) {
    //     return 1;
    // }

    // // 示例操作
    // Power_AN50.turnOff();
    // Power_AN50.setVoltage(24.00f);
    // Power_AN50.turnOn();
    // float voltage;
    // Power_AN50.getVoltage(voltage);

    // return 0;
}
// CODE BLOCK END Global_Definitions 

// GEN BLOCK BEGIN Custom_Function
s32 create_power_supply_object(void);
s32 delete_power_supply_object(void);
s32 connect(const s32 AIndex, const char* portName);
s32 power_on(const s32 AIndex);
s32 power_off(const s32 AIndex);
s32 set_voltage(const s32 AIndex, const float ATargetVoltageV);
s32 set_current(const s32 AIndex, const float ATargetCurrentA);
s32 get_voltage(const s32 AIndex, float* pVoltageV);
s32 get_current(const s32 AIndex, float* pCurrentA);
s32 get_state(const s32 AIndex, pu8 pPowerState);
// GEN BLOCK END Custom_Function

// CODE BLOCK BEGIN On_Start On_Start
// 启动事件 "On_Start"
void on_start_On_Start(void) { // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
    create_power_supply_object();
    connect(0, "COM2");
    connect(1, "COM6");

}
// CODE BLOCK END On_Start On_Start

// CODE BLOCK BEGIN On_Stop On_Stop
// 停止事件 "On_Stop"
void on_stop_On_Stop(void) { // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
    delete_power_supply_object();

}
// CODE BLOCK END On_Stop On_Stop

// CODE BLOCK BEGIN Custom_Function create_power_supply_object dm9pZA__
// 自定义函数 "create_power_supply_object"
s32 create_power_supply_object(void) { // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
 // 自定义函数: 创建电源连接对象
    pPowerSupply_A50 = new TPowerSupply(DEFAULT_ADDRESS); // 创建电源对象，使用COM2端口和默认地址0x01
    if(pPowerSupply_A50 != nullptr) {
        test_log_ok("电源1对象创建成功");
    }
    else {
        test_log_nok("电源1对象创建失败");
        delete pPowerSupply_A50; // 清理已创建的对象
        return IDX_ERR_COMM_CREATE_FAILED; // 返回错误代码
    }
    pPowerSupply_A51 = new TPowerSupply(DEFAULT_ADDRESS); // 创建第二个电源对象，使用COM3端口和地址0x02
    if(pPowerSupply_A51 != nullptr) {
        test_log_ok("电源2对象创建成功");
    }
    else {
        test_log_nok("电源2对象创建失败");
        delete pPowerSupply_A51;
        return IDX_ERR_COMM_CREATE_FAILED; // 返回错误代码
    }

    return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function create_power_supply_object

// CODE BLOCK BEGIN Custom_Function delete_power_supply_object dm9pZA__
// 自定义函数 "delete_power_supply_object"
s32 delete_power_supply_object(void) { // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
 // 自定义函数: 删除电源对象
    if(pPowerSupply_A50 != nullptr) {
        delete pPowerSupply_A50;
        pPowerSupply_A50 = nullptr;
        test_log_ok("电源1对象删除成功");
    }
    if(pPowerSupply_A51 != nullptr) {
        delete pPowerSupply_A51;
        pPowerSupply_A51 = nullptr;
        test_log_ok("电源2对象删除成功");
    }
    return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function delete_power_supply_object

// CODE BLOCK BEGIN Custom_Function connect Y29uc3QgczMyIEFJbmRleCwgY29uc3QgY2hhciogcG9ydE5hbWU_
// 自定义函数 "connect"
s32 connect(const s32 AIndex, const char* portName) { // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
 // 自定义函数: 连接电源
    if(AIndex == 0) {
        if(pPowerSupply_A50 == nullptr) {
            test_log_nok("电源1对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A50->connect(portName)) {
            test_log_nok("电源1连接失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源1连接成功");
    }
    else if(AIndex == 1) {
        if(pPowerSupply_A51 == nullptr) {
            test_log_nok("电源2对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A51->connect(portName)) {
            test_log_nok("电源2连接失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源2连接成功");
    }
    else {
        test_log_nok("无效的电源索引: %d", AIndex);
        return IDX_ERR_COMM_CREATE_FAILED;

    }

    return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function connect

// CODE BLOCK BEGIN Custom_Function power_on Y29uc3QgczMyIEFJbmRleA__
// 自定义函数 "power_on"
s32 power_on(const s32 AIndex) { // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
 // 自定义函数: 开启电源
    if(AIndex == 0) {
        if(pPowerSupply_A50 == nullptr) {
            test_log_nok("电源1对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A50->turnOn()) {
            test_log_nok("电源1开启失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源1开启成功");
    }
    else if(AIndex == 1) {
        if(pPowerSupply_A51 == nullptr) {
            test_log_nok("电源2对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A51->turnOn()) {
            test_log_nok("电源2开启失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源2开启成功");
    }
    else {
        test_log_nok("无效的电源索引: %d", AIndex);
        return IDX_ERR_COMM_CREATE_FAILED;

    }
    return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function power_on

// CODE BLOCK BEGIN Custom_Function power_off Y29uc3QgczMyIEFJbmRleA__
// 自定义函数 "power_off"
s32 power_off(const s32 AIndex) { // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
 // 自定义函数: 关闭电源
    if(AIndex == 0) {
        if(pPowerSupply_A50 == nullptr) {
            test_log_nok("电源1对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A50->turnOff()) {
            test_log_nok("电源1关闭失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源1关闭成功");
    }
    else if(AIndex == 1) {
        if(pPowerSupply_A51 == nullptr) {
            test_log_nok("电源2对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A51->turnOff()) {
            test_log_nok("电源2关闭失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源2关闭成功");
    }
    else {
        test_log_nok("无效的电源索引: %d", AIndex);
        return IDX_ERR_COMM_CREATE_FAILED;

    }
    return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function power_off

// CODE BLOCK BEGIN Custom_Function set_voltage Y29uc3QgczMyIEFJbmRleCwgY29uc3QgZmxvYXQgQVRhcmdldFZvbHRhZ2VW
// 自定义函数 "set_voltage"
s32 set_voltage(const s32 AIndex, const float ATargetVoltageV) { // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
    if(AIndex == 0) {
        if(pPowerSupply_A50 == nullptr) {
            test_log_nok("电源1对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A50->setVoltage(static_cast<float>(ATargetVoltageV))) {
            test_log_nok("电源1设置电压失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源1设置电压成功: %fV", ATargetVoltageV);
    }
    else if(AIndex == 1) {
        if(pPowerSupply_A51 == nullptr) {
            test_log_nok("电源2对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A51->setVoltage(static_cast<float>(ATargetVoltageV))) {
            test_log_nok("电源2设置电压失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源2设置电压成功: %fV", ATargetVoltageV);
    }
    else {
        test_log_nok("无效的电源索引: %d", AIndex);
        return IDX_ERR_COMM_CREATE_FAILED;

    }
    return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function set_voltage

// CODE BLOCK BEGIN Custom_Function set_current Y29uc3QgczMyIEFJbmRleCwgY29uc3QgZmxvYXQgQVRhcmdldEN1cnJlbnRB
// 自定义函数 "set_current"
s32 set_current(const s32 AIndex, const float ATargetCurrentA) { // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
 // 自定义函数: 设置电流
    if(AIndex == 0) {
        if(pPowerSupply_A50 == nullptr) {
            test_log_nok("电源1对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A50->setCurrent(static_cast<float>(ATargetCurrentA))) {
            test_log_nok("电源1设置电流失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源1设置电流成功: %fA", ATargetCurrentA);
    }
    else if(AIndex == 1) {
        if(pPowerSupply_A51 == nullptr) {
            test_log_nok("电源2对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A51->setCurrent(static_cast<float>(ATargetCurrentA))) {
            test_log_nok("电源2设置电流失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源2设置电流成功: %fA", ATargetCurrentA);
    }
    else {
        test_log_nok("无效的电源索引: %d", AIndex);
        return IDX_ERR_COMM_CREATE_FAILED;

    }
    return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function set_current

// CODE BLOCK BEGIN Custom_Function get_voltage Y29uc3QgczMyIEFJbmRleCwgZmxvYXQqIHBWb2x0YWdlVg__
// 自定义函数 "get_voltage"
s32 get_voltage(const s32 AIndex, float* pVoltageV) { // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
 // 自定义函数: 获取电压
    if(AIndex == 0) {
        if(pPowerSupply_A50 == nullptr) {
            test_log_nok("电源1对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A50->getVoltage(pVoltageV)) {
            test_log_nok("电源1获取电压失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源1当前输出电压: %fV", *pVoltageV);
    }
    else if(AIndex == 1) {
        if(pPowerSupply_A51 == nullptr) {
            test_log_nok("电源2对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A51->getVoltage(pVoltageV)) {
            test_log_nok("电源2获取电压失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源2当前输出电压: %fV", *pVoltageV);
    }
    else {
        test_log_nok("无效的电源索引: %d", AIndex);
        return IDX_ERR_COMM_CREATE_FAILED;

    }
    return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function get_voltage

// CODE BLOCK BEGIN Custom_Function get_current Y29uc3QgczMyIEFJbmRleCwgZmxvYXQqIHBDdXJyZW50QQ__
// 自定义函数 "get_current"
s32 get_current(const s32 AIndex, float* pCurrentA) { // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
 // 自定义函数: 获取电流
    if(AIndex == 0) {
        if(pPowerSupply_A50 == nullptr) {
            test_log_nok("电源1对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A50->getCurrent(pCurrentA)) {
            test_log_nok("电源1获取电流失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源1当前输出电流: %fA", *pCurrentA);
    }
    else if(AIndex == 1) {
        if(pPowerSupply_A51 == nullptr) {
            test_log_nok("电源2对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A51->getCurrent(pCurrentA)) {
            test_log_nok("电源2获取电流失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源2当前输出电流: %fA", *pCurrentA);
    }
    else {
        test_log_nok("无效的电源索引: %d", AIndex);
        return IDX_ERR_COMM_CREATE_FAILED;

    }
    return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function get_current

// CODE BLOCK BEGIN Custom_Function get_state Y29uc3QgczMyIEFJbmRleCwgcHU4IHBQb3dlclN0YXRl
// 自定义函数 "get_state"
s32 get_state(const s32 AIndex, pu8 pPowerState) { // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 

    if(AIndex == 0) {
        if(pPowerSupply_A50 == nullptr) {
            test_log_nok("电源1对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A50->getPowerState(pPowerState)) {
            test_log_nok("电源1获取状态失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源1当前状态: %d", *pPowerState);
    }
    else if(AIndex == 1) {
        if(pPowerSupply_A51 == nullptr) {
            test_log_nok("电源2对象未创建");
            return IDX_ERR_COMM_CREATE_FAILED;
        }
        if(!pPowerSupply_A51->getPowerState(pPowerState)) {
            test_log_nok("电源2获取状态失败");
            return IDX_ERR_COMM_CONNECT_FAILED;
        }
        test_log_ok("电源2当前状态: %d", *pPowerState);
    }
    else {
        test_log_nok("无效的电源索引: %d", AIndex);
        return IDX_ERR_COMM_CREATE_FAILED;

    }
    return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function get_state

// CODE BLOCK BEGIN Step_Function  NQ__
// 主step函数，执行周期 5 ms
void step(void) { // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms
 // 周期 = 5 ms

}
// CODE BLOCK END Step_Function 

// CODE BLOCK BEGIN Configuration
/* 
[UI]
UICommon=-1,-1,-1,0,QyC0+sLrseC8rcb3IFtQb3dlclN1cHBseV0_,100,5701,2635731897568340014,0
ScriptName=PowerSupply
DisplayName=PowerSupply
IsMPLib=1
DBDeps=CAN1_DUT1,CAN2_DUT2,CAN3_DUT3,CAN_LED_Sensors
LastBuildTime=2025-12-10 14:37:29
VarListClose=1
APICnt=10

[UI_Doc]
Cnt=10
d0=create_power_supply_object,tLS9qLXn1LTBrL3TttTP8w__,,0
d1=delete_power_supply_object,yb6z/bXn1LS21M/z,,0
d2=set_voltage,,,2,NCwtMSxBSW5kZXgs,NiwtMSxBVGFyZ2V0Vm9sdGFnZVYs
d3=get_voltage,u/HIobXn0bk_,,2,NCwtMSxBSW5kZXgs,MzYsMCxwVm9sdGFnZVYs
d4=power_on,v6rG9LXn1LQ_,,1,NCwtMSxBSW5kZXgs
d5=power_off,udix1bXn1LQ_,,1,NCwtMSxBSW5kZXgs
d6=get_current,u/HIobXnwfc_,,2,NCwtMSxBSW5kZXgs,MzYsMCxwQ3VycmVudEEs
d7=get_state,,,2,NCwtMSxBSW5kZXgs,OSwwLHBQb3dlclN0YXRlLA__
d8=set_current,yejWw7Xnwfc_,,2,NCwtMSxBSW5kZXgs,NiwtMSxBVGFyZ2V0Q3VycmVudEEs
d9=connect,way907Xn1LQ_,,2,NCwtMSxBSW5kZXgs,MTcsLTEscG9ydE5hbWUs
*/
// CODE BLOCK END Configuration

