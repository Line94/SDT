// dbc_demo_api.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "LabviewDBCDemoApi.h"
//#include <fstream>
//#include <iostream>
#include <string>
#include "zlgcan.h"
#include "canhandle.h"

struct DBCMessage_PoolData {
	DBCMessage    msg;
	uint8         is_used;
};

static struct DBCMessage_Pool {
	DBCMessage_PoolData data[DBCMessage_PoolDataSize];
	int is_init;
} __g_dbcmsg_pool;

// 处理帧发送
static bool __stdcall __OnSendFunc(void* ctx, void* pObj)
{
	LabviewDBCCtx* info = static_cast<LabviewDBCCtx*>(ctx);

    if (info->dllType == 0) {
        // ControlCAN
        VCI_CAN_OBJ*   obj = static_cast<VCI_CAN_OBJ*>(pObj);
        if (VCI_Transmit(info->DeviceType, info->DeviceInd, info->CANInd, obj, 1)) {
            return true;
        }
    }
    else if (info->dllType == 1) {
        // zlgcan
        can_frame* cc_frame = static_cast<can_frame*>(pObj);
        ZCAN_Transmit_Data data;
        data.frame = *cc_frame;
        data.transmit_type = 0;
        CAN_HANDLE hCan(info->DeviceType, info->DeviceInd, info->CANInd);
        if (ZCAN_Transmit((CHANNEL_HANDLE)hCan.handle, &data, 1) == STATUS_OK) {
            return true;
        }
    }

	return false;
}

static DBCMessage *__GetMessageByMsgHandle(LVMsgHandle hMsg)
{
	if (hMsg < 0 || hMsg >= DBCMessage_PoolDataSize) {
		return NULL;
	}

	return &__g_dbcmsg_pool.data[hMsg].msg;
}

extern "C" void __stdcall LabviewDBC_MessagePoolInit()
{
	for (int i = 0; i < DBCMessage_PoolDataSize; i++) {
		__g_dbcmsg_pool.data[i].is_used = 0;
	}

	__g_dbcmsg_pool.is_init = 1;
}

extern "C"  LVMsgHandle __stdcall LabviewDBC_MallocMessage()
{
	if (__g_dbcmsg_pool.is_init != 1) {
		return -1;
	}

	//std::unique_lock<std::mutex> lock;
	for (int i = 0; i < DBCMessage_PoolDataSize; i++) {
		if (!__g_dbcmsg_pool.data[i].is_used) {
			__g_dbcmsg_pool.data[i].is_used = 1;
			memset(&__g_dbcmsg_pool.data[i], 0, sizeof(DBCMessage_PoolData));
			return i;
		}
	}

	return -1;
}

extern "C"  void __stdcall LabviewDBC_FreeMessage(LVMsgHandle hMsg)
{
	if ((hMsg >= DBCMessage_PoolDataSize) || (hMsg < 0)) {
		return;
	}

	if (__g_dbcmsg_pool.data[hMsg].is_used) {
		__g_dbcmsg_pool.data[hMsg].is_used = 0;
	}
}

extern "C" DBCHandle __stdcall LabviewDBC_Init()
{
	return ZDBC_Init();
}

extern "C" void __stdcall LabviewDBC_Release(DBCHandle hDBC)
{
	ZDBC_Release(hDBC);
}

extern "C" void __stdcall LabviewDBC_SetSender(DBCHandle hDBC, LabviewDBCCtx* ctx)
{
	ZDBC_SetSender(hDBC, __OnSendFunc, (void *)ctx);
}

extern "C" bool __stdcall LabviewDBC_LoadFile(DBCHandle hDBC, const char *strPath, uint8 type, uint8 merge)
{
	FileInfo info;
	strcpy_s(info.strFilePath, _MAX_FILE_PATH_, strPath);
	info.type = type;
	info.merge = merge;
	return ZDBC_LoadFile(hDBC, &info);
}

extern "C" LVMsgHandle __stdcall LabviewDBC_GetFirstMessage(DBCHandle hDBC)
{
	LVMsgHandle hMsg = LabviewDBC_MallocMessage();
	if (hMsg < 0) {
		return INVALID_LVMSGHANDLE;
	}

	DBCMessage *msg = &__g_dbcmsg_pool.data[hMsg].msg;
	if (ZDBC_GetFirstMessage(hDBC, msg)) {	
		return hMsg;
	}
	LabviewDBC_FreeMessage(hMsg);
	return INVALID_LVMSGHANDLE;
}


extern "C" LVMsgHandle __stdcall LabviewDBC_GetNextMessage(DBCHandle hDBC)
{
	LVMsgHandle hMsg = LabviewDBC_MallocMessage();
	if (hMsg < 0) {
		return INVALID_LVMSGHANDLE;
	}

	DBCMessage *msg = &__g_dbcmsg_pool.data[hMsg].msg;
	if (ZDBC_GetNextMessage(hDBC, msg)) {
		return hMsg;
	}
	LabviewDBC_FreeMessage(hMsg);
	return INVALID_LVMSGHANDLE;
}

extern "C" LVMsgHandle __stdcall LabviewDBC_GetMessageById(DBCHandle hDBC, uint32 nID)
{
	LVMsgHandle hMsg = LabviewDBC_MallocMessage();
	if (hMsg < 0) {
		return INVALID_LVMSGHANDLE;
	}

	DBCMessage *msg = &__g_dbcmsg_pool.data[hMsg].msg;
	if (ZDBC_GetMessageById(hDBC, nID, msg)) {
		return hMsg;
	}
	LabviewDBC_FreeMessage(hMsg);
	return INVALID_LVMSGHANDLE;
}

extern "C" const char * __stdcall LabviewDBC_GetMessageName(LVMsgHandle hMsg)
{
	DBCMessage *msg = __GetMessageByMsgHandle(hMsg);
	if (!msg) {
		return NULL;
	}

	return msg->strName;
}

extern "C" unsigned int __stdcall LabviewDBC_GetMessageId(LVMsgHandle hMsg)
{
	DBCMessage *msg = __GetMessageByMsgHandle(hMsg);
	if (!msg) {
		return -1;
	}

	return msg->nID;
}

extern "C" int __stdcall LabviewDBC_GetMessageSignalNum(LVMsgHandle hMsg)
{
	DBCMessage *msg = __GetMessageByMsgHandle(hMsg);
	if (!msg) {
		return -1;
	}

	return msg->nSignalCount;
}

static DBCSignal *__GetDBCSignal(DBCHandle hDBC, uint32 nID, uint32 nSignalIndex)
{
	DBCMessage msg;
	if (ZDBC_GetMessageById(hDBC, nID, &msg)) {
		if (msg.nSignalCount >= nSignalIndex) {
			return NULL;
		}
		return &msg.vSignals[nSignalIndex];
	}

	return NULL;
}

extern "C" const char *__stdcall LabviewDBC_GetSignalNameByMessage(LVMsgHandle hMsg, uint32 nSignalIndex)
{
	DBCMessage *msg = __GetMessageByMsgHandle(hMsg);
	if (!msg) {
		return NULL;
	}

	if (nSignalIndex >= msg->nSignalCount) {
		return NULL;
	}

	return msg->vSignals[nSignalIndex].strName;
}

extern "C" const char *__stdcall LabviewDBC_GetSignalUnitByMessage(LVMsgHandle hMsg, uint32 nSignalIndex)
{
	DBCMessage *msg = __GetMessageByMsgHandle(hMsg);
	if (!msg) {
		return NULL;
	}

	if (nSignalIndex >= msg->nSignalCount) {
		return NULL;
	}

	return msg->vSignals[nSignalIndex].unit;
}

extern "C" double __stdcall LabviewDBC_GetSignalValueByMessage(LVMsgHandle hMsg, uint32 nSignalIndex)
{
	DBCMessage *msg = __GetMessageByMsgHandle(hMsg);
	if (!msg) {
		return -1;
	}

	if (nSignalIndex >= msg->nSignalCount) {
		return -1;
	}

	return msg->vSignals[nSignalIndex].nValue;
}

extern "C" bool __stdcall LabviewDBC_SetSignalValueByMessage(LVMsgHandle hMsg, uint32 nSignalIndex, double nValue)
{
	DBCMessage *msg = __GetMessageByMsgHandle(hMsg);
	if (!msg) {
		return false;
	}

	if (nSignalIndex >= msg->nSignalCount) {
		return false;
	}

	msg->vSignals[nSignalIndex].nValue = nValue;
	return true;
}

extern "C" const char *__stdcall LabviewDBC_GetSignalCommentByMessage(LVMsgHandle hMsg, uint32 nSignalIndex)
{
	DBCMessage *msg = __GetMessageByMsgHandle(hMsg);
	if (!msg) {
		return NULL;
	}

	if (nSignalIndex >= msg->nSignalCount) {
		return NULL;
	}

	return msg->vSignals[nSignalIndex].strComment;
}

extern "C" LVMsgHandle __stdcall LabviewDBC_Analyse(DBCHandle hDBC, const void *pObj)
{
	LVMsgHandle hMsg = LabviewDBC_MallocMessage();
	if (hMsg < 0) {
		return INVALID_LVMSGHANDLE;
	}

	DBCMessage *msg = &__g_dbcmsg_pool.data[hMsg].msg;
    if (ZDBC_Analyse(hDBC, pObj, FT_CAN, msg) == true) { //TODO CANFD
		return hMsg;
	}
	LabviewDBC_FreeMessage(hMsg);
	return INVALID_LVMSGHANDLE;
}

extern "C" bool __stdcall LabviewDBC_Send(DBCHandle hDBC, LVMsgHandle hMsg)
{
	const DBCMessage *msg = __GetMessageByMsgHandle(hMsg);
	if (!msg) {
		return false;
	}
	return DBC_Send(hDBC, msg);
}


