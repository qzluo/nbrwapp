/********************************************************************
 File: nbDataSender.h
 Function: Send data to listener by UDP.     
 Author: Luobihe
 Date: 2015-6-29
*********************************************************************/

#ifndef __NBDATASENDER_H__
#define __NBDATASENDER_H__

class CNbDataSender {
private:
    char szIpAddr[16];
    int  iPort;
public:
    CNbDataSender(void);
    int setPeerAddr(const char* szIpAddr, int iPort);
    int sendData(const char* szBuf);
};

#endif  //__NBDATASENDER_H__