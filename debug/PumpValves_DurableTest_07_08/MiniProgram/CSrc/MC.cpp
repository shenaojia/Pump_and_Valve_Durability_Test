// GEN BLOCK BEGIN Include
#define TSMP_IMPL
#include "TSMaster.h"
#include "MPLibrary.h"
#include "Database.h"
#include "TSMasterBaseInclude.h"
#include "Configuration.h"
// GEN BLOCK END Include

// CODE BLOCK BEGIN Variable gas_cylinder_deflate MSxNQV9fLCx4dmk1M3JmRnh2Z2dEUW9f
// 变量文档 "gas_cylinder_deflate"
TMPVarDouble gas_cylinder_deflate;
// CODE BLOCK END Variable gas_cylinder_deflate

// CODE BLOCK BEGIN Variable gas_cylinder_inflate MSxNQV9fLCx4dmk1M3JQa3h2Z05DZ19f
// 变量文档 "gas_cylinder_inflate"
TMPVarDouble gas_cylinder_inflate;
// CODE BLOCK END Variable gas_cylinder_inflate

// CODE BLOCK BEGIN Variable gas_cylinder_stop MSxNQV9fLCx4dmk1M3MyajFybXo1TWI0dDhYRytBMEs_
// 变量文档 "gas_cylinder_stop"
TMPVarDouble gas_cylinder_stop;
// CODE BLOCK END Variable gas_cylinder_stop

// CODE BLOCK BEGIN Global_Definitions 
#define VALVE_ID 1
TMVD_ValvesCurrentControl_1 MVD_ValvesCurrentControl_1;
TVD_Current_Control_1 VD_Current_Control_1;
TMVD_MotorsDutyControl_1 MVD_MotorsDutyControl_1;
TPWR_Control_1 PWR_Control_1;
TDUT1_Sensors_4 DUT1_Sensors_4;
// CODE BLOCK END Global_Definitions 

// GEN BLOCK BEGIN Custom_Function
s32 rv_valve(s32 ValveSequence, s32 SwitchState);
s32 ini(void);
s32 asv_valve(s32 ValveSequence, s32 SwitchState);
s32 vv_valve(double SwitchState);
s32 motors(u8 SwitchState);
s32 gas_cylinder(u8 GasCylinder, u8 ValveInflateParam);
s32 power_source(const s32 Power_button);
s32 pressure_limit(const s32 dut, s32 c, s32 pressure);
// GEN BLOCK END Custom_Function

// CODE BLOCK BEGIN On_Start init
// 启动事件 "init"
void on_start_init(void) { // 程序启动事件
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
	ini();

	ini();

}
// CODE BLOCK END On_Start init

// CODE BLOCK BEGIN Custom_Function rv_valve czMyIFZhbHZlU2VxdWVuY2UsIHMzMiBTd2l0Y2hTdGF0ZQ__
// 自定义函数 "rv_valve"
s32 rv_valve(s32 ValveSequence, s32 SwitchState) { // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
	// 自定义函数: 阀控制
	// 根据输入参数设置目标电流
	float targetCurrent = 0.0;

	if (SwitchState == 0) {
		// 关闭阀门模式
		targetCurrent = 0.0;
		if (ValveSequence == 1) {
			VD_Current_Control_1.RV1_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 2) {
			VD_Current_Control_1.RV2_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 3) {
			VD_Current_Control_1.RV3_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 4) {
			VD_Current_Control_1.RV4_Target_Current = targetCurrent;
		}
		// 发送电流控制命令
		com.transmit_canfd_async(&VD_Current_Control_1.FCAN);
	}
	else if (SwitchState == 1) {
		// 开启阀门模式 - 先设置为3.5A，然后调整到1.35A
		targetCurrent = 3.5;
		if (ValveSequence == 1) {
			VD_Current_Control_1.RV1_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 2) {
			VD_Current_Control_1.RV2_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 3) {
			VD_Current_Control_1.RV3_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 4) {
			VD_Current_Control_1.RV4_Target_Current = targetCurrent;
		}
		// 发送电流控制命令
		com.transmit_canfd_async(&VD_Current_Control_1.FCAN);
		app.wait(100, ""); // 短暂等待阀门响应

		// 逐步减小电流至1.4A：每次降低0.1A，每次间隔10ms（从当前targetCurrent开始）
		{
			float cur = targetCurrent; // 从刚才设置的3.5A开始
			// 每次减小0.1A，直到降到1.4A或更低为止（保守判断）
			while (cur - 0.1f >= 1.4f - 1e-6f) {
				cur -= 0.1f;
				// 根据阀序号写入目标电流
				if (ValveSequence == 1) {
					VD_Current_Control_1.RV1_Target_Current = cur;
				}
				else if (ValveSequence == 2) {
					VD_Current_Control_1.RV2_Target_Current = cur;
				}
				else if (ValveSequence == 3) {
					VD_Current_Control_1.RV3_Target_Current = cur;
				}
				else if (ValveSequence == 4) {
					VD_Current_Control_1.RV4_Target_Current = cur;
				}
				// 发送电流控制命令并等待10ms
				com.transmit_canfd_async(&VD_Current_Control_1.FCAN);
				app.wait(10, "");
			}
			// 将targetCurrent更新为当前值（循环结束时为1.4A或略高）
			targetCurrent = cur;
		}

		// 调整到维持电流
		targetCurrent = 1.35;
		if (ValveSequence == 1) {
			VD_Current_Control_1.RV1_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 2) {
			VD_Current_Control_1.RV2_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 3) {
			VD_Current_Control_1.RV3_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 4) {
			VD_Current_Control_1.RV4_Target_Current = targetCurrent;
		}
		// 发送最终的电流控制命令
		com.transmit_canfd_async(&VD_Current_Control_1.FCAN);
	}
	return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function rv_valve

// CODE BLOCK BEGIN Custom_Function ini dm9pZA__
// 自定义函数 "ini"
s32 ini(void) { // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
 // 自定义函数: 初始化
	VD_Current_Control_1.init();  	      //初始化
	MVD_ValvesCurrentControl_1.init();  	//初始化
	MVD_MotorsDutyControl_1.init();      	//初始化
	PWR_Control_1.init();                 //初始化
  DUT1_Sensors_4.init();                //初始化
	return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function ini

// CODE BLOCK BEGIN Custom_Function asv_valve czMyIFZhbHZlU2VxdWVuY2UsIHMzMiBTd2l0Y2hTdGF0ZQ__
// 自定义函数 "asv_valve"
s32 asv_valve(s32 ValveSequence, s32 SwitchState) { // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
	// 根据输入参数设置目标电流
	float targetCurrent = 0.0;
	if (SwitchState == 0) {
		// 关闭阀门模式
		targetCurrent = 0.0;
		if (ValveSequence == 1) {
			VD_Current_Control_1.ASV1_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 2) {
			VD_Current_Control_1.ASV2_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 3) {
			VD_Current_Control_1.ASV3_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 4) {
			VD_Current_Control_1.ASV4_Target_Current = targetCurrent;
		}
		// 发送电流控制命令
		com.transmit_canfd_async(&VD_Current_Control_1.FCAN);
	}
	else if (SwitchState == 1) {
		// 开启阀门模式 - 先设置为3.5A，然后调整到1.35A
		targetCurrent = 3.5;
		if (ValveSequence == 1) {
			VD_Current_Control_1.ASV1_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 2) {
			VD_Current_Control_1.ASV2_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 3) {
			VD_Current_Control_1.ASV3_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 4) {
			VD_Current_Control_1.ASV4_Target_Current = targetCurrent;
		}
		// 发送电流控制命令
		com.transmit_canfd_async(&VD_Current_Control_1.FCAN);
		app.wait(100, ""); // 短暂等待阀门响应

		// 逐步减小电流至1.4A：每次降低0.1A，每次间隔10ms（从当前targetCurrent开始）
		{
			float cur = targetCurrent; // 从刚才设置的3.5A开始
			// 每次减小0.1A，直到降到1.4A或更低为止（保守判断）
			while (cur - 0.1f >= 1.4f - 1e-6f) {
				cur -= 0.1f;
				// 根据阀序号写入目标电流
				if (ValveSequence == 1) {
					VD_Current_Control_1.ASV1_Target_Current = cur;
				}
				else if (ValveSequence == 2) {
					VD_Current_Control_1.ASV2_Target_Current = cur;
				}
				else if (ValveSequence == 3) {
					VD_Current_Control_1.ASV3_Target_Current = cur;
				}
				else if (ValveSequence == 4) {
					VD_Current_Control_1.ASV4_Target_Current = cur;
				}
				// 发送电流控制命令并等待10ms
				com.transmit_canfd_async(&VD_Current_Control_1.FCAN);
				app.wait(10, "");
			}
			// 将targetCurrent更新为当前值（循环结束时为1.4A或略高）
			targetCurrent = cur;
		}

		// 调整到维持电流
		targetCurrent = 1.35;
		if (ValveSequence == 1) {
			VD_Current_Control_1.ASV1_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 2) {
			VD_Current_Control_1.ASV2_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 3) {
			VD_Current_Control_1.ASV3_Target_Current = targetCurrent;
		}
		else if (ValveSequence == 4) {
			VD_Current_Control_1.ASV4_Target_Current = targetCurrent;
		}
		// 发送最终的电流控制命令
		com.transmit_canfd_async(&VD_Current_Control_1.FCAN);
	}
	return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function asv_valve

// CODE BLOCK BEGIN Custom_Function vv_valve ZG91YmxlIFN3aXRjaFN0YXRl
// 自定义函数 "vv_valve"
s32 vv_valve(double SwitchState) { // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
 // 自定义函数: 阀控制
	// 根据输入参数设置目标电流
	float targetCurrent = 0.0;
	if (SwitchState == 0) {
		// 关闭阀门模式
		targetCurrent = 0.0;
		MVD_ValvesCurrentControl_1.VV_Target_Current = targetCurrent;
	}
	else if (SwitchState == 1) {
		// 开启阀门模式 - 先设置为3.5A，然后调整到1.35A
		targetCurrent = 3.5;
		MVD_ValvesCurrentControl_1.VV_Target_Current = targetCurrent;
		com.transmit_canfd_async(&MVD_ValvesCurrentControl_1.FCAN);
		app.wait(100, ""); // 短暂等待阀门响应

		// 逐步减小电流至1.4A：每次降低0.1A，每次间隔10ms（从当前targetCurrent开始）
		{
			float cur = targetCurrent; // 从刚才设置的3.5A开始
			// 每次减小0.1A，直到降到1.4A或更低为止（保守判断）
			while (cur - 0.1f >= 1.4f - 1e-6f) {
				cur -= 0.1f;
				MVD_ValvesCurrentControl_1.VV_Target_Current = cur;
				// 发送电流控制命令并等待10ms
				com.transmit_canfd_async(&MVD_ValvesCurrentControl_1.FCAN);
				app.wait(10, "");
			}
			// 将targetCurrent更新为当前值（循环结束时为1.4A或略高）
			targetCurrent = cur;
		}

		// 调整到维持电流
		targetCurrent = 1.35;
	}
	// 发送最终的电流控制命令
	MVD_ValvesCurrentControl_1.VV_Target_Current = targetCurrent;
	com.transmit_canfd_async(&MVD_ValvesCurrentControl_1.FCAN);
	return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function vv_valve

// CODE BLOCK BEGIN Custom_Function motors dTggU3dpdGNoU3RhdGU_
// 自定义函数 "motors"
s32 motors(u8 SwitchState) { // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
 // 自定义函数: 电机控制
    //控制电机
  if (SwitchState == 1) {
		test_log("motors_80");
		MVD_MotorsDutyControl_1.Motor_Duty = 80;
		com.transmit_canfd_async(&MVD_MotorsDutyControl_1.FCAN);
	}
	else if (SwitchState == 0) {
		test_log("motors_20");
		MVD_MotorsDutyControl_1.Motor_Duty = 0;
		com.transmit_canfd_async(&MVD_MotorsDutyControl_1.FCAN);
	}
 
  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function motors

// CODE BLOCK BEGIN On_CAN_FD_PreTx OnPreTx_MVD_ValvesDutyControl LTEsLTEsMTI1NDc_
// CAN报文预发送事件 "OnPreTx_MVD_ValvesDutyControl" 针对标识符 = 0x3103 (FD)
void on_canfd_pretx_OnPreTx_MVD_ValvesDutyControl(const PCANFD ACANFD) {  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
  // 针对标识符 = 0x3103 (FD)
	if (ACANFD->FIdxChn != CH1) return; // if you want to filter channel
	TMVD_ValvesDutyControl_1 MVD_ValvesDutyControl_1;
	MVD_ValvesDutyControl_1.init();
	MVD_ValvesDutyControl_1.FCAN.FIdxChn = 1; // 若是在回调函数中，请用ACAN数据赋值FCAN
	// deal with signals using MVD_ValvesDutyControl_1.XXX
	MVD_ValvesDutyControl_1.RESV_CH3_Duty = 0;
	MVD_ValvesDutyControl_1.RESV_CH4_Duty = 0;
	MVD_ValvesDutyControl_1.VV_Duty = 0;
	MVD_ValvesDutyControl_1.RESV_CH6_Duty = 0;
	MVD_ValvesDutyControl_1.RESV_CH7_Duty = 0;
	MVD_ValvesDutyControl_1.RESV_CH8_Duty = 0;
	*ACANFD = MVD_ValvesDutyControl_1.FCAN;

}
// CODE BLOCK END On_CAN_FD_PreTx OnPreTx_MVD_ValvesDutyControl

// CODE BLOCK BEGIN On_CAN_FD_Rx OnRx_DUT1_Sensors0 LTEsLTEsMTIyODk_
// CAN报文接收事件 "OnRx_DUT1_Sensors0" 针对标识符 = 0x3001 (FD)
void on_canfd_rx_OnRx_DUT1_Sensors0(const TCANFD* ACANFD) {  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
  // 针对标识符 = 0x3001 (FD)
if (ACANFD->FIdxChn != CH4) return; // if you want to filter channel
TDUT1_Sensors_4 DUT1_Sensors_4;
DUT1_Sensors_4.init();
DUT1_Sensors_4.FCAN = *ACANFD; // 若是在回调函数中，请用ACAN数据赋值FCAN
 // deal with signals using DUT1_Sensors_4.XXX
(void) DUT1_Sensors_4.Pressure_FL;
(void) DUT1_Sensors_4.Pressure_FR;
(void) DUT1_Sensors_4.Pressure_RL;
(void) DUT1_Sensors_4.Pressure_RR;
(void) DUT1_Sensors_4.Pressure_RES;        
app.set_system_var_double("DUT1.DUT1_Pressure_FL", DUT1_Sensors_4.Pressure_FL);
app.set_system_var_double("DUT1.DUT1_Pressure_FR", DUT1_Sensors_4.Pressure_FR);
app.set_system_var_double("DUT1.DUT1_Pressure_RL", DUT1_Sensors_4.Pressure_RL);
app.set_system_var_double("DUT1.DUT1_Pressure_RR", DUT1_Sensors_4.Pressure_RR);
app.set_system_var_double("DUT1.DUT1_Pressure_RES", DUT1_Sensors_4.Pressure_RES);

}
// CODE BLOCK END On_CAN_FD_Rx OnRx_DUT1_Sensors0

// CODE BLOCK BEGIN On_CAN_FD_Rx OnRx_DUT2_Sensors LTEsLTEsMTI1NDU_
// CAN报文接收事件 "OnRx_DUT2_Sensors" 针对标识符 = 0x3101 (FD)
void on_canfd_rx_OnRx_DUT2_Sensors(const TCANFD* ACANFD) {  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
  // 针对标识符 = 0x3101 (FD)
if (ACANFD->FIdxChn != CH4) return; // if you want to filter channel
TDUT2_Sensors_4 DUT2_Sensors_4;
DUT2_Sensors_4.init();
DUT2_Sensors_4.FCAN = *ACANFD; // 若是在回调函数中，请用ACAN数据赋值FCAN
 // deal with signals using DUT2_Sensors_4.XXX
(void) DUT2_Sensors_4.Pressure_FL;
(void) DUT2_Sensors_4.Pressure_FR;
(void) DUT2_Sensors_4.Pressure_RL;
(void) DUT2_Sensors_4.Pressure_RR;
(void) DUT2_Sensors_4.Pressure_RES;
app.set_system_var_double("DUT2.DUT2_Pressure_FL", DUT2_Sensors_4.Pressure_FL);
app.set_system_var_double("DUT2.DUT2_Pressure_FR", DUT2_Sensors_4.Pressure_FR);
app.set_system_var_double("DUT2.DUT2_Pressure_RL", DUT2_Sensors_4.Pressure_RL);
app.set_system_var_double("DUT2.DUT2_Pressure_RR", DUT2_Sensors_4.Pressure_RR);
app.set_system_var_double("DUT2.DUT2_Pressure_RES", DUT2_Sensors_4.Pressure_RES);

}
// CODE BLOCK END On_CAN_FD_Rx OnRx_DUT2_Sensors

// CODE BLOCK BEGIN On_CAN_FD_Rx OnRx_DUT3_Sensors LTEsLTEsMTI4MDE_
// CAN报文接收事件 "OnRx_DUT3_Sensors" 针对标识符 = 0x3201 (FD)
void on_canfd_rx_OnRx_DUT3_Sensors(const TCANFD* ACANFD) {  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
  // 针对标识符 = 0x3201 (FD)
if (ACANFD->FIdxChn != CH4) return; // if you want to filter channel
TDUT3_Sensors_4 DUT3_Sensors_4;
DUT3_Sensors_4.init();
DUT3_Sensors_4.FCAN = *ACANFD; // 若是在回调函数中，请用ACAN数据赋值FCAN
 // deal with signals using DUT3_Sensors_4.XXX
(void) DUT3_Sensors_4.Pressure_FL;
(void) DUT3_Sensors_4.Pressure_FR;
(void) DUT3_Sensors_4.Pressure_RL;
(void) DUT3_Sensors_4.Pressure_RR;
(void) DUT3_Sensors_4.Pressure_RES;
app.set_system_var_double("DUT3.DUT3_Pressure_FL", DUT3_Sensors_4.Pressure_FL);
app.set_system_var_double("DUT3.DUT3_Pressure_FR", DUT3_Sensors_4.Pressure_FR);
app.set_system_var_double("DUT3.DUT3_Pressure_RL", DUT3_Sensors_4.Pressure_RL);
app.set_system_var_double("DUT3.DUT3_Pressure_RR", DUT3_Sensors_4.Pressure_RR);
app.set_system_var_double("DUT3.DUT3_Pressure_RES", DUT3_Sensors_4.Pressure_RES);

}
// CODE BLOCK END On_CAN_FD_Rx OnRx_DUT3_Sensors

// CODE BLOCK BEGIN On_Var_Change gas_cylinder_deflate Z2FzX2N5bGluZGVyX2RlZmxhdGUsLTE_
// 变量变化事件 "gas_cylinder_deflate" 针对变量 "gas_cylinder_deflate" [On Written]
void on_var_change_gas_cylinder_deflate(void) { // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
 // 变量 = gas_cylinder_deflate
	power_source(1);
	s32 GasTank1;
	app.get_system_var_int32("GasTank", &GasTank1);
	if (GasTank1 == 1) {  //FL
		if (gas_cylinder_deflate.get() == 1) {
			gas_cylinder_inflate.set(0);
			gas_cylinder(1, 2);  //放气
		}
		if (gas_cylinder_deflate.get() == 0) {
			gas_cylinder(1, 0);  //停止充气和放气 
		}
	}
	else if (GasTank1 == 2) {  //FR
		if (gas_cylinder_deflate.get() == 1) {
			gas_cylinder_inflate.set(0);
			gas_cylinder(2, 2);  //放气
		}
		if (gas_cylinder_deflate.get() == 0) {
			gas_cylinder(2, 0);  //停止充气和放气 
		}
	}
	else if (GasTank1 == 3) {  //RL
		if (gas_cylinder_deflate.get() == 1) {
			gas_cylinder_inflate.set(0);
			gas_cylinder(3, 2);  //放气
		}
		if (gas_cylinder_deflate.get() == 0) {
			gas_cylinder(3, 0);  //停止充气和放气 
		}
	}
	else if (GasTank1 == 4) {  //RR
		if (gas_cylinder_deflate.get() == 1) {
			gas_cylinder_inflate.set(0);
			gas_cylinder(4, 2);  //放气
		}
		if (gas_cylinder_deflate.get() == 0) {
			gas_cylinder(4, 0);  //停止充气和放气 
		}
	}
	else if (GasTank1 == 5) {  //RES
		if (gas_cylinder_deflate.get() == 1) {
			gas_cylinder_inflate.set(0);
			gas_cylinder(5, 2);  //放气
		}
		if (gas_cylinder_deflate.get() == 0) {
			gas_cylinder(5, 0);  //停止充气和放气 
		}
	}

}
// CODE BLOCK END On_Var_Change gas_cylinder_deflate

// CODE BLOCK BEGIN On_Var_Change gas_cylinder_inflate Z2FzX2N5bGluZGVyX2luZmxhdGUsLTE_
// 变量变化事件 "gas_cylinder_inflate" 针对变量 "gas_cylinder_inflate" [On Written]
void on_var_change_gas_cylinder_inflate(void) { // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
 // 变量 = gas_cylinder_inflate
	power_source(1);
	s32 GasTank1;
	app.get_system_var_int32("GasTank", &GasTank1);
	if (GasTank1 == 1) {  //FL
		if (gas_cylinder_inflate.get() == 1) {
      test_log("FL气罐充气");
			gas_cylinder_deflate.set(0);
			gas_cylinder(1, 1);  //充气
		}
		if (gas_cylinder_inflate.get() == 0) {
      test_log("FL气罐放气");
			gas_cylinder(1, 0);  //停止充气和放气 
		}
	}
	else if (GasTank1 == 2) {  //FR
		if (gas_cylinder_inflate.get() == 1) {
			gas_cylinder_deflate.set(0);
			gas_cylinder(2, 1);  //充气
		}
		if (gas_cylinder_inflate.get() == 0) {
			gas_cylinder(2, 0);  //停止充气和放气 
		}
	}
	else if (GasTank1 == 3) {  //RL
		if (gas_cylinder_inflate.get() == 1) {
			gas_cylinder_deflate.set(0);
			gas_cylinder(3, 1);  //充气
		}
		if (gas_cylinder_inflate.get() == 0) {
			gas_cylinder(3, 0);  //停止充气和放气 
		}
	}
	else if (GasTank1 == 4) {  //RR
		if (gas_cylinder_inflate.get() == 1) {
			gas_cylinder_deflate.set(0);
			gas_cylinder(4, 1);  //充气
		}
		if (gas_cylinder_inflate.get() == 0) {
			gas_cylinder(4, 0);  //停止充气和放气 
		}
	}
	else if (GasTank1 == 5) {  //RES
		if (gas_cylinder_inflate.get() == 1) {
			gas_cylinder_deflate.set(0);
			gas_cylinder(5, 1);  //充气
		}
		if (gas_cylinder_inflate.get() == 0) {
			gas_cylinder(5, 0);  //停止充气和放气 
		}
	}

}
// CODE BLOCK END On_Var_Change gas_cylinder_inflate

// CODE BLOCK BEGIN On_Var_Change gas_cylinder_stop Z2FzX2N5bGluZGVyX3N0b3AsLTE_
// 变量变化事件 "gas_cylinder_stop" 针对变量 "gas_cylinder_stop" [On Written]
void on_var_change_gas_cylinder_stop(void) { // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
 // 变量 = gas_cylinder_stop
	power_source(1);
	if (gas_cylinder_stop.get() == 1) {
		test_log("fl_gas_cylinder_0已触发");
		gas_cylinder_inflate.set(0);
		gas_cylinder_deflate.set(0);
		app.wait(0, "set");
		gas_cylinder_stop.set(0);
		gas_cylinder(1, 0);  //停止
	}
	power_source(0);

}
// CODE BLOCK END On_Var_Change gas_cylinder_stop

// CODE BLOCK BEGIN Custom_Function gas_cylinder dTggR2FzQ3lsaW5kZXIsIHU4IFZhbHZlSW5mbGF0ZVBhcmFt
// 自定义函数 "gas_cylinder"
s32 gas_cylinder(u8 GasCylinder, u8 ValveInflateParam) { // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
 // 自定义函数: 气罐气压控制
	if (GasCylinder == 1) { //FL储气罐
		if (ValveInflateParam == 0) { //停止充气和放气 
			//test_log("停止充气和放气");
			//关闭阀和电机
			asv_valve(1, 0);
			asv_valve(2, 0);
			asv_valve(3, 0);
			asv_valve(4, 0);
			rv_valve(1, 0);
			rv_valve(2, 0);
			rv_valve(3, 0);
			rv_valve(4, 0);
			vv_valve(0);
			motors(0);
		}
		else if (ValveInflateParam == 1) { //充气
			//test_log("充气");
			motors(1);
			rv_valve(1, 1);
			asv_valve(4, 1);
		}
		else if (ValveInflateParam == 2) { //放气
			//test_log("放气");
			asv_valve(4, 1);
			rv_valve(2, 1);
			vv_valve(1);
		}
	}
	else if (GasCylinder == 2) { //FR储气罐
		if (ValveInflateParam == 0) { //停止充气和放气 
			//test_log("停止充气和放气");
			//关闭阀和电机
			asv_valve(1, 0);
			asv_valve(2, 0);
			asv_valve(3, 0);
			asv_valve(4, 0);
			rv_valve(1, 0);
			rv_valve(2, 0);
			rv_valve(3, 0);
			rv_valve(4, 0);
			vv_valve(0);
			motors(0);
		}
		else if (ValveInflateParam == 1) { //充气
			//test_log("充气");
			motors(1);
			rv_valve(1, 1);
			asv_valve(3, 1);
		}
		else if (ValveInflateParam == 2) { //放气
			//test_log("放气");
			asv_valve(3, 1);
			rv_valve(2, 1);
			vv_valve(1);
		}
	}
	else if (GasCylinder == 3) { //RL储气罐
		if (ValveInflateParam == 0) { //停止充气和放气 
			//test_log("停止充气和放气");
			//关闭阀和电机
			asv_valve(1, 0);
			asv_valve(2, 0);
			asv_valve(3, 0);
			asv_valve(4, 0);
			rv_valve(1, 0);
			rv_valve(2, 0);
			rv_valve(3, 0);
			rv_valve(4, 0);
			vv_valve(0);
			motors(0);
		}
		else if (ValveInflateParam == 1) { //充气
			//test_log("充气");
			motors(1);
			rv_valve(1, 1);
			asv_valve(2, 1);
		}
		else if (ValveInflateParam == 2) { //放气
			//test_log("放气");
			asv_valve(2, 1);
			rv_valve(2, 1);
			vv_valve(1);
		}
	}
	else if (GasCylinder == 4) { //RR储气罐
		if (ValveInflateParam == 0) { //停止充气和放气 
			//test_log("停止充气和放气");
			//关闭阀和电机
			asv_valve(1, 0);
			asv_valve(2, 0);
			asv_valve(3, 0);
			asv_valve(4, 0);
			rv_valve(1, 0);
			rv_valve(2, 0);
			rv_valve(3, 0);
			rv_valve(4, 0);
			vv_valve(0);
			motors(0);
		}
		else if (ValveInflateParam == 1) { //充气
			//test_log("充气");
			motors(1);
			rv_valve(1, 1);
			asv_valve(1, 1);
		}
		else if (ValveInflateParam == 2) { //放气
			//test_log("放气");
			asv_valve(1, 1);
			rv_valve(2, 1);
			vv_valve(1);
		}
	}
	else if (GasCylinder == 5) { //RES储气罐
		if (ValveInflateParam == 0) { //停止充气和放气 
			//test_log("停止充气和放气");
			//关闭阀和电机
			asv_valve(1, 0);
			asv_valve(2, 0);
			asv_valve(3, 0);
			asv_valve(4, 0);
			rv_valve(1, 0);
			rv_valve(2, 0);
			rv_valve(3, 0);
			rv_valve(4, 0);
			vv_valve(0);
			motors(0);
		}
		else if (ValveInflateParam == 1) { //充气
			//test_log("充气");
			motors(1);
			rv_valve(4, 1);
		}
		else if (ValveInflateParam == 2) { //放气
			//test_log("放气");
			rv_valve(3, 1);
			vv_valve(1);
		}
	}
	return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function gas_cylinder

// CODE BLOCK BEGIN Custom_Function power_source Y29uc3QgczMyIFBvd2VyX2J1dHRvbg__
// 自定义函数 "power_source"
s32 power_source(const s32 Power_button) { // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
 // 自定义函数: 电源控制
	PWR_Control_1.Enable12VPowerSet = 1;
	PWR_Control_1.Enable48VPowerSet = 1;
	if (Power_button == 0) {
		PWR_Control_1.SetPower12V = 1;
		PWR_Control_1.SetPower48V = 1;
	}
	else if (Power_button == 1) {
		PWR_Control_1.SetPower12V = 1;
		PWR_Control_1.SetPower48V = 1;
	}
	com.transmit_canfd_async(&PWR_Control_1.FCAN);

	return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function power_source

// CODE BLOCK BEGIN Custom_Function pressure_limit Y29uc3QgczMyIGR1dCwgczMyIGMsIHMzMiBwcmVzc3VyZQ__
// 自定义函数 "pressure_limit"
s32 pressure_limit(const s32 dut, s32 c, s32 pressure) { // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
 // 自定义函数: 压力限制
  double DUT1_Pressure_FL;
  double fl;
  app.get_system_var_double("fl", &fl);


  
  app.get_system_var_double("DUT1.DUT1_Pressure_FL", &DUT1_Pressure_FL);
  while (DUT1_Pressure_FL <= fl) {
  	app.wait(500, "");
   }
     gas_cylinder_stop.set(1);
  return IDX_ERR_OK;

}
// CODE BLOCK END Custom_Function pressure_limit

// CODE BLOCK BEGIN Step_Function  MQ__
// 主step函数，执行周期 1 ms
void step(void) { // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
 // 周期 = 1 ms
	s32 Channel;
	app.get_system_var_int32("CH_CAN", &Channel);
	Channel = Channel - 1;
	VD_Current_Control_1.FCAN.FIdxChn = Channel;//通道  	     
	MVD_ValvesCurrentControl_1.FCAN.FIdxChn = Channel;//通道  	      	
	MVD_MotorsDutyControl_1.FCAN.FIdxChn = Channel;//通道  	
  PWR_Control_1.FCAN.FIdxChn = Channel;//通道            
  //pressure_limit(0,0,0);

}
// CODE BLOCK END Step_Function 

// CODE BLOCK BEGIN Configuration
/* 
[UI]
UICommon=-1,-1,-1,0,QyC0+sLrseC8rcb3IFtNQ10_,100,759,8688288388706036402,-1
ScriptName=MC
DisplayName=MC
IsMPLib=1
DBDeps=CAN_LED_Sensors,CAN1_DUT1,CAN2_DUT2,CAN3_DUT3
LastBuildTime=2025-12-14 13:21:54
APICnt=8

[UI_Doc]
Cnt=8
d0=asv_valve,t6e/2NbG,,2,NCwwLFZhbHZlU2VxdWVuY2UsdDZmUThzSFFVbFlf,NCwwLFN3aXRjaFN0YXRlLFd6QmR1ZGl4MVR0Yk1WMjA4citx
d1=vv_valve,t6e/2NbG,,1,NywwLFN3aXRjaFN0YXRlLFd6QmR1ZGl4MVR0Yk1WMjA4citx
d2=rv_valve,t6e/2NbG,,2,NCwwLFZhbHZlU2VxdWVuY2UsdDZmUThzSFFVbFlf,NCwwLFN3aXRjaFN0YXRlLFd6QmR1ZGl4MVR0Yk1WMjA4citx
d3=ini,s/XKvLuv,,0
d4=gas_cylinder,xvi53sb40bm/2NbG,,2,MSwwLEdhc0N5bGluZGVyLFd6RmRSa3c3V3pKZFJsSTdXek5kVWt3N1d6UmRVbEk3V3pWZFVrVlQ_,MSwwLFZhbHZlSW5mbGF0ZVBhcmFtLFd6QmR6YVBXdWJmRnh2aTZ6YlBreHZnN1d6RmRzK1RHK0R0Yk1sMjN4Y2I0
d5=power_source,tefUtL/Y1sY_,,1,NCwtMSxQb3dlcl9idXR0b24s
d6=motors,tee7+r/Y1sY_,,1,MSwwLFN3aXRjaFN0YXRlLA__
d7=pressure_limit,0bnBps/e1sY_,,3,NCwtMSxkdXQs,NCwwLGMs,NCwwLHByZXNzdXJlLA__
*/
// CODE BLOCK END Configuration

