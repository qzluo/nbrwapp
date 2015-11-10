/********************************************************************
 File: publicinclude.h
 Function: include file need by the project.     
 Author: Luobihe
 Date: 2015-6-29
*********************************************************************/

#ifndef __PUBLICINCLUDE_H__
#define __PUBLICINCLUDE_H__

#ifdef _WIN32

#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")
#include <Windows.h>
#include <WinDef.h>

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/in.h>

#endif

#endif //__PUBLICINCLUDE_H__