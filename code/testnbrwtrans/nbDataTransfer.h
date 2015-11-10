/********************************************************************
 File: nbDataTransfer.h
 Function: Provide class to ger data from device and transfer it to
           set address.     
 Author: Luobihe
 Date: 2015-6-29
*********************************************************************/

#ifndef __NBDATATRANSFER_H__
#define __NBDATATRANSFER_H__

#include "redWoodHandle.h"
#include "nbDataSender.h" 
#include "redWoodDataParse.h"

#include <Windows.h>

#ifdef __cplusplus
extern "C"
{
#endif

//定时器参数
#define TRANSFER_TIMER_RESOLUTION       1          //转发数据的定时器分辨率
#define TRANSFER_DELAY                  2000       //数据转发的时间间隔

//转发的数据规模
#define TRANSFER_ALL_DATA               0          //转发所有数据
#define TRANSFER_CHANGED_DATA           1          //转发与上次发送数据不同的数据


//转发器状态宏定义
#define TRANSFER_NOT_START              0
#define TRANSFER_START                  1
   
#define TRANSFER_ERROR_READRWDATA       2    //读 redwood 数据不成功
#define TRANSFER_ERROR_PARSEDATA        3    //解析 redwood 数据不成功
#define TRANSFER_ERROR_SENDDATA         4    //发送 redwood 数据不成功


class CNbDataTransfer {
private:
    CRedWoodHandle rwhandler;
    CNbDataSender  nbDataSender;
    CRedWoodDataParse rwDataParse;

    int iTimerID;      //定时器编号
    int iTimeDelay;    //定时间隔
    int iState;        //转发器状态
    int iTransOption;  //转发的数据规模

private:
public:
    //进行一次测试及数据转发
    int oneOp(void);

    //释放资源
    void Free(void);

    //定时器回调函数
    static void CALLBACK CallBackFunc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);

public:
    CNbDataTransfer(void);
    ~CNbDataTransfer(void);

    //设置参数
    int SetParameter(const char* szSrcIp, int iSrcPort, 
        const char* szUser, const char* szPw, 
        const char* szUdpIp, int iUdpPort, 
        int iInterval, int iTransferOption);

    //启动转发流程
    int StartTransferFlow(void);

    //停止转发流程
    int StopTransferFlow(void);

    //获取状态,返回的状态为上面定义的值之一
    int GetTransferStates(void);
};

#ifdef __cplusplus
}
#endif

#endif  //__NBDATATRANSFER_H__