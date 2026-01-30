// GEN BLOCK BEGIN Include
#define TSMP_IMPL
#include "TSMaster.h"
#include "MPLibrary.h"
#include "Database.h"
#include "TSMasterBaseInclude.h"
#include "Configuration.h"
// GEN BLOCK END Include

// CODE BLOCK BEGIN On_Start On_Start
// 启动事件 "On_Start"
void on_start_On_Start(void) { // 程序启动事件
app.set_system_var_int32("DUT1.AMIsRunning", 1);

}
// CODE BLOCK END On_Start On_Start

// CODE BLOCK BEGIN On_Stop On_Stop
// 停止事件 "On_Stop"
void on_stop_On_Stop(void) { // 程序停止事件
app.set_system_var_int32("DUT1.AMIsRunning", 1);

}
// CODE BLOCK END On_Stop On_Stop

// CODE BLOCK BEGIN Step_Function  NQ__
// 主step函数，执行周期 5 ms
void step(void) { // 周期 = 5 ms

}
// CODE BLOCK END Step_Function 

