/********************************************************************
 File: nbRwTrans.cpp
 Function: newbest 转发 redwood 设备数据的接口     
 Author: Luobihe
 Date: 2015-7-1
*********************************************************************/

#define DLL_NBRWTRANS_API __declspec(dllexport)

#include "nbRwTrans.h"
#include "nbDataTransfer.h"

static CNbDataTransfer s_DataTranser;   //操作对象

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
                                                  int iTransferOption)
{
    return s_DataTranser.SetParameter(szSrcIp, iSrcPort, 
        szUser, szPw, szUdpIp, iUdpPort,
        iInterval, iTransferOption);
}

/*-------------------------------------------------------------------
 Function: StartTransferFlow()
 Purpose: 启动数据转发流程
 Parameters:
 return: 0 --  成功
         -1 -- 失败
-------------------------------------------------------------------*/
int StartTransferFlow(void)
{
    return s_DataTranser.StartTransferFlow();
}

/*-------------------------------------------------------------------
 Function: StopTransferFlow()
 Purpose: 停止数据转发流程
 Parameters: 
 return: 0 --  成功
         -1 -- 失败
-------------------------------------------------------------------*/
int StopTransferFlow(void)
{
    return s_DataTranser.StopTransferFlow();
}

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
int DLL_NBRWTRANS_API GetTransferStates(void)
{
    return s_DataTranser.GetTransferStates();
}