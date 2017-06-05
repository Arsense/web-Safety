//=============================================================================================
/*
	XFile.h
	File operate function of ACL file

	Project	: XFILTER 1.0 Personal Firewall
	Author	: Tony Zhu
	Create Date	: 2001/08/03
	Email	: xstudio@xfilt.com
	URL		: http://www.xfilt.com

	Copyright (c) 2001-2002 XStudio Technology.
	All Rights Reserved.

	WARNNING: 
*/
//=============================================================================================
#ifndef XFILE_H
#define XFILE_H

#include "XFileRes.h"
#include "Debug.h"
#include "TypeStruct.h"

//=============================================================================================
// class of Acl file

class CAclFile
{
private:
	CRITICAL_SECTION	gCriticalSectionFile;

private:
	void	InitDefaultValue();
	int		CreateAcl	(const TCHAR *sPathName);
	int		WriteAcl	(const TCHAR *sPathName);
	int		OpenAcl		();
	int		AddAclAcl	(XACL *pAddAcl, int AddAclCount);
	int		AddDistrustIP(XACL_IP *pDistrustIP, int AddCount);
	int		AddTrustIP	(XACL_IP *pTrustIP, int AddCount);
	int		AddCustomIP	(XACL_IP *pCustomIP, int AddCount);
	int		DelAclIP	(XACL_IP *pAclIP, DWORD* pCount, DWORD iIndex);
	int		DelAcl		(XACL *pAcl, DWORD* pCount, DWORD iIndex);

public:
	CAclFile();
	virtual ~CAclFile	();
	void	CloseAcl	();
	int		ReadAcl		(BOOL IsDLL = FALSE, HINSTANCE instance = NULL);
	int		SaveAcl		();
	int		AddAcl		(void *pAddAcl, int AddAclCount, int AclType = 0);
	int		DelAcl		(int iIndex, int AclType);
	int		FreeMemory	();

public:
	static	int		GetBit		(BYTE bit, int index, int count = 1);
	static	int		SetBit		(BYTE* bit, int index, BOOL isTrue);
	static	CString	DIPToSIP	(DWORD* pIP);
	static  CString	GetAppPath	(BOOL IsDLL = FALSE, HINSTANCE instance = NULL,  BOOL IsFullPathName = FALSE);
	static	CString GetPath		(TCHAR *sFilename);
	static	CString GetName		(TCHAR *sFilename);

public:
	CFile			mAclFile;
	XACL_HEADER		mAclHeader;
	XACL_IP			mAclIntranetIP;
	XACL_TIME		mAllTime;
	XACL_TIME		mAclTime[ACL_HEADER_TIME_COUNT];
	XACL_IP			*mpAclDistrustIP;
	XACL_IP			*mpAclTrustIP;
	XACL_IP			*mpAclCustomIP;
	XACL			*mpAcl;
	CString			mAppPath;
};

#endif