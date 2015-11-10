#include <stdio.h>
         
#ifdef _WIN32

#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")
#include <Windows.h>

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/in.h>

#endif

#define INIFILENAME  "udpserver.ini"      //ini ÎÄ¼þÃû

int main(int argc, char** argv)
{
    printf("UPD server start ...\n");
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

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        return -1;

    //prepare address
    //get ip address and port
    
    FILE *pFile = NULL;
    char szIp[64] = {0};
    int  iPort = 0;

    pFile = fopen(INIFILENAME, "rb");
    if (pFile) {
        fscanf(pFile, "%[^:]%*c%d", szIp, &iPort);
        fclose(pFile);
        pFile = NULL;
    }

    struct hostent *phostinfo = gethostbyname("");
    struct sockaddr_in srvaddr = {0};
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr = *(struct in_addr *)phostinfo->h_addr_list[0];
    srvaddr.sin_port = htons(iPort);

    ret = bind(sockfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
    if (ret == -1) 
        return -1;
    printf("bind OK\n");

    char buf[8192] = {0};

    while(1) {
        struct sockaddr_in from;
        int len = sizeof(from);
        ret = recvfrom(sockfd, buf, sizeof(buf), 0,
            (struct sockaddr*)&from, &len);

        buf[ret] = 0;

        printf("\nReceive Data:\n");
        printf(buf);                  
    }

#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif

    return 0;
}
