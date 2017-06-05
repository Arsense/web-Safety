#if !defined(CLIENT_H)
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
void memblast(void* dest,void* src,DWORD count);

#ifdef __cplusplus
extern "C" {
#endif
//客户端有关网络的代码，接收服务器端发送到本机的桌面图片以及发送到服务器的各种操作
void InitGrids();
int LoadWinsock(HWND hWnd,char *szIP);
void SetGrids(HWND hWnd);
void GetResolution();
int SendCommand(HWND hWnd,int iParm,char *szWMMessage);
DWORD Receive(SOCKET Socket,char *pData,DWORD iLength);
void UpdateRegionalScreen(HWND hWnd,SOCKET sClient,BOOL fFirstTime);
int Send(SOCKET Socket,char *szSend);
int Recv(SOCKET Socket,char *szRecv,int iRecvLen);

#ifdef __cplusplus
}
#endif

#endif