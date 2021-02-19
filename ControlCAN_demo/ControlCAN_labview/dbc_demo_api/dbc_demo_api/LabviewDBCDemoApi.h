#ifndef _LABVIEWDBCDEMOAPI_H_
#define _LABVIEWDBCDEMOAPI_H_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

/*
	说明：本文件所描述API仅用户LABVIEW DBC Demo使用。
		  用户可在该工程基础上编写扩展API，用户Labview DBC开发。
*/

#include "ControlCAN.h"
#include "zdbc.h"

struct LabviewDBCCtx
{
    UINT dllType;        // 0:ControlCAN, 1:zlgcan
	DWORD DeviceType;
	DWORD DeviceInd;
	DWORD CANInd;
};

typedef unsigned int LVMsgHandle;

#define INVALID_LVMSGHANDLE  (-1)
#define DBCMessage_PoolDataSize 512

/*
功能说明：DBC消息池初始化， 最多支持DBCMessage_PoolDataSize条消息
返 回 值：void
*/
EXTERN_C void __stdcall LabviewDBC_MessagePoolInit();

/*
功能说明：获取DBC消息句柄
返 回 值：LVMsgHandle
*/
EXTERN_C LVMsgHandle __stdcall LabviewDBC_MallocMessage();

/*
功能说明    ：释放DBC消息句柄
参    数[in]：ctx-回调函数的上下文
*/
EXTERN_C void __stdcall LabviewDBC_FreeMessage(LVMsgHandle hMsg);

/*
功能说明：DBC初始化，并获取DBC句柄
返 回 值：INVALID_DBC_HANDLE:初始化失败
		  其他值:初始化成功
		  保存该值作后续使用调用
*/
EXTERN_C DBCHandle __stdcall LabviewDBC_Init();

/*
功能说明：释放资源, 与DBC_Init配对使用
参    数[in]：hDBC-句柄, DBC_Load的返回值
*/
EXTERN_C void __stdcall LabviewDBC_Release(DBCHandle hDBC);

/*
功能说明：设置VCI_Transmit到DBC发送回调函数中
参    数[in]：hDBC-句柄, DBC_Init的返回值
参    数[in]：ctx-回调函数的上下文
返 回 值：void
*/
EXTERN_C void __stdcall LabviewDBC_SetSender(DBCHandle hDBC, LabviewDBCCtx* ctx);

/*
功能说明：加载DBC格式文件。
参    数[in]：hDBC-句柄, DBC_Init的返回值
参    数[in]：strPath, DBC文件路径
参    数[in]: type, DBC文件类型，0-PROTOCOL_J1939， 1-PROTOCOL_OTHER
参    数[in]: merge, 是否合并DBC文件
返 回 值：true加载成功
*/
EXTERN_C bool __stdcall LabviewDBC_LoadFile(DBCHandle hDBC, const char *strPath, uint8 type, uint8 merge);

/*
功 能 说 明 ：获取第一条DBC消息句柄
参    数[in]：hDBC-句柄, DBC_Init的返回值
返   回   值：DBC消息句柄。该句柄使用完后需调用LabviewDBC_FreeMessage释放。
              为INVALID_LVMSGHANDLE无效
*/
EXTERN_C LVMsgHandle __stdcall LabviewDBC_GetFirstMessage(DBCHandle hDBC);

/*
功 能 说 明 ：获取下一条DBC消息句柄
参    数[in]：hDBC-句柄, DBC_Init的返回值
返   回   值：DBC消息句柄。该句柄使用完后需调用LabviewDBC_FreeMessage释放。
			  为INVALID_LVMSGHANDLE无效
*/
EXTERN_C LVMsgHandle __stdcall LabviewDBC_GetNextMessage(DBCHandle hDBC);

/*
功 能 说 明 ：根据ID获取消息，返回消息句柄
参    数[in]：hDBC-句柄, DBC_Init的返回值
参    数[in]：消息ID
返   回   值：DBC消息句柄。该句柄使用完后需调用LabviewDBC_FreeMessage释放。
              为INVALID_LVMSGHANDLE无效。
*/
EXTERN_C LVMsgHandle __stdcall LabviewDBC_GetMessageById(DBCHandle hDBC, uint32 nID);

/*
功 能 说 明 ：获取消息名
参    数[in]：hMsg消息句柄
返   回   值：消息名称字符串，为NULL
*/
EXTERN_C const char * __stdcall LabviewDBC_GetMessageName(LVMsgHandle hMsg);

/*
功 能 说 明 ：获取消息ID
参    数[in]：hMsg消息句柄
返   回   值：消息ID，为-1无效
*/
EXTERN_C unsigned int __stdcall LabviewDBC_GetMessageId(LVMsgHandle hMsg);

/*
功 能 说 明 ：获取消息信号数
参    数[in]：hMsg消息句柄
返   回   值：消息信号数，为-1无效
*/
EXTERN_C int __stdcall LabviewDBC_GetMessageSignalNum(LVMsgHandle hMsg);

/*
功 能 说 明 ：获取消息第nSignalIndex个信号的信号名
参    数[in]：hMsg消息句柄
参    数[in]: nSignalIndex，信号索引，范围[0, LabviewDBC_GetMessageSignalNum - 1]
返   回   值：消息名称字符串，为NULL
*/
EXTERN_C const char *__stdcall LabviewDBC_GetSignalNameByMessage(LVMsgHandle hMsg, uint32 nSignalIndex);

/*
功 能 说 明 ：获取消息第nSignalIndex个信号值
参    数[in]：hMsg消息句柄
参    数[in]: nSignalIndex，信号索引，范围[0, LabviewDBC_GetMessageSignalNum - 1]
返   回   值：信号值,返回-1无效
*/
EXTERN_C double __stdcall LabviewDBC_GetSignalValueByMessage(LVMsgHandle hMsg, uint32 nSignalIndex);

/*
功 能 说 明 ：设置消息第nSignalIndex个信号的值
参    数[in]：hMsg消息句柄
参    数[in]: nSignalIndex，信号索引，范围[0, LabviewDBC_GetMessageSignalNum - 1]
参    数[in]: nValue, 信号值
返   回   值：true设置成功，false设置失败
*/
EXTERN_C bool __stdcall LabviewDBC_SetSignalValueByMessage(LVMsgHandle hMsg, uint32 nSignalIndex, double nValue);

/*
功 能 说 明 ：获取消息第nSignalIndex个信号的信号值单位
参    数[in]：hMsg消息句柄
参    数[in]: nSignalIndex，信号索引，范围[0, LabviewDBC_GetMessageSignalNum - 1]
返   回   值：信号单位字符串
*/
EXTERN_C const char *__stdcall LabviewDBC_GetSignalUnitByMessage(LVMsgHandle hMsg, uint32 nSignalIndex);

/*
功 能 说 明 ：获取消息第nSignalIndex个信号的信号注释
参    数[in]：hMsg消息句柄
参    数[in]: nSignalIndex，信号索引，范围[0, LabviewDBC_GetMessageSignalNum - 1]
返   回   值：信号注释字符串
*/
EXTERN_C const char *__stdcall LabviewDBC_GetSignalCommentByMessage(LVMsgHandle hMsg, uint32 nSignalIndex);

/*
功 能 说 明 ：解析CAN帧为DBC消息
参    数[in]：hDBC, DBC句柄
参    数[in]: pObj, CAN帧数据
返   回   值：解析后的DBC消息句柄，为INVALID_LVMSGHANDLE无效。该句柄使用完需调用LabviewDBC_FreeMessage释放
*/
EXTERN_C LVMsgHandle __stdcall LabviewDBC_Analyse(DBCHandle hDBC, const void *pObj);

/*
功 能 说 明 ：发送DBC消息
参    数[in]：hDBC, DBC句柄
参    数[in]: hMsg，DBC消息句柄
返   回   值：true，发送成功，false发送失败
*/
EXTERN_C bool __stdcall LabviewDBC_Send(DBCHandle hDBC, LVMsgHandle hMsg);

#endif // _LABVIEWDBCDEMOAPI_H_
