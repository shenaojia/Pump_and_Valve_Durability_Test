// GEN BLOCK BEGIN Include
#define TSMP_IMPL
#include "TSMaster.h"
#include "MPLibrary.h"
#include "Database.h"
#include "TSMasterBaseInclude.h"
#include "Configuration.h"
// GEN BLOCK END Include

// CODE BLOCK BEGIN Global_Definitions 
#define TOL_ERROR   0.5f 
#define IP_ADDRESS  "192.168.0.11"

native_int vHandle;
s32 vDeviceAddr;
bool vConnected;
bool vDebugMode = true;
double vActualTemperature;
double vActualHumidty;


void __stdcall OnTCPData(const pu8 APointer, const s32 ASize) {
  // handle the data stream starting from APointer with size = ASize
}

void __stdcall OnUDPData(const pu8 APointer, const s32 ASize) {
  // handle the data stream starting from APointer with size = ASize
}
// CODE BLOCK END Global_Definitions 

// GEN BLOCK BEGIN Custom_Function
s32 set_temperature_syn(const double ATargetTemperature, const s32 ATimeoutS);
s32 connect(const char* AIPAddress, const s32 ADeviceAddr);
s32 disconnect(void);
s32 get_temperature(double* ATempature);
s32 set_temperature(const double ATempature);
s32 is_connected(void);
s32 start(void);
s32 stop(void);
s32 set_temp_slope(const double ATempSlope);
s32 set_humidity(const double Ahumidity);
s32 get_humidty(double*   Ahumidity);
// GEN BLOCK END Custom_Function

// CODE BLOCK BEGIN Custom_Function set_temperature_syn Y29uc3QgZG91YmxlIEFUYXJnZXRUZW1wZXJhdHVyZSwgY29uc3QgczMyIEFUaW1lb3V0Uw__
// 自定义函数 "set_temperature_syn"
s32 set_temperature_syn(const double ATargetTemperature, const s32 ATimeoutS) { // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  stop();
  disconnect();
  app.wait(5000,"");
  if(0 != connect(IP_ADDRESS, 1))  return -1;
  app.wait(5000,"");
  if(0 != start())  return -2;
  app.wait(1000,"");
  if(0 != set_temperature(ATargetTemperature))  return -2;
  app.wait(1000,"");
  if(0 != set_temperature(ATargetTemperature))  return -2;
  app.wait(1000,"");
  if(0 != set_temperature(ATargetTemperature))  return -2;
  
  
  s32 ATimeTick = 0;
  while(1) {
    double ADeltaT = abs(ATargetTemperature - vActualTemperature);
    if(ADeltaT < TOL_ERROR) {
      test_log_ok("Target temperature reached");
      return IDX_ERR_OK;
    }
    app.wait(1000, "");
    ATimeTick++;
    if(ATimeTick > ATimeoutS) {
      test_log_nok("Cann't reached target temperature in %d s", ATimeoutS);
      return -1;
    }

  }
  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function set_temperature_syn

// CODE BLOCK BEGIN Custom_Function connect Y29uc3QgY2hhciogQUlQQWRkcmVzcywgY29uc3QgczMyIEFEZXZpY2VBZGRy
// 自定义函数 "connect"
s32 connect(const char* AIPAddress, const s32 ADeviceAddr) { // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  if(app.is_connected()) return IDX_ERR_REQUIRE_APP_CONNECTED;
  s32 r = 0;
  if(0 == vHandle) {
    r = com.ioip_create(0, 0, OnTCPData, OnUDPData, &vHandle);
    if(r) {
      log_nok("ioip_create failed, requires application connected");
      return r;
    }
  }
  vDeviceAddr = ADeviceAddr;
  r = com.ioip_connect_tcp_server(vHandle, AIPAddress, 8000);
  if(r) {
    log_nok("ioip_connect_tcp_server failed");
    return r;
  }
  vConnected = true;
  if(vDebugMode) {
    log_ok("TOMILO Connected: %s, Device addr = %d", AIPAddress, ADeviceAddr);
  }
  return 0;

}
// CODE BLOCK END Custom_Function connect

// CODE BLOCK BEGIN Custom_Function disconnect dm9pZA__
// 自定义函数 "disconnect"
s32 disconnect(void) { // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  vConnected = false;
  if(vHandle) {
    com.ioip_disconnect_tcp_server(vHandle);
    com.ioip_delete(vHandle);
    vHandle = 0;
    if(vDebugMode) {
      log_ok("TOMILO device disconnected");
    }
  }
  return 0;

}
// CODE BLOCK END Custom_Function disconnect

// CODE BLOCK BEGIN Custom_Function get_temperature ZG91YmxlKiBBVGVtcGF0dXJl
// 自定义函数 "get_temperature"
s32 get_temperature(double* ATempature) { // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 读取当前温度
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  /*
  ① 读取当前温度：
  Tx:00 00 00 00 00 06 01 03 1F 37 00 01
  前面 6 个字节是帧头，06 表示后面有 6 个字节的数据，01 代表设备地址码，03 代表功能码，1F 37
  代表地址 7991，00 01 代表读取一个字
  Rx: 00 00 00 00 00 05 01 03 02 02 7A
  05 表示后面有 5 个字节数，01 代表设备地址码，03 代表地址功能码，02 代表 02 的后面有 2 个字节
  数，02 7A 代表 1F 37 这个地址 7991 内读出的数据是 02 7A → 转为 10 进制为 634，则当前温度值为
  634/10=634℃
  读取当前温度需要将最后读取的数值除以 10，因为是一位小数。
  */
  if(!app.check(is_connected())) {
    return IDX_ERR_REQUIRE_APP_CONNECTED;
  }
  u8 buffer[1024] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x1F, 0x37, 0x00, 0x01};
  s32 len = 1024;
  buffer[6] = vDeviceAddr;
  if(com.ioip_send_buffer_tcp(vHandle, &buffer[0], 12)) {
    log_nok("cannot transmit get tempature command to TOMILO");
    return IDX_ERR_TSLIB_GENERIC_OPERATION_FAILED;
  }
  if(0 == com.ioip_receive_tcp_client_response(vHandle, 1000, &buffer[0], &len)) {
    if(11 == len) {
      *ATempature = (s16(buffer[9] << 8) + buffer[10]) / 10.0;
      if(vDebugMode) {
        //log_ok("TOMILO device temperature read = 0x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10]);
        //log_ok("TOMILO device temperature read = %f", *ATempature);
      }
      return 0;
    }
    else {
      log_nok("Invalid response data from TOMILO, len = %d", len);
    }
  }
  else {
    log_nok("cannot receive TOMILO response in 1000 ms");
    return IDX_ERR_TSLIB_GENERIC_OPERATION_FAILED;
  }
  return 0;

}
// CODE BLOCK END Custom_Function get_temperature

// CODE BLOCK BEGIN Custom_Function set_temperature Y29uc3QgZG91YmxlIEFUZW1wYXR1cmU_
// 自定义函数 "set_temperature"
s32 set_temperature(const double ATempature) { // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 设定温度
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  /*
  ② 设定温度：
  Tx: 00 00 00 00 00 06 01 06 1F A4 01 90
  前面 6 个字节是帧头，06 表示后面有 6 个字节的数据，01 代表设备地址码，06 代表功能码，1F A4
  代表地址 8100，01 90 代表往这个地址内写入的数据是 400 ，则写入的温度值为
  */
  if(!app.check(is_connected())) {
    return IDX_ERR_REQUIRE_APP_CONNECTED;
  }
  u8 buffer[1024] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x06, 0x1F, 0xA4, 0x01, 0x90};
  s32 len = 1024;
  buffer[6] = vDeviceAddr;
  u16 temp = ATempature * 10.0;
  buffer[10] = temp >> 8;
  buffer[11] = temp & 0xFF;
  if(com.ioip_send_buffer_tcp(vHandle, &buffer[0], 12)) {
    log_nok("cannot transmit set tempature command to TOMILO");
    return IDX_ERR_TSLIB_GENERIC_OPERATION_FAILED;
  }
  // read dummy response
  if(0 == com.ioip_receive_tcp_client_response(vHandle, 1000, &buffer[0], &len)) {
    // just discard
  }
  if(vDebugMode) {
    log_ok("TOMILO device temperature set = %f", ATempature);
  }
  return 0;

}
// CODE BLOCK END Custom_Function set_temperature

// CODE BLOCK BEGIN Custom_Function is_connected dm9pZA__
// 自定义函数 "is_connected"
s32 is_connected(void) { // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  if(vConnected) {
    return 0;
  }
  else {
    return IDX_ERR_REQUIRE_APP_CONNECTED;
  }

}
// CODE BLOCK END Custom_Function is_connected

// CODE BLOCK BEGIN Custom_Function start dm9pZA__
// 自定义函数 "start"
s32 start(void) { // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 启动设备
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  /*
  ③ 启动设备：
  00 00 00 00 00 06 01 05 1F 40 FF 00
  前面 6 个字节是帧头，06 表示后面有 6 个字节的数据，01 代表设备地址码，05 代表功能码，1F 40
  代表地址（位地址）8000，FF 00 代表往这个位写 1。
  */

  if(!app.check(is_connected())) {
    return IDX_ERR_REQUIRE_APP_CONNECTED;
  }
  u8 buffer[1024] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x05, 0x1F, 0x40, 0xFF, 0x00};
  s32 len = 1024;
  buffer[6] = vDeviceAddr;
  if(com.ioip_send_buffer_tcp(vHandle, &buffer[0], 12)) {
    log_nok("cannot transmit start command to TOMILO");
    return IDX_ERR_TSLIB_GENERIC_OPERATION_FAILED;
  }
  // read dummy response
  if(0 == com.ioip_receive_tcp_client_response(vHandle, 1000, &buffer[0], &len)) {
    // just discard
  }
  if(vDebugMode) {
    log_ok("TOMILO device started");
  }
  return 0;

}
// CODE BLOCK END Custom_Function start

// CODE BLOCK BEGIN Custom_Function stop dm9pZA__
// 自定义函数 "stop"
s32 stop(void) { // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 停止设备
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
  /*
  ④ 停止设备：
  00 00 00 00 00 06 01 05 1F 41 FF 00
  前面 6 个字节是帧头，06 表示后面有 6 个字节的数据，01 代表设备地址码，05 代表功能码，1F 41
  代表地址（位地址）8001，FF 00 代表往这个位写 1。
  */
  if(!app.check(is_connected())) {
    return IDX_ERR_REQUIRE_APP_CONNECTED;
  }
  u8 buffer[1024] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x05, 0x1F, 0x41, 0xFF, 0x00};
  s32 len = 1024;
  buffer[6] = vDeviceAddr;
  if(com.ioip_send_buffer_tcp(vHandle, &buffer[0], 12)) {
    log_nok("cannot transmit stop command to TOMILO");
    return IDX_ERR_TSLIB_GENERIC_OPERATION_FAILED;
  }
  if(vDebugMode) {
    log_ok("TOMILO device stopped");
  }
  return 0;

}
// CODE BLOCK END Custom_Function stop

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
 // 程序停止事件
  disconnect();

}
// CODE BLOCK END On_Stop On_Stop

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
 // 程序启动事件
  if(0 != app.get_system_var_double("ActualTemperature", &vActualTemperature)) {
    app.create_system_var("ActualTemperature", svtDouble, "0.0", "环境仓实际温度");
  }
  vConnected = false;

}
// CODE BLOCK END On_Start On_Start

// CODE BLOCK BEGIN Custom_Function set_temp_slope Y29uc3QgZG91YmxlIEFUZW1wU2xvcGU_
// 自定义函数 "set_temp_slope"
s32 set_temp_slope(const double ATempSlope) { // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 设定温度斜率
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 设置温度斜率
 // 自定义函数: 设置温度斜率
 // 自定义函数: 设置温度斜率
 // 自定义函数: 设置温度斜率
 // 自定义函数: 设置温度斜率
 // 自定义函数: 设置温度斜率
 // 自定义函数: 设置温度斜率
 // 自定义函数: 设置温度斜率
 // 自定义函数: 
  /*
  ② 设定温度斜率：
  Tx: 00 00 00 00 00 06 01 06 20 60 01 90
  前面 6 个字节是帧头，06 表示后面有 6 个字节的数据，01 代表设备地址码，06 代表功能码，1F A4
  代表地址 8100，01 90 代表往这个地址内写入的数据是 400 ，则写入的温度斜率值为
  */
  if(!app.check(is_connected())) {
    return IDX_ERR_REQUIRE_APP_CONNECTED;
  }
  u8 buffer[1024] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x06, 0x20, 0x60, 0x01, 0x90};
  s32 len = 1024;
  buffer[6] = vDeviceAddr;
  u16 temp = ATempSlope * 10.0;
  buffer[10] = temp >> 8;
  buffer[11] = temp & 0xFF;
  if(com.ioip_send_buffer_tcp(vHandle, &buffer[0], 12)) {
    log_nok("cannot transmit set ATempSlope command to TOMILO");
    return IDX_ERR_TSLIB_GENERIC_OPERATION_FAILED;
  }
  // read dummy response
  if(0 == com.ioip_receive_tcp_client_response(vHandle, 1000, &buffer[0], &len)) {
    // just discard
  }
  if(vDebugMode) {
    log_ok("TOMILO device ATempSlope set = %f", ATempSlope);
  }
  return 0;

}
// CODE BLOCK END Custom_Function set_temp_slope

// CODE BLOCK BEGIN Custom_Function set_humidity Y29uc3QgZG91YmxlIEFodW1pZGl0eQ__
// 自定义函数 "set_humidity"
s32 set_humidity(const double Ahumidity) { // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 设定湿度
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 设置湿度
 // 自定义函数: 设置湿度
 // 自定义函数: 设置湿度
 // 自定义函数: 设置湿度
 // 自定义函数: 设置湿度
 // 自定义函数: 设置湿度
 // 自定义函数: 设置湿度
 // 自定义函数: 设置湿度
 // 自定义函数: 
/*
② 设定湿度：
Tx: 00 00 00 00 00 06 01 06 1F A4 01 90
前面 6 个字节是帧头，06 表示后面有 6 个字节的数据，01 代表设备地址码，06 代表功能码，1F A5
代表地址 8101，01 90 代表往这个地址内写入的数据是 400 ，则写入的湿度值为
*/       
if (!app.check(is_connected())) {
  return IDX_ERR_REQUIRE_APP_CONNECTED;
}
u8 buffer[1024] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x06, 0x1F, 0xA5, 0x01, 0x90};
s32 len = 1024;
buffer[6] = vDeviceAddr;
u16 temp = Ahumidity * 10.0;
buffer[10] = temp >> 8;
buffer[11] = temp & 0xFF;
if (com.ioip_send_buffer_tcp(vHandle, &buffer[0], 12)){
  log_nok("cannot transmit set humidity command to TOMILO");
  return IDX_ERR_TSLIB_GENERIC_OPERATION_FAILED;
}
// read dummy response
if (0 == com.ioip_receive_tcp_client_response(vHandle, 1000, &buffer[0], &len)){
  // just discard
}
if (vDebugMode){
  log_ok("TOMILO device humidity set = %f", Ahumidity);
}
return 0;

}
// CODE BLOCK END Custom_Function set_humidity

// CODE BLOCK BEGIN Custom_Function get_humidty ZG91YmxlKiAgIEFodW1pZGl0eQ__
// 自定义函数 "get_humidty"
s32 get_humidty(double*   Ahumidity) { // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 读取当前湿度
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 
 // 自定义函数: 获取湿度
 // 自定义函数: 获取湿度
 // 自定义函数: 获取湿度
 // 自定义函数: 获取湿度
 // 自定义函数: 获取湿度
 // 自定义函数: 获取湿度
 // 自定义函数: 获取湿度
 // 自定义函数: 获取湿度
 // 自定义函数: 
/*
① 读取当前湿度：
Tx:00 00 00 00 00 06 01 03 1F 38 00 01
前面 6 个字节是帧头，06 表示后面有 6 个字节的数据，01 代表设备地址码，03 代表功能码，1F 38
代表地址 7992，00 01 代表读取一个字
Rx: 00 00 00 00 00 05 01 03 02 02 7A
05 表示后面有 5 个字节数，01 代表设备地址码，03 代表地址功能码，02 代表 02 的后面有 2 个字节
数，02 7A 代表 1F 38 这个地址 7992 内读出的数据是 02 7A → 转为 10 进制为 634，则当前温度值为
634/10=63.4%
读取当前温度需要将最后读取的数值除以 10，因为是一位小数。
*/
if (!app.check(is_connected())) {
  return IDX_ERR_REQUIRE_APP_CONNECTED;
}
u8 buffer[1024] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x1F, 0x38, 0x00, 0x01};
s32 len = 1024;
buffer[6] = vDeviceAddr;
if (com.ioip_send_buffer_tcp(vHandle, &buffer[0], 12)){
  log_nok("cannot transmit get tempature command to TOMILO");
  return IDX_ERR_TSLIB_GENERIC_OPERATION_FAILED;
}
if (0 == com.ioip_receive_tcp_client_response(vHandle, 1000, &buffer[0], &len)){
  if (11 == len){
    *Ahumidity = ((buffer[9] << 8) + buffer[10]) / 10.0;
    if (vDebugMode){
      //log_ok("TOMILO device temperature read = 0x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10]);
      //log_ok("TOMILO device temperature read = %f", *Ahumidity);
    }
    return 0; 
  } else {
    log_nok("Invalid response data from TOMILO, len = %d", len);
  }
} else {
  log_nok("cannot receive TOMILO response in 1000 ms");
  return IDX_ERR_TSLIB_GENERIC_OPERATION_FAILED;
}
return 0;

}
// CODE BLOCK END Custom_Function get_humidty

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
  if(vConnected) {
    get_temperature(&vActualTemperature);
    get_humidty(&vActualHumidty);
    app.set_system_var_double("ActualTemperature", vActualTemperature);
    app.set_system_var_double("ActualHumidty", vActualHumidty);


  }

}
// CODE BLOCK END Step_Function 

// CODE BLOCK BEGIN Configuration
/* 
[UI]
UICommon=-1,-1,-1,0,QyC0+sLrseC8rcb3IFtFQ10_,100,671,8928949396166239920,0
ScriptName=EC
DisplayName=EC
IsMPLib=1
DBDeps=CAN_LED_Sensors,CAN1_DUT1,CAN2_DUT2,CAN3_DUT3
LastBuildTime=2025-12-15 16:18:16
APICnt=11

[UI_Doc]
Cnt=11
d0=set_temperature_syn,,,2,NywtMSxBVGFyZ2V0VGVtcGVyYXR1cmUs,NCwtMSxBVGltZW91dFMs
d1=connect,,,2,MTcsLTEsQUlQQWRkcmVzcyw_,NCwtMSxBRGV2aWNlQWRkciw_
d2=disconnect,,,0
d3=get_temperature,tsHIobWxx7DOwrbI,,1,MzUsMCxBVGVtcGF0dXJlLA__
d4=set_temperature,yei2qM7Ctsg_,,1,NywtMSxBVGVtcGF0dXJlLA__
d5=is_connected,,,0
d6=start,xvS2r8nosbg_,,0
d7=stop,zaPWucnosbg_,,0
d8=set_humidity,yei2qMqqtsg_,,1,NywtMSxBaHVtaWRpdHks
d9=get_humidty,tsHIobWxx7DKqrbI,,1,MzUsMCxBaHVtaWRpdHks
d10=set_temp_slope,yei2qM7CtsjQscLK,,1,NywtMSxBVGVtcFNsb3BlLA__
*/
// CODE BLOCK END Configuration

