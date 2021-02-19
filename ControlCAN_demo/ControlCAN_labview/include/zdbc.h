#ifndef _ZDBC_H_
#define _ZDBC_H_
#include "IDBCManager.h"

#define FT_CAN   0 //CAN帧
#define FT_CANFD 1 //CANFD帧

/*
功能说明：发送CANFD帧的回调
参    数[in]：ctx-回调函数的上下文
参    数[in]：pObj-要发送的数据
返 回 值：true发送成功
*/
typedef bool (__stdcall *OnSendCANFD)(void* ctx, void* pObj);

/*
功能说明：初始化解析模块。
返 回 值：INVALID_DBC_HANDLE:初始化失败
		  其他值:初始化成功
		  保存该值作后续使用调用
*/
EXTERN_C DBCHandle __stdcall ZDBC_Init();

/*
功能说明：释放资源, 与ZDBC_Init配对使用
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
*/
EXTERN_C void __stdcall ZDBC_Release( DBCHandle hDBC );

/*
功能说明：加载DBC格式文件。
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
参    数[in]：pFileInfo-文件信息, 参考struct FileInfo
返 回 值：true加载成功
*/
EXTERN_C bool __stdcall ZDBC_LoadFile( DBCHandle hDBC, const FileInfo* pFileInfo );

/*
功能说明：获取第一条消息。
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
参    数[out]：pMsg 存储消息的信息
返 回 值：true表示成功
*/
EXTERN_C bool __stdcall ZDBC_GetFirstMessage( DBCHandle hDBC, DBCMessage* pMsg );

/*
功能说明：获取下一条消息。
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
参    数[out]：pMsg 存储消息的信息
返 回 值：true表示成功
*/
EXTERN_C bool __stdcall ZDBC_GetNextMessage( DBCHandle hDBC, DBCMessage* pMsg );

/*
功能说明：根据ID获取消息。
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
参    数[in]：nID-消息的ID
参    数[out]：pMsg 存储消息的信息
返 回 值：true表示成功
*/
EXTERN_C bool __stdcall ZDBC_GetMessageById( DBCHandle hDBC, uint32 nID, DBCMessage* pMsg );
/*
功能说明：获取消息个数。
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
返 回 值：消息个数
*/
EXTERN_C uint32 __stdcall ZDBC_GetMessageCount( DBCHandle hDBC );

/*
功能说明：根据DBC协议解析帧数据
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
参    数[in]：pObj 原始帧数据
参    数[in]：frame_type 帧类型 参考FT_CAN FT_CANFD
参    数[out]：pMsg 解析结果
返 回 值：true表示成功
*/
EXTERN_C bool __stdcall ZDBC_Analyse( DBCHandle hDBC, const void* pObj, uint8 frame_type, DBCMessage* pMsg );

/*
功能说明：调用者需要调用该函数把接收到的帧数据传进来,
		  涉及多帧传输必须要这样调用, 否则无法实现设备交互。
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
参    数[in]：pObj 原始帧数据
参    数[in]：frame_type 帧类型 参考FT_CAN FT_CANFD
返 回 值：void
*/
EXTERN_C void __stdcall ZDBC_OnReceive( DBCHandle hDBC, const void* pObj, uint8 frame_type );

/*
功能说明：设置实际发送数据的处理者, 涉及数据发送时必须设置，只需要设置一次。
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
参    数[in]：sender回调函数指针
参    数[in]：ctx-回调函数的上下文
返 回 值：void
*/
EXTERN_C void __stdcall ZDBC_SetSender( DBCHandle hDBC, OnSend sender, void* ctx );
EXTERN_C void __stdcall ZDBC_SetCANFDSender( DBCHandle hDBC, OnSendCANFD sender, void* ctx );
/*
功能说明：设置处理多帧传输数据的回调函数，只需要设置一次
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
参    数[in]：OnMultiTransDone回调函数指针
参    数[in]：ctx-回调函数的上下文
返 回 值：void
*/
EXTERN_C void __stdcall ZDBC_SetOnMultiTransDoneFunc( DBCHandle hDBC, OnMultiTransDone func, void* ctx);
/*
功能说明：发送DBC消息。
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
参    数[in]：pMsg 要发送的消息
参    数[in]：frame_type 使用CAN或者CANFD帧发送 参考FT_CAN FT_CANFD
返 回 值：参考//error code
*/
EXTERN_C ERR_CODE __stdcall ZDBC_Send( DBCHandle hDBC, const DBCMessage* pMsg, uint8 frame_type );

/*
功能说明：获取具体信号的值与含义对个数。
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
参    数[in]：msg_id, 消息ID
参    数[in]：signal_name, 信号名
返 回 值：值与含义对个数
*/
EXTERN_C uint32 __stdcall ZDBC_GetValDescPairCount( DBCHandle hDBC, uint32 msg_id, char* signal_name);
/*
功能说明：获取值与含义对。
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
参    数[in]：msg_id, 消息ID
参    数[in]：signal_name, 信号名
参    数[in,out]：pair 存储值与含义对,调用者构造
返 回 值：true表示成功
*/
EXTERN_C void __stdcall ZDBC_GetValDescPair( DBCHandle hDBC, uint32 msg_id, char* signal_name, ValDescPair* pair);
/*
功能说明：根据原始数据解码为DBCMessage, 目前不支持canfd帧
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
参    数[out]：pMsg 解析结果
参    数[in]：pObj 原始帧数据
参    数[in]：nCount 原始帧数据个数
返 回 值：true表示成功
*/
EXTERN_C bool __stdcall ZDBC_Decode( DBCHandle hDBC, DBCMessage* pMsg, const void* pObj, size_t nCount );
/*
功能说明：根据DBCMessage编码为原始数据, 目前不支持canfd帧
参    数[in]：hDBC-句柄, ZDBC_Load的返回值
参    数[out]：pObj 编码的原始数据缓冲区
参    数[in,out]：pObj 缓冲区大小, 返回时为实际原始数据个数
参    数[in]：pMsg 要编码的消息
返 回 值：true表示成功
*/
EXTERN_C bool __stdcall ZDBC_Encode( DBCHandle hDBC, void* pObj, size_t* nCount, const DBCMessage* pMsg );

#endif //_ZDBC_H_