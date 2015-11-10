/********************************************************************
 File: httpResHandle.h
 Function: 处理 http 响应数据
 Author: Luobihe
 Date: 2015-6-26
********************************************************************/

#ifndef __HTTPRESHANDLE_H__
#define __HTTPRESHANDLE_H__

#define HTTPREADBUFF (1024*4)

class CHttpResHandler {         
public:
    //读取http响应的头部
    static int readHttpResHead(int sockfd, char* pBuffer);

    //读Tcp数据头，得到后续需读取的长度
    static int readTcpDataLen(int sockfd);

    //读http响应数据体
    static int readHttpResBody(int sockfd, char* pBuffer);

};

#endif  //  __HTTPRESHANDLE_H__