///////////////////////////////////////////////////////////////////////
// If this program works, it was written by Alexander Fedorov.
// If not, I don't know who wrote it.
// mailto:lamer2000@hotmail.com
// Home Page: http://members.xoom.com/lamer2000/
// This file is part of Alexf Dialer.
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
///////////////////////////////////////////////////////////////////////

// AlexfPerf.cpp: implementation of the CAlexfPerf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AlexfPerf.h"
#include <WINPERF.H>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CAlexfPerf95 - class to receive performance stats under '95-98
//////////////////////////////////////////////////////////////////////

CAlexfPerf95::CAlexfPerf95(LPCTSTR pszObjName, LPCTSTR pszCounterName, LPCTSTR pszComputerName)
{
	m_bSuccess = FALSE;
	//Open up the HKEY_DYN_DATA key on the specified machine
	HKEY hKeyDynData;
	DWORD dwConnect;
	if (pszComputerName)
	{
		TCHAR pszComputer[_MAX_PATH];
		_tcscpy(pszComputer, pszComputerName);
		dwConnect = RegConnectRegistry(pszComputer, HKEY_DYN_DATA, &hKeyDynData);
	}
	else
		dwConnect = RegConnectRegistry(NULL, HKEY_DYN_DATA, &hKeyDynData);
	if (dwConnect != ERROR_SUCCESS) return;
	//Open the "HKEY_DYN_DATA\PerfStats\StartStat" registry key
	HKEY hOpen;
	if (RegOpenKeyEx(hKeyDynData, _T("PerfStats\\StartStat"), 0, KEY_READ, &hOpen) != ERROR_SUCCESS)
	{RegCloseKey(hKeyDynData); return;}
	//Query to get data size
	DWORD dwType;
	DWORD cbData;
	//Form the counter name which we are querying
	CString sCounterName;
	sCounterName.Format(_T("%s\\%s"), pszObjName, pszCounterName);
	if (ERROR_SUCCESS == RegQueryValueEx(hOpen, sCounterName, NULL, &dwType, NULL, &cbData))
	{
		//Query the performance start key to initialise the performace data
		BYTE* pByte = new BYTE[cbData];
		if (pByte)
		{
			m_bSuccess = (ERROR_SUCCESS == RegQueryValueEx(hOpen, sCounterName, NULL, &dwType, pByte, &cbData));
			//Remember the name of the computer we want
			m_sComputerName = pszComputerName;
			//Remember the name of the counter we want
			m_sCounterName = sCounterName;
			//Don't forget to delete the bit of memory we allocated
			delete [] pByte;
		}
	}
	//Don't forget to close the registry key
	RegCloseKey(hOpen);
	//Don't forget to close the registry key
	RegCloseKey(hKeyDynData);
}

CAlexfPerf95::~CAlexfPerf95()
{
	m_bSuccess = FALSE;
	//Handle calling StopCollecting twice
	if (m_sCounterName.IsEmpty()) return;
	//Open up the HKEY_DYN_DATA key on the specified machine
	HKEY hKeyDynData, hOpen;
	LPTSTR pszComputerName = m_sComputerName.GetBuffer(m_sComputerName.GetLength());
	DWORD dwConnect = RegConnectRegistry(pszComputerName, HKEY_DYN_DATA, &hKeyDynData);
	m_sComputerName.ReleaseBuffer();
	if (dwConnect != ERROR_SUCCESS) return;
	//Open the "HKEY_DYN_DATA\PerfStats\StopStat" registry key
	if (RegOpenKeyEx(hKeyDynData, _T("PerfStats\\StopStat"), 0, KEY_READ, &hOpen) != ERROR_SUCCESS)
	{RegCloseKey(hKeyDynData); return;}
	//Query to get data size
	DWORD dwType;
	DWORD cbData;
	if (RegQueryValueEx(hOpen, m_sCounterName, NULL, &dwType, NULL, &cbData) != ERROR_SUCCESS)
	{RegCloseKey(hKeyDynData); RegCloseKey(hOpen); return;}
	//Query the performance stop key to disable collection of performace data
	BYTE* pByte = new BYTE[cbData];
	if (pByte)
	{
		RegQueryValueEx(hOpen, m_sCounterName, NULL, &dwType, pByte, &cbData);
		delete [] pByte;
	}
	//Don't forget to close the registry key
	RegCloseKey(hOpen);
	RegCloseKey(hKeyDynData);
	m_sCounterName.Empty();
}

//接收数据统计
BOOL CAlexfPerf95::Collect(DWORD& dwData)
{
	if (!m_bSuccess) return FALSE;
	m_bSuccess = FALSE;
	ASSERT(m_sCounterName.GetLength()); //did you forget to call Start
	//打开注册表项HKEY_DYN_DATA
	HKEY hKeyDynData, hOpen;
	LPTSTR pszComputerName = m_sComputerName.GetBuffer(m_sComputerName.GetLength());
	DWORD dwConnect = RegConnectRegistry(pszComputerName, HKEY_DYN_DATA, &hKeyDynData);
	m_sComputerName.ReleaseBuffer();
	if (dwConnect != ERROR_SUCCESS) return FALSE;
	//打开注册表"HKEY_DYN_DATA\PerfStats\StatData"
	if (RegOpenKeyEx(hKeyDynData, _T("PerfStats\\StatData"), 0, KEY_READ, &hOpen) != ERROR_SUCCESS)
	//关闭注册表
	{RegCloseKey(hKeyDynData); return FALSE;}
	//获得数据
	DWORD dwType = REG_BINARY;
	DWORD cbData = 4;
	m_bSuccess = TRUE;

	if (RegQueryValueEx(hOpen, m_sCounterName, NULL, &dwType,
	(LPBYTE) &dwData, &cbData) != ERROR_SUCCESS) m_bSuccess = FALSE;
	RegCloseKey(hOpen);
	RegCloseKey(hKeyDynData);
	return m_bSuccess;
}

DWORD CAlexfPerf95::GetData()
{
	DWORD dw = 0;
	if (!m_bSuccess) return dw;
	Collect(dw);
	return dw;
}

//////////////////////////////////////////////////////////////////////
// CAlexfPerfNT - class to receive performance stats under NT
//////////////////////////////////////////////////////////////////////

#define Key "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib\\009"

CAlexfPerfNT::	CAlexfPerfNT(LPCTSTR pszObjName, LPCTSTR pszCounterName, LPCTSTR pszComputerName)
{
	m_bSuccess = FALSE;
	BOOL b = GetIndex(pszObjName, m_SObj.GetBuffer(512));
	m_SObj.ReleaseBuffer();
	if (!b) return;
	if (m_SObj.IsEmpty()) return;
	b = GetIndex(pszCounterName, m_sCounter.GetBuffer(512));
	m_sCounter.ReleaseBuffer();
	if (!b) return;
	if (m_sCounter.IsEmpty()) return;
	m_bSuccess = TRUE;
}

CAlexfPerfNT::~CAlexfPerfNT()
{

}

BOOL CAlexfPerfNT::Collect(DWORD& dwData)
{
	if (!m_bSuccess) return FALSE;
	DWORD dwBytes = 12000;
	DWORD dwCounterIdOffset = 0;
	PPERF_DATA_BLOCK pdb;
	PPERF_OBJECT_TYPE pot;
	PPERF_COUNTER_DEFINITION pcd;
	PPERF_INSTANCE_DEFINITION pid;
	// 为PPERF_DATA_BLOCK申请空间.
	pdb = (PPERF_DATA_BLOCK) HeapAlloc(GetProcessHeap(),
	HEAP_ZERO_MEMORY, dwBytes);
	// Get performance data.
	//RegQueryValueEx获得特定注册表项的的键值以及类型,函数结构如下：
	/*LONG RegQueryValueEx(
	HKEY hKey,            // handle to key
	LPCTSTR lpValueName,  // 值名称
	LPDWORD lpReserved,   // 保留
	LPDWORD lpType,       // 类型
	LPBYTE lpData,        // 数据
	LPDWORD lpcbData      // 数据缓冲大小
	);*/

	while(ERROR_MORE_DATA == RegQueryValueEx(HKEY_PERFORMANCE_DATA, m_SObj, NULL,
	NULL, (LPBYTE)pdb, &dwBytes))
	{
		// 增加缓冲.
		dwBytes += 1000;
		// 重新申请
		pdb = (PPERF_DATA_BLOCK) HeapReAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY, (LPVOID)pdb, dwBytes);
	}
	//获得对象类型PERF_OBJECT_TYPE.
	pot = (PPERF_OBJECT_TYPE)((PBYTE)pdb + pdb->HeaderLength);
	// 获得第一个计数器类型.
	pcd = (PPERF_COUNTER_DEFINITION)((PBYTE)pot + pot->HeaderLength);
	for(int i=0; i< (int)pot->NumCounters; i++)
	{
		if (pcd->CounterNameTitleIndex == (DWORD)atoi(m_sCounter))
		{
			dwCounterIdOffset = pcd->CounterOffset;
			break;
		}
		pcd = ((PPERF_COUNTER_DEFINITION)((PBYTE)pcd + pcd->ByteLength));
	}
	// 获得该对象的第一个实例
	pid = (PPERF_INSTANCE_DEFINITION)((PBYTE)pot + pot->DefinitionLength);
	// 获得信息
	dwData = *((DWORD *) ((PBYTE)pid + dwCounterIdOffset));
	// 释放空间
	if(!HeapFree(GetProcessHeap(), 0, (LPVOID)pdb)) m_bSuccess = FALSE;
	// 关闭注册表
	RegCloseKey(HKEY_PERFORMANCE_DATA);
	return TRUE;
}

BOOL CAlexfPerfNT::GetIndex(const char * pszCounter, char * szIndex)
{
	char*  pszBuffer;
	char*  pszTemp;
	char   szObject[256] = "";
	DWORD  dwBytes;
	HKEY hKeyIndex;
	int i = 0;
	int j = 0;
	// 打开注册表
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, Key, 0, KEY_READ, &hKeyIndex);
	//获得Counter的大小
	RegQueryValueEx(hKeyIndex, "Counter", NULL, NULL, NULL, &dwBytes);
	// 申请空间
	pszBuffer = (char *) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytes);
	if (!pszBuffer) {RegCloseKey(hKeyIndex); return FALSE;}
	// 获得标题和计数器
	RegQueryValueEx(hKeyIndex, "Counter", NULL, NULL, (LPBYTE)pszBuffer, &dwBytes);
	// 获得索引值
	pszTemp = pszBuffer;
	while (i != (int)dwBytes)
	{
		while (*(pszTemp+i) != '\0')
		{
			szIndex[j] = *(pszTemp+i);
			i++;
			j++;
		}
		szIndex[j] = '\0';
		i++;
		j = 0;
		while (*(pszTemp+i) != '\0')
		{
			szObject[j] = *(pszTemp+i);
			i++;
			j++;
		}
		szObject[j] = '\0';
		i++;
		j = 0;
		if ('\0' == *(pszTemp+i))
		i++;
		if (0 == strcmp(szObject, pszCounter)) break;
	}
	// 释放空间
	HeapFree(GetProcessHeap(), 0, (LPVOID)pszBuffer);
	// 关闭注册表
	RegCloseKey(hKeyIndex);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// CAlexfPerf - class to receive performance stats
//////////////////////////////////////////////////////////////////////

CAlexfPerf::CAlexfPerf(int iPerfType)
{
	m_p95 = NULL;
	m_pNT = NULL;
	m_bSuccess = FALSE;
	m_iPerfType = iPerfType;
	// check os version
	OSVERSIONINFO OSVersionInfo;
	OSVersionInfo.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	GetVersionEx(&OSVersionInfo);
	if (VER_PLATFORM_WIN32_NT == OSVersionInfo.dwPlatformId)
	{
		m_bSuccess = bInitNT(iPerfType);
	}
    else
	{
		m_bSuccess = bInit95(iPerfType);
	}
}

CAlexfPerf::~CAlexfPerf()
{
	if (m_p95) delete m_p95;
	if (m_pNT) delete m_pNT;
}

BOOL CAlexfPerf::bInit95(int iPerfType)
{
	CString sAdapterName;
	CString sss;
	for (int i = 0; i < 10; i++)
	{
		sprintf(sss.GetBuffer(512), "%04ld", i); sss.ReleaseBuffer();
		sAdapterName = GetRegKey(HKEY_LOCAL_MACHINE,
		"System\\CurrentControlSet\\Services\\Class\\Net\\" + sss,
		"AdapterName");
		if (sAdapterName == "MS$PPP") break;
	}
	if (sAdapterName != "MS$PPP") return FALSE;
	sAdapterName = GetRegKey(HKEY_LOCAL_MACHINE,
	"System\\CurrentControlSet\\Services\\Class\\Net\\" + sss,
	"DriverDesc");
	switch (iPerfType)
	{
	case ALEXF_PERF_DIALUP_CONNECTSPEED:
		m_p95 = new CAlexfPerf95(sAdapterName, "ConnectSpeed"); break;
	case ALEXF_PERF_DIALUP_BYTESRECVD:
		m_p95 = new CAlexfPerf95(sAdapterName, "BytesRecvd"); break;
	case ALEXF_PERF_DIALUP_BYTESXMIT:
		m_p95 = new CAlexfPerf95(sAdapterName, "BytesXmit"); break;
	default:
		return FALSE;
	}
	return m_p95->IsOK();
}

BOOL CAlexfPerf::bInitNT(int iPerfType)
{
	switch (iPerfType)
	{
	case ALEXF_PERF_DIALUP_CONNECTSPEED:
		m_pNT = new CAlexfPerfNT("RAS Total", "Total Connections"); break;
	case ALEXF_PERF_DIALUP_BYTESRECVD:
		m_pNT = new CAlexfPerfNT("RAS Total", "Bytes Received"); break;
	case ALEXF_PERF_DIALUP_BYTESXMIT:
		m_pNT = new CAlexfPerfNT("RAS Total", "Bytes Transmitted"); break;
	default:
		return FALSE;
	}
	return m_pNT->IsOK();
}

BOOL CAlexfPerf::Collect(DWORD& dwData)
{
	if (!m_bSuccess) return FALSE;
	if (m_p95) m_bSuccess = m_p95->Collect(dwData);
	if (m_pNT)
	{
		// temporary!!!
		if (ALEXF_PERF_DIALUP_CONNECTSPEED == m_iPerfType)
		dwData = 33600;
		else
		m_bSuccess = m_pNT->Collect(dwData);
	}
	return m_bSuccess;
}

DWORD CAlexfPerf::GetData()
{
	DWORD dw = 0;
	if (!m_bSuccess) return dw;
	Collect(dw);
	return dw;
}

CString CAlexfPerf::GetRegKey(HKEY hOpen, CString key1,CString key2)
{
	CString sss;
	HKEY hkey;
	LONG l = RegOpenKeyEx(hOpen, key1, 0, KEY_READ, &hkey);
	if (l != ERROR_SUCCESS) return sss;
	ULONG LS = _MAX_PATH;
	DWORD lpType = REG_EXPAND_SZ;
	l = RegQueryValueEx(hkey, key2, NULL,
	& lpType, (unsigned char *) sss.GetBuffer(LS), & LS);
	sss.ReleaseBuffer();
	RegCloseKey(hkey);
	if (l != ERROR_SUCCESS) sss.Empty();
	return sss;
}
