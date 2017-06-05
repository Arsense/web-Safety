//=============================================================================================
/*
	文件：		XFile.cpp

	说明:
	---------------------------------------------------
		控管规则文件操作，用来创建、读、写、删除控管规
	则文件以及其它一些辅助操作。
	---------------------------------------------------

	工程：		Xfilter 个人防火墙
	作者：		朱雁辉，朱雁冰
	创建日期：	2001/08/03
	网址：		http://www.xfilt.com
	电子邮件：	xstudio@xfilt.com
	版权所有 (c) 2001-2002 X 工作室

	警告:
	---------------------------------------------------
		本电脑程序受著作权法的保护。未经授权，不能使用
	和修改本软件全部或部分源代码。凡擅自复制、盗用或散
	布此程序或部分程序或者有其它任何越权行为，将遭到民
	事赔偿及刑事的处罚，并将依法以最高刑罚进行追诉。
	
		凡通过合法途径购买本软件源代码的用户被默认授权
	可以在自己的程序中使用本软件的部分代码，但作者不对
	代码产生的任何后果负责。
	
		使用了本软件代码的程序只能以可执行文件形式发布，
	未经特别许可，不能将含有本软件源代码的源程序以任何
	形式发布。
	---------------------------------------------------	
*/
//=============================================================================================

#include "stdafx.h"

//============================================================================================
//Acl file operation
//initialize public function

CAclFile::CAclFile()
{
	mAclFile.m_hFile 	= NULL;
	mpAclDistrustIP		= NULL;
	mpAclTrustIP		= NULL;
	mpAclCustomIP		= NULL;
	mpAcl				= NULL;
	mAllTime.bWeekDay	= 0xFF;	//11111111
	mAllTime.tStartTime	= 0;	//00:00
	mAllTime.tEndTime	= 0;	//00:00

	InitializeCriticalSection(&gCriticalSectionFile);
}

CAclFile::~CAclFile()
{
	CloseAcl();
	FreeMemory();
}

//============================================================================================
//private function

void CAclFile::InitDefaultValue()
{
	EnterCriticalSection(&gCriticalSectionFile);
	{
	// Initalize header default value
	_tcscpy(mAclHeader.sSignature	, ACL_HEADER_SIGNATURE);
	mAclHeader.ulTimeOffset			= ACL_HEADER_LENTH;
	mAclHeader.bMajor				= ACL_HEADER_MAJOR;
	mAclHeader.bMinor				= ACL_HEADER_MINOR;
	mAclHeader.uiSerial				= ACL_HEADER_SERIAL;
	mAclHeader.ulVersion			= ACL_HEADER_VERSION;
	mAclHeader.bSet					= ACL_HEADER_SET;
	mAclHeader.uiLogSize			= ACL_HEADER_LOG_SIZE;
	mAclHeader.bUpdateInterval		= ACL_HEADER_UPDATE_INTERVAL;
	_tcscpy(mAclHeader.sUserName	, ACL_HEADER_USER_NAME);
	_tcscpy(mAclHeader.sACode		, ACL_HEADER_ACODE);
	_tcscpy(mAclHeader.sWebURL		, ACL_HEADER_WEB_URL);

	CString tmpStr;
	tmpStr.Format(ACL_HEADER_COMMAND_URL
		, GUI_LANGUAGE
		, ACL_HEADER_VERSION
		, ACL_HEADER_MAJOR
		, ACL_HEADER_MINOR
		);
	_tcscpy(mAclHeader.sCommandURL	, tmpStr);

	_tcscpy(mAclHeader.sUserRegisterURL	, ACL_HEADER_REGISTER_URL);
	_tcscpy(mAclHeader.sEmail		, ACL_HEADER_EMAIL);
	mAclHeader.ulAclCount			= ACL_HEADER_INITIALIZE;
	mAclHeader.ulAclOffset			= ACL_HEADER_INITIALIZE;
	mAclHeader.ulCustomIPCount		= ACL_HEADER_INITIALIZE;
	mAclHeader.ulCustomIPOffset		= ACL_HEADER_INITIALIZE;
	mAclHeader.ulDistrustIPCount	= ACL_HEADER_INITIALIZE;
	mAclHeader.ulDistrustIPOffset	= ACL_HEADER_INITIALIZE;
	mAclHeader.ulHeaderLenth		= ACL_HEADER_LENTH;
	mAclHeader.ulIntranetIPCount	= ACL_HEADER_INTRANET_IP_COUNT;
	mAclHeader.ulIntranetIPOffset	= ACL_HEADER_LENTH + ACL_TIME_TOTAL_LENTH;
	mAclHeader.ulTimeCount			= ACL_HEADER_TIME_COUNT;
	mAclHeader.ulTrustIPCount		= ACL_HEADER_INITIALIZE;
	mAclHeader.ulTrustIPOffset		= ACL_HEADER_INITIALIZE;

	// Initalize intranet IP aria default value
	mAclIntranetIP.ulStartIP		= ACL_INTRANET_START_IP;
	mAclIntranetIP.ulEndIP			= ACL_INTRANET_END_IP;

	// Initalize time aria default value
	mAclTime[0].bWeekDay			= ACL_WORK_TIME_WEEK;
	mAclTime[0].tStartTime			= ACL_WORK_TIME_START;
	mAclTime[0].tEndTime			= ACL_WORK_TIME_END;
	mAclTime[1].bWeekDay			= ACL_NONWORK_TIME_WEEK;
	mAclTime[1].tStartTime			= ACL_NONWORK_TIME_START;
	mAclTime[1].tEndTime			= ACL_NONWORK_TIME_END;
	mAclTime[2].bWeekDay			= ACL_WEEK_END_TIME_WEEK;
	mAclTime[2].tStartTime			= ACL_WEEK_END_TIME_START;
	mAclTime[2].tEndTime			= ACL_WEEK_END_TIME_END;
	mAclTime[3].bWeekDay			= ACL_DISTRUST_TIME_WEEK;
	mAclTime[3].tStartTime			= ACL_DISTRUST_TIME_START;
	mAclTime[3].tEndTime			= ACL_DISTRUST_TIME_END;
	mAclTime[4].bWeekDay			= ACL_TRUST_TIME_WEEK;
	mAclTime[4].tStartTime			= ACL_TRUST_TIME_START;
	mAclTime[4].tEndTime			= ACL_TRUST_TIME_END;
	mAclTime[5].bWeekDay			= ACL_CUSTOM_TIME_WEEK;
	mAclTime[5].tStartTime			= ACL_CUSTOM_TIME_START;
	mAclTime[5].tEndTime			= ACL_CUSTOM_TIME_END;
	}
	LeaveCriticalSection(&gCriticalSectionFile);

	ODS(_T("XFILE: Success initalize the default value."));
}

int CAclFile::CreateAcl(const TCHAR *sPathName)
{
	InitDefaultValue();

	return WriteAcl(sPathName);
}

int CAclFile::OpenAcl()
{
	int		iRet;

	if(_taccess(mAppPath + ACL_FILE_NAME, 0) == -1)	// file not exists & create
	{
		ODS(_T("XFILE: Acl file xacl.cfg not exists."));
		if((iRet = CreateAcl(mAppPath + ACL_FILE_NAME)) != XERR_SUCCESS)
			return iRet;
	}

	TRY												// open file
	{
		mAclFile.Open(	mAppPath + ACL_FILE_NAME,
						CFile::modeRead			|
						CFile::typeBinary		| 
						CFile::shareDenyWrite
						);
		ODS(_T("XFILE: Success open the file of xacl.cfg"));
	}
	CATCH( CFileException, e )
	{
		ODS(_T("XFILE: Can't open the file of xacl.cfg"));
		return XERR_FILE_CAN_NOT_OPEN;
	}
	END_CATCH

	return XERR_SUCCESS;
}

int CAclFile::WriteAcl(const TCHAR *sPathName)
{
	CFile	FileAcl;

	TRY
	{
		FileAcl.Open(	sPathName,
						CFile::modeCreate	| 
						CFile::modeWrite	| 
						CFile::typeBinary	| 
						CFile::shareExclusive
						);

		ODS2(_T("XFILE: Success create the file "),sPathName);

		FileAcl.Write(&mAclHeader,ACL_HEADER_LENTH);
		FileAcl.Write(&mAclTime,ACL_TIME_TOTAL_LENTH);
		FileAcl.Write(&mAclIntranetIP,ACL_IP_LENTH);

		if(mAclHeader.ulDistrustIPCount > 0)
			FileAcl.WriteHuge(mpAclDistrustIP,mAclHeader.ulDistrustIPCount * ACL_IP_LENTH);

		if(mAclHeader.ulTrustIPCount > 0)
			FileAcl.WriteHuge(mpAclTrustIP,mAclHeader.ulTrustIPCount * ACL_IP_LENTH);

		if(mAclHeader.ulCustomIPCount > 0)
			FileAcl.WriteHuge(mpAclCustomIP,mAclHeader.ulCustomIPCount * ACL_IP_LENTH);

		if(mAclHeader.ulAclCount > 0)
			FileAcl.WriteHuge(mpAcl,mAclHeader.ulAclCount * ACL_ACL_LENTH);

		FileAcl.Close();

		ODS2(_T("XFILE: Success wrote the data and close "),sPathName);
	}
	CATCH( CFileException, e )
	{
		if(FileAcl.m_hFile != NULL)
		{
			FileAcl.Close();

			ODS2(_T("XFILE: Can't write "),sPathName);
		}
		else
			ODS2(_T("XFILE: Can't create "),sPathName);

		return XERR_FILE_CREATE_FAILURE;
	}
	END_CATCH

	return XERR_SUCCESS;
}

int	CAclFile::AddAclAcl(XACL *pAddAcl, int AddAclCount)
{
	if(pAddAcl == NULL || AddAclCount <= 0)
		return XERR_INVALID_PARAMETER;

	EnterCriticalSection(&gCriticalSectionFile);
	{
	void	*tmpAcl =  malloc(mAclHeader.ulAclCount * ACL_ACL_LENTH);
	memcpy	(tmpAcl,mpAcl,mAclHeader.ulAclCount * ACL_ACL_LENTH);
	delete[]	(mpAcl);

	mpAcl	= new XACL[mAclHeader.ulAclCount + AddAclCount];
	memcpy	(mpAcl,tmpAcl,mAclHeader.ulAclCount * ACL_ACL_LENTH);
	free	(tmpAcl);

	memcpy(	mpAcl + mAclHeader.ulAclCount,
			pAddAcl,
			AddAclCount * ACL_ACL_LENTH
			);

	mAclHeader.ulAclCount += AddAclCount;
	}
	LeaveCriticalSection(&gCriticalSectionFile);

	ODS2(_T("XFILE: Success Add ACL, "),mpAcl[mAclHeader.ulAclCount - 1].sApplication);

	return XERR_SUCCESS;
}

int	CAclFile::AddDistrustIP(XACL_IP *pDistrustIP, int AddCount)
{
	DWORD	OldTotalLenth = mAclHeader.ulDistrustIPCount * ACL_IP_LENTH;

	if(pDistrustIP == NULL || AddCount <= 0)
		return XERR_INVALID_PARAMETER;

	EnterCriticalSection(&gCriticalSectionFile);
	{
	void	*tmpAcl =  malloc(OldTotalLenth);
	memcpy	(tmpAcl,mpAclDistrustIP,OldTotalLenth);
	delete[]	(mpAclDistrustIP);

	mpAclDistrustIP = new XACL_IP[mAclHeader.ulDistrustIPCount + AddCount];
	memcpy	(mpAclDistrustIP,tmpAcl,OldTotalLenth);
	free	(tmpAcl);

	memcpy(	mpAclDistrustIP + mAclHeader.ulDistrustIPCount,
			pDistrustIP,
			AddCount * ACL_IP_LENTH
			);

	mAclHeader.ulDistrustIPCount += AddCount;
	}
	LeaveCriticalSection(&gCriticalSectionFile);

	ODS2(_T("XFILE: Success Add DistrustIP, "),DIPToSIP(&mpAclDistrustIP[mAclHeader.ulDistrustIPCount - 1].ulStartIP));

	return XERR_SUCCESS;
}

int	CAclFile::AddTrustIP(XACL_IP *pTrustIP, int AddCount)
{
	DWORD	OldTotalLenth = mAclHeader.ulTrustIPCount * ACL_IP_LENTH;

	if(pTrustIP == NULL || AddCount <= 0)
		return XERR_INVALID_PARAMETER;

	EnterCriticalSection(&gCriticalSectionFile);
	{
	void	*tmpAcl =  malloc(OldTotalLenth);
	memcpy	(tmpAcl,mpAclTrustIP,OldTotalLenth);
	delete[]	(mpAclTrustIP);

	mpAclTrustIP = new XACL_IP[mAclHeader.ulTrustIPCount + AddCount];
	memcpy	(mpAclTrustIP,tmpAcl,OldTotalLenth);
	free	(tmpAcl);

	memcpy(	mpAclTrustIP + mAclHeader.ulTrustIPCount,
			pTrustIP,
			AddCount * ACL_IP_LENTH
			);

	mAclHeader.ulTrustIPCount += AddCount;
	}
	LeaveCriticalSection(&gCriticalSectionFile);

	ODS2(_T("XFILE: Success Add TrustIP, "),DIPToSIP(&mpAclTrustIP[mAclHeader.ulTrustIPCount - 1].ulStartIP));

	return XERR_SUCCESS;
}

int	CAclFile::AddCustomIP(XACL_IP *pCustomIP, int AddCount)
{
	DWORD	OldTotalLenth = mAclHeader.ulCustomIPCount * ACL_IP_LENTH;

	if(pCustomIP == NULL || AddCount <= 0)
		return XERR_INVALID_PARAMETER;

	EnterCriticalSection(&gCriticalSectionFile);
	{
	void	*tmpAcl =  malloc(OldTotalLenth);
	memcpy	(tmpAcl,mpAclCustomIP,OldTotalLenth);
	delete[]	(mpAclCustomIP);

	mpAclCustomIP = new XACL_IP[mAclHeader.ulCustomIPCount + AddCount];
	memcpy	(mpAclCustomIP,tmpAcl,OldTotalLenth);
	free	(tmpAcl);

	memcpy(	mpAclCustomIP + mAclHeader.ulCustomIPCount,
			pCustomIP,
			AddCount * ACL_IP_LENTH
			);

	mAclHeader.ulCustomIPCount += AddCount;
	}
	LeaveCriticalSection(&gCriticalSectionFile);

	ODS2(_T("XFILE: Success Add CustomIP, "),
		DIPToSIP(&mpAclCustomIP[mAclHeader.ulCustomIPCount - 1].ulStartIP));

	return XERR_SUCCESS;
}

int	CAclFile::DelAclIP(XACL_IP *pAclIP, DWORD* pCount, DWORD iIndex)
{
	if(pAclIP == NULL || iIndex >= *pCount)
		return XERR_INVALID_PARAMETER;

	EnterCriticalSection(&gCriticalSectionFile);
	{
	memcpy(pAclIP + iIndex, pAclIP + iIndex + 1, (*pCount - iIndex) * ACL_IP_LENTH);
	*pCount = *pCount - 1 ;
	}
	LeaveCriticalSection(&gCriticalSectionFile);

	ODS(_T("XFILE: Success Del IP aria. "));

	return XERR_SUCCESS;
}

int	CAclFile::DelAcl(XACL *pAcl, DWORD* pCount, DWORD iIndex)
{
	if(pAcl == NULL || iIndex >= *pCount)
		return XERR_INVALID_PARAMETER;

	EnterCriticalSection(&gCriticalSectionFile);
	{
	memcpy(pAcl + iIndex, pAcl + iIndex + 1, (*pCount - iIndex) * ACL_ACL_LENTH);
	*pCount = *pCount - 1 ;
	}
	LeaveCriticalSection(&gCriticalSectionFile);

	ODS(_T("XFILE: Success Del ACL. "));

	return XERR_SUCCESS;
}

//============================================================================================
//Export function

int CAclFile::ReadAcl(BOOL IsDLL, HINSTANCE instance)	//main function
{
	int		iRet	= XERR_SUCCESS;

	if(IsDLL)
		mAppPath		= CAclFile::GetAppPath(IsDLL, instance);
	else
		mAppPath		= CAclFile::GetAppPath();

	if(mAclFile.m_hFile == NULL && (iRet = OpenAcl()) != XERR_SUCCESS)
		return iRet;

	TRY
	{
		FreeMemory();

		mAclFile.SeekToBegin();
		mAclFile.Read(&mAclHeader,ACL_HEADER_LENTH);	//read the header

		if(_tcscmp(mAclHeader.sSignature , ACL_HEADER_SIGNATURE) != 0)
		{
			ODS(_T("XFILE: Invalid signatrue, It's not the XFILTER ACL file."));
			CloseAcl();
			return XERR_FILE_INVALID_SIGNATURE;
		}

		mAclFile.Read(&mAclTime,ACL_TIME_TOTAL_LENTH);	//read the time aria
		mAclFile.Read(&mAclIntranetIP,ACL_IP_LENTH);	//read the intranet ip aria

		if(mAclHeader.ulDistrustIPCount > 0)			//read the distrust ip aria
		{
			mpAclDistrustIP	= new XACL_IP[mAclHeader.ulDistrustIPCount];
			mAclFile.ReadHuge(mpAclDistrustIP,ACL_IP_LENTH * mAclHeader.ulDistrustIPCount);
		}

		if(mAclHeader.ulTrustIPCount	> 0)			//read the trust ip aria
		{
			mpAclTrustIP	= new XACL_IP[mAclHeader.ulTrustIPCount];
			mAclFile.ReadHuge(mpAclTrustIP,ACL_IP_LENTH * mAclHeader.ulTrustIPCount);
		}

		if(mAclHeader.ulCustomIPCount	> 0)			//read the custom ip aria
		{
			mpAclCustomIP	= new XACL_IP[mAclHeader.ulCustomIPCount];
			mAclFile.ReadHuge(mpAclCustomIP,ACL_IP_LENTH * mAclHeader.ulCustomIPCount);
		}

		if(mAclHeader.ulAclCount		> 0)			//read the ACL
		{
			mpAcl			= new XACL[mAclHeader.ulAclCount];
			mAclFile.ReadHuge(mpAcl,ACL_ACL_LENTH * mAclHeader.ulAclCount);
		}
		
		CloseAcl();

		ODS(_T("XFILE: Read xacl.cfg success."));
	}
	CATCH( CFileException, e )
	{
		CloseAcl();

		ODS(_T("XFILE: Read xacl.cfg error."));
		return XERR_FILE_READ_ERROR;
	}
	END_CATCH

	return iRet;
}

int	CAclFile::SaveAcl()
{
	int		iRet;

	if((iRet = WriteAcl(mAppPath + ACL_TEMP_FILE_NAME)) != XERR_SUCCESS)
		return iRet;

	TRY
	{
		CFile::Remove(mAppPath + ACL_FILE_NAME);

		CFile::Rename(mAppPath + ACL_TEMP_FILE_NAME,mAppPath + ACL_FILE_NAME);

		ODS(_T("XFILE: Success remove xacl.cfg and rename xacl.tmp to xacl.cfg"));
	}
	CATCH( CFileException, e )
	{
		ODS(_T("XFILE: Save error."));
		return XERR_FILE_SAVE_ERROR;
	}
	END_CATCH

	return XERR_SUCCESS;
}

void CAclFile::CloseAcl()
{
	if(mAclFile.m_hFile != NULL)
	{
		mAclFile.Close();
		mAclFile.m_hFile = NULL;
	}

	ODS(_T("XFILE: Success close the acl file."));
}

int	CAclFile::AddAcl(void *pAddAcl, int AddAclCount, int AclType)
{
	switch(AclType)
	{
	case ACL_TYPE_ACL:
		return AddAclAcl((XACL*)pAddAcl,AddAclCount);

	case ACL_TYPE_DISTRUST_IP:
		return AddDistrustIP((XACL_IP*)pAddAcl,AddAclCount);

	case ACL_TYPE_TRUST_IP:
		return AddTrustIP((XACL_IP*)pAddAcl,AddAclCount);

	case ACL_TYPE_CUSTOM_IP:
		return AddCustomIP((XACL_IP*)pAddAcl,AddAclCount);

	default:
		return XERR_INVALID_PARAMETER;
	}

	return XERR_SUCCESS;
}

int	CAclFile::DelAcl(int iIndex, int AclType)
{
	switch(AclType)
	{
	case ACL_TYPE_ACL:
		return DelAcl(mpAcl, &mAclHeader.ulAclCount, iIndex);

	case ACL_TYPE_DISTRUST_IP:
		return DelAclIP(mpAclDistrustIP, &mAclHeader.ulDistrustIPCount, iIndex);

	case ACL_TYPE_TRUST_IP:
		return DelAclIP(mpAclTrustIP, &mAclHeader.ulTrustIPCount, iIndex);

	case ACL_TYPE_CUSTOM_IP:
		return DelAclIP(mpAclCustomIP, &mAclHeader.ulCustomIPCount, iIndex);

	default:
		return XERR_INVALID_PARAMETER;
	}

	return XERR_SUCCESS;
}

int	CAclFile::FreeMemory()
{
	if(mpAcl)
	{
		delete[](mpAcl);
		mpAcl				= NULL;
	}

	if(mpAclDistrustIP)
	{
		delete[](mpAclDistrustIP);
		mpAclDistrustIP		= NULL;
	}

	if(mpAclTrustIP)
	{
		delete[](mpAclTrustIP);
		mpAclTrustIP		= NULL;
	}

	if(mpAclCustomIP)
	{
		delete[](mpAclCustomIP);
		mpAclCustomIP		= NULL;
	}

	ODS(_T("XFILE: Success delete[] the ACL and IP aria memory."));

	return XERR_SUCCESS;
}

CString CAclFile::DIPToSIP(DWORD* pIP)
{
	if(pIP == NULL)
		return _T("");

	CString	s;
	BYTE	*b = (BYTE*)pIP;
	s.Format(_T("%d.%d.%d.%d"),b[3],b[2],b[1],b[0]);

	return s;
}

/*---------------------------------------------------------------------------------------------
	index from 0 start, for example:
	index:				0 1 2 3 4 5 6 7
	Binary value:		0 0 0 0 0 0 0 0
*/
int	CAclFile::GetBit(BYTE bit, int index, int count)
{
	bit <<= index;
	bit >>= (8 - count);

	return bit;
}

int CAclFile::SetBit(BYTE* bit, int index, BOOL isTrue)
{
	BYTE bOr = 0xFF,bAnd = 0x00;

	bOr <<= index;
	bOr >>= 7;
	bOr <<= (7 - index);
	bAnd = ~bOr;

	if(isTrue)
		*bit = *bit | bOr;
	else
		*bit = *bit & bAnd;

	return 0;
}

//============================================================================================
//static function

CString CAclFile::GetAppPath(BOOL IsDLL, HINSTANCE instance, BOOL IsFullPathName) 
{
	TCHAR sFilename[_MAX_PATH];
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];

	if(IsDLL)
		GetModuleFileName(instance, sFilename, _MAX_PATH);
	else
		GetModuleFileName(AfxGetInstanceHandle(), sFilename, _MAX_PATH);

	if(IsFullPathName)
		return sFilename;

	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

	CString rVal(CString(sDrive) + CString(sDir));
	int nLen = rVal.GetLength();

	if (rVal.GetAt(nLen-1) != _T('\\'))
		rVal += _T("\\");

	return rVal;
}

CString CAclFile::GetPath(TCHAR *sFilename) 
{
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];

	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

	CString rVal(CString(sDrive) + CString(sDir));
	int nLen = rVal.GetLength();

	if (rVal.GetAt(nLen-1) != _T('\\'))
		rVal += _T("\\");

	return rVal;
}  

CString CAclFile::GetName(TCHAR *sFilename) 
{
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];

	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

	CString rVal;
	rVal.Format(_T("%s%s"), sFname, sExt);

	return rVal;
}  