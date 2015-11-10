/********************************************************************
 File: nbRwTrans.h
 Function: newbest 转发 redwood 设备数据的接口     
 Author: Luobihe
 Date: 2015-7-1
*********************************************************************/

#ifndef __NBRWTRANS_H__
#define __NBRWTRANS_H__


#ifndef DLL_NBRWTRANS_API
#define DLL_NBRWTRANS_API 
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    
//转发器状态宏定义
#define TRANSFER_NOT_START              0    //服务未启动
#define TRANSFER_START                  1    //服务已启动
   
#define TRANSFER_ERROR_READRWDATA       2    //读 redwood 数据不成功
#define TRANSFER_ERROR_PARSEDATA        3    //解析 redwood 数据不成功
#define TRANSFER_ERROR_SENDDATA         4    //发送 redwood 数据不成功


/*-------------------------------------------------------------------
 Function: SetParameter(...)
 Purpose: 设置转发器的状态
 Parameters: szSrcIp -- [IN], redwood 设备 IP 地址
             iInterval -- [IN], 数据源 端口
             szUser -- [IN], user name
             szPw -- [IN], password
             szUdpIp -- [IN], 接收数据的udp服务器地址
             iUdpPort -- [IN], 接收数据的udp服务器监听端口
             iInterval -- [IN], 转发数据的间隔，单位为秒        
             iTransferOption -- [IN], 转发数据是所有数据或只
                       转变化的数据； 
                       0： 转所有数据； 1： 转只变化的数据
 return: -1 --  参数不合理
         0  -- 成功
-------------------------------------------------------------------*/
extern int DLL_NBRWTRANS_API SetTransferParameter(const char* szSrcIp,
                                                  int iSrcPort,
                                                  const char* szUser,
                                                  const char* szPw,
                                                  const char* szUdpIp,
                                                  int iUdpPort,
                                                  int iInterval,
                                                  int iTransferOption);

/*-------------------------------------------------------------------
 Function: StartTransferFlow()
 Purpose: 启动数据转发流程
 Parameters:
 return: 0 --  成功
         -1 -- 失败
-------------------------------------------------------------------*/
extern int DLL_NBRWTRANS_API StartTransferFlow(void);

/*-------------------------------------------------------------------
 Function: StopTransferFlow()
 Purpose: 停止数据转发流程
 Parameters: 
 return: 0 --  成功
         -1 -- 失败
-------------------------------------------------------------------*/
extern int DLL_NBRWTRANS_API StopTransferFlow(void);

/*-------------------------------------------------------------------
 Function: GetTransferStates()
 Purpose: 获取 Transfer 的状态
 Parameters: 
 return: 0 --  未启动
         1 -- 正常启动中
         2 -- 读取 redwood 数据不成功
         3 -- 解析 redwood 数据不成功
         4 -- 发送 redwood 数据不成功
-------------------------------------------------------------------*/
extern int DLL_NBRWTRANS_API GetTransferStates(void);

#ifdef __cplusplus
}
#endif

#endif  //__NBRWTRANS_H__
