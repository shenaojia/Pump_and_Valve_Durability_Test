#include "TSMaster.h"
#include "MPLibrary.h"
#include "Configuration.h"
#include "TSMasterBaseInclude.h"
#include <memory>
#include <string>
#include <vector>

// Variables definition
TTSApp app;
TTSCOM com;
TTSTest test;
TTSMBD mbd;
TTSTAC tac;

// Templates Instantiation
template <>
void output<TCAN>(TCAN* canMsg) {
    com.transmit_can_async(canMsg);
}

template <>
void output<TLIN>(TLIN* linMsg) {
    com.transmit_lin_async(linMsg);
}

template <>
void output<TFlexRay>(TFlexRay* frMsg) {
    com.transmit_flexray_async(frMsg);
}

template <>
void output<TEthernetHeader>(TEthernetHeader* ethMsg) {
    com.transmit_ethernet_async(ethMsg);
}

// logger functions definition
#define TEMP_STR_LEN 1024
typedef char* va_list;
#ifndef __va_rounded_size
#define __va_rounded_size(TYPE) (((sizeof(TYPE)+sizeof(int)-1)/sizeof(int))*sizeof(int))
#endif
 
#ifndef va_start
#define va_start(AP, LASTARG)   (AP = ((char *)& (LASTARG) + __va_rounded_size(LASTARG)))
#endif
 
#ifndef va_arg
#define va_arg(AP, TYPE)        (AP += __va_rounded_size(TYPE), *((TYPE *)(AP - __va_rounded_size(TYPE))))
#endif
 
#ifndef va_end
#define va_end(AP)              (AP = (va_list)0 )
#endif

// Utility functions definition
void internal_log(const char* AFile, const char* AFunc, const s32 ALine, const TLogLevel ALevel, const char* fmt, ...)
{ 
  va_list ap;    
  va_start(ap, fmt);
  std::vector<char> buf(1024);
  int ret;  
#if __cplusplus == 201103L
  while((ret = vsnprintf_s(&buf[0], buf.size(), fmt, ap)) == -1){
#else
  // in VC++ version is 199711L, log("__cplusplus: %d\n", __cplusplus);
  while ((ret = vsnprintf_s(&buf[0], buf.size(), buf.size(), fmt, ap)) == -1) {
#endif
    buf.resize(buf.size() * 2);    
  }
  va_end(ap);
  if (ret == static_cast<int>(buf.size())){
    buf[buf.size() - 1] = '\0';    
  }         
  app.debug_log(AFile, AFunc, ALine, &buf[0], ALevel);
}

void internal_test_log(const char* AFile, const char* AFunc, const s32 ALine, const TLogLevel ALevel, const char* fmt, ...)
{ 
  va_list ap;    
  va_start(ap, fmt);
  std::vector<char> buf(1024);
  int ret;  
#if __cplusplus == 201103L
  while ((ret = vsnprintf_s(&buf[0], buf.size(), fmt, ap)) == -1) {
#else
  while ((ret = vsnprintf_s(&buf[0], buf.size(), buf.size(), fmt, ap)) == -1) {
#endif
    buf.resize(buf.size() * 2);    
  }
  va_end(ap);
  if (ret == static_cast<int>(buf.size())){
    buf[buf.size() - 1] = '\0';    
  }         
  test.debug_log_info(AFile, AFunc, ALine, &buf[0], ALevel);
}

void test_logCAN(const char* ADesc, PCAN ACAN, const TLogLevel ALevel)
{
    char s[TEMP_STR_LEN];
    // channel, id, dlc, [data]
    if (ACAN->is_tx){
        if (ACAN->is_data){
            sprintf(s, "%s %d %03X Tx d %d [%02X %02X %02X %02X %02X %02X %02X %02X]", ADesc, ACAN->FIdxChn, ACAN->FIdentifier, ACAN->FDLC, ACAN->FData[0], ACAN->FData[1], ACAN->FData[2], ACAN->FData[3], ACAN->FData[4], ACAN->FData[5], ACAN->FData[6], ACAN->FData[7]);
        } else {
            sprintf(s, "%s %d %03X Tx r %d", ADesc, ACAN->FIdxChn, ACAN->FIdentifier, ACAN->FDLC);
        }
    } else {
        if (ACAN->is_data){
            sprintf(s, "%s %d %03X Rx d %d [%02X %02X %02X %02X %02X %02X %02X %02X]", ADesc, ACAN->FIdxChn, ACAN->FIdentifier, ACAN->FDLC, ACAN->FData[0], ACAN->FData[1], ACAN->FData[2], ACAN->FData[3], ACAN->FData[4], ACAN->FData[5], ACAN->FData[6], ACAN->FData[7]);
        } else {
            sprintf(s, "%s %d %03X Rx r %d", ADesc, ACAN->FIdxChn, ACAN->FIdentifier, ACAN->FDLC);
        }
    }
    test.log_info(s, ALevel);
}

DLLEXPORT s32 __stdcall initialize_miniprogram(const PTSMasterConfiguration AConf)
{
    app = AConf->FTSApp;
    com = AConf->FTSCOM;
    test = AConf->FTSTest;
    mbd = AConf->FTSMBD;
    tac = AConf->FTSTAC;
    return 0;
    
}

DLLEXPORT s32 __stdcall finalize_miniprogram(void)
{
    return 0;
    
}

// MP library functions definition

// Variables defintions
extern TMPVarDouble gas_cylinder_deflate;
extern TMPVarDouble gas_cylinder_inflate;
extern TMPVarDouble gas_cylinder_stop;

// Retrieve TSMP abilities
typedef s32 (__stdcall* TRegTSMasterFunction)(const void* AObj, const char* AFuncType, const char* AFuncName, const char* AData, const void* AFuncPointer, const char* ADescription);
extern void step(void);
extern void on_canfd_rx_OnRx_DUT1_Sensors0(const TCANFD* ACANFD);
extern void on_canfd_rx_OnRx_DUT2_Sensors(const TCANFD* ACANFD);
extern void on_canfd_rx_OnRx_DUT3_Sensors(const TCANFD* ACANFD);
extern void on_canfd_pretx_OnPreTx_MVD_ValvesDutyControl(const PCANFD ACANFD);
extern void on_var_change_gas_cylinder_deflate(void);
extern void on_var_change_gas_cylinder_inflate(void);
extern void on_var_change_gas_cylinder_stop(void);
extern void on_start_init(void);
extern s32 rv_valve(s32 ValveSequence, s32 SwitchState);
extern s32 ini(void);
extern s32 asv_valve(s32 ValveSequence, s32 SwitchState);
extern s32 vv_valve(double SwitchState);
extern s32 motors(u8 SwitchState);
extern s32 gas_cylinder(u8 GasCylinder, u8 ValveInflateParam);
extern s32 power_source(const s32 Power_button);
extern s32 pressure_limit(const s32 dut, s32 c, s32 pressure);
DLLEXPORT s32 __stdcall retrieve_mp_abilities(const void* AObj, const TRegTSMasterFunction AReg) {
  #define TSMASTER_VERSION "2025.11.12.1716"
  if (!AReg(AObj, "check_mp_internal", "version", TSMASTER_VERSION, 0, "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_app", (void *)sizeof(TTSMasterConfiguration), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_tcan", (void *)sizeof(TCAN), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_tcanfd", (void *)sizeof(TCANFD), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_tlin", (void *)sizeof(TLIN), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_tflexray", (void *)sizeof(TFlexRay), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_tethernetheader", (void *)sizeof(TEthernetHeader), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TMPVarInt", (void *)sizeof(TMPVarInt), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TMPVarDouble", (void *)sizeof(TMPVarDouble), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TMPVarString", (void *)sizeof(TMPVarString), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TMPVarCAN", (void *)sizeof(TMPVarCAN), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TMPVarCANFD", (void *)sizeof(TMPVarCANFD), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TMPVarLIN", (void *)sizeof(TMPVarLIN), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TLIBTSMapping", (void *)sizeof(TLIBTSMapping), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TLIBSystemVarDef", (void *)sizeof(TLIBSystemVarDef), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "auto_start", "1", 0, "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "addr_conf", "app", &app, "")) return -1;
  if (!AReg(AObj, "step_function", "step", "1", reinterpret_cast<const void*>(&step), "")) return -1;
  if (!AReg(AObj, "var", "gas_cylinder_deflate", "1,MA__,", reinterpret_cast<const void*>(&gas_cylinder_deflate), "")) return -1;
  if (!AReg(AObj, "var", "gas_cylinder_inflate", "1,MA__,", reinterpret_cast<const void*>(&gas_cylinder_inflate), "")) return -1;
  if (!AReg(AObj, "var", "gas_cylinder_stop", "1,MA__,", reinterpret_cast<const void*>(&gas_cylinder_stop), "")) return -1;
  if (!AReg(AObj, "on_can_rx_callback", "on_canfd_rx_OnRx_DUT1_Sensors0", "12289,-1,-1", reinterpret_cast<const void*>(&on_canfd_rx_OnRx_DUT1_Sensors0), "")) return -1;
  if (!AReg(AObj, "on_can_rx_callback", "on_canfd_rx_OnRx_DUT2_Sensors", "12545,-1,-1", reinterpret_cast<const void*>(&on_canfd_rx_OnRx_DUT2_Sensors), "")) return -1;
  if (!AReg(AObj, "on_can_rx_callback", "on_canfd_rx_OnRx_DUT3_Sensors", "12801,-1,-1", reinterpret_cast<const void*>(&on_canfd_rx_OnRx_DUT3_Sensors), "")) return -1;
  if (!AReg(AObj, "on_can_pretx_callback", "on_canfd_pretx_OnPreTx_MVD_ValvesDutyControl", "12547,-1,-1", reinterpret_cast<const void*>(&on_canfd_pretx_OnPreTx_MVD_ValvesDutyControl), "")) return -1;
  if (!AReg(AObj, "on_var_change_callback", "on_var_change_gas_cylinder_deflate", "gas_cylinder_deflate", reinterpret_cast<const void*>(&on_var_change_gas_cylinder_deflate), "")) return -1;
  if (!AReg(AObj, "on_var_change_callback", "on_var_change_gas_cylinder_inflate", "gas_cylinder_inflate", reinterpret_cast<const void*>(&on_var_change_gas_cylinder_inflate), "")) return -1;
  if (!AReg(AObj, "on_var_change_callback", "on_var_change_gas_cylinder_stop", "gas_cylinder_stop", reinterpret_cast<const void*>(&on_var_change_gas_cylinder_stop), "")) return -1;
  if (!AReg(AObj, "on_start_callback", "on_start_init", "", reinterpret_cast<const void*>(&on_start_init), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "rv_valve", "s32 ValveSequence, s32 SwitchState", reinterpret_cast<const void*>(&rv_valve), "阀控制")) return -1;
  if (!AReg(AObj, "on_custom_callback", "ini", "void", reinterpret_cast<const void*>(&ini), "初始化")) return -1;
  if (!AReg(AObj, "on_custom_callback", "asv_valve", "s32 ValveSequence, s32 SwitchState", reinterpret_cast<const void*>(&asv_valve), "阀控制")) return -1;
  if (!AReg(AObj, "on_custom_callback", "vv_valve", "double SwitchState", reinterpret_cast<const void*>(&vv_valve), "阀控制")) return -1;
  if (!AReg(AObj, "on_custom_callback", "motors", "u8 SwitchState", reinterpret_cast<const void*>(&motors), "电机控制")) return -1;
  if (!AReg(AObj, "on_custom_callback", "gas_cylinder", "u8 GasCylinder, u8 ValveInflateParam", reinterpret_cast<const void*>(&gas_cylinder), "气罐气压控制")) return -1;
  if (!AReg(AObj, "on_custom_callback", "power_source", "const s32 Power_button", reinterpret_cast<const void*>(&power_source), "电源控制")) return -1;
  if (!AReg(AObj, "on_custom_callback", "pressure_limit", "const s32 dut, s32 c, s32 pressure", reinterpret_cast<const void*>(&pressure_limit), "压力限制")) return -1;
  // MP library functions

  return 20;
}
