// dac_demo_api_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../dbc_demo_api/LabviewDBCDemoApi.h"
#include <process.h>
#include <string.h>
#include "../../include/zdbc.h"

static bool __stdcall OnFrameSent(void* ctx, void* data)
{
    return true;
}

static bool __stdcall OnCANFDFrameSent(void* ctx, void* data)
{
    return true;
}

static void __stdcall OnMultiTransDoneFunc(void* ctx, DBCMessage* pMsg, uint8* data, uint16 nLen, uint8 nDirection)
{

}

int _tmain(int argc, _TCHAR* argv[])
{
    char * filepath = "E:\\Project\\LabVIEW\\zlgcan\\ZlgCan_Demo\\dbc_demo\\dbc_files\\j1939.dbc";

#if 1
    DBCHandle hDbc = LabviewDBC_Init();
    if (hDbc == INVALID_DBC_HANDLE) {
        printf("Init dbc failed! \n");
        return 0;
    }

    LabviewDBC_MessagePoolInit();

    if (LabviewDBC_LoadFile(hDbc, filepath, 1, 0)) {
        LVMsgHandle msg = LabviewDBC_GetFirstMessage(hDbc);
        while (msg != INVALID_LVMSGHANDLE) {
            printf("Msg: %s \n", LabviewDBC_GetMessageName(msg));
            msg = LabviewDBC_GetNextMessage(hDbc);
        }
    }
    else {
        printf("Load dbc file failed! \n");
    }
    LabviewDBC_Release(hDbc);
#else
    DBCHandle hdl = ZDBC_Init();
    if (hdl != INVALID_DBC_HANDLE){
        //ZDBC_SetSender(hdl, OnFrameSent, NULL);
        //ZDBC_SetCANFDSender(hdl, OnCANFDFrameSent, NULL);
        //ZDBC_SetOnMultiTransDoneFunc(hdl, OnMultiTransDoneFunc, NULL);

        FileInfo fi;
        strcpy_s(fi.strFilePath, filepath);
        fi.merge = 0;
        fi.type = 0;
        if (ZDBC_LoadFile(hdl, &fi)) {
            printf("Message count = %d \n", DBC_GetMessageCount(hdl));
            DBCMessage msg;
            if (ZDBC_GetFirstMessage(hdl, &msg)) {
                printf("Msg: %s \n", msg.strName);
                while (ZDBC_GetNextMessage(hdl, &msg)) {
                    printf("Msg: %s \n", msg.strName);
                }
            }
        }

        ZDBC_Release(hdl);
    }
#endif

    system("pause");
    return 0;
}

