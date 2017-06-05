
/*	
	Document.cpp : implementation of the CSnaggerDoc class
	
	Implements project file persistence for options, tree data and statistics.
	Also performs the actual retrieval of files from the host using the 
	CInet class.

	Author: Steven E. Sipe
*/

#include "stdafx.h"

#include "SiteSnag.h"
#include "Document.h"
#include "View.h"
#include "progress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// File version for serialization
const long g_lFileVersion = 0x0101;

/////////////////////////////////////////////////////////////////////////////
// CSnaggerDoc

IMPLEMENT_DYNCREATE(CSnaggerDoc, CDocument)

BEGIN_MESSAGE_MAP(CSnaggerDoc, CDocument)
	//{{AFX_MSG_MAP(CSnaggerDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSnaggerDoc construction/destruction

// Constructor
CSnaggerDoc::CSnaggerDoc()
{
	// Set some default project options
	m_Options.nMaxDepth = 2;
	m_Options.nMaxPages = 0;
	m_Options.bFixupLinks = TRUE;
	m_Options.bContents = TRUE;
	m_Options.bMultimedia = TRUE;
	m_Options.bOffsiteLinks = FALSE;


	// Set the initial hash table sizes
	m_arrPagesDone.InitHashTable(1200);
	m_arrMediaDone.InitHashTable(2400);

	// Initialize some flags
	m_bProjectLoaded = FALSE;
	m_pProgress = NULL;
	m_bAutoMode = FALSE;

	m_nLevel = 0;
}

// Destructor
CSnaggerDoc::~CSnaggerDoc()
{
	try
	{
		// Remove the page and media maps
		ClearCacheMaps();
	}
	catch(...)
	{
	}
}

//建立一个新文档
BOOL CSnaggerDoc::OnNewDocument()
{

	static bFirstTime = TRUE;

	// Is is this the empty project file?
	if(bFirstTime)
	{
		bFirstTime = FALSE;

		// Yes, set the title to "(No project)"
		CString strDefName;
		strDefName.LoadString(IDS_NO_PROJECT);
		SetTitle(strDefName);

		// Call the base class and get out...
		if (!CDocument::OnNewDocument())
			return FALSE;

		return(TRUE);
	}

	// Call the base class
	if (!CDocument::OnNewDocument())
		return FALSE;

	// Clear the statisitics and indicate that we now have a 
	// project loaded
	m_bProjectLoaded = TRUE;
	m_strStartPage.Empty();
	m_nGottenPageCount = 0;
	m_nGottenFileCount = 0;
	m_nQueuedPageCount = 0;
	m_nTotalBytes = 0;

	// Make sure that the info in the statistics window is reset
	POSITION pos = GetFirstViewPosition();
	CSnaggerView* pView = (CSnaggerView *) GetNextView(pos);
	m_pProgress = pView->GetProgress();
	m_pProgress->SetActionTitle("");

	return TRUE;
	
}

// 打开一个已经保存的文档
BOOL CSnaggerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	POSITION pos = GetFirstViewPosition();
	CSnaggerView* pView = (CSnaggerView *) GetNextView(pos);

	// Save the current project (if necessary)
	SaveModified();

	// Initialize the tree
	pView->ClearTree();

	// Call the base class to read the document's contents from disk
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// Set the project location
	SetPathName(lpszPathName);
	m_strDirectory = CInet::SplitFileName(lpszPathName,
				CInet::DRIVE|CInet::PATH|CInet::FNAME)+"\\";

	// Set the document's title
	SetTitle(CInet::SplitFileName(lpszPathName,CInet::FNAME|CInet::EXT));


	// Indicate that the project is loaded
	m_bProjectLoaded = TRUE;

	// Update the project's information in the statistics window
	if(m_pProgress)
	{
		m_pProgress = pView->GetProgress();
		m_pProgress->SetActionTitle("");
		UpdateStatus();
	}

	// Make sure this document gets saved at the end
	SetModifiedFlag(TRUE);

	return TRUE;
}

// Saves files that have been change (DoFileSave() calls the document's 
// ::Serialize() method
BOOL CSnaggerDoc::SaveModified() 
{
	// Was the document changed??
	if(IsModified())
		return CDocument::DoFileSave();
	return(TRUE);
}

// Prevents the user from exiting the application if a snagging operation is
// in progress
BOOL CSnaggerDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	POSITION pos = GetFirstViewPosition();
	CSnaggerView* pView = (CSnaggerView *) GetNextView(pos);

	return(!pView->GetSnagging());
}

// 载入一个空的工程
void CSnaggerDoc::Reset(LPCTSTR lpszProjName)
{
	CString strNewProjName;
		
	if(lpszProjName)
		strNewProjName = lpszProjName;

	// Use the default name -- (No Project)
	strNewProjName.LoadString(IDS_NO_PROJECT);
	m_strPathName.Empty();
	m_strDirectory.Empty();
	m_bProjectLoaded = FALSE;
	SetModifiedFlag(FALSE);
	SetTitle(strNewProjName);

	// Reset the statistics window's information
	m_strStartPage.Empty();
	m_nGottenPageCount = 0;
	m_nGottenFileCount = 0;
	m_nQueuedPageCount = 0;
	m_nTotalBytes = 0;

	// Update the statistics window
	POSITION pos = GetFirstViewPosition();
	CSnaggerView* pView = (CSnaggerView *) GetNextView(pos);
	m_pProgress = pView->GetProgress();
	m_pProgress->SetActionTitle("");

	UpdateStatus();
}

// Handles closing the document -- i.e. the user pressed the X button or chose the
// close menu item
void CSnaggerDoc::OnCloseDocument() 
{
	// Make sure that we save the current document
	SaveModified();

	CDocument::OnCloseDocument();
}

// 获得工程选项
void CSnaggerDoc::GetOptions(CSnagOptions& Options)
{
	Options.nMaxDepth = m_Options.nMaxDepth;
	Options.nMaxPages = m_Options.nMaxPages;
	Options.bFixupLinks = m_Options.bFixupLinks;
	Options.bContents = m_Options.bContents;
	Options.bMultimedia = m_Options.bMultimedia;
	Options.bOffsiteLinks = m_Options.bOffsiteLinks;
}

//设置工程选项,当通过命令行来执行的时候调用
void CSnaggerDoc::SetOptions(CSnagOptions& Options) 
{ 
	m_Options.nMaxDepth = Options.nMaxDepth;
	m_Options.nMaxPages = Options.nMaxPages;
	m_Options.bFixupLinks = Options.bFixupLinks;
	m_Options.bContents = Options.bContents;
	m_Options.bMultimedia = Options.bMultimedia;
	m_Options.bOffsiteLinks = Options.bOffsiteLinks;
}

//文件序列化
void CSnaggerDoc::Serialize(CArchive& ar)
{
	long lFileVersion;
	POSITION pos = GetFirstViewPosition();
	CSnaggerView* pView = (CSnaggerView *) GetNextView(pos);	

	// 是否要写入硬盘
	if (ar.IsStoring())
	{
		// 写入版本信息
		ar << g_lFileVersion;
		lFileVersion = g_lFileVersion;

		// 写入参数窗口的信息
		ar << m_nGottenPageCount;
		ar << m_nGottenFileCount;
		ar << m_nTotalBytes;
	}
	else
	{
		// 获得信息
		ar >> lFileVersion;

		// 读入参数信息
		ar >> m_nGottenPageCount;
		ar >> m_nGottenFileCount;
		ar >> m_nTotalBytes;

		// Reset the queued page count (used in the statistics window)
		m_nQueuedPageCount = 0;
	}

	// 获得或者保存工程信息
	m_Options.SetVersion(lFileVersion);
	m_Options.Serialize(ar);

	// 获得或者保存树形控件的内容
	pView->SerializeTree(ar);

	m_nLevel = 0;
}

//下载指定的页面，通过CInet或者从硬盘上现在
//如果通过CInet下载，则保存程文件名strFileName，同时分析该文件中的链接
BOOL CSnaggerDoc::GetPage(CString& strPage, CString& strFileName, LINKS& linkEntry)
{
	BYTE *pbyBuffer = m_byBuffer;
	int nLen;
	BOOL bPageInCache = FALSE;
	BOOL bRet = FALSE;
	CInet::RESULTS ret;
	MAP_FILES* pMapEntry;

	// 初始化链接堆栈入口
	linkEntry.arrLinks.SetSize(0,100);
	linkEntry.arrMedia.SetSize(0,100);
	linkEntry.arrOffsite.SetSize(0,100);
	linkEntry.nIndex = 0;

	// 判断是否需要下载该页
	if(ShouldGetPage(strPage,pMapEntry))
	{
		// 向服务器提出申请，要下载该文件
		ret = m_Inet.GetPage(strPage,&pbyBuffer,nLen,TRUE);
		if(ret == CInet::SUCCESS)
		{
			// 将总下载字节数增加
			bRet = TRUE;
			m_nTotalBytes += nLen;
		}
	}
	else
	{
		// 如果已经下载过了，则复制本地文件
		// 因为需要链接指向原来下载过的文件
		CFile fileIn;
		CFileException ex;

		// 获得完整的文件路径
		strFileName = pMapEntry->strFileName;
		CString strTempFileName = m_strDirectory+strFileName;

		// 打开该文件
		if(fileIn.Open(strTempFileName,CFile::modeRead,&ex))
		{
			// 读入数据
			nLen = fileIn.Read(pbyBuffer,MAX_INET_BUFFER);
			fileIn.Close();
			bRet = TRUE;
		}

		// 指示并没有重新下载该文件
		bPageInCache = TRUE;
	}

	// 是否完毕
	if(bRet)
	{
		// 获得链接以及多媒体文件信息
		CSnaggerHtmlParser Parser;
		Parser.SetPageURL(strPage);
		m_pProgress->SetActionTitle("Parsing Page: "+strPage);

		// 判断是否超出最大缓冲区
		if(nLen > MAX_INET_BUFFER)
			nLen = MAX_INET_BUFFER;

		// 初始化并调用分析子程序parser
		pbyBuffer = m_byBuffer;
		Parser.SetFixupMode(FALSE);
		Parser.ResetArrays();
		Parser.SetGetMedia(m_Options.bMultimedia);
		Parser.ParseText((char *)pbyBuffer,nLen);
		m_strPageTitle = Parser.GetTitle();

		// 保存该文件
		if(!bPageInCache)
		{
			m_pProgress->SetActionTitle("Saving Page: "+strPage);
			pbyBuffer = m_byBuffer;
			m_Inet.SaveFile(strFileName,m_strDirectory,pbyBuffer,nLen);
		}

		// 分析被链接的页面的数量
		int nLinks;
		BOOL bOffsite;
		CString strNewPage;
		nLinks = Parser.GetLinks().GetSize();

		// 分析每一个链接，是否要添加到下载等待列表中
		for(int i = 0; i < nLinks; i++)
		{
			// 获得网页的URL地址
			strNewPage = Parser.GetLinks().GetAt(i);

			// Get the offsite link flag for this page
			bOffsite = Parser.GetOffsiteFlags().GetAt(i);

			// See if we should at it to the download queue
			if(ShouldQueuePage(strNewPage,bOffsite)) 
			{
				linkEntry.arrLinks.Add(strNewPage);
				linkEntry.arrOffsite.Add(bOffsite);
			}
		}

		// Don't need the images if we've already parsed this page
		// before
		if(!bPageInCache)
		{
			// New page, so get the all of the media information
			int nMedia = Parser.GetMedia().GetSize();
			CString strMedia;
			for(i = 0; i < nMedia; i++)
			{
				strMedia = Parser.GetMedia().GetAt(i);
				if(ShouldGetMedia(strMedia,pMapEntry))
					linkEntry.arrMedia.Add(strMedia);
			}
		}

		// Success
		bRet = TRUE;
	}

	return(bRet);
}

// 获得指定的多媒体文件
BOOL CSnaggerDoc::GetMedia(CString& strMedia, CString& strFileName)
{
	BYTE *pbyBuffer = m_byBuffer;
	int nLen;
	BOOL bRet = FALSE;

	// 获得指定的页面
	CInet::RESULTS ret;

	// 从INet获得文件
	ret	= m_Inet.GetFile(strMedia,&pbyBuffer,nLen);

	if(ret == CInet::SUCCESS)
	{
		// 总下载数据增加该下载文件大小
		m_nTotalBytes += nLen;

		// 写入文件
		m_pProgress->SetActionTitle("Saving File: "+strMedia);
		m_Inet.SaveFile(strFileName,m_strDirectory,pbyBuffer,nLen);

		bRet = TRUE;
	}

	return(bRet);
}

//旁断是否该页面已经下载过，如果已经下载过，则返回true
//如果已经下载，则返回指向指针
BOOL CSnaggerDoc::ShouldGetPage(CString& strPage, MAP_FILES*& pMapEntry)
{
	// Page names shouldn't be case sensitive
	CString strNewPage = strPage;
	strNewPage.MakeLower();
	strNewPage = strNewPage.SpanExcluding("#");

	// 设定一个相对路径
	CString strExt = CInet::SplitFileName(strNewPage,CInet::EXT);
	if(strExt.IsEmpty() && strNewPage.Right(1) != "/")
		strNewPage += "/";

	// Did we find it??
	return(!m_arrPagesDone.Lookup(strNewPage,(CObject *&) pMapEntry));
}

//判断是否下载多媒体文件，如果以前已经下载过，则返回true
//如果已经下载过，则返回一个指针真想已经下载的多媒体列表的入口
BOOL CSnaggerDoc::ShouldGetMedia(CString& strMedia, MAP_FILES*& pMapEntry)
{
	// Page names shouldn't be case sensitive
	CString strNewMedia = strMedia;
	strNewMedia.MakeLower();
	strNewMedia = strNewMedia.SpanExcluding("#");

	// Page names shouldn't be case sensitive
	return(!m_arrMediaDone.Lookup(strNewMedia,(CObject *&) pMapEntry));
}

//该函数返回true，如果指定的页面要被增加到下载队列中
//主要是确定是否该页面是否在前一层中被下载
//同时还要检查是否是本站页面
BOOL CSnaggerDoc::ShouldQueuePage(CString& strNewPage, BOOL bOffsite)
{
	MAP_FILES* pMapEntry;

	// 判断是否以前下载过
	if(ShouldGetPage(strNewPage,pMapEntry))
	{
		// 如果还没有，则在等待列表中寻找是否已经在前一层的等待列表中
		for(int i = 0; i < m_nLevel; i++)
		{
			for(int j = 0; j < m_aLinks[i].arrLinks.GetSize(); j++)
			{
				if(strNewPage == m_aLinks[i].arrLinks.GetAt(j))
					return(FALSE);
			}
		}
	}
	else
	{
		// 如果达到最大层数，则返回true 
		if(m_Options.nMaxDepth && m_nLevel >= pMapEntry->nMaxLevel)
			return(TRUE);
	}

	// 是否允许其他站点的页面下载
	if(bOffsite && !m_Options.bOffsiteLinks)
		return(FALSE);

	return(TRUE);
}

// 初始化指定链接的堆栈入口
void CSnaggerDoc::ResetLink(int nLevel)
{
	m_aLinks[nLevel].nIndex = 0;
	m_aLinks[nLevel].arrLinks.SetSize(0,100);
	m_aLinks[nLevel].arrMedia.SetSize(0,100);
	m_aLinks[nLevel].arrOffsite.SetSize(0,100);
}

// 更新参数显示
void CSnaggerDoc::UpdateStatus()
{
	// Does the statistics window exist?
	if(m_pProgress)
	{
		// Yep...update the info in its fields
		m_pProgress->SetQueuedFiles(m_nQueuedPageCount);
		m_pProgress->SetDownloadedPages(m_nGottenPageCount);
		m_pProgress->SetDownloadedFiles(m_nGottenFileCount);
		m_pProgress->SetKBDownloaded(m_nTotalBytes);
		m_pProgress->SetLevel(m_nLevel+1);
	}
}

// 现在网页以及网页中的多媒体元素
UINT CSnaggerDoc::DownloadThread(LPVOID lpvData)
{
	HTREEITEM htreePage;

	CSnaggerDoc *pThis = (CSnaggerDoc *) lpvData;

	int nMaxDepth = pThis->m_Options.nMaxDepth-1;
	int nCount;
	CString strPage = pThis->m_strStartPage;
	CString strFileName;
	CString strLogData;
	CString strText;
	POSITION pos = pThis->GetFirstViewPosition();
	CSnaggerView* pView = (CSnaggerView *) pThis->GetNextView(pos);	
	BOOL bIsOffsite = FALSE;

	// 建立WinInet会话
	try
	{
		pThis->m_Inet.OpenSession(pThis->m_Options.bUseProxy,pThis->m_Options.strProxyName);
	}
	catch(...)
	{
	}

	// 创建日志文件
	pThis->m_fileLog.Open(pThis->m_strDirectory+"sitesnag.log",
						CFile::modeCreate|CFile::modeWrite);

	// 创建内容列表文件
	if(pThis->m_Options.bContents)
	{
		pThis->m_fileContents.Open(pThis->m_strDirectory+"SnagCon1.htm",
								CFile::modeCreate|CFile::modeWrite);

		// 将下载内容增加到下载文件列表中
		pThis->SetPageCacheEntry("snagcon1.htm","SnagCon1.htm",0);

		// 内容列表加入到树形控件中
		CString strTitle = "Contents Page 1 (SnagCon1.htm)";
		pView->AddTreeContent(strTitle);

		// 写入第一个内容列表也的开始
		strText = "<HTML>\r\n<HEAD>\r\n<TITLE>SiteSnagger Contents</TITLE>\r\n";
		strText += "</HEAD\r\n<BODY>\r\n";
		strText += "<H1><center>SiteSnagger Table of Contents</center><br><br></H1>\r\n<UL>\r\n";
		pThis->m_fileContents.Write(strText,strText.GetLength());
	}

	// 初始化第一层文件链接索引
	//从第一层开始
	pThis->m_nLevel = 0;
	pThis->m_aLinks[0].nIndex = 0;
	pThis->m_Inet.ResetUniqueCount();

	// 搜索网络链接知道已经完成所有搜索
	//或者被人为停止
	while(pThis->m_nLevel >= 0 && !pThis->m_pProgress->IsAborted())
	{
		// 获得第二级的新页面名
		if(pThis->m_aLinks[pThis->m_nLevel].nIndex > 0)
		{
			// 保存本页面UR，兵判断是否是本站网页
			int nIndex = pThis->m_aLinks[pThis->m_nLevel].nIndex;
			strPage = pThis->m_aLinks[pThis->m_nLevel].arrLinks.GetAt(nIndex);
			bIsOffsite = pThis->m_aLinks[pThis->m_nLevel].arrOffsite.GetAt(nIndex);

			// 连接到下一层
			pThis->m_nLevel++;
		}

		// 为该页生成一个唯一的名称
		pThis->m_Inet.GenerateUniqueFileName(strPage,strFileName,
						pThis->m_arrPagesDone,TRUE);
		pThis->m_pProgress->SetActionTitle("Getting Page: "+strPage);

		// 为该页写一个日志入口，方便写入结果
		strLogData.Format("[%02d] Getting page %s ",pThis->m_nLevel+1,strPage);
		pThis->m_fileLog.Write(strLogData,strLogData.GetLength());

		CString strOrigPage = strPage;

		// 获得该页从Inet本地文件
		if(pThis->GetPage(strPage,strFileName,pThis->m_aLinks[pThis->m_nLevel]))
		{
			MAP_FILES *pMapEntry;
		
			// 获得连接数量
			nCount = pThis->m_aLinks[pThis->m_nLevel].arrLinks.GetSize();

			// 是否只需要下载文件
			if(pThis->ShouldGetPage(strPage,pMapEntry))
			{
				// 增加到已经接收页面的列表中
				pThis->SetPageCacheEntry(strPage,strFileName,pThis->m_nLevel);

				// 如果页面被重新定向，则
				//也添加原始名字
				if(strPage != strOrigPage && pThis->ShouldGetPage(strOrigPage,pMapEntry))
					pThis->SetPageCacheEntry(strOrigPage,strFileName,pThis->m_nLevel);

				// 如果是其他站点的网页，加上前缀，如主机域名
				if(bIsOffsite)
					strText = strPage+" - ";
				else strText.Empty();

				// 加入页面的名称和文件名
				strText += pThis->m_strPageTitle+"  ("+
							strFileName.SpanExcluding("#")+")";

				htreePage = pView->AddTreePage(strText,bIsOffsite);
				strText.Format("<a href=%s><li> %s (%s - %s)<br>\r\n",strFileName,
									pThis->m_strPageTitle,
									strFileName.SpanExcluding("#"),strPage);
				if(pThis->m_Options.bContents)
					pThis->m_fileContents.Write(strText,strText.GetLength());

				// 更新参数
				pThis->m_nGottenPageCount++;
				pThis->m_nGottenFileCount++;
				pThis->UpdateStatus();
			}
			else
			{
				// 设置新的搜索层数
				if(nMaxDepth)
				{
					// 是否已经到达最大层数
					if(pThis->m_nLevel >= pMapEntry->nMaxLevel)
						nCount = 0;
					else pMapEntry->nMaxLevel = pThis->m_nLevel;
				}
			}

			// 将结果写入日志文件
			pThis->m_fileLog.Write("[OK]\n",5);

			// 检查其他站点的连接
			if(bIsOffsite)
				nCount = 0;

			// 是否下载多媒体文件
			if(pThis->m_Options.bMultimedia)
			{
				// 分析多媒体文件链接列表
				CString strMedia;
				for(int j = 0; j < pThis->m_aLinks[pThis->m_nLevel].arrMedia.GetSize() &&
							!pThis->m_pProgress->IsAborted(); j++)
				{
					strMedia = pThis->m_aLinks[pThis->m_nLevel].arrMedia.GetAt(j);

					// 是否获取该文件
					if(pThis->ShouldGetMedia(strMedia,pMapEntry))
					{
						// 保证是一个唯一的文件名
						pThis->m_Inet.GenerateUniqueFileName(strMedia,
										strFileName,pThis->m_arrMediaDone,FALSE);
						pThis->m_pProgress->SetActionTitle("Getting File: "+strFileName);

						// 写入日志文件
						strLogData.Format("[%02d] Getting media %s ",pThis->m_nLevel,
														strMedia);
						pThis->m_fileLog.Write(strLogData,strLogData.GetLength());

						// 下载EMAIL链接
						BOOL bMail;
						if(strMedia.Left(7) == "mailto:")
						{
							bMail = TRUE;
							strFileName = strMedia;
						}
						else bMail = FALSE;

						//如果成功
						if(bMail || pThis->GetMedia(strMedia,strFileName))
						{
							//将文件增加到文件列表中
							pThis->SetMediaCacheEntry(strMedia,strFileName);
							pView->AddTreeMedia(strFileName.SpanExcluding("#"),
											CTree::GetMediaType(strFileName));

							// 将静态参数更新
							if(!bMail)
								pThis->m_nGottenFileCount++;
							pThis->UpdateStatus();

							//将结果写入日志
							pThis->m_fileLog.Write("[OK]\n",5);
						}
						else
						{
							// 将失败结果写入日志
							pThis->m_fileLog.Write("[FAILED] ",9);

							// 写入详细的错误日志
							CString strError = pThis->m_Inet.GetErrorText();
							pThis->m_fileLog.Write(strError,strError.GetLength());
							pThis->m_fileLog.Write("\n",1);
						}
					}
				}
			}
		}
		else
		{
			// 写入日志
			pThis->m_fileLog.Write("[FAILED] ",9);

			// 显示详细的错误
			CString strError = pThis->m_Inet.GetErrorText();
			pThis->m_fileLog.Write(strError,strError.GetLength());
			pThis->m_fileLog.Write("\n",1);
			
			nCount = 0;
		}

		// 更新参数
		pThis->UpdateStatus();

		// 如果到达了设定的最大页数，则退出
		if(pThis->m_Options.nMaxPages > 0 && 
				pThis->m_nGottenPageCount >= pThis->m_Options.nMaxPages) 
			break;

		// 只要还有连接，则继续下载
		if(pThis->m_nLevel < nMaxDepth && nCount > 0)
		{
			// 获得下一个页面
			strPage = pThis->m_aLinks[pThis->m_nLevel].arrLinks.GetAt(0);
			bIsOffsite = pThis->m_aLinks[pThis->m_nLevel].arrOffsite.GetAt(0);

			// 进入下一层，并初始化
			pThis->m_nLevel++;
			pThis->ResetLink(pThis->m_nLevel);

			// Queue the links
			pThis->m_nQueuedPageCount += nCount;
			continue;
		}

		// 完成所有链接，重置链接信息
		pThis->ResetLink(pThis->m_nLevel);

		// 移到前一层
		pThis->m_nLevel--;

		// 寻找下一页
		if(pThis->m_nLevel >= 0)
		{
			int nMaxCount;

			// 
			while(pThis->m_nLevel >= 0)
			{
				// 获得第二层的入口数量
				nMaxCount = pThis->m_aLinks[pThis->m_nLevel].arrLinks.GetSize();

				// 是否还有其他合法的页面在这一层
				if(pThis->m_aLinks[pThis->m_nLevel].nIndex < nMaxCount-1)
				{
					// 获得下一个页面
					pThis->m_aLinks[pThis->m_nLevel].nIndex++;
					pThis->m_nQueuedPageCount--;
					break;
				}
				else 
				{
					// 在树形控件上回到前一层
					pThis->m_nLevel--;
					pThis->m_nQueuedPageCount--;
				}
			}
		}
	}

	// Make sure the "stopping, please wait" message isn't displayed
	pView->EndWait();

	
	// 确定nNodeCount总是0
	pThis->m_nLevel = pThis->m_nQueuedPageCount;

	// 是否需要整理链接
	if(pThis->m_Options.bFixupLinks)
	{
		// 用户取消下载
		if(pThis->m_pProgress->IsAborted() && 
			pView->ShowPrompt(IDS_FIXUP_PAGES,MB_ICONQUESTION|MB_YESNO) != IDYES)
			;
		else
		{
			// Everything was normal so just fixup the links
			pThis->m_pProgress->SetAborted(FALSE);
			pThis->FixupLinks();
		}
	}

	//  关闭日志文件
	pThis->m_fileLog.Close();

	// Are we doing the TOC??
	if(pThis->m_Options.bContents)
	{
		// 写入文件的尾部
		CString strText;
		strText = "</UL>\r\n</BODY>\r\n</HTML>\r\n";
		pThis->m_fileContents.Write(strText,strText.GetLength());
		pThis->m_fileContents.Close();
	}

	// 输出信息表示已经完成
	pThis->m_pProgress->SetActionTitle("********* 处理完毕 *********");
	pView->SetSnagging(FALSE);

	// 释放内存
	pThis->ClearCacheMaps();

	// Make sure the "stopping, please wait" message isn't displayed
	//pView->EndWait();

	// 如果是自动模式，关闭住窗口
	if(pThis->m_bAutoMode)
		AfxGetMainWnd()->PostMessage(WM_CLOSE);

	// 关闭WinInet会话
	try
	{
		pThis->m_Inet.CloseSession();
	}
	catch(...)
	{
	}

	pThis->m_nLevel = 0;

	return(0);
}

//释放资源，即所有文件入口以及文件下载列表
void CSnaggerDoc::ClearCacheMaps()
{
	MAP_FILES *pMapEntry;
	CString strPage;
	POSITION pos;
	int i;

	// 删除页面列表
	pos = m_arrPagesDone.GetStartPosition();
	for(i = 0; i < m_arrPagesDone.GetCount(); i++)
	{
		m_arrPagesDone.GetNextAssoc(pos,strPage,(CObject *&) pMapEntry);
		delete pMapEntry;
	}
	m_arrPagesDone.RemoveAll();

	// 删除多媒体页面列表
	pos = m_arrMediaDone.GetStartPosition();
	for(i = 0; i < m_arrMediaDone.GetCount(); i++)
	{
		m_arrMediaDone.GetNextAssoc(pos,strPage,(CObject *&) pMapEntry);
		delete pMapEntry;
	}
	m_arrMediaDone.RemoveAll();
}

// 增加一个进入下载页面文件列表的入口
void CSnaggerDoc::SetPageCacheEntry(LPCTSTR lpszPage, LPCTSTR lpszFileName, int nLevel)
{
	CString strTempPage = lpszPage;
	MAP_FILES *pMapEntry = new MAP_FILES();

	// 将网页名称以小写保存
	pMapEntry->strFileName = lpszFileName;
	pMapEntry->nMaxLevel = nLevel;
	strTempPage.MakeLower();
	strTempPage = strTempPage.SpanExcluding("#");

	// 让所有页面有一个相对路径
	CString strExt = CInet::SplitFileName(strTempPage,CInet::EXT);
	if(strExt.IsEmpty() && strTempPage.Right(1) != "/")
		strTempPage += "/";

	// Assimilate it into the collective
	m_arrPagesDone.SetAt(strTempPage,(CObject *&) pMapEntry);
	TRACE("Pages Done: "+strTempPage+"\n");

	// 设定默认名称
	if(pMapEntry->strFileName == "index.htm" && strTempPage.Find("index.htm") == -1)
	{
		// 常用的默认页文件名
		char *aszDefPages[4] = { "index.htm", "index.html", "default.htm", 
										"default.html"};

		MAP_FILES *pTempMapEntry;

		CString strServer, strObject, strUser, strPassword;
		INTERNET_PORT nPort;
		DWORD dwServiceType;
		AfxParseURLEx(strTempPage,dwServiceType,strServer,strObject,nPort,
									strUser,strPassword,
									ICU_NO_ENCODE);

		// 加入默认问卷爱你们
		if(!strServer.IsEmpty())
		{
			strServer = "http://"+strServer+"/";

			for(int i = 0; i < 4; i++)
			{
				pTempMapEntry = new MAP_FILES();
				pTempMapEntry->strFileName = pMapEntry->strFileName;
				pTempMapEntry->nMaxLevel = pMapEntry->nMaxLevel;
				m_arrPagesDone.SetAt(strServer+aszDefPages[i],(CObject *&) pTempMapEntry);
				TRACE("Pages Done: "+CString(aszDefPages[i])+"\n");
			}
		}
	}	
}

// 增家一个新的文件列表的入口
void CSnaggerDoc::SetMediaCacheEntry(LPCTSTR lpszMedia, LPCTSTR lpszFileName)
{
	CString strTempMedia = lpszMedia;
	MAP_FILES *pMapEntry = new MAP_FILES();

	// 文件名保存成小写
	pMapEntry->strFileName = lpszFileName;
	pMapEntry->nMaxLevel = -1;
	strTempMedia.MakeLower();
	strTempMedia = strTempMedia.SpanExcluding("#");

	// Assimilate it into the collective
	m_arrMediaDone.SetAt(strTempMedia,(CObject *&) pMapEntry);
}

//开始下载网站内容
void CSnaggerDoc::RecursiveDownload(LPCTSTR lpszURL)
{
	// 初始化参数
	m_nGottenPageCount = 0;
	m_nGottenFileCount = 0;
	m_nQueuedPageCount = 0;
	m_nTotalBytes = 0;
	m_strStartPage = lpszURL;
	ClearCacheMaps();

	// 初始化参数显示窗口
	POSITION pos = GetFirstViewPosition();
	CSnaggerView* pView = (CSnaggerView *) GetNextView(pos);
	m_pProgress = pView->GetProgress();

	m_pProgress->SetAborted(FALSE);
	UpdateStatus();

	// 启动下载线程
	CWinThread *pThread = AfxBeginThread(DownloadThread,this);
}

// Fixs up each of the downloaded pages to allow local browsing.  It
// basically goes through the downloaded page list and opens each 
// file specified then parses the page and fixes up each of the 
// links found.
//
void CSnaggerDoc::FixupLinks()
{
	BYTE *pbyBuffer = m_byBuffer;
	int nLen;
	CFile fileIn, fileOut;
	CFileException ex;
	MAP_FILES* pMapEntry;
	int nIndex;
	int nBytesWritten;
	int nSectionLength;
	int nOffset;
	CString strFixupFileName;
	CString strPage;
	CString strFileName;
	CString strTempName = m_strDirectory+"$ssnag$.htm";
	BOOL bFixup;
	int nFiles = m_arrPagesDone.GetCount();
	BOOL bIndexDone = FALSE;

	//更新参数显示窗口指示还未处理的文件数量
	m_pProgress->SetQueuedFiles(nFiles);

	// Iterate through the downloaded page list
	//获得下载页列表
	POSITION pos = m_arrPagesDone.GetStartPosition();
	for(int i = 0; i < m_arrPagesDone.GetCount() && !m_pProgress->IsAborted(); i++)
	{
		m_arrPagesDone.GetNextAssoc(pos,strPage,(CObject *&) pMapEntry);

		if(pMapEntry->strFileName == "index.htm" || 
						pMapEntry->strFileName == "index.html")
		{
			if(bIndexDone)
			{
				// 更新参数显示窗口
				m_pProgress->SetQueuedFiles(--nFiles);
				continue;
			}	
			else bIndexDone = TRUE;
		}

		//显示参数
		strFileName = m_strDirectory+pMapEntry->strFileName;
		m_pProgress->SetActionTitle("Fixing up links in file: "+pMapEntry->strFileName);

		// 打开本地文件
		TRACE("Fixing up: "+strFileName+"\n");
		if(fileIn.Open(strFileName.SpanExcluding("#"),CFile::modeRead,&ex))
		{
			pbyBuffer = m_byBuffer;

			// 把文件读入内存
			nLen = fileIn.Read(pbyBuffer,MAX_INET_BUFFER);
			fileIn.Close();

			// Parse it (tell the parser to look for only links, i.e. fixup mode)
			CSnaggerHtmlParser Parser;
			Parser.SetFixupMode(TRUE);
			Parser.ResetArrays();
			Parser.SetGetMedia(m_Options.bMultimedia);
			Parser.SetPageURL(strPage);
			Parser.ParseText((char *)pbyBuffer,nLen);

			CString strServer, strObject, strUser, strPassword;
			INTERNET_PORT nPort;
			DWORD dwServiceType;
			AfxParseURLEx(strPage,dwServiceType,strServer,strObject,nPort,
									strUser,strPassword,
									ICU_NO_ENCODE);
			CString strOrigHost = "http://"+strServer+"/";

			// 打开个临时文件
			if(fileOut.Open(strTempName,CFile::modeCreate|CFile::modeWrite,&ex))
			{
				CString strURL;

				nIndex = 0;
				nBytesWritten = 0;
				nOffset = 0;

				// 写入文件并整理(如果有必要）
				for(int j = 0; j < Parser.GetFixups().arrURL.GetSize(); j++)
				{
					strURL = Parser.GetFixups().arrURL.GetAt(j);

					// 是否整理完文本文件
					if(Parser.GetFixups().arrTextPage.GetAt(j))
					{
						// 是否要下载这个文件
						if(!ShouldGetPage(strURL,pMapEntry))
						{
							// 获得本地文件
							strFixupFileName = pMapEntry->strFileName;
							bFixup = TRUE;
						}
						else
						{							
							AfxParseURLEx(strURL,dwServiceType,strServer,
									strObject,nPort,strUser,strPassword,
									ICU_NO_ENCODE);

							if(!strServer.IsEmpty())
							{
								strFixupFileName = strURL;
								bFixup = TRUE;
							}
							else bFixup = FALSE;
						}
					}
					else // 多媒体文件整理
					{
						// 是否下载多媒体文件
						if(!ShouldGetMedia(strURL,pMapEntry))
						{
							// 获取文件名
							strFixupFileName = pMapEntry->strFileName;
							bFixup = TRUE;
						}
						else bFixup = FALSE;
					}

					// 写入数据
					nSectionLength = Parser.GetFixups().arrIndex.GetAt(j)-nIndex-nOffset;

					try
					{
						fileOut.Write(pbyBuffer,nSectionLength);
					}
					catch(...)
					{
					}

					nBytesWritten += nSectionLength;
					pbyBuffer += nSectionLength;
					nOffset = 0;

					// 是否获得整理的入口
					if(bFixup)
					{
						// 写入文件
						try
						{
							fileOut.Write(" \"",2);
							fileOut.Write(strFixupFileName,strFixupFileName.GetLength());
						}
						catch(...)
						{
						}

						// 找到原来文件的开始除
						while(*pbyBuffer == ' ') 
						{
							pbyBuffer++;
							nBytesWritten++;
							nOffset++;
						}

						// Is the filename in quotes?
						if(*pbyBuffer == '"') 
						{
							// 找到最后
							do
							{
								pbyBuffer++;
								nBytesWritten++;
								nOffset++;
							}
							while(*pbyBuffer != '"');

							pbyBuffer++;
							nBytesWritten++;
							nOffset++;
						}
						else
						{
							// Not in quotes, so just look for the first
							// trailing blank
							do
							{
								pbyBuffer++;
								nBytesWritten++;
								nOffset++;
							}
							while(*pbyBuffer != ' ');
						}

						// Write a trailing quote and blank, just to be safe
						fileOut.Write("\" ",2);
					}

					// 保存开始整理的位置
					nIndex = Parser.GetFixups().arrIndex.GetAt(j);
				}

				// 完成整理，然后写入剩余的数据
				try
				{
					if(nLen-nBytesWritten > 0)
						fileOut.Write(pbyBuffer,nLen-nBytesWritten);
				}
				catch(...)
				{
				}

				fileOut.Close();
			}
		}

		// 用整理过的文件替代原始文件
		remove(strFileName.SpanExcluding("#"));
		rename(strTempName,strFileName.SpanExcluding("#"));

		// 更新参数显示串口
		m_pProgress->SetQueuedFiles(--nFiles);
	}

	// 删除临时文件，因为有可能中间用户要硬性停止
	remove(strTempName);
}

/////////////////////////////////////////////////////////////////////////////
// CSnaggerDoc diagnostics

#ifdef _DEBUG

// Check validity of object
void CSnaggerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

// Dump the object
void CSnaggerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

