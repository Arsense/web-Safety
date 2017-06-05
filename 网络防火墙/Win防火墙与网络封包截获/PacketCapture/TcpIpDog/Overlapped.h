#ifndef OVERLAPPED_H
#define OVERLAPPED_H

#include <afxtempl.h>

//
// 保存重叠操作参数信息的结构
//
typedef struct _OVERLAPPED_RECORDER
{
	SOCKET			s;
	LPWSABUF		lpBuffers;
	DWORD			dwBufferCount;
	LPDWORD			lpNumberOfBytesRecvd;
	LPDWORD			lpFlags;
	LPWSAOVERLAPPED lpOverlapped;
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine;
	struct sockaddr FAR * lpFrom;
	LPINT			lpFromlen;
	int				FunctionType; //0: WSPRecv; 1:WSPRecvFrom
} OVERLAPPED_RECORDER, *POVERLAPPED_RECORDER;

//
// 重叠操作类
//
class COverlapped
{
public:
	COverlapped();

	int FindOverlapped(LPWSAOVERLAPPED lpOverlapped);
	BOOL DeleteOverlapped(int iIndex);
	BOOL AddOverlapped(
		SOCKET			s,
		LPWSABUF		lpBuffers,
		DWORD			dwBufferCount,
		LPDWORD			lpNumberOfBytesRecvd,
		LPDWORD			lpFlags,
		LPWSAOVERLAPPED lpOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
		struct sockaddr FAR * lpFrom,
		LPINT			lpFromlen,	 
		int				FunctionType //0: WSPRecv; 1:WSPRecvFrom
		);

public:
	//
	// 所有没有完成的重叠操作数组
	//
	CArray<OVERLAPPED_RECORDER, OVERLAPPED_RECORDER> m_OverlappedRecorder;

private:
	CRITICAL_SECTION m_CriticalSection;
};

#endif //OVERLAPPED_H