/********************************************************************
 File: nbDataSender.cpp
 Function: Send data to listener by UDP.     
 Author: Luobihe
 Date: 2015-6-29
*********************************************************************/

#include <stdio.h>
#include "nbDataSender.h"   
#include "publicinclude.h"

CNbDataSender::CNbDataSender(void)
{
    strcpy(szIpAddr, "127.0.0.1");
    iPort = 2222;
}

int CNbDataSender::setPeerAddr(const char* szIpAddr, int iPort)
{
    if (szIpAddr == NULL)
        return -1;

    strcpy(this->szIpAddr, szIpAddr);
    this->iPort = iPort;

    return 0;
}

int CNbDataSender::sendData(const char* szBuf)
{                   
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
        
    //1 socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1) 
        return -1;

    //2 通信地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;//与socket一致
    addr.sin_port = htons(iPort);//改端口
    addr.sin_addr.s_addr = inet_addr(szIpAddr);//改IP

    sendto(sockfd, szBuf, strlen(szBuf), 0,
        (struct sockaddr*)&addr, sizeof(addr));   
    
#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
             
    return 0;
}