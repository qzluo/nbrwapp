// 头文件 base64.h

#ifndef __BASE64_H__
#define __BASE64_H__

/* Base64 编码 */ 
char* base64_encode(const char* data, char* pDest, int data_len); 

/* Base64 解码 */ 
char *base64_decode(const char* data, char* pDest, int data_len); 

#endif