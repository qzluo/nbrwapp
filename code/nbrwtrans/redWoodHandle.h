/********************************************************************
 File: redWoodHandle.h
 Function: Provide class to read data from redwood device.
           Parse the read json data.
           Save the read json data.
           Transfer the data to target.     
 Author: Luobihe
 Date: 2015-6-26
*********************************************************************/

#ifndef __REDWOODHANDLE_H__
#define __REDWOODHANDLE_H__

#define REDWOOD_DATA_LEN       (8*1024)   //数据缓冲区大小

class CRedWoodHandle {
private:
    char szIpAddr[16];
    int  iPort;         
    char szUser[64];
    char szPw[64];

    char szDataBuf[REDWOOD_DATA_LEN];

public:
    CRedWoodHandle(void);
    void setNetAddr(char* szIpAddr, int iPort);
    void setUserAndPw(const char* pUser, const char* pPw);
    int readRedWoodData(void);
    const char* getRedWoodData(void);
};

#endif //__REDWOODHANDLE_H__