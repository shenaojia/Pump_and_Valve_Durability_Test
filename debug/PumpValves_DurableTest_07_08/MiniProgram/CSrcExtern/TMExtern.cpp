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

// Retrieve TSMP abilities
typedef s32 (__stdcall* TRegTSMasterFunction)(const void* AObj, const char* AFuncType, const char* AFuncName, const char* AData, const void* AFuncPointer, const char* ADescription);
extern void step(void);
extern void on_canfd_rx_On_CAN_FD_Rx(const TCANFD* ACANFD);
extern void on_canfd_pretx_On_CAN_FD_PreTx(const PCANFD ACANFD);
extern void on_start_On_Start_System_Set(void);
extern void on_stop_On_DUT_Stop(void);
extern s32 dut_on_start(const s32 ADUTIndex);
extern s32 dut_on_stop(const s32 ADUTIndex);
extern s32 dut_on_recieve(const s32 ADUTIndex, const TCANFD* ACANFD);
extern s32 dut_on_pretx(const PCANFD ACANFD);
extern s32 stop_test_action(const s32 ADUTIndex);
extern s32 dut_rbs_init(const s32 ADUTIndex);
extern s32 dut_step(const s32 ADUTIndex);
extern s32 bench_state_step(void);
extern s32 on_tsmaster_disconnect(void);
extern s32 fault_action_deflate_fl(const s32 ADUTIndex, const float ATargetPressureBar);
extern s32 fault_action_deflate_fr(const s32 ADUTIndex, const float ATargetPressureBar);
extern s32 fault_action_deflate_rl(const s32 ADUTIndex, const float ATargetPressureBar);
extern s32 fault_action_deflate_rr(const s32 ADUTIndex, const float ATargetPressureBar);
extern s32 fault_action_deflate_res(const s32 ADUTIndex, const float ATargetPressureBar);
extern s32 fault_action_clear_test_case_error(const s32 ADUTIndex);
extern s32 dut_reset_test_statistics_data(const s32 ADUTIndex);
extern s32 on_start_button_click(const double ADummy);
extern s32 on_stop_button_click(const double ADummy);
extern s32 on_reset_data_button_click(const double ADummy);
extern s32 on_clear_fault_button_click(const double ADummy);
extern s32 error_stop_all_test(void);
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
  if (!AReg(AObj, "step_function", "step", "1000", reinterpret_cast<const void*>(&step), "")) return -1;
  if (!AReg(AObj, "on_can_rx_callback", "on_canfd_rx_On_CAN_FD_Rx", "-1,-1,-1", reinterpret_cast<const void*>(&on_canfd_rx_On_CAN_FD_Rx), "")) return -1;
  if (!AReg(AObj, "on_can_pretx_callback", "on_canfd_pretx_On_CAN_FD_PreTx", "-1,-1,-1", reinterpret_cast<const void*>(&on_canfd_pretx_On_CAN_FD_PreTx), "")) return -1;
  if (!AReg(AObj, "on_start_callback", "on_start_On_Start_System_Set", "", reinterpret_cast<const void*>(&on_start_On_Start_System_Set), "")) return -1;
  if (!AReg(AObj, "on_stop_callback", "on_stop_On_DUT_Stop", "", reinterpret_cast<const void*>(&on_stop_On_DUT_Stop), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "dut_on_start", "const s32 ADUTIndex", reinterpret_cast<const void*>(&dut_on_start), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "dut_on_stop", "const s32 ADUTIndex", reinterpret_cast<const void*>(&dut_on_stop), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "dut_on_recieve", "const s32 ADUTIndex, const TCANFD* ACANFD", reinterpret_cast<const void*>(&dut_on_recieve), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "dut_on_pretx", "const PCANFD ACANFD", reinterpret_cast<const void*>(&dut_on_pretx), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "stop_test_action", "const s32 ADUTIndex", reinterpret_cast<const void*>(&stop_test_action), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "dut_rbs_init", "const s32 ADUTIndex", reinterpret_cast<const void*>(&dut_rbs_init), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "dut_step", "const s32 ADUTIndex", reinterpret_cast<const void*>(&dut_step), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "bench_state_step", "void", reinterpret_cast<const void*>(&bench_state_step), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "on_tsmaster_disconnect", "void", reinterpret_cast<const void*>(&on_tsmaster_disconnect), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "fault_action_deflate_fl", "const s32 ADUTIndex, const float ATargetPressureBar", reinterpret_cast<const void*>(&fault_action_deflate_fl), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "fault_action_deflate_fr", "const s32 ADUTIndex, const float ATargetPressureBar", reinterpret_cast<const void*>(&fault_action_deflate_fr), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "fault_action_deflate_rl", "const s32 ADUTIndex, const float ATargetPressureBar", reinterpret_cast<const void*>(&fault_action_deflate_rl), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "fault_action_deflate_rr", "const s32 ADUTIndex, const float ATargetPressureBar", reinterpret_cast<const void*>(&fault_action_deflate_rr), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "fault_action_deflate_res", "const s32 ADUTIndex, const float ATargetPressureBar", reinterpret_cast<const void*>(&fault_action_deflate_res), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "fault_action_clear_test_case_error", "const s32 ADUTIndex", reinterpret_cast<const void*>(&fault_action_clear_test_case_error), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "dut_reset_test_statistics_data", "const s32 ADUTIndex", reinterpret_cast<const void*>(&dut_reset_test_statistics_data), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "on_start_button_click", "const double ADummy", reinterpret_cast<const void*>(&on_start_button_click), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "on_stop_button_click", "const double ADummy", reinterpret_cast<const void*>(&on_stop_button_click), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "on_reset_data_button_click", "const double ADummy", reinterpret_cast<const void*>(&on_reset_data_button_click), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "on_clear_fault_button_click", "const double ADummy", reinterpret_cast<const void*>(&on_clear_fault_button_click), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "error_stop_all_test", "void", reinterpret_cast<const void*>(&error_stop_all_test), "")) return -1;
  // MP library functions

  return 26;
}
