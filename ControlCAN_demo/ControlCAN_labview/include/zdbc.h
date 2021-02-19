#ifndef _ZDBC_H_
#define _ZDBC_H_
#include "IDBCManager.h"

#define FT_CAN   0 //CAN֡
#define FT_CANFD 1 //CANFD֡

/*
����˵��������CANFD֡�Ļص�
��    ��[in]��ctx-�ص�������������
��    ��[in]��pObj-Ҫ���͵�����
�� �� ֵ��true���ͳɹ�
*/
typedef bool (__stdcall *OnSendCANFD)(void* ctx, void* pObj);

/*
����˵������ʼ������ģ�顣
�� �� ֵ��INVALID_DBC_HANDLE:��ʼ��ʧ��
		  ����ֵ:��ʼ���ɹ�
		  �����ֵ������ʹ�õ���
*/
EXTERN_C DBCHandle __stdcall ZDBC_Init();

/*
����˵�����ͷ���Դ, ��ZDBC_Init���ʹ��
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
*/
EXTERN_C void __stdcall ZDBC_Release( DBCHandle hDBC );

/*
����˵��������DBC��ʽ�ļ���
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
��    ��[in]��pFileInfo-�ļ���Ϣ, �ο�struct FileInfo
�� �� ֵ��true���سɹ�
*/
EXTERN_C bool __stdcall ZDBC_LoadFile( DBCHandle hDBC, const FileInfo* pFileInfo );

/*
����˵������ȡ��һ����Ϣ��
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
��    ��[out]��pMsg �洢��Ϣ����Ϣ
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C bool __stdcall ZDBC_GetFirstMessage( DBCHandle hDBC, DBCMessage* pMsg );

/*
����˵������ȡ��һ����Ϣ��
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
��    ��[out]��pMsg �洢��Ϣ����Ϣ
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C bool __stdcall ZDBC_GetNextMessage( DBCHandle hDBC, DBCMessage* pMsg );

/*
����˵��������ID��ȡ��Ϣ��
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
��    ��[in]��nID-��Ϣ��ID
��    ��[out]��pMsg �洢��Ϣ����Ϣ
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C bool __stdcall ZDBC_GetMessageById( DBCHandle hDBC, uint32 nID, DBCMessage* pMsg );
/*
����˵������ȡ��Ϣ������
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
�� �� ֵ����Ϣ����
*/
EXTERN_C uint32 __stdcall ZDBC_GetMessageCount( DBCHandle hDBC );

/*
����˵��������DBCЭ�����֡����
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
��    ��[in]��pObj ԭʼ֡����
��    ��[in]��frame_type ֡���� �ο�FT_CAN FT_CANFD
��    ��[out]��pMsg �������
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C bool __stdcall ZDBC_Analyse( DBCHandle hDBC, const void* pObj, uint8 frame_type, DBCMessage* pMsg );

/*
����˵������������Ҫ���øú����ѽ��յ���֡���ݴ�����,
		  �漰��֡�������Ҫ��������, �����޷�ʵ���豸������
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
��    ��[in]��pObj ԭʼ֡����
��    ��[in]��frame_type ֡���� �ο�FT_CAN FT_CANFD
�� �� ֵ��void
*/
EXTERN_C void __stdcall ZDBC_OnReceive( DBCHandle hDBC, const void* pObj, uint8 frame_type );

/*
����˵��������ʵ�ʷ������ݵĴ�����, �漰���ݷ���ʱ�������ã�ֻ��Ҫ����һ�Ρ�
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
��    ��[in]��sender�ص�����ָ��
��    ��[in]��ctx-�ص�������������
�� �� ֵ��void
*/
EXTERN_C void __stdcall ZDBC_SetSender( DBCHandle hDBC, OnSend sender, void* ctx );
EXTERN_C void __stdcall ZDBC_SetCANFDSender( DBCHandle hDBC, OnSendCANFD sender, void* ctx );
/*
����˵�������ô����֡�������ݵĻص�������ֻ��Ҫ����һ��
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
��    ��[in]��OnMultiTransDone�ص�����ָ��
��    ��[in]��ctx-�ص�������������
�� �� ֵ��void
*/
EXTERN_C void __stdcall ZDBC_SetOnMultiTransDoneFunc( DBCHandle hDBC, OnMultiTransDone func, void* ctx);
/*
����˵��������DBC��Ϣ��
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
��    ��[in]��pMsg Ҫ���͵���Ϣ
��    ��[in]��frame_type ʹ��CAN����CANFD֡���� �ο�FT_CAN FT_CANFD
�� �� ֵ���ο�//error code
*/
EXTERN_C ERR_CODE __stdcall ZDBC_Send( DBCHandle hDBC, const DBCMessage* pMsg, uint8 frame_type );

/*
����˵������ȡ�����źŵ�ֵ�뺬��Ը�����
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
��    ��[in]��msg_id, ��ϢID
��    ��[in]��signal_name, �ź���
�� �� ֵ��ֵ�뺬��Ը���
*/
EXTERN_C uint32 __stdcall ZDBC_GetValDescPairCount( DBCHandle hDBC, uint32 msg_id, char* signal_name);
/*
����˵������ȡֵ�뺬��ԡ�
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
��    ��[in]��msg_id, ��ϢID
��    ��[in]��signal_name, �ź���
��    ��[in,out]��pair �洢ֵ�뺬���,�����߹���
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C void __stdcall ZDBC_GetValDescPair( DBCHandle hDBC, uint32 msg_id, char* signal_name, ValDescPair* pair);
/*
����˵��������ԭʼ���ݽ���ΪDBCMessage, Ŀǰ��֧��canfd֡
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
��    ��[out]��pMsg �������
��    ��[in]��pObj ԭʼ֡����
��    ��[in]��nCount ԭʼ֡���ݸ���
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C bool __stdcall ZDBC_Decode( DBCHandle hDBC, DBCMessage* pMsg, const void* pObj, size_t nCount );
/*
����˵��������DBCMessage����Ϊԭʼ����, Ŀǰ��֧��canfd֡
��    ��[in]��hDBC-���, ZDBC_Load�ķ���ֵ
��    ��[out]��pObj �����ԭʼ���ݻ�����
��    ��[in,out]��pObj ��������С, ����ʱΪʵ��ԭʼ���ݸ���
��    ��[in]��pMsg Ҫ�������Ϣ
�� �� ֵ��true��ʾ�ɹ�
*/
EXTERN_C bool __stdcall ZDBC_Encode( DBCHandle hDBC, void* pObj, size_t* nCount, const DBCMessage* pMsg );

#endif //_ZDBC_H_