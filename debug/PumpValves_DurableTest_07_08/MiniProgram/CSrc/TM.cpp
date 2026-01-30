// GEN BLOCK BEGIN Include
#define TSMP_IMPL
#include "TSMaster.h"
#include "MPLibrary.h"
#include "Database.h"
#include "TSMasterBaseInclude.h"
#include "Configuration.h"
// GEN BLOCK END Include

// CODE BLOCK BEGIN Global_Definitions 
#include <string>
#include <vector>
#define DEBUG_MODE
//#define DEBUG_MODE_DUT1
//#define DEBUG_MODE_DUT2
//#define DEBUG_MODE_DUT3

//#define DEBUG_TEST_CASE_MODE

#define DUT_CHANNEL_COUNT 3

/*************安全保护逻辑***********************/
#define MAX_MONITOR_WINDOW  5
#define MAX_STUCK_COUNT  15

#define MAX_VALVE_CURRENT 8.0f
#define MIN_VALVE_CURRENT -1.0f

#define MAX_MOTOR_CURRENT 130.0f
#define MIN_MOTOR_CURRENT -1.0f

#define MAX_SENSOR_PRESSURE 25.0f
#define MIN_SENSOR_PRESSURE -1.0f

#define MAX_SAFETY_PRESSURE  16.0f //安全压力值

s32 vTotalTestCount = 0; //总测试次数
s32 vRunCount[DUT_CHANNEL_COUNT] = {0, 0, 0}; //DUT运行次数

// 机柜状态控制
s32 vBenchState = 0;   //0 - yellow; 1-green; 2-red

/*Part: DUT1*/
// TM初始化是否成功
s32 vTMHasError = 0;             //0: no error; 1:有错误

//related with system var
s32 vDUTEnabled[DUT_CHANNEL_COUNT] = {0, 0 , 0};    //0: disable; 1：enable
s32 vDUTState[DUT_CHANNEL_COUNT] = {0, 0 , 0};    //0: ready; 1：Running; 2: Error;
s32 vDUTIsStopped[DUT_CHANNEL_COUNT] = {0, 0 , 0};    //0: ready; 1：Running; 2: Error;
s32 vDUTIsRunning[DUT_CHANNEL_COUNT] = {0, 0 , 0};    //0: ready; 1：Running; 2: Error;
s32 vDUTHasError[DUT_CHANNEL_COUNT] = {0, 0 , 0};    //0: ready; 1：Running; 2: Error;
s32 vDUTTotalTestCount[DUT_CHANNEL_COUNT] = {0, 0 , 0};

// 上电后板卡诊断——中间变量
u8 vDriverDiagStep = 0; //0: 发送所有阀体和电机关闭报文; 1: 打开电源开关； 2：等待1s，完成电流采样； 3：进行诊断，并关闭电源； 4：诊断完成
// 上电后板卡诊断——系统变量
s32 vVDDriverHasError[DUT_CHANNEL_COUNT] = {0, 0 , 0};             //0: no error; 1:有错误
s32 vMVDDriverHasError[DUT_CHANNEL_COUNT] = {0, 0 , 0};             //0: no error; 1:有错误

// 信号丢失——系统变量
s32 vPWRIsDisconnected[DUT_CHANNEL_COUNT] = {0, 0 , 0};    //0: PWR Connected; 1: PWR Disconnected
s32 vVDIsDisconnected[DUT_CHANNEL_COUNT] = {0, 0 , 0};    //0: VD Connected; 1: VD Disconnected
s32 vMVDIsDisconnected[DUT_CHANNEL_COUNT] = {0, 0 , 0};    //0: MVD Connected; 1: MVD Disconnected
s32 vDUTSensorIsDisconnected[DUT_CHANNEL_COUNT] = {0, 0 , 0};    //0: DUT Sensor Connected; 1: DUT Sensor  Disconnected
s32 vDeviceSensorIsDisconnected[DUT_CHANNEL_COUNT] = {0, 0 , 0};    //0: Device Sensors Connected; 1: Device Sensors Disconnected
// 信号丢失——中间记录变量
u8 vPWRMonitorCount[DUT_CHANNEL_COUNT] = {0, 0, 0};  //PWR
u8 vMVDMonitorCount[DUT_CHANNEL_COUNT] = {0, 0, 0};  //MVD
u8 vVDMonitorCount[DUT_CHANNEL_COUNT] = {0, 0, 0};   //VD
u8 vDeviceSensorsMonitorCount[DUT_CHANNEL_COUNT] = {0, 0, 0};  //Device Sensors
u8 vDUTSensorMonitorCount[DUT_CHANNEL_COUNT] = {0, 0, 0};  //DUT Sensors
// 信号卡滞——系统变量
s32 vVDIsStuck[DUT_CHANNEL_COUNT] = {0, 0 , 0};    //0: VD Normal; 1: VD Stuck
s32 vMVDIsStuck[DUT_CHANNEL_COUNT] = {0, 0 , 0};    //0: MVD Normal; 1: MVD Stuck
s32 vDeviceSensorIsStuck[DUT_CHANNEL_COUNT] = {0, 0 , 0};    //0: Device Sensors Normal; 1: Device Sensors Stuck
// 信号卡滞——中间记录变量
u16 vVDLastCurrentRawValue[DUT_CHANNEL_COUNT][8] = {0};
u16 vMVDLastCurrentRawValue[DUT_CHANNEL_COUNT][8] = {0};
u16 vDeviceSensorLastPressureRawValue[DUT_CHANNEL_COUNT][5] = {0}; //Device Sensors Pressure Raw Value
// 信号卡滞——中间记录变量
u8 vMVDStuckMonitorCount[DUT_CHANNEL_COUNT] = {0, 0, 0};  //MVD
u8 vVDStuckMonitorCount[DUT_CHANNEL_COUNT] = {0, 0, 0};   //VD
u8 vDeviceSensorsStuckMonitorCount[DUT_CHANNEL_COUNT] = {0, 0, 0};  //Device Sensors

// 记录最新的CAN 信号值
float vVDCurrentValue[DUT_CHANNEL_COUNT][8] = {0};  //VD 电流   
float vMVDCurrentValue[DUT_CHANNEL_COUNT][8] = {0};  //MVD 电流
float vDeviceSensorPressureValue[DUT_CHANNEL_COUNT][5] = {0}; //压力传感器值
// 信号值错误--系统变量
s32 vVDSignalError[DUT_CHANNEL_COUNT] = {0, 0, 0}; //VD Signal Error Flag
s32 vMVDSignalError[DUT_CHANNEL_COUNT] = {0, 0, 0}; //MVD Signal Error Flag
s32 vDUTSensorSignalError[DUT_CHANNEL_COUNT] = {0, 0, 0}; //DUT Sensor Signal Error Flag
s32 vDeviceSensorSignalError[DUT_CHANNEL_COUNT] = {0, 0, 0}; //Device Sensor Signal Error Flag
s32 vDeviceSensorMaxPressureReached[DUT_CHANNEL_COUNT] = {0, 0, 0}; //Device Sensor Max Pressure Reached Flag
// 板卡保护--系统变量
s32 vVDInProtection[DUT_CHANNEL_COUNT] = {0, 0, 0}; //VD In Protection Flag
s32 vMVDInProtection[DUT_CHANNEL_COUNT] = {0, 0, 0}; //MVD In Protection Flag
// 测试用例错误--系统变量
s32 vTestCaseError[DUT_CHANNEL_COUNT] = {0, 0, 0}; //DUT Test Case Error Flag
s32 vTestCaseErrorCode[DUT_CHANNEL_COUNT] = {0, 0, 0}; //DUT Test Case Error code
s32 vTestCaseRunState[DUT_CHANNEL_COUNT] = {0, 0, 0}; //DUT Test Case Run State

// 手动清除错误Action相关变量
s32 vManualClearErrorHeartbeatActived[DUT_CHANNEL_COUNT] = {0, 0, 0}; //DUT Manual Clear Error Action Flag




std::string dut_sysvar_name(const s32 index, const std::string& AName) {
  return "DUT" + std::to_string(index + 1) + "." + AName;
}

void change_sysvar_dut_state(const s32 ADUTIndex, const s32 AState) {
  app.enter_critical_section();
  vDUTState[ADUTIndex] = AState;
  app.leave_critical_section();
  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("ADUTIndex is not valid");
    vTMHasError = 1; // TM has error
    return;
  }
  std::string ADUTSystemVarName = "DUT" + std::to_string(ADUTIndex + 1) + ".State";
  std::string ADUTSystemVarStopedName = "DUT" + std::to_string(ADUTIndex + 1) + ".IsStopped";
  std::string ADUTSystemVarRunningName = "DUT" + std::to_string(ADUTIndex + 1) + ".IsRunning";
  std::string ADUTSystemVarFaultName = "DUT" + std::to_string(ADUTIndex + 1) + ".HasError";

  s32 ALastValue = 0;

  if(0 != app.get_system_var_int32(ADUTSystemVarName.c_str(), &ALastValue)) {
    std::string strErrorInfo = "get_system_var_int32 failed: " + ADUTSystemVarName;
    app.log_text(strErrorInfo.c_str(), lvlError);
    vTMHasError = 1; // TM has error
  }

  if(ALastValue != vDUTState[ADUTIndex]) {
    if(0 == vDUTState[ADUTIndex]) {
      app.set_system_var_int32(ADUTSystemVarStopedName.c_str(), 1);
      app.set_system_var_int32(ADUTSystemVarRunningName.c_str(), 0);
      app.set_system_var_int32(ADUTSystemVarFaultName.c_str(), 0);
      std::string strInfo = "DUT" + std::to_string(ADUTIndex + 1) + " is ready";
      app.log_text(strInfo.c_str(), lvlHint);
      app.panel_set_enable("测试监控窗体", "PBStart", true);
      app.panel_set_bkgd_color("测试监控窗体", "PBStart", 0xFF008000);
      app.panel_set_enable("测试监控窗体", "PBStop", false);
      app.panel_set_bkgd_color("测试监控窗体", "PBStop", 0xFF808080);
      app.panel_set_enable("测试监控窗体", "PBClearFault", false);
      app.panel_set_bkgd_color("测试监控窗体", "PBClearFault", 0xFF808080);
      app.panel_set_enable("测试监控窗体", "PBResetData", true);
      app.panel_set_bkgd_color("测试监控窗体", "PBResetData", 0xFF0000FF);
      app.panel_set_enable("测试监控窗体", "CBDUT1Enable", true);
      app.panel_set_enable("测试监控窗体", "CBDUT2Enable", true);
      app.panel_set_enable("测试监控窗体", "CBDUT3Enable", true);

    }
    else if(1 == vDUTState[ADUTIndex]) {
      app.set_system_var_int32(ADUTSystemVarStopedName.c_str(), 0);
      app.set_system_var_int32(ADUTSystemVarRunningName.c_str(), 1);
      app.set_system_var_int32(ADUTSystemVarFaultName.c_str(), 0);
      std::string strInfo = "DUT" + std::to_string(ADUTIndex + 1) + " is running";
      app.log_text(strInfo.c_str(), lvlOK);
      app.panel_set_enable("测试监控窗体", "PBStart", false);
      app.panel_set_bkgd_color("测试监控窗体", "PBStart", 0xFF808080);
      app.panel_set_enable("测试监控窗体", "PBStop", true);
      app.panel_set_bkgd_color("测试监控窗体", "PBStop", 0xFF00FF00);
      app.panel_set_enable("测试监控窗体", "PBClearFault", false);
      app.panel_set_bkgd_color("测试监控窗体", "PBClearFault", 0xFF808080);
      app.panel_set_enable("测试监控窗体", "PBResetData", false);
      app.panel_set_bkgd_color("测试监控窗体", "PBResetData", 0xFF808080);
      app.panel_set_enable("测试监控窗体", "CBDUT1Enable", false);
      app.panel_set_enable("测试监控窗体", "CBDUT2Enable", false);
      app.panel_set_enable("测试监控窗体", "CBDUT3Enable", false);

    }
    else if(2 == vDUTState[ADUTIndex]) {
      app.set_system_var_int32(ADUTSystemVarStopedName.c_str(), 0);
      app.set_system_var_int32(ADUTSystemVarRunningName.c_str(), 0);
      app.set_system_var_int32(ADUTSystemVarFaultName.c_str(), 1);
      std::string strInfo = "DUT" + std::to_string(ADUTIndex + 1) + " has Error";
      app.log_text(strInfo.c_str(), lvlError);

      app.panel_set_enable("测试监控窗体", "PBStart", false);
      app.panel_set_bkgd_color("测试监控窗体", "PBStart", 0xFF808080);
      app.panel_set_enable("测试监控窗体", "PBStop", false);
      app.panel_set_bkgd_color("测试监控窗体", "PBStop", 0xFF808080);
      app.panel_set_enable("测试监控窗体", "PBClearFault", true);
      app.panel_set_bkgd_color("测试监控窗体", "PBClearFault", 0xFFFF0000);
      app.panel_set_enable("测试监控窗体", "PBResetData", false);
      app.panel_set_bkgd_color("测试监控窗体", "PBResetData", 0xFF808080);
      app.panel_set_enable("测试监控窗体", "CBDUT1Enable", true);
      app.panel_set_enable("测试监控窗体", "CBDUT2Enable", true);
      app.panel_set_enable("测试监控窗体", "CBDUT3Enable", true);
    }
    else {
      test_log_nok("DUT %d State is not valid", ADUTIndex);
      vTMHasError = 1; // TM has error
      return;
    }

    if(0 != app.set_system_var_int32(ADUTSystemVarName.c_str(), vDUTState[ADUTIndex])) {
      std::string strErrorInfo = "set_system_var_int32 failed: " + ADUTSystemVarName;
      app.log_text(strErrorInfo.c_str(), lvlError);
      vTMHasError = 1; // TM has error

    }
  }


}

void set_dut_sysvar_error_state(const s32 index, const std::string& AName, const s32 AValue) {
  if((index < 0) || (index > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    vTMHasError = 1; // TM has error
    return;
  }
  std::string ADUTSystemVarName = "DUT" + std::to_string(index + 1) + "." + AName;

  s32 ALastValue = 0;

  if(0 != app.get_system_var_int32(ADUTSystemVarName.c_str(), &ALastValue)) {
    std::string strErrorInfo = "get_system_var_int32 failed: " + ADUTSystemVarName;
    app.log_text(strErrorInfo.c_str(), lvlError);
    vTMHasError = 1; // TM has error
  }

  if(ALastValue != AValue) {
    if(AValue) {
      std::string strErrorInfo = "Error Occured: " + ADUTSystemVarName + " set to " + std::to_string(AValue);
      app.log_text(strErrorInfo.c_str(), lvlError);
    }
    else {
      std::string strInfo = "Error Cleared: " + ADUTSystemVarName + " set to " + std::to_string(AValue);
      app.log_text(strInfo.c_str(), lvlOK);
    }
    if(0 != app.set_system_var_int32(ADUTSystemVarName.c_str(), AValue)) {
      std::string strErrorInfo = "set_system_var_int32 failed: " + ADUTSystemVarName;
      app.log_text(strErrorInfo.c_str(), lvlError);
      vTMHasError = 1; // TM has error
    }
  }

}

void get_dut_sysvar_int32(const s32 index, const std::string& AName, s32* AValue) {
  if((index < 0) || (index > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    vTMHasError = 1; // TM has error
    return;
  }
  std::string ADUTSystemVarName = "DUT" + std::to_string(index + 1) + "." + AName;
  if(0 != app.get_system_var_int32(ADUTSystemVarName.c_str(), AValue)) {
    std::string strErrorInfo = "get_system_var_int32 failed: " + ADUTSystemVarName;
    app.log_text(strErrorInfo.c_str(), lvlError);
    vTMHasError = 1; // TM has error
  }
}

// 心跳帧
void dut_heartbeat(const s32 ADUTIndex) {
  // 检查ADUTIndex是否有效
  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    return;
  }
  // 发送心跳帧
  // [1] CAN FD 0x1 PC_Heartbeat 
  TCANFD f0 = {u8(ADUTIndex),0x5,1,0x3,0x1,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
  com.transmit_canfd_async(&f0);
  app.wait(1, "");
}

//关闭PWR
void dut_power_off(const s32 ADUTIndex) {
  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    return;
  }
  // [1] CAN FD 0xF001 PWR_Control 
  TCANFD f0 = {u8(ADUTIndex),0x5,2,0x3,0xF001,0,{0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
  com.transmit_canfd_async(&f0);
  app.wait(1, "");
}

// 打开PWR
void dut_power_on(const s32 ADUTIndex) {
  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    return;
  }
  // [1] CAN FD 0xF001 PWR_Control 
  TCANFD f0 = {u8(ADUTIndex),0x5,2,0x3,0xF001,0,{0x3, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
  com.transmit_canfd_async(&f0);
  app.wait(1, "");
}

// 关闭所有VD阀体
void dut_vd_valve_off(const s32 ADUTIndex) {
  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    return;
  }
  // [3] CAN FD 0x3005 VD_Current_Control 
  TCANFD f0 = {u8(ADUTIndex),0x5,10,0x3,0x3005,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
  com.transmit_canfd_async(&f0);
  app.wait(1, "");
}

// 关闭所有VD阀体
void dut_mvd_valve_off(const s32 ADUTIndex) {
  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    return;
  }
  // [2] CAN FD 0x3105 MVD_ValvesCurrentControl 
  TCANFD f0 = {u8(ADUTIndex),0x5,10,0x3,0x3105,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
  com.transmit_canfd_async(&f0);
  app.wait(1, "");
}

// 关闭所有VD阀体
void dut_mvd_motor_off(const s32 ADUTIndex) {
  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    return;
  }
  // [1] CAN FD 0x3104 MVD_MotorsDutyControl 
  TCANFD f0 = {u8(ADUTIndex),0x5,4,0x3,0x3104,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
  com.transmit_canfd_async(&f0);
  app.wait(1, "");
}

void self_driver_check(void) {
  u8 index = 0;
  s32 AVDErrorFlag = 0;
  s32 AMVDErrorFlag = 0;
  if(vDriverDiagStep >= 4) return;
  if(0 == vDriverDiagStep) {
    // Action 1.1: 发送心跳帧
    for(index = 0; index < DUT_CHANNEL_COUNT; index++) {
      dut_heartbeat(index);
      app.wait(5, "");
    }

    // Action 1.2: 关闭所有阀和电机
    for(index = 0; index < DUT_CHANNEL_COUNT; index++) {
      dut_vd_valve_off(index);
      app.wait(5, "");
      dut_mvd_valve_off(index);
      app.wait(5, "");
      dut_mvd_motor_off(index);
      app.wait(5, "");
    }
    vDriverDiagStep = 1;
  }
  else if(1 == vDriverDiagStep) {
    // Action 2: 打开电源
    for(index = 0; index < DUT_CHANNEL_COUNT; index++) {
      dut_power_on(index);
      app.wait(5, "");

    }
    vDriverDiagStep = 2;
  }
  else if(2 == vDriverDiagStep) {
    // Action 3 等待1s
    vDriverDiagStep = 3;
  }
  else if(3 == vDriverDiagStep) {
    // Action 4.1: 测试步骤
    for(index = 0; index < DUT_CHANNEL_COUNT; index++) {
      for(u8 ADriverChannelIndex = 0; ADriverChannelIndex < 8; ADriverChannelIndex++) {
        if((vVDCurrentValue[index][ADriverChannelIndex] > 1.0f) || (vVDCurrentValue[index][ADriverChannelIndex] < -1.0f)) {
          AVDErrorFlag = 1;
        }
        if((vMVDCurrentValue[index][ADriverChannelIndex] > 1.0f) || (vMVDCurrentValue[index][ADriverChannelIndex] < -1.0f)) {
          AMVDErrorFlag = 1;
        }
      }

      if(AVDErrorFlag) {
        test_log_nok("VD Driver %d Value Error", index + 1);
        vVDDriverHasError[index] = 1;
      }
      else {
        test_log_ok("VD Driver %d Value OK", index + 1);
        vVDDriverHasError[index] = 0;
      }
      if(AMVDErrorFlag) {
        test_log_nok("MVD Driver %d Value Error", index + 1);
        vMVDDriverHasError[index] = 1;
      }
      else {
        test_log_ok("MVD Driver %d Value OK", index + 1);
        vMVDDriverHasError[index] = 0;
      }

      set_dut_sysvar_error_state(index, "VDDriverHasError", vVDDriverHasError[index]);
      set_dut_sysvar_error_state(index, "MVDDriverHasError", vMVDDriverHasError[index]);


      vDriverDiagStep = 4;
    }
    // Action 4.1: 关闭样件电源
    for(index = 0; index < DUT_CHANNEL_COUNT; index++) {
      dut_power_off(index);
      app.wait(5, "");
    }

    vDriverDiagStep = 4;
  }

}



s32 fault_action_deflate(const s32 ADUTIndex, const u8 ATankIndex, const float ATargetPressureBar) { // 自定义函数: 
  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    app.terminate_application();
    return -1;
  }

  if(ATargetPressureBar > 10 || ATargetPressureBar < 0) {
    test_log_nok("Fault action deflate target pressure should be in range [0, 10]bar, current set is %f bar.", ATargetPressureBar);
    return -2;
  }
  // 空簧：RV1和RV2打开，打开对应的ASV阀
  // 储气罐：打开RV1和RV3
  // 然后打开ASV阀放气
  if(ATankIndex < 0 || ATankIndex > 4) {
    test_log_nok("TankIndex is not valid");
    return -3;
  }

  if(vManualClearErrorHeartbeatActived[ADUTIndex] != 0) {
    test_log_nok("Fault Action for DUT %d is already in progress", ADUTIndex + 1);
    return -4;
  }

  vManualClearErrorHeartbeatActived[ADUTIndex] = 1; // 设置为正在执行状态

  u8 ACurrentRV1Actived = 0;
  u8 ACurrentRV2Actived = 0;
  u8 ACurrentRV3Actived = 0;
  u8 ACurrentRV4Actived = 0;
  u8 ACurrentASV1Actived = 0;
  u8 ACurrentASV2Actived = 0;
  u8 ACurrentASV3Actived = 0;
  u8 ACurrentASV4Actived = 0;

  if(ATankIndex < 4) {  //FL,FR, RL,RR
    ACurrentRV1Actived = 1; // RV1 打开
    ACurrentRV2Actived = 1; // RV2 打开
    ACurrentRV3Actived = 0; // RV3 关闭
    ACurrentRV4Actived = 0; // RV4 关闭
    if(0 == ATankIndex) {  //FL   
      ACurrentASV1Actived = 0; // ASV1 关闭
      ACurrentASV2Actived = 0; // ASV2 关闭
      ACurrentASV3Actived = 0; // ASV3 关闭
      ACurrentASV4Actived = 1; // ASV4 打开
    }
    else if(1 == ATankIndex) { //FR
      ACurrentASV1Actived = 0; // ASV1 关闭
      ACurrentASV2Actived = 0; // ASV2 关闭
      ACurrentASV3Actived = 1; // ASV3 打开
      ACurrentASV4Actived = 0; // ASV4 关闭
    }
    else if(2 == ATankIndex) { //RL
      ACurrentASV1Actived = 0; // ASV1 关闭
      ACurrentASV2Actived = 1; // ASV2 打开
      ACurrentASV3Actived = 0; // ASV3 关闭
      ACurrentASV4Actived = 0; // ASV4 关闭
    }
    else if(3 == ATankIndex) { //RR
      ACurrentASV1Actived = 1; // ASV1 打开
      ACurrentASV2Actived = 0; // ASV2 关闭
      ACurrentASV3Actived = 0; // ASV3 关闭
      ACurrentASV4Actived = 0; // ASV4 关闭
    }
  }
  else if(4 == ATankIndex) { //储气罐
    ACurrentRV1Actived = 1; // RV1 打开
    ACurrentRV2Actived = 0; // RV2 关闭
    ACurrentRV3Actived = 1; // RV3 打开
    ACurrentRV4Actived = 0; // RV4 关闭
    ACurrentASV1Actived = 0; // ASV1 关闭
    ACurrentASV2Actived = 0; // ASV2 关闭
    ACurrentASV3Actived = 0; // ASV3 关闭
    ACurrentASV4Actived = 0; // ASV4 关闭
  }
  // 打开心跳帧
  dut_heartbeat(ADUTIndex);
  app.wait(100, ""); // 等待100ms
  // 打开电源
  dut_power_on(ADUTIndex);
  app.wait(1000, ""); // 等待100ms


  // 打开需要释放气体的储气罐
  // [1] CAN FD 0x3005 VD_Current_Control 
  TCANFD f0 = {u8(ADUTIndex),0x5,10,0x3,0x3005,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
  TVD_Current_Control_1 VD_Current_Control_1;
  VD_Current_Control_1.init();
  VD_Current_Control_1.FCAN.load_data(&f0.FData[0]);
  VD_Current_Control_1.RV1_Target_Current = 3.5 * ACurrentRV1Actived;
  VD_Current_Control_1.RV2_Target_Current = 3.5 * ACurrentRV2Actived;
  VD_Current_Control_1.RV3_Target_Current = 3.5 * ACurrentRV3Actived;
  VD_Current_Control_1.RV4_Target_Current = 3.5 * ACurrentRV4Actived;
  VD_Current_Control_1.ASV1_Target_Current = 3.5 * ACurrentASV1Actived;;
  VD_Current_Control_1.ASV2_Target_Current = 3.5 * ACurrentASV2Actived;;
  VD_Current_Control_1.ASV3_Target_Current = 3.5 * ACurrentASV3Actived;;
  VD_Current_Control_1.ASV4_Target_Current = 3.5 * ACurrentASV4Actived;;
  com.transmit_canfd_async(&VD_Current_Control_1.FCAN);

  app.wait(100, "");

  VD_Current_Control_1.RV1_Target_Current = 1.35 * ACurrentRV1Actived;
  VD_Current_Control_1.RV2_Target_Current = 1.35 * ACurrentRV2Actived;
  VD_Current_Control_1.RV3_Target_Current = 1.35 * ACurrentRV3Actived;
  VD_Current_Control_1.RV4_Target_Current = 1.35 * ACurrentRV4Actived;
  VD_Current_Control_1.ASV1_Target_Current = 1.35 * ACurrentASV1Actived;;
  VD_Current_Control_1.ASV2_Target_Current = 1.35 * ACurrentASV2Actived;;
  VD_Current_Control_1.ASV3_Target_Current = 1.35 * ACurrentASV3Actived;;
  VD_Current_Control_1.ASV4_Target_Current = 1.35 * ACurrentASV4Actived;;
  com.transmit_canfd_async(&VD_Current_Control_1.FCAN);

  app.wait(10, "");

  // 打开VV阀体
  // [1] CAN FD 0x3105 MVD_ValvesCurrentControl 
  TCANFD f1 = {u8(ADUTIndex),0x5,10,0x3,0x3105,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
  TMVD_ValvesCurrentControl_1 MVD_ValvesCurrentControl_1;
  MVD_ValvesCurrentControl_1.init();
  MVD_ValvesCurrentControl_1.FCAN.load_data(&f0.FData[0]);
  MVD_ValvesCurrentControl_1.RESV_CH3_Target_Current = 0;
  MVD_ValvesCurrentControl_1.RESV_CH4_Target_Current = 0;
  MVD_ValvesCurrentControl_1.VV_Target_Current = 3.5;
  MVD_ValvesCurrentControl_1.RESV_CH6_Target_Current = 0;
  MVD_ValvesCurrentControl_1.RESV_CH7_Target_Current = 0;
  MVD_ValvesCurrentControl_1.RESV_CH8_Target_Current = 0;
  com.transmit_canfd_async(&MVD_ValvesCurrentControl_1.FCAN);
  app.wait(100, "");
  MVD_ValvesCurrentControl_1.VV_Target_Current = 1.35;
  com.transmit_canfd_async(&MVD_ValvesCurrentControl_1.FCAN);

  u32 ATimerTick = 0;
  s32 AResult = 0;

  while(1) {
    if(vDeviceSensorPressureValue[ADUTIndex][ATankIndex] < ATargetPressureBar) {
      test_log_nok("Deflate tank to target pressure finished!");
      AResult = 0;
      break;
    }
    app.wait(1, "");
    ATimerTick++;
    if(ATimerTick >= 30000) {
      test_log_nok("Deflate tank to target pressure time out (30s)!");
      AResult = -10;
      break;
    }
  }
  // 关闭所有阀体
  dut_vd_valve_off(ADUTIndex);
  dut_mvd_valve_off(ADUTIndex);
  app.wait(5, "");
  dut_power_off(ADUTIndex);
  app.wait(5, "");
  vManualClearErrorHeartbeatActived[ADUTIndex] = 0; // 关闭执行状态
  return AResult;

}
// CODE BLOCK END Global_Definitions 

// GEN BLOCK BEGIN Custom_Function
s32 dut_on_start(const s32 ADUTIndex);
s32 dut_on_stop(const s32 ADUTIndex);
s32 dut_on_recieve(const s32 ADUTIndex, const TCANFD* ACANFD);
s32 dut_on_pretx(const PCANFD ACANFD);
s32 stop_test_action(const s32 ADUTIndex);
s32 dut_rbs_init(const s32 ADUTIndex);
s32 dut_step(const s32 ADUTIndex);
s32 bench_state_step(void);
s32 on_tsmaster_disconnect(void);
s32 fault_action_deflate_fl(const s32 ADUTIndex, const float ATargetPressureBar);
s32 fault_action_deflate_fr(const s32 ADUTIndex, const float ATargetPressureBar);
s32 fault_action_deflate_rl(const s32 ADUTIndex, const float ATargetPressureBar);
s32 fault_action_deflate_rr(const s32 ADUTIndex, const float ATargetPressureBar);
s32 fault_action_deflate_res(const s32 ADUTIndex, const float ATargetPressureBar);
s32 fault_action_clear_test_case_error(const s32 ADUTIndex);
s32 dut_reset_test_statistics_data(const s32 ADUTIndex);
s32 on_start_button_click(const double ADummy);
s32 on_stop_button_click(const double ADummy);
s32 on_reset_data_button_click(const double ADummy);
s32 on_clear_fault_button_click(const double ADummy);
s32 error_stop_all_test(void);
// GEN BLOCK END Custom_Function

// CODE BLOCK BEGIN Custom_Function dut_on_start Y29uc3QgczMyIEFEVVRJbmRleA__
// 自定义函数 "dut_on_start"
s32 dut_on_start(const s32 ADUTIndex) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    app.terminate_application();
    return -1;
  }


  // TM初始化是否成功 {第一个OnStart中初始化}
  // vTMHasError = 0;             //0: no error; 1:有错误

  //related with system var
//   vDUTEnabled[ADUTIndex] = 0;    //0: disable; 1：enable
  vDUTState[ADUTIndex] = 0;    //0: ready; 1：Running; 2: Error;
  vDUTIsStopped[ADUTIndex] = 1;    //0: ready; 1：Running; 2: Error;
  vDUTIsRunning[ADUTIndex] = 0;    //0: ready; 1：Running; 2: Error;
  vDUTHasError[ADUTIndex] = 0;    //0: ready; 1：Running; 2: Error;
  vDUTTotalTestCount[ADUTIndex] = 0;

  // 上电后板卡诊断——中间变量 {第一个OnStart中初始化}
  //vDriverDiagStep = 0; //0: 发送所有阀体和电机关闭报文; 1: 打开电源开关； 2：等待1s，完成电流采样； 3：进行诊断，并关闭电源； 4：诊断完成
  // 上电后板卡诊断——系统变量
  vVDDriverHasError[ADUTIndex] = 0;             //0: no error; 1:有错误
  vMVDDriverHasError[ADUTIndex] = 0;             //0: no error; 1:有错误

  // 信号丢失——系统变量
  vPWRIsDisconnected[ADUTIndex] = 0;    //0: PWR Connected; 1: PWR Disconnected
  vVDIsDisconnected[ADUTIndex] = 0;    //0: VD Connected; 1: VD Disconnected
  vMVDIsDisconnected[ADUTIndex] = 0;    //0: MVD Connected; 1: MVD Disconnected
  vDUTSensorIsDisconnected[ADUTIndex] = 0;    //0: DUT Sensor Connected; 1: DUT Sensor  Disconnected
  vDeviceSensorIsDisconnected[ADUTIndex] = 0;    //0: Device Sensors Connected; 1: Device Sensors Disconnected
  // 信号丢失——中间记录变量
  vPWRMonitorCount[ADUTIndex] = 0;  //PWR
  vMVDMonitorCount[ADUTIndex] = 0;  //MVD
  vVDMonitorCount[ADUTIndex] = 0;   //VD
  vDeviceSensorsMonitorCount[ADUTIndex] = 0;  //Device Sensors
  vDUTSensorMonitorCount[ADUTIndex] = 0;  //DUT Sensors
  // 信号卡滞——系统变量
  vVDIsStuck[ADUTIndex] = 0;    //0: VD Normal; 1: VD Stuck
  vMVDIsStuck[ADUTIndex] = 0;    //0: MVD Normal; 1: MVD Stuck
  vDeviceSensorIsStuck[ADUTIndex] = 0;    //0: Device Sensors Normal; 1: Device Sensors Stuck
  // 信号卡滞——中间记录变量
  for(u8 AIndex = 0; AIndex < 8; AIndex++) {
    vVDLastCurrentRawValue[ADUTIndex][AIndex] = 0;
    vMVDLastCurrentRawValue[ADUTIndex][AIndex] = 0;
  }
  for(u8 AIndex = 0; AIndex < 5; AIndex++) {
    vDeviceSensorLastPressureRawValue[ADUTIndex][AIndex] = 0; //Device Sensors Pressure Raw Value
  }
  // 信号卡滞——中间记录变量
  vMVDStuckMonitorCount[ADUTIndex] = 0;  //MVD
  vVDStuckMonitorCount[ADUTIndex] = 0;   //VD
  vDeviceSensorsStuckMonitorCount[ADUTIndex] = 0;  //Device Sensors

  // 记录最新的CAN 信号值
  for(u8 AIndex = 0; AIndex < 8; AIndex++) {
    vVDCurrentValue[ADUTIndex][AIndex] = 0;  //vD 电流   
    vMVDCurrentValue[ADUTIndex][AIndex] = 0;  //MVD 电流
  }
  for(u8 AIndex = 0; AIndex < 5; AIndex++) {

    vDeviceSensorPressureValue[ADUTIndex][AIndex] = 0; //压力传感器值
  }
  // 信号值错误--系统变量
  vVDSignalError[ADUTIndex] = 0; //VD Signal Error Flag
  vMVDSignalError[ADUTIndex] = 0; //MVD Signal Error Flag
  vDUTSensorSignalError[ADUTIndex] = 0; //DUT Sensor Signal Error Flag
  vDeviceSensorSignalError[ADUTIndex] = 0; //Device Sensor Signal Error Flag
  vDeviceSensorMaxPressureReached[ADUTIndex] = 0; //Device Sensor Max Pressure Reached Flag
  // 板卡保护--系统变量
  vVDInProtection[ADUTIndex] = 0; //VD In Protection Flag
  vMVDInProtection[ADUTIndex] = 0; //MVD In Protection Flag
  // 测试用例错误--系统变量
  vTestCaseError[ADUTIndex] = 0; //DUT Test Case Error Flag
  vTestCaseErrorCode[ADUTIndex] = 0; //DUT Test Case Error Flag
  vTestCaseRunState[ADUTIndex] = 0; // DUT Test Case Run State

  // 手动清除错误Action相关变量
  vManualClearErrorHeartbeatActived[ADUTIndex] = 0; //DUT Manual Clear Error Action Flag

  // 根据全局变量设置系统变量
  // 样件状态
  change_sysvar_dut_state(ADUTIndex, 0);
  // 如下变量在dut state 中切换
  // set_dut_sysvar_error_state(ADUTIndex, "IsStopped", vDUTIsStopped[ADUTIndex]);
  // set_dut_sysvar_error_state(ADUTIndex, "IsRunning", vDUTIsRunning[ADUTIndex]);
  // set_dut_sysvar_error_state(ADUTIndex, "HasError", vDUTHasError[ADUTIndex]);
  // 上电诊断
  set_dut_sysvar_error_state(ADUTIndex, "VDDriverHasError", vVDDriverHasError[ADUTIndex]);
  set_dut_sysvar_error_state(ADUTIndex, "MVDDriverHasError", vMVDDriverHasError[ADUTIndex]);
  // 链接错误重置
  set_dut_sysvar_error_state(ADUTIndex, "PWRIsDisconnected", vPWRIsDisconnected[ADUTIndex]);
  set_dut_sysvar_error_state(ADUTIndex, "VDIsDisconnected", vVDIsDisconnected[ADUTIndex]);
  set_dut_sysvar_error_state(ADUTIndex, "MVDIsDisconnected", vMVDIsDisconnected[ADUTIndex]);
  set_dut_sysvar_error_state(ADUTIndex, "DUTSensorIsDisconnected", vDUTSensorIsDisconnected[ADUTIndex]);
  set_dut_sysvar_error_state(ADUTIndex, "DeviceSensorIsDisconnected", vDeviceSensorIsDisconnected[ADUTIndex]);
  // 卡滞错误重置
  set_dut_sysvar_error_state(ADUTIndex, "VDIsStuck", vVDIsStuck[ADUTIndex]);
  set_dut_sysvar_error_state(ADUTIndex, "MVDIsStuck", vMVDIsStuck[ADUTIndex]);
  set_dut_sysvar_error_state(ADUTIndex, "DeviceSensorIsStuck", vDeviceSensorIsStuck[ADUTIndex]);
  // 信号错误重置
  set_dut_sysvar_error_state(ADUTIndex, "VDSignalError", vVDSignalError[ADUTIndex]);
  set_dut_sysvar_error_state(ADUTIndex, "MVDSignalError", vMVDSignalError[ADUTIndex]);
  set_dut_sysvar_error_state(ADUTIndex, "DUTSensorSignalError", vDUTSensorSignalError[ADUTIndex]);
  set_dut_sysvar_error_state(ADUTIndex, "DeviceSensorSignalError", vDeviceSensorSignalError[ADUTIndex]);
  set_dut_sysvar_error_state(ADUTIndex, "DeviceSensorMaxPressureReached", vDeviceSensorMaxPressureReached[ADUTIndex]);
  // 板卡保护重置
  set_dut_sysvar_error_state(ADUTIndex, "VDInProtection", vVDInProtection[ADUTIndex]);
  set_dut_sysvar_error_state(ADUTIndex, "MVDInProtection", vMVDInProtection[ADUTIndex]);
  // 测试用例错误重置
  set_dut_sysvar_error_state(ADUTIndex, "TestCaseError", vTestCaseError[ADUTIndex]);
  set_dut_sysvar_error_state(ADUTIndex, "TestCaseErrorCode", vTestCaseErrorCode[ADUTIndex]);


  // app.check(app.get_system_var_int32("TC.TotalTestCount", &vTotalTestCount));

  // std::string strRunCount = dut_sysvar_name(ADUTIndex, "RunCount");
  // app.check(app.get_system_var_int32(strRunCount.c_str(), &vRunCount[ADUTIndex]));

  // std::string strDUTState = dut_sysvar_name(ADUTIndex, "State");
  // //app.check(app.get_system_var_int32(strDUTState.c_str(), &vDUTState[ADUTIndex]));
  // std::string strTotalTestCount = dut_sysvar_name(ADUTIndex, "TotalTestCount");
  // app.check(app.get_system_var_int32("DUT1.TotalTestCount", &vDUTTotalTestCount[ADUTIndex]));

  // if(0 == vDUTState[ADUTIndex]) {
  //   ;
  // }
  // else if(1 == vDUTState[ADUTIndex]) {
  //   ;
  // }
  // else if(2 == vDUTState[ADUTIndex]) {
  //   ;
  // }
  // else {
  //   test_log_nok("DUT%d.State is not valid", ADUTIndex);
  //   app.terminate_application();
  //   return -1;
  // }
  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function dut_on_start

// CODE BLOCK BEGIN Custom_Function dut_on_stop Y29uc3QgczMyIEFEVVRJbmRleA__
// 自定义函数 "dut_on_stop"
s32 dut_on_stop(const s32 ADUTIndex) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 

  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function dut_on_stop

// CODE BLOCK BEGIN On_Start On_Start_System_Set
// 启动事件 "On_Start_System_Set"
void on_start_On_Start_System_Set(void) { // 程序启动事件
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
 // 程序启动事件
 // 程序启动事件
 // 程序启动事件
  vTMHasError = 0;
  vDriverDiagStep = 0;
  if(0 != app.get_system_var_int32("DUT1.Enabled", &vDUTEnabled[0])) {
    test_log_nok("DUT1 Enabled System Variable is not exist");
    vTMHasError = 1; // TM has error
    return;
  }
  if(0 != app.get_system_var_int32("DUT2.Enabled", &vDUTEnabled[1])) {
    test_log_nok("DUT2 Enabled System Variable is not exist");
    vTMHasError = 1; // TM has error
    return;
  }
  if(0 != app.get_system_var_int32("DUT3.Enabled", &vDUTEnabled[2])) {
    test_log_nok("DUT3 Enabled System Variable is not exist");
    vTMHasError = 1; // TM has error
    return;
  }
  
  //初始化面板显示
  app.panel_set_enable("测试监控窗体", "PBStart", true);
  app.panel_set_bkgd_color("测试监控窗体", "PBStart", 0xFF008000);
  app.panel_set_enable("测试监控窗体", "PBStop", false);
  app.panel_set_bkgd_color("测试监控窗体", "PBStop", 0xFF808080);
  app.panel_set_enable("测试监控窗体", "PBClearFault", false);
  app.panel_set_bkgd_color("测试监控窗体", "PBClearFault", 0xFF808080);
  app.panel_set_enable("测试监控窗体", "PBResetData", true);
  app.panel_set_bkgd_color("测试监控窗体", "PBResetData", 0xFF0000FF);
  app.panel_set_enable("测试监控窗体", "CBDUT1Enable", true);
  app.panel_set_enable("测试监控窗体", "CBDUT2Enable", true);
  app.panel_set_enable("测试监控窗体", "CBDUT3Enable", true);

  // 初始化样件启动变量
  for(u8 i = 0; i < DUT_CHANNEL_COUNT; i++) {
    if(1 == vDUTEnabled[i]) {
      dut_on_start(i);
    }
  }

  // 开启网络仿真，并激活对应样件的报文发送功能
  com.can_rbs_start();
  com.can_rbs_enable(false);
  for(u8 i = 0; i < DUT_CHANNEL_COUNT; i++) {
    if(1 == vDUTEnabled[i]) {
      dut_rbs_init(i);
    }
  }
  com.can_rbs_enable(true);

}
// CODE BLOCK END On_Start On_Start_System_Set

// CODE BLOCK BEGIN Custom_Function dut_on_recieve Y29uc3QgczMyIEFEVVRJbmRleCwgY29uc3QgVENBTkZEKiBBQ0FORkQ_
// 自定义函数 "dut_on_recieve"
s32 dut_on_recieve(const s32 ADUTIndex, const TCANFD* ACANFD) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    app.terminate_application();
    return -1;
  }
  if(ACANFD->FIdxChn == ADUTIndex) {
    // PWR
    if(0xF002 == ACANFD->FIdentifier) {
      vPWRMonitorCount[ADUTIndex] = 0;
    }
    else if(0x3001 == ACANFD->FIdentifier) {  //VD电流
      vVDMonitorCount[ADUTIndex] = 0;   //重置Count
      u8 ASensorsValueErrorFlag = 0;
      u8 ASensorsStuckErrorFlag = 0;
      TVD_ActualCurrent_1 VD_ActualCurrent_1;
      VD_ActualCurrent_1.init(); // 在使用此结构体前调用这段初始化代码
      VD_ActualCurrent_1.FCAN = *ACANFD; // 若是在回调函数中，请用ACAN数据赋值FCAN
      // deal with signals using VD_ActualCurrent_1.XXX
      // 获取当前电流值
      vVDCurrentValue[ADUTIndex][0] = VD_ActualCurrent_1.RV1_I; // RV1_I
      vVDCurrentValue[ADUTIndex][1] = VD_ActualCurrent_1.RV2_I; // RV2_I
      vVDCurrentValue[ADUTIndex][2] = VD_ActualCurrent_1.RV3_I; // RV2_I
      vVDCurrentValue[ADUTIndex][3] = VD_ActualCurrent_1.RV4_I; // RV2_I
      vVDCurrentValue[ADUTIndex][4] = VD_ActualCurrent_1.ASV1_I; // RV2_I
      vVDCurrentValue[ADUTIndex][5] = VD_ActualCurrent_1.ASV2_I; // RV2_I
      vVDCurrentValue[ADUTIndex][6] = VD_ActualCurrent_1.ASV3_I; // RV2_I
      vVDCurrentValue[ADUTIndex][7] = VD_ActualCurrent_1.ASV4_I; // RV2_I
      for(u8 AIndex = 0; AIndex < 8; AIndex++) {
        u16 ARawData = *(u16*) &(VD_ActualCurrent_1.FCAN.FData[2 * AIndex]);
        if(vVDLastCurrentRawValue[ADUTIndex][AIndex] == ARawData) vVDStuckMonitorCount[ADUTIndex]++;
        else vVDStuckMonitorCount[ADUTIndex] = 0; // 重置Count
        // 更新最新的原始数据
        vVDLastCurrentRawValue[ADUTIndex][AIndex] = ARawData;
        // 判断是否存在信号Stuck
        if(vVDStuckMonitorCount[ADUTIndex] > MAX_STUCK_COUNT) {
          vVDStuckMonitorCount[ADUTIndex] = MAX_STUCK_COUNT;
          ASensorsStuckErrorFlag = 1; // VD 电流卡滞
        }
        // 判断是否存在信号超限
        if((vVDCurrentValue[ADUTIndex][AIndex] > MAX_VALVE_CURRENT) || (vVDCurrentValue[ADUTIndex][AIndex] < MIN_VALVE_CURRENT)) {
          ASensorsValueErrorFlag = 1; // VD 电流异常
        }
      }

      if(ASensorsStuckErrorFlag > 0) vVDIsStuck[ADUTIndex] = 1;   // VD 电流卡滞
      else vVDIsStuck[ADUTIndex] = 0; // VD 电流正常
      set_dut_sysvar_error_state(ADUTIndex, "VDIsStuck", vVDIsStuck[ADUTIndex]);


      if(ASensorsValueErrorFlag > 0) vVDSignalError[ADUTIndex] = 1; // VD 电流异常
      else vVDSignalError[ADUTIndex] = 0; // VD 电流正常 
      set_dut_sysvar_error_state(ADUTIndex, "VDSignalError", vVDSignalError[ADUTIndex]);


    }
    else if(0x3002 == ACANFD->FIdentifier) { //VD状态
      u8 AInProtectionFlag = 0;
      u8 AIndex = 0;
      for(AIndex = 0; AIndex < 8; AIndex++) {
        if(ACANFD->FData[AIndex] == 3) { // VD In Protection Flag
          AInProtectionFlag = 1;
        }
      }
      if(AInProtectionFlag)  vVDInProtection[ADUTIndex] = 1; // VD In Protection Flag
      else vVDInProtection[ADUTIndex] = 0; // VD In Protection Flag

      set_dut_sysvar_error_state(ADUTIndex, "VDInProtection", vVDInProtection[ADUTIndex]);

    }
    else if(0x3101 == ACANFD->FIdentifier) {  //MVD电流
      vMVDMonitorCount[ADUTIndex] = 0;  //重置Count
      u8 ASensorsValueErrorFlag = 0;  //错误标志位
      u8 ASensorsStuckErrorFlag = 0;  //卡滞标志位
      TMVD_ActualCurrent_1 MVD_ActualCurrent_1;
      MVD_ActualCurrent_1.init(); // 在使用此结构体前调用这段初始化代码
      MVD_ActualCurrent_1.FCAN = *ACANFD; // 若是在回调函数中，请用ACAN数据赋值FCAN
      // deal with signals using MVD_ActualCurrent_1.XXX
      // 获取当前电流值
      vMVDCurrentValue[ADUTIndex][0] = MVD_ActualCurrent_1.Motor_I;
      vMVDCurrentValue[ADUTIndex][1] = MVD_ActualCurrent_1.RESV_M_CH2_I;
      vMVDCurrentValue[ADUTIndex][2] = MVD_ActualCurrent_1.RESV_V_CH3_I;
      vMVDCurrentValue[ADUTIndex][3] = MVD_ActualCurrent_1.RESV_V_CH4_I;
      vMVDCurrentValue[ADUTIndex][4] = MVD_ActualCurrent_1.VV_I;
      vMVDCurrentValue[ADUTIndex][5] = MVD_ActualCurrent_1.RESV_V_CH6_I;
      vMVDCurrentValue[ADUTIndex][6] = MVD_ActualCurrent_1.RESV_V_CH7_I;
      vMVDCurrentValue[ADUTIndex][7] = MVD_ActualCurrent_1.RESV_V_CH8_I;

      for(u8 AIndex = 0; AIndex < 8; AIndex++) {
        u16 ARawData = *(u16*) &(MVD_ActualCurrent_1.FCAN.FData[2 * AIndex]);
        if(vMVDLastCurrentRawValue[ADUTIndex][AIndex] == ARawData) vMVDStuckMonitorCount[ADUTIndex]++;
        else vMVDStuckMonitorCount[ADUTIndex] = 0; // 重置Count
        // 更新最新的原始数据
        vMVDLastCurrentRawValue[ADUTIndex][AIndex] = ARawData;
        if(0 == AIndex) {  //motor 单独判断
          // 判断是否存在信号Stuck
          if(vMVDStuckMonitorCount[ADUTIndex] > MAX_STUCK_COUNT) {
            vMVDStuckMonitorCount[ADUTIndex] = MAX_STUCK_COUNT;
            ASensorsStuckErrorFlag = 1; // MVD 电流卡滞 Motor
          }
          // 判断是否存在信号超限
          if((vMVDCurrentValue[ADUTIndex][AIndex] > MAX_MOTOR_CURRENT) || (vMVDCurrentValue[ADUTIndex][AIndex] < MIN_MOTOR_CURRENT)) {
            ASensorsValueErrorFlag = 1; // MVD 电流异常 Motor
          }
        }
        else if(AIndex == 4) {  //VV_I 单独判断
          // 判断是否存在信号Stuck
          if(vMVDStuckMonitorCount[ADUTIndex] > MAX_STUCK_COUNT) {
            vMVDStuckMonitorCount[ADUTIndex] = MAX_STUCK_COUNT;
            ASensorsStuckErrorFlag = 1; // MVD 电流卡滞 VV
          }
          // 判断是否存在信号超限
          if((vMVDCurrentValue[ADUTIndex][AIndex] > MAX_VALVE_CURRENT) || (vMVDCurrentValue[ADUTIndex][AIndex] < MIN_VALVE_CURRENT)) {
            ASensorsValueErrorFlag = 1; // MVD 电流异常 VV
          }
        }
      }
      if(ASensorsStuckErrorFlag > 0) vMVDIsStuck[ADUTIndex] = 1;   // VD 电流卡滞
      else vMVDIsStuck[ADUTIndex] = 0; // VD 电流正常


      if(ASensorsValueErrorFlag > 0) vMVDSignalError[ADUTIndex] = 1; // VD 电流异常
      else vMVDSignalError[ADUTIndex] = 0; // VD 电流正常 

      set_dut_sysvar_error_state(ADUTIndex, "MVDIsStuck", vMVDIsStuck[ADUTIndex]);
      set_dut_sysvar_error_state(ADUTIndex, "MVDSignalError", vMVDSignalError[ADUTIndex]);
    }
    else if(0x3102 == ACANFD->FIdentifier) {  //MVD状态
      u8 AInProtectionFlag = 0;
      u8 AIndex = 0;
      for(AIndex = 0; AIndex < 8; AIndex++) {
        if(ACANFD->FData[AIndex] == 3) { // MVD In Protection Flag
          AInProtectionFlag = 1;
        }
      }
      if(AInProtectionFlag) { // MVD In Protection Flag
        vMVDInProtection[ADUTIndex] = 1; // MVD In Protection Flag
      }
      else {
        vMVDInProtection[ADUTIndex] = 0; // MVD In Protection Flag
      }
      set_dut_sysvar_error_state(ADUTIndex, "MVDInProtection", vMVDInProtection[ADUTIndex]);
    }
    else if(0x3205 == ACANFD->FIdentifier) {
      vDUTSensorMonitorCount[ADUTIndex] = 0;
      //todo: 增加传感器失效检测逻辑
      TADSensor_SersorValues_1 ADSensor_SersorValues_1;
      ADSensor_SersorValues_1.init(); // 在使用此结构体前调用这段初始化代码
      ADSensor_SersorValues_1.FCAN = *ACANFD; // 若是在回调函数中，请用ACAN数据赋值FCAN
      // deal with signals using ADSensor_SersorValues_1.XXX
      if(ADSensor_SersorValues_1.P2_Invalid > 0.5) {
        vDUTSensorSignalError[ADUTIndex] = 1; // DUT Sensor 失效
      }
      else {
        vDUTSensorSignalError[ADUTIndex] = 0; // DUT Sensor 正常 
      }
      set_dut_sysvar_error_state(ADUTIndex, "DUTSensorSignalError", vDUTSensorSignalError[ADUTIndex]);

    }
  }
  else if(ACANFD->FIdxChn == CH4) {
    s32 AIdentifier = 0x3001 + 0x100 * ADUTIndex;
    // test_log_nok("AIdentifier is %x, index is %d", AIdentifier, ADUTIndex);
    if(AIdentifier == ACANFD->FIdentifier) {
      vDeviceSensorsMonitorCount[ADUTIndex] = 0;
      // test_log_nok("index is %d cleared", ADUTIndex);
      u8 ASensorsValueErrorFlag = 0;  //错误标志位
      u8 ASensorsStuckErrorFlag = 0;  //卡滞标志位
      u8 AMaxPressureReached = 0;
      TDUT1_Sensors_4 DUT1_Sensors_4;
      DUT1_Sensors_4.init(); // 在使用此结构体前调用这段初始化代码
      DUT1_Sensors_4.FCAN = *ACANFD; // 若是在回调函数中，请用ACAN数据赋值FCAN
      // deal with signals using DUT1_Sensors_4.XXX
      // 获取当前压力值
      vDeviceSensorPressureValue[ADUTIndex][0] = DUT1_Sensors_4.Pressure_FL;
      vDeviceSensorPressureValue[ADUTIndex][1] = DUT1_Sensors_4.Pressure_FR;
      vDeviceSensorPressureValue[ADUTIndex][2] = DUT1_Sensors_4.Pressure_RL;
      vDeviceSensorPressureValue[ADUTIndex][3] = DUT1_Sensors_4.Pressure_RR;
      vDeviceSensorPressureValue[ADUTIndex][4] = DUT1_Sensors_4.Pressure_RES;
      // 更新最新的原始数据
      for(u8 AIndex = 0; AIndex < 5; AIndex++) {
        u16 ARawData = *(u16*) &(DUT1_Sensors_4.FCAN.FData[2 * AIndex]);
        if(vDeviceSensorLastPressureRawValue[ADUTIndex][AIndex] == ARawData) vDeviceSensorsStuckMonitorCount[ADUTIndex]++;
        else vDeviceSensorsStuckMonitorCount[ADUTIndex] = 0; // 重置Count
        // 更新最新的原始数据
        vDeviceSensorLastPressureRawValue[ADUTIndex][AIndex] = ARawData;
        // 判断是否存在信号Stuck
        if(vDeviceSensorsStuckMonitorCount[ADUTIndex] > MAX_STUCK_COUNT) {
          vDeviceSensorsStuckMonitorCount[ADUTIndex] = MAX_STUCK_COUNT;
          ASensorsStuckErrorFlag = 1; // Device Sensors 卡滞
        }
        // 判断是否存在信号超限
        if((vDeviceSensorPressureValue[ADUTIndex][AIndex] > MAX_SENSOR_PRESSURE) || (vDeviceSensorPressureValue[ADUTIndex][AIndex] < MIN_SENSOR_PRESSURE)) {
          ASensorsValueErrorFlag = 1; // Device Sensors 失效
        }

        if(vDeviceSensorPressureValue[ADUTIndex][AIndex] > MAX_SAFETY_PRESSURE) {
          AMaxPressureReached = 1; // Device Sensors 超过 安全值
        }
      }

      if(ASensorsStuckErrorFlag > 0) vDeviceSensorIsStuck[ADUTIndex] = 1;   // Device Sensors 卡滞
      else vDeviceSensorIsStuck[ADUTIndex] = 0; // Device Sensors 正常

      if(ASensorsValueErrorFlag > 0) vDeviceSensorSignalError[ADUTIndex] = 1; // Device Sensors 失效
      else vDeviceSensorSignalError[ADUTIndex] = 0; // Device Sensors 正常

      if(AMaxPressureReached) vDeviceSensorMaxPressureReached[ADUTIndex] = 1; // Device Sensors 超过 安全值
      else vDeviceSensorMaxPressureReached[ADUTIndex] = 0; // Device Sensors 正常

      set_dut_sysvar_error_state(ADUTIndex, "DeviceSensorIsStuck", vDeviceSensorIsStuck[ADUTIndex]);
      set_dut_sysvar_error_state(ADUTIndex, "DeviceSensorSignalError", vDeviceSensorSignalError[ADUTIndex]);
      set_dut_sysvar_error_state(ADUTIndex, "DeviceSensorMaxPressureReached", vDeviceSensorMaxPressureReached[ADUTIndex]);
    }
  }

  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function dut_on_recieve

// CODE BLOCK BEGIN Custom_Function dut_on_pretx Y29uc3QgUENBTkZEIEFDQU5GRA__
// 自定义函数 "dut_on_pretx"
s32 dut_on_pretx(const PCANFD ACANFD) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  s32 APressureSensorFault = vDeviceSensorSignalError[0] + vDeviceSensorSignalError[1] + vDeviceSensorSignalError[2];
  APressureSensorFault = APressureSensorFault + vDeviceSensorMaxPressureReached[0] + vDeviceSensorMaxPressureReached[1] + vDeviceSensorMaxPressureReached[2];
  // 压力传感器超限或者信号错误，禁止电机操作
  if(APressureSensorFault == 0) return IDX_ERR_OK;  //无传感器错误，不用干预
  if(ACANFD->FIdxChn > CH3) return IDX_ERR_OK; // if you want to filter channel
  if(ACANFD->FIdentifier != 0x3104) return IDX_ERR_OK; // if you want to filter identifier
  TMVD_MotorsDutyControl_1 MVD_MotorsDutyControl_1;
  MVD_MotorsDutyControl_1.init();
  MVD_MotorsDutyControl_1.FCAN = *ACANFD; // 若是在回调函数中，请用ACAN数据赋值FCAN
  // deal with signals using MVD_MotorsDutyControl_1.XXX
  MVD_MotorsDutyControl_1.Motor_Duty = 0;
  MVD_MotorsDutyControl_1.RESV_M_CH2_Duty = 0;
  *ACANFD = MVD_MotorsDutyControl_1.FCAN;
  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function dut_on_pretx

// CODE BLOCK BEGIN On_Stop On_DUT_Stop
// 停止事件 "On_DUT_Stop"
void on_stop_On_DUT_Stop(void) { // 程序停止事件
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
 // 程序停止事件
 // 程序停止事件
 // 程序停止事件
  //dut_on_stop(0);
  on_tsmaster_disconnect();
  app.disconnect();

}
// CODE BLOCK END On_Stop On_DUT_Stop

// CODE BLOCK BEGIN Custom_Function stop_test_action Y29uc3QgczMyIEFEVVRJbmRleA__
// 自定义函数 "stop_test_action"
s32 stop_test_action(const s32 ADUTIndex) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    app.terminate_application();
    return -1;
  }
  // 关闭对应自动化模块
  std::string strModuleName = "DUT" + std::to_string(ADUTIndex + 1) + "_Cycle";
  if(0 != app.am_stop(strModuleName.c_str(), true)) {
    std::string strErrorInfo = "stop test cycle failed: " + strModuleName;
    app.log_text(strErrorInfo.c_str(), lvlError);
    app.terminate_application();
  }
  //关闭PWR
  {
    // [1] CAN FD 0xF001 PWR_Control 
    TCANFD f0 = {u8(ADUTIndex),0x5,2,0x3,0xF001,0,{0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(1, "");
  }

  //关闭所有泵阀
  {
    // [1] CAN FD 0x3104 MVD_MotorsDutyControl 
    TCANFD f0 = {u8(ADUTIndex),0x5,4,0x3,0x3104,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(1, "");
  }


  {
    // [1] CAN FD 0x3005 VD_Current_Control 
    TCANFD f0 = {u8(ADUTIndex),0x5,10,0x3,0x3005,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(1, "");
  }


  {
    // [1] CAN FD 0x3105 MVD_ValvesCurrentControl 
    TCANFD f0 = {u8(ADUTIndex),0x5,10,0x3,0x3105,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(1, "");
  }





  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function stop_test_action

// CODE BLOCK BEGIN Custom_Function dut_rbs_init Y29uc3QgczMyIEFEVVRJbmRleA__
// 自定义函数 "dut_rbs_init"
s32 dut_rbs_init(const s32 ADUTIndex) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 

  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    app.terminate_application();
    return -1;
  }

  if(0 == ADUTIndex) {
    // Network: CAN1_DUT1
    com.can_rbs_activate_network_by_name(CH1, true, "CAN1_DUT1", false);
    // Node: ControlNode
    com.can_rbs_activate_node_by_name(CH1, true, "CAN1_DUT1", "ControlNode", false);
    // Message: PWR_Control
    com.can_rbs_activate_message_by_name(CH1, true, "CAN1_DUT1", "ControlNode", "PWR_Control");
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/PWR_Control/SetPower12V", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/PWR_Control/SetPower48V", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/PWR_Control/Enable12VPowerSet", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/PWR_Control/Enable48VPowerSet", 0);
    // Message: VD_Current_Control
    com.can_rbs_activate_message_by_name(CH1, true, "CAN1_DUT1", "ControlNode", "VD_Current_Control");
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/VD_Current_Control/RV1_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/VD_Current_Control/RV2_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/VD_Current_Control/RV3_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/VD_Current_Control/RV4_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/VD_Current_Control/ASV1_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/VD_Current_Control/ASV2_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/VD_Current_Control/ASV3_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/VD_Current_Control/ASV4_Target_Current", 0);
    // Message: MVD_MotorsDutyControl
    com.can_rbs_activate_message_by_name(CH1, true, "CAN1_DUT1", "ControlNode", "MVD_MotorsDutyControl");
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/MVD_MotorsDutyControl/Motor_Duty", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/MVD_MotorsDutyControl/RESV_M_CH2_Duty", 0);
    // Message: MVD_ValvesCurrentControl
    com.can_rbs_activate_message_by_name(CH1, true, "CAN1_DUT1", "ControlNode", "MVD_ValvesCurrentControl");
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/MVD_ValvesCurrentControl/RESV_CH3_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/MVD_ValvesCurrentControl/RESV_CH4_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/MVD_ValvesCurrentControl/VV_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/MVD_ValvesCurrentControl/RESV_CH6_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/MVD_ValvesCurrentControl/RESV_CH7_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("0/CAN1_DUT1/ControlNode/MVD_ValvesCurrentControl/RESV_CH8_Target_Current", 0);

  }

  else if(1 == ADUTIndex) {


    // Network: CAN2_DUT2
    com.can_rbs_activate_network_by_name(CH2, true, "CAN2_DUT2", false);
    // Node: ControlNode
    com.can_rbs_activate_node_by_name(CH2, true, "CAN2_DUT2", "ControlNode", false);
    // Message: PWR_Control
    com.can_rbs_activate_message_by_name(CH2, true, "CAN2_DUT2", "ControlNode", "PWR_Control");
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/PWR_Control/SetPower12V", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/PWR_Control/SetPower48V", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/PWR_Control/Enable12VPowerSet", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/PWR_Control/Enable48VPowerSet", 0);
    // Message: VD_Current_Control
    com.can_rbs_activate_message_by_name(CH2, true, "CAN2_DUT2", "ControlNode", "VD_Current_Control");
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/VD_Current_Control/RV1_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/VD_Current_Control/RV2_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/VD_Current_Control/RV3_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/VD_Current_Control/RV4_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/VD_Current_Control/ASV1_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/VD_Current_Control/ASV2_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/VD_Current_Control/ASV3_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/VD_Current_Control/ASV4_Target_Current", 0);
    // Message: MVD_MotorsDutyControl
    com.can_rbs_activate_message_by_name(CH2, true, "CAN2_DUT2", "ControlNode", "MVD_MotorsDutyControl");
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/MVD_MotorsDutyControl/Motor_Duty", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/MVD_MotorsDutyControl/RESV_M_CH2_Duty", 0);
    // Message: MVD_ValvesCurrentControl
    com.can_rbs_activate_message_by_name(CH2, true, "CAN2_DUT2", "ControlNode", "MVD_ValvesCurrentControl");
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/MVD_ValvesCurrentControl/RESV_CH3_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/MVD_ValvesCurrentControl/RESV_CH4_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/MVD_ValvesCurrentControl/VV_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/MVD_ValvesCurrentControl/RESV_CH6_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/MVD_ValvesCurrentControl/RESV_CH7_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("1/CAN2_DUT2/ControlNode/MVD_ValvesCurrentControl/RESV_CH8_Target_Current", 0);
  }

  else if(2 == ADUTIndex) {

    // Network: CAN3_DUT3
    com.can_rbs_activate_network_by_name(CH3, true, "CAN3_DUT3", false);
    // Node: ControlNode
    com.can_rbs_activate_node_by_name(CH3, true, "CAN3_DUT3", "ControlNode", false);
    // Message: PWR_Control
    com.can_rbs_activate_message_by_name(CH3, true, "CAN3_DUT3", "ControlNode", "PWR_Control");
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/PWR_Control/SetPower12V", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/PWR_Control/SetPower48V", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/PWR_Control/Enable12VPowerSet", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/PWR_Control/Enable48VPowerSet", 0);
    // Message: VD_Current_Control
    com.can_rbs_activate_message_by_name(CH3, true, "CAN3_DUT3", "ControlNode", "VD_Current_Control");
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/VD_Current_Control/RV1_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/VD_Current_Control/RV2_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/VD_Current_Control/RV3_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/VD_Current_Control/RV4_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/VD_Current_Control/ASV1_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/VD_Current_Control/ASV2_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/VD_Current_Control/ASV3_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/VD_Current_Control/ASV4_Target_Current", 0);
    // Message: MVD_MotorsDutyControl
    com.can_rbs_activate_message_by_name(CH3, true, "CAN3_DUT3", "ControlNode", "MVD_MotorsDutyControl");
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/MVD_MotorsDutyControl/Motor_Duty", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/MVD_MotorsDutyControl/RESV_M_CH2_Duty", 0);
    // Message: MVD_ValvesCurrentControl
    com.can_rbs_activate_message_by_name(CH3, true, "CAN3_DUT3", "ControlNode", "MVD_ValvesCurrentControl");
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/MVD_ValvesCurrentControl/RESV_CH3_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/MVD_ValvesCurrentControl/RESV_CH4_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/MVD_ValvesCurrentControl/VV_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/MVD_ValvesCurrentControl/RESV_CH6_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/MVD_ValvesCurrentControl/RESV_CH7_Target_Current", 0);
    com.can_rbs_set_signal_value_by_address("2/CAN3_DUT3/ControlNode/MVD_ValvesCurrentControl/RESV_CH8_Target_Current", 0);
  }

  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function dut_rbs_init

// CODE BLOCK BEGIN Custom_Function dut_step Y29uc3QgczMyIEFEVVRJbmRleA__
// 自定义函数 "dut_step"
s32 dut_step(const s32 ADUTIndex) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    app.terminate_application();
    return -1;
  }
  s32 ADUTErrorFlag = 0;
  // 心跳检测和状态更新
  // PWR
  std::string strPWRIsDisconnected = dut_sysvar_name(ADUTIndex, "PWRIsDisconnected");
  vPWRMonitorCount[ADUTIndex] = vPWRMonitorCount[ADUTIndex] + 1;
  if(vPWRMonitorCount[ADUTIndex] >= MAX_MONITOR_WINDOW) {
    vPWRMonitorCount[ADUTIndex] = MAX_MONITOR_WINDOW;
    vPWRIsDisconnected[ADUTIndex] = 1; // PWR Disconnected
    // test_log_nok("DUT%d PWR Disconnected", ADUTIndex + 1);
  }
  else {
    vPWRIsDisconnected[ADUTIndex] = 0;
  }
  set_dut_sysvar_error_state(ADUTIndex, "PWRIsDisconnected", vPWRIsDisconnected[ADUTIndex]);

  // MVD
  std::string strMVDIsDisconnected = dut_sysvar_name(ADUTIndex, "MVDIsDisconnected");
  vMVDMonitorCount[ADUTIndex] = vMVDMonitorCount[ADUTIndex] + 1;
  if(vMVDMonitorCount[ADUTIndex] >= MAX_MONITOR_WINDOW) {
    vMVDMonitorCount[ADUTIndex] = MAX_MONITOR_WINDOW;
    vMVDIsDisconnected[ADUTIndex] = 1; // MVD Disconnected
    // test_log_nok("DUT%d MVD Disconnected", ADUTIndex + 1);
  }
  else {
    vMVDIsDisconnected[ADUTIndex] = 0;
  }
  set_dut_sysvar_error_state(ADUTIndex, "MVDIsDisconnected", vMVDIsDisconnected[ADUTIndex]);
  // VD
  vVDMonitorCount[ADUTIndex] = vVDMonitorCount[ADUTIndex] + 1;
  if(vVDMonitorCount[ADUTIndex] >= MAX_MONITOR_WINDOW) {
    vVDMonitorCount[ADUTIndex] = MAX_MONITOR_WINDOW;
    vVDIsDisconnected[ADUTIndex] = 1; // VD Disconnected
    // test_log_nok("DUT%d VD Disconnected", ADUTIndex + 1);
  }
  else {
    vVDIsDisconnected[ADUTIndex] = 0;
  }
  set_dut_sysvar_error_state(ADUTIndex, "VDIsDisconnected", vVDIsDisconnected[ADUTIndex]);
  // Device Sensors
  std::string strDeviceSensorIsDisconnected = dut_sysvar_name(ADUTIndex, "DeviceSensorIsDisconnected");
  vDeviceSensorsMonitorCount[ADUTIndex] = vDeviceSensorsMonitorCount[ADUTIndex] + 1;
  if(vDeviceSensorsMonitorCount[ADUTIndex] >= MAX_MONITOR_WINDOW) {
    vDeviceSensorsMonitorCount[ADUTIndex] = MAX_MONITOR_WINDOW;
    vDeviceSensorIsDisconnected[ADUTIndex] = 1; // Device Sensors Disconnected
    // test_log_nok("DUT%d Device Sensors Disconnected", ADUTIndex + 1);
  }
  else {
    vDeviceSensorIsDisconnected[ADUTIndex] = 0;
  }
  set_dut_sysvar_error_state(ADUTIndex, "DeviceSensorIsDisconnected", vDeviceSensorIsDisconnected[ADUTIndex]);

  // DUT Sensors
  std::string strDUTSensorIsDisconnected = dut_sysvar_name(ADUTIndex, "DUTSensorIsDisconnected");
  vDUTSensorMonitorCount[ADUTIndex] = vDUTSensorMonitorCount[ADUTIndex] + 1;
  if(vDUTSensorMonitorCount[ADUTIndex] >= MAX_MONITOR_WINDOW) {
    vDUTSensorMonitorCount[ADUTIndex] = MAX_MONITOR_WINDOW;
    vDUTSensorIsDisconnected[ADUTIndex] = 1; // DUT Sensors Disconnected
    // test_log_nok("DUT%d DUT Sensors Disconnected", ADUTIndex + 1);
  }
  else {
    vDUTSensorIsDisconnected[ADUTIndex] = 0;
  }
  set_dut_sysvar_error_state(ADUTIndex, "DUTSensorIsDisconnected", vDUTSensorIsDisconnected[ADUTIndex]);

  // 检查错误
  ADUTErrorFlag = 0;
  // 驱动板卡错误
  ADUTErrorFlag = vVDDriverHasError[ADUTIndex] + vMVDDriverHasError[ADUTIndex];
  // 信号丢失错误
  ADUTErrorFlag = ADUTErrorFlag + vPWRIsDisconnected[ADUTIndex] + vMVDIsDisconnected[ADUTIndex] + vVDIsDisconnected[ADUTIndex] + vDeviceSensorIsDisconnected[ADUTIndex] + vDUTSensorIsDisconnected[ADUTIndex];
  // 信号卡滞错误
  ADUTErrorFlag = ADUTErrorFlag + vVDIsStuck[ADUTIndex] + vMVDIsStuck[ADUTIndex] + vDeviceSensorIsStuck[ADUTIndex];
  // 信号值错误；忽略DUTSensor
  ADUTErrorFlag = ADUTErrorFlag + vVDSignalError[ADUTIndex] + vMVDSignalError[ADUTIndex] + vDeviceSensorSignalError[ADUTIndex];
  // 板卡保护错误
  ADUTErrorFlag = ADUTErrorFlag + vVDInProtection[ADUTIndex] + vMVDInProtection[ADUTIndex];
  // 压力超限错误
  ADUTErrorFlag = ADUTErrorFlag + vDeviceSensorMaxPressureReached[ADUTIndex];
  // 测试 序列错误
  get_dut_sysvar_int32(ADUTIndex, "TestCaseErrorCode", &vTestCaseErrorCode[ADUTIndex]);
  if(vTestCaseErrorCode[ADUTIndex] != 0) {
    ADUTErrorFlag = ADUTErrorFlag + 1;
    vTestCaseError[ADUTIndex] = 1;
  }
  else vTestCaseError[ADUTIndex] = 0;
  set_dut_sysvar_error_state(ADUTIndex, "TestCaseError", vTestCaseError[ADUTIndex]);

  char* mName;
  char* grpName;
  TAutomationModuleRunningState state;
  std::string strModuleName = "DUT" + std::to_string(ADUTIndex + 1) + "_Cycle";
  if(0 == app.am_get_running_state(strModuleName.c_str(), &state, &mName, &grpName)) {
    if(amrsRunning == state) {
      vTestCaseRunState[ADUTIndex] = 1;
    }
    else {
      vTestCaseRunState[ADUTIndex] = 0;
    }
  }





  if(0 == vDUTState[ADUTIndex]) {
    // 更新DUT Error Flag
    if(ADUTErrorFlag > 0) {
      change_sysvar_dut_state(ADUTIndex, 2); // 设置为Error状态
      // Error Action
      test_log_nok("DUT %d Has Error", ADUTIndex + 1);
      //stop_test_action(ADUTIndex); // 执行停止测试动作
      error_stop_all_test();
    }
    else {
      dut_heartbeat(ADUTIndex);
      // 根据Case是否运行自动设置运行状态
      if(1 == vTestCaseRunState[ADUTIndex]) {
        change_sysvar_dut_state(ADUTIndex, 1); // 设置为Running状态
      }
    }

  }
  else if(1 == vDUTState[ADUTIndex]) {
    //开启心跳帧
    if(ADUTErrorFlag > 0) {
      change_sysvar_dut_state(ADUTIndex, 2); // 设置为Error状态
      // Error Action
      test_log_nok("DUT %d Has Error", ADUTIndex + 1);
      //stop_test_action(ADUTIndex); // 执行停止测试动作
      error_stop_all_test();
    }
    else {
      dut_heartbeat(ADUTIndex);
      // 根据Case是否运行自动设置运行状态
      if(0 == vTestCaseRunState[ADUTIndex]) {
        change_sysvar_dut_state(ADUTIndex, 0); // 设置为就绪状态
      }
    }

  }
  else if(2 == vDUTState[ADUTIndex]) {
    if(ADUTErrorFlag == 0) {
      change_sysvar_dut_state(ADUTIndex, 0); // 设置为就绪状态
      // Error Action
      test_log_ok("DUT %d Error Cleared", ADUTIndex + 1);
    }
  }
  else {
    test_log_nok("DUT%d.State is not valid", ADUTIndex);
    app.terminate_application();
  }

  // 手动清除故障Action时，确保心跳帧正常
  if(1 == vManualClearErrorHeartbeatActived[ADUTIndex]) {
    dut_heartbeat(ADUTIndex);
  }

  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function dut_step

// CODE BLOCK BEGIN Custom_Function bench_state_step dm9pZA__
// 自定义函数 "bench_state_step"
s32 bench_state_step(void) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  u8 AIndex;
  s32 ABenchState = 0;
  app.get_system_var_int32("DUT1.Enabled", &vDUTEnabled[0]);
  app.get_system_var_int32("DUT2.Enabled", &vDUTEnabled[1]);
  app.get_system_var_int32("DUT3.Enabled", &vDUTEnabled[2]);
  for(AIndex = 0; AIndex < DUT_CHANNEL_COUNT; AIndex++) {
    if((1 == vDUTState[AIndex]) && (1 == vDUTEnabled[AIndex])) {
      ABenchState = 1;
    }
  }
  for(AIndex = 0; AIndex < DUT_CHANNEL_COUNT; AIndex++) {
    if((2 == vDUTState[AIndex]) && (1 == vDUTEnabled[AIndex])) {
      ABenchState = 2;
    }
  }
  vBenchState = ABenchState;
  app.set_system_var_int32("BenchState", vBenchState);
  if(0 == vBenchState) {
    // 亮黄灯
    TCANFD f0 = {3,0x5,2,0x3,0xF101,0,{0x2, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(0, "");

  }
  else if(1 == vBenchState) {
    // 亮绿灯
    TCANFD f0 = {3,0x5,2,0x3,0xF101,0,{0x1, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(0, "");

  }
  else if(2 == vBenchState) {
    // 亮红灯
    TCANFD f0 = {3,0x5,2,0x3,0xF101,0,{0x4, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(0, "");

  }
  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function bench_state_step

// CODE BLOCK BEGIN Custom_Function on_tsmaster_disconnect dm9pZA__
// 自定义函数 "on_tsmaster_disconnect"
s32 on_tsmaster_disconnect(void) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  {
    // [1] CAN FD 0x3005 VD_Current_Control 
    TCANFD f0 = {0,0x5,10,0x3,0x3005,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(1, "");
  }

  {
    // [2] CAN FD 0x3105 MVD_ValvesCurrentControl 
    TCANFD f1 = {0,0x5,10,0x3,0x3105,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f1);
    app.wait(1, "");
  }

  {
    // [3] CAN FD 0x3104 MVD_MotorsDutyControl 
    TCANFD f2 = {0,0x5,4,0x3,0x3104,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f2);
    app.wait(1, "");
  }

  {
    // [4] CAN FD 0xF001 PWR_Control 
    TCANFD f3 = {0,0x5,2,0x3,0xF001,0,{0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f3);
    app.wait(1, "");
  }


  {
    // [1] CAN FD 0x3005 VD_Current_Control 
    TCANFD f0 = {1,0x5,10,0x3,0x3005,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(1, "");
  }

  {
    // [2] CAN FD 0x3105 MVD_ValvesCurrentControl 
    TCANFD f1 = {1,0x5,10,0x3,0x3105,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f1);
    app.wait(1, "");
  }

  {
    // [3] CAN FD 0x3104 MVD_MotorsDutyControl 
    TCANFD f2 = {1,0x5,4,0x3,0x3104,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f2);
    app.wait(1, "");
  }

  {
    // [4] CAN FD 0xF001 PWR_Control 
    TCANFD f3 = {1,0x5,2,0x3,0xF001,0,{0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f3);
    app.wait(1, "");
  }


  {
    // [1] CAN FD 0x3005 VD_Current_Control 
    TCANFD f0 = {2,0x5,10,0x3,0x3005,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(1, "");
  }

  {
    // [2] CAN FD 0x3105 MVD_ValvesCurrentControl 
    TCANFD f1 = {2,0x5,10,0x3,0x3105,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f1);
    app.wait(1, "");
  }

  {
    // [3] CAN FD 0x3104 MVD_MotorsDutyControl 
    TCANFD f2 = {2,0x5,4,0x3,0x3104,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f2);
    app.wait(1, "");
  }

  {
    // [4] CAN FD 0xF001 PWR_Control 
    TCANFD f3 = {2,0x5,2,0x3,0xF001,0,{0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f3);
    app.wait(1, "");
  }

  {
    // 亮红灯
    TCANFD f0 = {3,0x5,2,0x3,0xF101,0,{0x4, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(0, "");

  }

  app.set_system_var_int32("BenchState", 2);


  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function on_tsmaster_disconnect

// CODE BLOCK BEGIN On_CAN_FD_Rx On_CAN_FD_Rx LTEsLTEsLTE_
// CAN报文接收事件 "On_CAN_FD_Rx" 针对标识符 = 任意 (FD)
void on_canfd_rx_On_CAN_FD_Rx(const TCANFD* ACANFD) {  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)

  dut_on_recieve(0, ACANFD);
  dut_on_recieve(1, ACANFD);
  dut_on_recieve(2, ACANFD);

}
// CODE BLOCK END On_CAN_FD_Rx On_CAN_FD_Rx

// CODE BLOCK BEGIN On_CAN_FD_PreTx On_CAN_FD_PreTx LTEsLTEsLTE_
// CAN报文预发送事件 "On_CAN_FD_PreTx" 针对标识符 = 任意 (FD)
void on_canfd_pretx_On_CAN_FD_PreTx(const PCANFD ACANFD) {  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // 针对标识符 = 任意 (FD)
  // if (ACANFD->FIdxChn != CH1) return; // if you want to filter channel
  dut_on_pretx(ACANFD);

}
// CODE BLOCK END On_CAN_FD_PreTx On_CAN_FD_PreTx


// CODE BLOCK BEGIN Custom_Function fault_action_deflate_fl Y29uc3QgczMyIEFEVVRJbmRleCwgY29uc3QgZmxvYXQgQVRhcmdldFByZXNzdXJlQmFy
// 自定义函数 "fault_action_deflate_fl"
s32 fault_action_deflate_fl(const s32 ADUTIndex, const float ATargetPressureBar) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  return fault_action_deflate(ADUTIndex, TANK_INDEX_FL, ATargetPressureBar);

}
// CODE BLOCK END Custom_Function fault_action_deflate_fl

// CODE BLOCK BEGIN Custom_Function fault_action_deflate_fr Y29uc3QgczMyIEFEVVRJbmRleCwgY29uc3QgZmxvYXQgQVRhcmdldFByZXNzdXJlQmFy
// 自定义函数 "fault_action_deflate_fr"
s32 fault_action_deflate_fr(const s32 ADUTIndex, const float ATargetPressureBar) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  return fault_action_deflate(ADUTIndex, TANK_INDEX_FR, ATargetPressureBar);

}
// CODE BLOCK END Custom_Function fault_action_deflate_fr

// CODE BLOCK BEGIN Custom_Function fault_action_deflate_rl Y29uc3QgczMyIEFEVVRJbmRleCwgY29uc3QgZmxvYXQgQVRhcmdldFByZXNzdXJlQmFy
// 自定义函数 "fault_action_deflate_rl"
s32 fault_action_deflate_rl(const s32 ADUTIndex, const float ATargetPressureBar) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  return fault_action_deflate(ADUTIndex, TANK_INDEX_RL, ATargetPressureBar);

}
// CODE BLOCK END Custom_Function fault_action_deflate_rl

// CODE BLOCK BEGIN Custom_Function fault_action_deflate_rr Y29uc3QgczMyIEFEVVRJbmRleCwgY29uc3QgZmxvYXQgQVRhcmdldFByZXNzdXJlQmFy
// 自定义函数 "fault_action_deflate_rr"
s32 fault_action_deflate_rr(const s32 ADUTIndex, const float ATargetPressureBar) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  return fault_action_deflate(ADUTIndex, TANK_INDEX_RR, ATargetPressureBar);

}
// CODE BLOCK END Custom_Function fault_action_deflate_rr

// CODE BLOCK BEGIN Custom_Function fault_action_deflate_res Y29uc3QgczMyIEFEVVRJbmRleCwgY29uc3QgZmxvYXQgQVRhcmdldFByZXNzdXJlQmFy
// 自定义函数 "fault_action_deflate_res"
s32 fault_action_deflate_res(const s32 ADUTIndex, const float ATargetPressureBar) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  return fault_action_deflate(ADUTIndex, TANK_INDEX_RES, ATargetPressureBar);

}
// CODE BLOCK END Custom_Function fault_action_deflate_res

// CODE BLOCK BEGIN Custom_Function fault_action_clear_test_case_error Y29uc3QgczMyIEFEVVRJbmRleA__
// 自定义函数 "fault_action_clear_test_case_error"
s32 fault_action_clear_test_case_error(const s32 ADUTIndex) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  if((ADUTIndex < 0) || (ADUTIndex > DUT_CHANNEL_COUNT)) {
    test_log_nok("DUTIndex is not valid");
    app.terminate_application();
    return -1;
  }

  std::string AStrTitle = "重置DUT" + std::to_string(ADUTIndex + 1) + "错误码";
  std::string AStrInfo = "请检查DUT" + std::to_string(ADUTIndex + 1) + "上次测试错误问题，根据测试问题，调整参数，如果确认无问题，点击确认，清除错误码！";

  if(0 == app.show_confirm_dialog(AStrTitle.c_str(), AStrInfo.c_str(), "", 100000, false)) {
    test_log("用户确认清除DUT%d测试用例错误码，测试序列错误码被清除！", ADUTIndex + 1);
    vTestCaseError[ADUTIndex] = 0;
    vTestCaseErrorCode[ADUTIndex] = 0;
    set_dut_sysvar_error_state(ADUTIndex, "TestCaseErrorCode", 0);
    set_dut_sysvar_error_state(ADUTIndex, "TestCaseError", 0);
    return IDX_ERR_OK;
  }
  else {
    test_log("用户取消清除DUT%d测试用例错误码！", ADUTIndex + 1);
    return -1;
  }

}
// CODE BLOCK END Custom_Function fault_action_clear_test_case_error

// CODE BLOCK BEGIN Custom_Function dut_reset_test_statistics_data Y29uc3QgczMyIEFEVVRJbmRleA__
// 自定义函数 "dut_reset_test_statistics_data"
s32 dut_reset_test_statistics_data(const s32 ADUTIndex) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  std::string AStrTitle = "重置DUT" + std::to_string(ADUTIndex + 1) + "测试数据";
  std::string AStrInfo = "是否确认重置DUT" + std::to_string(ADUTIndex + 1) + "测试数据，确认后，该通道样件所有测试数据将被清零！";

  if(0 == app.show_confirm_dialog(AStrTitle.c_str(), AStrInfo.c_str(), "", 100000, false)) {
    test_log("用户确认清除DUT%d测试统计数据，测试数据将被清零！", ADUTIndex + 1);
    std::string AMotorCloseLoopTime = "DUT" + std::to_string(ADUTIndex + 1) + ".MotorCloseLoopTime";
    std::string AMotorOpenLoopTime = "DUT" + std::to_string(ADUTIndex + 1) + ".MotorOpenLoopTime";
    std::string AMotorIdleLoopTime = "DUT" + std::to_string(ADUTIndex + 1) + ".MotorIdleLoopTime";
    app.set_system_var_double(AMotorCloseLoopTime.c_str(), 0);
    app.set_system_var_double(AMotorOpenLoopTime.c_str(), 0);
    app.set_system_var_double(AMotorIdleLoopTime.c_str(), 0);
    std::string ASystemVarName;
    for(u8 i = 0; i < 10; i++) {
      ASystemVarName = "DUT" + std::to_string(ADUTIndex + 1) + ".TestCount_Temp" + std::to_string(i + 1);
      app.set_system_var_int32(ASystemVarName.c_str(), 0);
    }
    // todo: add all related test data here
    return IDX_ERR_OK;
  }
  else {
    test_log("用户取消清除DUT%d测试统计数据！", ADUTIndex + 1);
    return -1;
  }

}
// CODE BLOCK END Custom_Function dut_reset_test_statistics_data

// CODE BLOCK BEGIN Custom_Function on_start_button_click Y29uc3QgZG91YmxlIEFEdW1teQ__
// 自定义函数 "on_start_button_click"
s32 on_start_button_click(const double ADummy) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  app.am_run("Duarable_Test_Sequence", "am_Temperature_Sequence", "", false);
  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function on_start_button_click

// CODE BLOCK BEGIN Custom_Function on_stop_button_click Y29uc3QgZG91YmxlIEFEdW1teQ__
// 自定义函数 "on_stop_button_click"
s32 on_stop_button_click(const double ADummy) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  app.am_stop("Duarable_Test_Sequence", false);
  app.am_stop("DUT1_Cycle", false);
  app.am_stop("DUT2_Cycle", false);
  app.am_stop("DUT3_Cycle", false);
  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function on_stop_button_click

// CODE BLOCK BEGIN Custom_Function on_reset_data_button_click Y29uc3QgZG91YmxlIEFEdW1teQ__
// 自定义函数 "on_reset_data_button_click"
s32 on_reset_data_button_click(const double ADummy) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  for(u8 i = 0; i < DUT_CHANNEL_COUNT; i++) {
    dut_reset_test_statistics_data(i);
  }

  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function on_reset_data_button_click

// CODE BLOCK BEGIN Custom_Function on_clear_fault_button_click Y29uc3QgZG91YmxlIEFEdW1teQ__
// 自定义函数 "on_clear_fault_button_click"
s32 on_clear_fault_button_click(const double ADummy) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 

  std::string AStrTitle = "重置所有测试通道错误码";
  std::string AStrInfo = "请检查上次测试错误问题，根据测试问题，调整参数，如果确认无问题，点击确认，清除错误码！";

  if(0 == app.show_confirm_dialog(AStrTitle.c_str(), AStrInfo.c_str(), "", 100000, false)) {
    test_log("用户确认清除测试用例错误码，测试序列错误码被清除！");
    for(u8 ADUTIndex = 0; ADUTIndex < DUT_CHANNEL_COUNT; ADUTIndex++) {
      vTestCaseError[ADUTIndex] = 0;
      vTestCaseErrorCode[ADUTIndex] = 0;
      set_dut_sysvar_error_state(ADUTIndex, "TestCaseErrorCode", 0);
      set_dut_sysvar_error_state(ADUTIndex, "TestCaseError", 0);
    }
  }
  else {
    test_log("用户取消清除测试用例错误码数据！");
    return -1;
  }

  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function on_clear_fault_button_click

// CODE BLOCK BEGIN Custom_Function error_stop_all_test dm9pZA__
// 自定义函数 "error_stop_all_test"
s32 error_stop_all_test(void) { // 自定义函数: 
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
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  for(u8 i = 0; i < DUT_CHANNEL_COUNT; i++) {
    if(1 == vDUTEnabled[i]) {
      stop_test_action(i);
    }
  }
  // 关闭温度序列自动化模块
  if(0 != app.am_stop("Duarable_Test_Sequence", true)) {
    std::string strErrorInfo = "停止温度主循环失败！";
    app.log_text(strErrorInfo.c_str(), lvlError);
    app.terminate_application();
  }
  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function error_stop_all_test

// CODE BLOCK BEGIN Step_Function  MTAwMA__
// 主step函数，执行周期 1000 ms
void step(void) { // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
 // 周期 = 1000 ms
  //TM Error Check
  if(1 == vTMHasError) {
    app.make_toast("TM has Error, please check!", lvlError);
    app.terminate_application(); // TM Error
  }

  // 自诊断
  self_driver_check();
  dut_step(0);
  dut_step(1);
  dut_step(2);
  bench_state_step();
  //test_log("step actived");

#ifdef DEBUG_MODE
  {
    // [1] CAN FD 0x2 PC_Heartbeat 
    TCANFD f0 = {3,0x5,1,0x3,0x2,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(0, "");
  }
#endif

#ifdef DEBUG_MODE_DUT1
  {
    // [1] CAN FD 0x1 PC_Heartbeat 
    TCANFD f0 = {0,0x5,1,0x3,0x1,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(0, "");
  }
#endif
#ifdef DEBUG_MODE_DUT2
  {
    // [1] CAN FD 0x1 PC_Heartbeat 
    TCANFD f0 = {1,0x5,1,0x3,0x1,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(0, "");
  }
#endif
#ifdef DEBUG_MODE_DUT3
  {
    // [1] CAN FD 0x1 PC_Heartbeat 
    TCANFD f0 = {2,0x5,1,0x3,0x1,0,{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    com.transmit_canfd_async(&f0);
    app.wait(0, "");
  }
#endif

}
// CODE BLOCK END Step_Function 

// CODE BLOCK BEGIN Configuration
/* 
[UI]
UICommon=-1,-1,-1,0,QyC0+sLrseC8rcb3IFtUTV0_,100,643,3827778297939034615,0
ScriptName=TM
DisplayName=TM
IsMPLib=1
DBDeps=CAN1_DUT1,CAN2_DUT2,CAN3_DUT3,CAN_LED_Sensors
LastBuildTime=2025-12-15 09:45:15
APICnt=21

[UI_Doc]
Cnt=21
d0=dut_step,,,1,NCwtMSxBRFVUSW5kZXgs
d1=dut_on_start,,,1,NCwtMSxBRFVUSW5kZXgs
d2=dut_on_stop,,,1,NCwtMSxBRFVUSW5kZXgs
d3=dut_on_recieve,,,2,NCwtMSxBRFVUSW5kZXgs,NzgsLTEsQUNBTkZELA__
d4=stop_test_action,,,1,NCwtMSxBRFVUSW5kZXgs
d5=dut_rbs_init,,,1,NCwtMSxBRFVUSW5kZXgs
d6=bench_state_step,,,0
d7=on_tsmaster_disconnect,,,0
d8=dut_on_pretx,,,1,MjIsLTEsQUNBTkZELA__
d9=fault_action_deflate_fl,,,2,NCwtMSxBRFVUSW5kZXgs,NiwtMSxBVGFyZ2V0UHJlc3N1cmVCYXIs
d10=fault_action_deflate_fr,,,2,NCwtMSxBRFVUSW5kZXgs,NiwtMSxBVGFyZ2V0UHJlc3N1cmVCYXIs
d11=fault_action_deflate_rl,,,2,NCwtMSxBRFVUSW5kZXgs,NiwtMSxBVGFyZ2V0UHJlc3N1cmVCYXIs
d12=fault_action_deflate_rr,,,2,NCwtMSxBRFVUSW5kZXgs,NiwtMSxBVGFyZ2V0UHJlc3N1cmVCYXIs
d13=fault_action_deflate_res,,,2,NCwtMSxBRFVUSW5kZXgs,NiwtMSxBVGFyZ2V0UHJlc3N1cmVCYXIs
d14=fault_action_clear_test_case_error,,,1,NCwtMSxBRFVUSW5kZXgs
d15=dut_reset_test_statistics_data,,,1,NCwtMSxBRFVUSW5kZXgs
d16=on_start_button_click,,,1,NywtMSxBRHVtbXks
d17=on_stop_button_click,,,1,NywtMSxBRHVtbXks
d18=on_reset_data_button_click,,,1,NywtMSxBRHVtbXks
d19=on_clear_fault_button_click,,,1,NywtMSxBRHVtbXks
d20=error_stop_all_test,,,0
*/
// CODE BLOCK END Configuration

