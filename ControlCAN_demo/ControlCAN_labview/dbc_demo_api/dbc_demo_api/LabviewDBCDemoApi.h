#ifndef _LABVIEWDBCDEMOAPI_H_
#define _LABVIEWDBCDEMOAPI_H_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

/*
	˵�������ļ�������API���û�LABVIEW DBC Demoʹ�á�
		  �û����ڸù��̻����ϱ�д��չAPI���û�Labview DBC������
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
����˵����DBC��Ϣ�س�ʼ���� ���֧��DBCMessage_PoolDataSize����Ϣ
�� �� ֵ��void
*/
EXTERN_C void __stdcall LabviewDBC_MessagePoolInit();

/*
����˵������ȡDBC��Ϣ���
�� �� ֵ��LVMsgHandle
*/
EXTERN_C LVMsgHandle __stdcall LabviewDBC_MallocMessage();

/*
����˵��    ���ͷ�DBC��Ϣ���
��    ��[in]��ctx-�ص�������������
*/
EXTERN_C void __stdcall LabviewDBC_FreeMessage(LVMsgHandle hMsg);

/*
����˵����DBC��ʼ��������ȡDBC���
�� �� ֵ��INVALID_DBC_HANDLE:��ʼ��ʧ��
		  ����ֵ:��ʼ���ɹ�
		  �����ֵ������ʹ�õ���
*/
EXTERN_C DBCHandle __stdcall LabviewDBC_Init();

/*
����˵�����ͷ���Դ, ��DBC_Init���ʹ��
��    ��[in]��hDBC-���, DBC_Load�ķ���ֵ
*/
EXTERN_C void __stdcall LabviewDBC_Release(DBCHandle hDBC);

/*
����˵��������VCI_Transmit��DBC���ͻص�������
��    ��[in]��hDBC-���, DBC_Init�ķ���ֵ
��    ��[in]��ctx-�ص�������������
�� �� ֵ��void
*/
EXTERN_C void __stdcall LabviewDBC_SetSender(DBCHandle hDBC, LabviewDBCCtx* ctx);

/*
����˵��������DBC��ʽ�ļ���
��    ��[in]��hDBC-���, DBC_Init�ķ���ֵ
��    ��[in]��strPath, DBC�ļ�·��
��    ��[in]: type, DBC�ļ����ͣ�0-PROTOCOL_J1939�� 1-PROTOCOL_OTHER
��    ��[in]: merge, �Ƿ�ϲ�DBC�ļ�
�� �� ֵ��true���سɹ�
*/
EXTERN_C bool __stdcall LabviewDBC_LoadFile(DBCHandle hDBC, const char *strPath, uint8 type, uint8 merge);

/*
�� �� ˵ �� ����ȡ��һ��DBC��Ϣ���
��    ��[in]��hDBC-���, DBC_Init�ķ���ֵ
��   ��   ֵ��DBC��Ϣ������þ��ʹ����������LabviewDBC_FreeMessage�ͷš�
              ΪINVALID_LVMSGHANDLE��Ч
*/
EXTERN_C LVMsgHandle __stdcall LabviewDBC_GetFirstMessage(DBCHandle hDBC);

/*
�� �� ˵ �� ����ȡ��һ��DBC��Ϣ���
��    ��[in]��hDBC-���, DBC_Init�ķ���ֵ
��   ��   ֵ��DBC��Ϣ������þ��ʹ����������LabviewDBC_FreeMessage�ͷš�
			  ΪINVALID_LVMSGHANDLE��Ч
*/
EXTERN_C LVMsgHandle __stdcall LabviewDBC_GetNextMessage(DBCHandle hDBC);

/*
�� �� ˵ �� ������ID��ȡ��Ϣ��������Ϣ���
��    ��[in]��hDBC-���, DBC_Init�ķ���ֵ
��    ��[in]����ϢID
��   ��   ֵ��DBC��Ϣ������þ��ʹ����������LabviewDBC_FreeMessage�ͷš�
              ΪINVALID_LVMSGHANDLE��Ч��
*/
EXTERN_C LVMsgHandle __stdcall LabviewDBC_GetMessageById(DBCHandle hDBC, uint32 nID);

/*
�� �� ˵ �� ����ȡ��Ϣ��
��    ��[in]��hMsg��Ϣ���
��   ��   ֵ����Ϣ�����ַ�����ΪNULL
*/
EXTERN_C const char * __stdcall LabviewDBC_GetMessageName(LVMsgHandle hMsg);

/*
�� �� ˵ �� ����ȡ��ϢID
��    ��[in]��hMsg��Ϣ���
��   ��   ֵ����ϢID��Ϊ-1��Ч
*/
EXTERN_C unsigned int __stdcall LabviewDBC_GetMessageId(LVMsgHandle hMsg);

/*
�� �� ˵ �� ����ȡ��Ϣ�ź���
��    ��[in]��hMsg��Ϣ���
��   ��   ֵ����Ϣ�ź�����Ϊ-1��Ч
*/
EXTERN_C int __stdcall LabviewDBC_GetMessageSignalNum(LVMsgHandle hMsg);

/*
�� �� ˵ �� ����ȡ��Ϣ��nSignalIndex���źŵ��ź���
��    ��[in]��hMsg��Ϣ���
��    ��[in]: nSignalIndex���ź���������Χ[0, LabviewDBC_GetMessageSignalNum - 1]
��   ��   ֵ����Ϣ�����ַ�����ΪNULL
*/
EXTERN_C const char *__stdcall LabviewDBC_GetSignalNameByMessage(LVMsgHandle hMsg, uint32 nSignalIndex);

/*
�� �� ˵ �� ����ȡ��Ϣ��nSignalIndex���ź�ֵ
��    ��[in]��hMsg��Ϣ���
��    ��[in]: nSignalIndex���ź���������Χ[0, LabviewDBC_GetMessageSignalNum - 1]
��   ��   ֵ���ź�ֵ,����-1��Ч
*/
EXTERN_C double __stdcall LabviewDBC_GetSignalValueByMessage(LVMsgHandle hMsg, uint32 nSignalIndex);

/*
�� �� ˵ �� ��������Ϣ��nSignalIndex���źŵ�ֵ
��    ��[in]��hMsg��Ϣ���
��    ��[in]: nSignalIndex���ź���������Χ[0, LabviewDBC_GetMessageSignalNum - 1]
��    ��[in]: nValue, �ź�ֵ
��   ��   ֵ��true���óɹ���false����ʧ��
*/
EXTERN_C bool __stdcall LabviewDBC_SetSignalValueByMessage(LVMsgHandle hMsg, uint32 nSignalIndex, double nValue);

/*
�� �� ˵ �� ����ȡ��Ϣ��nSignalIndex���źŵ��ź�ֵ��λ
��    ��[in]��hMsg��Ϣ���
��    ��[in]: nSignalIndex���ź���������Χ[0, LabviewDBC_GetMessageSignalNum - 1]
��   ��   ֵ���źŵ�λ�ַ���
*/
EXTERN_C const char *__stdcall LabviewDBC_GetSignalUnitByMessage(LVMsgHandle hMsg, uint32 nSignalIndex);

/*
�� �� ˵ �� ����ȡ��Ϣ��nSignalIndex���źŵ��ź�ע��
��    ��[in]��hMsg��Ϣ���
��    ��[in]: nSignalIndex���ź���������Χ[0, LabviewDBC_GetMessageSignalNum - 1]
��   ��   ֵ���ź�ע���ַ���
*/
EXTERN_C const char *__stdcall LabviewDBC_GetSignalCommentByMessage(LVMsgHandle hMsg, uint32 nSignalIndex);

/*
�� �� ˵ �� ������CAN֡ΪDBC��Ϣ
��    ��[in]��hDBC, DBC���
��    ��[in]: pObj, CAN֡����
��   ��   ֵ���������DBC��Ϣ�����ΪINVALID_LVMSGHANDLE��Ч���þ��ʹ���������LabviewDBC_FreeMessage�ͷ�
*/
EXTERN_C LVMsgHandle __stdcall LabviewDBC_Analyse(DBCHandle hDBC, const void *pObj);

/*
�� �� ˵ �� ������DBC��Ϣ
��    ��[in]��hDBC, DBC���
��    ��[in]: hMsg��DBC��Ϣ���
��   ��   ֵ��true�����ͳɹ���false����ʧ��
*/
EXTERN_C bool __stdcall LabviewDBC_Send(DBCHandle hDBC, LVMsgHandle hMsg);

#endif // _LABVIEWDBCDEMOAPI_H_
