/********************************************************************
 File: redWoodHandle.cpp
 Function: Provide class to read data from redwood device.
           Parse the read json data.
           Save the read json data.
           Transfer the data to target.     
 Author: Luobihe
 Date: 2015-6-26
*********************************************************************/

#include <string.h>
#include <stdio.h>
#include "redWoodHandle.h"
#include "httpResHandle.h"

#include "publicinclude.h"
#include "base64.h"

CRedWoodHandle::CRedWoodHandle()
{
    strncpy(szIpAddr, "127.0.0.1", sizeof(szIpAddr));
    iPort = 80;
    memset(szDataBuf, 0, REDWOOD_DATA_LEN);

    strncpy(szUser, "admin", sizeof(szUser));
    strncpy(szPw, "redwood", sizeof(szPw));
}

void CRedWoodHandle::setNetAddr(char* szIpAddr, int iPort)
{
    strncpy(this->szIpAddr, szIpAddr, sizeof(this->szIpAddr));
    this->iPort = iPort;
}

void CRedWoodHandle::setUserAndPw(const char* pUser, const char* pPw)
{
    strncpy(szUser, pUser, sizeof(szUser));
    strncpy(szPw, pPw, sizeof(szPw));
}

int CRedWoodHandle::readRedWoodData()
{
    int sockfd = 0;
    int ret = 0;

#ifdef _WIN32

    WORD wVersionRequested = 0;
    WSADATA wsaData = {0};    
    wVersionRequested = MAKEWORD(1, 1);
    ret = WSAStartup(wVersionRequested, &wsaData);
    if ( ret != 0 )
        return -1;

    // check winsock version
    if ( LOBYTE( wsaData.wVersion ) != 1 ||
        HIBYTE( wsaData.wVersion ) != 1 ) {
            WSACleanup( );
            return -1;
    }

#endif
        
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        return -1;

    //prepare address
    struct sockaddr_in srvaddr = {0};
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.S_un.S_addr = inet_addr(szIpAddr);
    srvaddr.sin_port = htons(iPort);

    ret = connect(sockfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
    if (ret == -1) 
        return -1;

    //http communictate: send request
    char szRequest[1024] = {0};
    char szUserAndPw[256] = {0};
    char szEncodeUserAndPw[256] = {0};

    sprintf(szUserAndPw, "%s:%s", szUser, szPw);
    base64_encode(szUserAndPw, szEncodeUserAndPw, 
        strlen(szUserAndPw)); 

    sprintf(szRequest, "GET /rApi HTTP/1.1\r\n"
        "connection: close\r\n"
        "Authorization: Basic %s\r\n"
        "Host: %s\r\n\r\n",
        szEncodeUserAndPw, szIpAddr);

    ret = send(sockfd, szRequest, strlen(szRequest), 0);
    if (ret < 0) 
        return -1;

    Sleep(100);

    //read response
    memset(szDataBuf, 0, REDWOOD_DATA_LEN);
    if (CHttpResHandler::readHttpResBody(sockfd, szDataBuf) < 0) 
        return -1;

#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
             
    return 0;
}

const char* CRedWoodHandle::getRedWoodData()
{
    return szDataBuf;
} 
