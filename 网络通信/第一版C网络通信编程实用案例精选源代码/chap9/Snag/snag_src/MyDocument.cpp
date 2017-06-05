#include "stdafx.h"

#include "SiteDownload.h"
#include "MyDocument.h"
#include "MyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMyDoc

IMPLEMENT_DYNCREATE(CMyDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyDoc, CDocument)
	//{{AFX_MSG_MAP(CMyDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyDoc construction/destruction

CMyDoc::CMyDoc()
{
	m_Options.nMaxDepth = 3;
	m_Options.nMaxPages = 0;
	m_Options.bFixupLinks = TRUE;
	m_Options.bContents = TRUE;
	m_Options.bMultimedia = TRUE;
	m_Options.bOffsiteLinks = TRUE;

	// 设置哈希表尺寸
	m_arrPagesDone.InitHashTable(1200);
	m_arrMediaDone.InitHashTable(2400);

	m_bProjectLoaded = FALSE;//空项目
	m_bAutoMode = FALSE;

	m_nLevel = 0;//处在第零层
}

CMyDoc::~CMyDoc()
{
	try
	{
		ClearCacheMaps();
	}
	catch(...)
	{
	}
}

//处理打开的新文档，这个函数只在程序刚执行的时候调用一次
BOOL CMyDoc::OnNewDocument()
{

	static bFirstTime = TRUE;

	if(bFirstTime)
	{
		bFirstTime = FALSE;

		CString strDefName;
		strDefName.LoadString(IDS_NO_PROJECT);
		SetTitle(strDefName);

		if (!CDocument::OnNewDocument())
			return FALSE;
		return TRUE;
	}

	if (!CDocument::OnNewDocument())
		return FALSE;

	m_bProjectLoaded = TRUE;
	m_strStartPage.Empty();
	m_nGottenPageCount = 0;
	m_nGottenFileCount = 0;
	m_nQueuedPageCount = 0;
	m_nTotalBytes = 0;

	POSITION pos = GetFirstViewPosition();
	CMyView* pView = (CMyView *) GetNextView(pos);
	return TRUE;
}

//打开一个文件
BOOL CMyDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	POSITION pos = GetFirstViewPosition();
	CMyView* pView = (CMyView *) GetNextView(pos);

	// 先要保存当前的项目
	SaveModified();
	//清空树形控件显示的内容
	pView->ClearTree();

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	SetPathName(lpszPathName);
	m_strDirectory = CInternetDownload::SplitFileName(lpszPathName,
				CInternetDownload::DRIVE|CInternetDownload::PATH|CInternetDownload::FNAME)+"\\";

	SetTitle(CInternetDownload::SplitFileName(lpszPathName,CInternetDownload::FNAME|CInternetDownload::EXT));
	m_bProjectLoaded = TRUE;

	SetModifiedFlag(TRUE);

	return TRUE;
}

//保存修改的信息
BOOL CMyDoc::SaveModified() 
{
	if(IsModified())
		return CDocument::DoFileSave();
	return(TRUE);
}

//判断用户是否可以安全的退出程序
BOOL CMyDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	POSITION pos = GetFirstViewPosition();
	CMyView* pView = (CMyView *) GetNextView(pos);

	return(!pView->GetSnagging());
}

//重置一个新的文档：所有的文件信息和统计信息都需要重置
void CMyDoc::Reset(LPCTSTR lpszProjName)
{
	CString strNewProjName;
		
	if(lpszProjName)
		strNewProjName = lpszProjName;

	strNewProjName.LoadString(IDS_NO_PROJECT);
	m_strPathName.Empty();
	m_strDirectory.Empty();
	m_bProjectLoaded = FALSE;
	SetModifiedFlag(FALSE);
	SetTitle(strNewProjName);

	m_strStartPage.Empty();
	m_nGottenPageCount = 0;
	m_nGottenFileCount = 0;
	m_nQueuedPageCount = 0;
	m_nTotalBytes = 0;

	POSITION pos = GetFirstViewPosition();
	CMyView* pView = (CMyView *) GetNextView(pos);
}

//保存然后关闭文档
void CMyDoc::OnCloseDocument() 
{
	SaveModified();
	CDocument::OnCloseDocument();
}

//得到项目下载的配置信息
void CMyDoc::GetOptions(CConfigure& Options)
{
	Options.nMaxDepth = m_Options.nMaxDepth;
	Options.nMaxPages = m_Options.nMaxPages;
	Options.bFixupLinks = m_Options.bFixupLinks;
	Options.bContents = m_Options.bContents;
	Options.bMultimedia = m_Options.bMultimedia;
	Options.bOffsiteLinks = m_Options.bOffsiteLinks;
}

//设置项目下载的配置信息
void CMyDoc::SetOptions(CConfigure& Options) 
{ 
	m_Options.nMaxDepth = Options.nMaxDepth;
	m_Options.nMaxPages = Options.nMaxPages;
	m_Options.bFixupLinks = Options.bFixupLinks;
	m_Options.bContents = Options.bContents;
	m_Options.bMultimedia = Options.bMultimedia;
	m_Options.bOffsiteLinks = Options.bOffsiteLinks;
}

//文档信息的串行化
void CMyDoc::Serialize(CArchive& ar)
{
	POSITION pos = GetFirstViewPosition();
	CMyView* pView = (CMyView *) GetNextView(pos);	

	if (ar.IsStoring())
	{
		ar << m_nGottenPageCount;
		ar << m_nGottenFileCount;
		ar << m_nTotalBytes;
	}
	else
	{
		ar >> m_nGottenPageCount;
		ar >> m_nGottenFileCount;
		ar >> m_nTotalBytes;
		m_nQueuedPageCount = 0;
	}

	m_Options.Serialize(ar);

	// 视图中的树形控件信息也要串行化
	pView->SerializeTree(ar);

	m_nLevel = 0;
}


//使用CInternetDownload类或者直接从硬盘上得到想要的具体的页面。
//如果页面是从网络中得到的，那么将它保存在硬盘中，并且使用分析器分析该页面所包含的链接和多媒体资源。
BOOL CMyDoc::GetPage(CString& strPage, CString& strFileName, LINKS& linkEntry)
{
	BYTE *pbyBuffer = m_byBuffer;
	int nLen;
	BOOL bPageInCache = FALSE;
	BOOL bRet = FALSE;
	CInternetDownload::RESULTS ret;
	MAP_FILES* pMapEntry;

	//初始化链接栈的入口信息
	linkEntry.arrLinks.SetSize(0,100);
	linkEntry.arrMedia.SetSize(0,100);
	linkEntry.arrOffsite.SetSize(0,100);
	linkEntry.nIndex = 0;

	//判断是否应该从网络中得到该页面，还是从本地硬盘中得到
	if(ShouldGetPage(strPage,pMapEntry))//网络资源
	{
		ret = m_Inet.GetPage(strPage,&pbyBuffer,nLen,TRUE);
		if(ret == CInternetDownload::SUCCESS)
		{
			bRet = TRUE;
			m_nTotalBytes += nLen;
		}
	}
	else//硬盘资源
	{
		CFile fileIn;
		CFileException ex;

		strFileName = pMapEntry->strFileName;
		CString strTempFileName = m_strDirectory+strFileName;

		if(fileIn.Open(strTempFileName,CFile::modeRead,&ex))
		{
			nLen = fileIn.Read(pbyBuffer,MAX_INET_BUFFER);
			fileIn.Close();
			bRet = TRUE;
		}

		// 标注我们不是从CInternetDownload中得到的页面
		bPageInCache = TRUE;
	}

	if(bRet)
	{
		// 用分析器分析该页面所包含的链接和媒体信息
		CHTMLFileParser Parser;
		Parser.SetPageURL(strPage);

		if(nLen > MAX_INET_BUFFER)
			nLen = MAX_INET_BUFFER;

		pbyBuffer = m_byBuffer;
		Parser.SetFixupMode(FALSE);
		Parser.ResetArrays();
		Parser.SetGetMedia(m_Options.bMultimedia);
		Parser.ParseText((char *)pbyBuffer,nLen);
		m_strPageTitle = Parser.GetTitle();

		//把新下载的页面保存到硬盘中
		if(!bPageInCache)
		{
			pbyBuffer = m_byBuffer;
			m_Inet.SaveFile(strFileName,m_strDirectory,pbyBuffer,nLen);
		}

		// 判断链接的数目
		int nLinks;
		BOOL bOffsite;
		CString strNewPage;
		nLinks = Parser.GetLinks().GetSize();

		//评估每一个链接然后决定是否将他们放入下载的队列中
		for(int i = 0; i < nLinks; i++)
		{
			// 得到这个链接的url
			strNewPage = Parser.GetLinks().GetAt(i);

			// 得到这个页面的偏移链接号
			bOffsite = Parser.GetOffsiteFlags().GetAt(i);

			// 判断是否应该将其放入下载队列中
			if(ShouldQueuePage(strNewPage,bOffsite)) 
			{
				linkEntry.arrLinks.Add(strNewPage);
				linkEntry.arrOffsite.Add(bOffsite);
			}
		}

		//是否是旧页面
		if(!bPageInCache)
		{
			int nMedia = Parser.GetMedia().GetSize();
			CString strMedia;
			for(i = 0; i < nMedia; i++)
			{
				strMedia = Parser.GetMedia().GetAt(i);
				if(ShouldGetMedia(strMedia,pMapEntry))
					linkEntry.arrMedia.Add(strMedia);
			}
		}

		bRet = TRUE;
	}

	return(bRet);
}

//使用CInternetDownload类得到网页中的多媒体项，并保存
BOOL CMyDoc::GetMedia(CString& strMedia, CString& strFileName)
{
	BYTE *pbyBuffer = m_byBuffer;
	int nLen;
	BOOL bRet = FALSE;

	CInternetDownload::RESULTS ret;

	// 从Inet中得到文件
	ret	= m_Inet.GetFile(strMedia,&pbyBuffer,nLen);

	if(ret == CInternetDownload::SUCCESS)
	{
		m_nTotalBytes += nLen;
		// 保存文件
		m_Inet.SaveFile(strFileName,m_strDirectory,pbyBuffer,nLen);
		bRet = TRUE;
	}
	return bRet;
}

//判断这个页面是否已经通过INet下载了，如果已经下载了，
//那么就用一个指针指向返回页面列表的该页面
BOOL CMyDoc::ShouldGetPage(CString& strPage, MAP_FILES*& pMapEntry)
{
	CString strNewPage = strPage;
	strNewPage.MakeLower();
	strNewPage = strNewPage.SpanExcluding("#");
	return(!m_arrPagesDone.Lookup(strNewPage,(CObject *&) pMapEntry));
}

//判断这个多媒体项是否已经通过INet下载了，如果已经下载了，
//那么就用一个指针指向返回多媒体项列表的该元素
BOOL CMyDoc::ShouldGetMedia(CString& strMedia, MAP_FILES*& pMapEntry)
{
	CString strNewMedia = strMedia;
	strNewMedia.MakeLower();
	strNewMedia = strNewMedia.SpanExcluding("#");
	return(!m_arrMediaDone.Lookup(strNewMedia,(CObject *&) pMapEntry));
}

// Returns TRUE if the specified page should be added to the download queue.
// This basically means that the page is not in the list of queued pages at a 
// lower level or that it hasn't been previously downloaded at a lower level.
// If the above criteria are met -- it thens check to see if this is an offsite
// page and determines whether it should be downloaded.

BOOL CMyDoc::ShouldQueuePage(CString& strNewPage, BOOL bOffsite)
{
	MAP_FILES* pMapEntry;

	// Have we downloaded this page yet???
	if(ShouldGetPage(strNewPage,pMapEntry))
	{
		// No...then look for it in the queue of waiting pages at previous
		// levels
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
		// Yes...did we follow its links all the way to the 
		// maximum level?
		if(m_Options.nMaxDepth && m_nLevel >= pMapEntry->nMaxLevel)
			return(TRUE);
	}

	// Make sure that we allow offsite links for offsite pages
	if(bOffsite && !m_Options.bOffsiteLinks)
		return(FALSE);

	return(TRUE);
}

// Initializes the specified link stack entry
void CMyDoc::ResetLink(int nLevel)
{
	m_aLinks[nLevel].nIndex = 0;
	m_aLinks[nLevel].arrLinks.SetSize(0,100);
	m_aLinks[nLevel].arrMedia.SetSize(0,100);
	m_aLinks[nLevel].arrOffsite.SetSize(0,100);
}



// The workhouse thread routine that recursively navigates linked web pages and
// retrieves each of them along with their multimedia files.  This process is
// spawned indirectrly in RecursiveDownload() using the AfxBeginThread() call.
UINT CMyDoc::DownloadThread(LPVOID lpvData)
{
	HTREEITEM htreePage;

	// Static methods can't have a "this" pointer to get the parent class's 
	// pointer which the call passes as a parameter
	CMyDoc *pThis = (CMyDoc *) lpvData;

	int nMaxDepth = pThis->m_Options.nMaxDepth-1;
	int nCount;
	CString strPage = pThis->m_strStartPage;
	CString strFileName;
	CString strLogData;
	CString strText;
	POSITION pos = pThis->GetFirstViewPosition();
	CMyView* pView = (CMyView *) pThis->GetNextView(pos);	
	BOOL bIsOffsite = FALSE;

	// Establish the WinInet Session
	try
	{
		pThis->m_Inet.OpenSession(pThis->m_Options.bUseProxy,pThis->m_Options.strProxyName);
	}
	catch(...)
	{
	}

	// Create the log file
	pThis->m_fileLog.Open(pThis->m_strDirectory+"sitesnag.log",
						CFile::modeCreate|CFile::modeWrite);

	// Create the table of contents file
	if(pThis->m_Options.bContents)
	{
		pThis->m_fileContents.Open(pThis->m_strDirectory+"SnagCon1.htm",
								CFile::modeCreate|CFile::modeWrite);

		// Add the TOC to the list of downloaded files 
		pThis->SetPageCacheEntry("snagcon1.htm","SnagCon1.htm",0);

		// Add the TOC to the tree control
		CString strTitle = "Contents Page 1 (SnagCon1.htm)";
		pView->AddTreeContent(strTitle);

		// Write the beginning of the first TOC page
		strText = "<HTML>\r\n<HEAD>\r\n<TITLE>SiteSnagger Contents</TITLE>\r\n";
		strText += "</HEAD\r\n<BODY>\r\n";
		strText += "<H1><center>SiteSnagger Table of Contents</center><br><br></H1>\r\n<UL>\r\n";
		pThis->m_fileContents.Write(strText,strText.GetLength());
	}

	// Initialize the index for the first link level, start with the first level
	pThis->m_nLevel = 0;
	pThis->m_aLinks[0].nIndex = 0;
	pThis->m_Inet.ResetUniqueCount();

	// Recusively search web links until either we've searched them all (m_nLevel is
	// -1 or if the user decides to abort
	while(pThis->m_nLevel >= 0 )
	{
		// Get the name of a new page in a second dimension element
		if(pThis->m_aLinks[pThis->m_nLevel].nIndex > 0)
		{
			// Save the URL and whether it's offsite
			int nIndex = pThis->m_aLinks[pThis->m_nLevel].nIndex;
			strPage = pThis->m_aLinks[pThis->m_nLevel].arrLinks.GetAt(nIndex);
			bIsOffsite = pThis->m_aLinks[pThis->m_nLevel].arrOffsite.GetAt(nIndex);

			// Bump to the next level so we can get the page's links
			pThis->m_nLevel++;
		}

		// Generate a unique filename for this page
		pThis->m_Inet.GenerateUniqueFileName(strPage,strFileName,
						pThis->m_arrPagesDone,TRUE);

		// Write a log entry for this page -- leave room for the result
		strLogData.Format("[%02d] Getting page %s ",pThis->m_nLevel+1,strPage);
		pThis->m_fileLog.Write(strLogData,strLogData.GetLength());

		CString strOrigPage = strPage;

		// Get the page from Inet or from local file
		if(pThis->GetPage(strPage,strFileName,pThis->m_aLinks[pThis->m_nLevel]))
		{
			MAP_FILES *pMapEntry;
		
			// Get the count of links
			nCount = pThis->m_aLinks[pThis->m_nLevel].arrLinks.GetSize();

			// Did we just download this new page??
			if(pThis->ShouldGetPage(strPage,pMapEntry))
			{
				// Yes, add it to the list of retrieved pages
				pThis->SetPageCacheEntry(strPage,strFileName,pThis->m_nLevel);

				// If the page was redirected then add its original name too
				if(strPage != strOrigPage && pThis->ShouldGetPage(strOrigPage,pMapEntry))
					pThis->SetPageCacheEntry(strOrigPage,strFileName,pThis->m_nLevel);

				// Prefix offsite pages with their URL (i.e. http://www.xxx.yyy)
				if(bIsOffsite)
					strText = strPage+" - ";
				else strText.Empty();

				// Add the page's title and local filename 
				strText += pThis->m_strPageTitle+"  ("+
							strFileName.SpanExcluding("#")+")";

				htreePage = pView->AddTreePage(strText,bIsOffsite);
				strText.Format("<a href=%s><li> %s (%s - %s)<br>\r\n",strFileName,
									pThis->m_strPageTitle,
									strFileName.SpanExcluding("#"),strPage);
				pThis->m_fileContents.Write(strText,strText.GetLength());

				// Update the statistics
				pThis->m_nGottenPageCount++;
				pThis->m_nGottenFileCount++;
			}
			else
			{
				// Set the new depth level if necessary
				if(nMaxDepth)
				{
					// Have we gone to the max level yet???
					if(pThis->m_nLevel >= pMapEntry->nMaxLevel)
						nCount = 0;
					else pMapEntry->nMaxLevel = pThis->m_nLevel;
				}
			}

			// Log the results
			pThis->m_fileLog.Write("[OK]\n",5);

			// Check for offsite links, don't follow the current page's
			// links if it is an offsite page
			if(bIsOffsite)
				nCount = 0;

			// Should we get multimedia files??
			if(pThis->m_Options.bMultimedia)
			{
				// Iterate through the list of multimedia links
				CString strMedia;
				for(int j = 0; j < pThis->m_aLinks[pThis->m_nLevel].arrMedia.GetSize(); j++)
				{
					strMedia = pThis->m_aLinks[pThis->m_nLevel].arrMedia.GetAt(j);

					// Should we get this file?
					if(pThis->ShouldGetMedia(strMedia,pMapEntry))
					{
						// Yep, make sure it has a unique name
						pThis->m_Inet.GenerateUniqueFileName(strMedia,
										strFileName,pThis->m_arrMediaDone,FALSE);

						// Log the info
						strLogData.Format("[%02d] Getting media %s ",pThis->m_nLevel,
														strMedia);
						pThis->m_fileLog.Write(strLogData,strLogData.GetLength());

						// We don't need to download EMAIL links so just make
						// them look like a successful file entry
						BOOL bMail;
						if(strMedia.Left(7) == "mailto:")
						{
							bMail = TRUE;
							strFileName = strMedia;
						}
						else bMail = FALSE;

						// Did everything work okay??
						if(bMail || pThis->GetMedia(strMedia,strFileName))
						{
							// Yep...add this file to our file list and to the tree
							pThis->SetMediaCacheEntry(strMedia,strFileName);
							pView->AddTreeMedia(strFileName.SpanExcluding("#"),
											CMyTreeCtrl::GetMediaType(strFileName));

							// Increment the statistics count
							if(!bMail)
								pThis->m_nGottenFileCount++;

							// Log the results
							pThis->m_fileLog.Write("[OK]\n",5);
						}
						else
						{
							// Log the results
							pThis->m_fileLog.Write("[FAILED] ",9);

							// Show a detailed error -- if possible
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
			// Log the results
			pThis->m_fileLog.Write("[FAILED] ",9);

			// Show a detailed error -- if possible
			CString strError = pThis->m_Inet.GetErrorText();
			pThis->m_fileLog.Write(strError,strError.GetLength());
			pThis->m_fileLog.Write("\n",1);
			
			nCount = 0;
		}

		// Make sure the statistics window is updated properly

		// If we've hit the max page count then just get out
		if(pThis->m_Options.nMaxPages > 0 && 
				pThis->m_nGottenPageCount >= pThis->m_Options.nMaxPages) 
			break;

		// Continue recursion if we haven't hit maximum depth yet
		// and as long as we have links on this page
		if(pThis->m_nLevel < nMaxDepth && nCount > 0)
		{
			// Get the next page to parse
			strPage = pThis->m_aLinks[pThis->m_nLevel].arrLinks.GetAt(0);
			bIsOffsite = pThis->m_aLinks[pThis->m_nLevel].arrOffsite.GetAt(0);

			// Move to the next level, initialize its link info
			pThis->m_nLevel++;
			pThis->ResetLink(pThis->m_nLevel);

			// Queue the links
			pThis->m_nQueuedPageCount += nCount;
			continue;
		}

		// Finished will all links on this page, reset its link info
		pThis->ResetLink(pThis->m_nLevel);

		// Move back to the previous level
		pThis->m_nLevel--;

		// Find the next page on the second dimension
		if(pThis->m_nLevel >= 0)
		{
			int nMaxCount;

			// Find another page that has links
			while(pThis->m_nLevel >= 0)
			{
				// How many second dimension entries do we have??
				nMaxCount = pThis->m_aLinks[pThis->m_nLevel].arrLinks.GetSize();

				// Did we have another valid page at this level?
				if(pThis->m_aLinks[pThis->m_nLevel].nIndex < nMaxCount-1)
				{
					// Yes, get the next page
					pThis->m_aLinks[pThis->m_nLevel].nIndex++;
					pThis->m_nQueuedPageCount--;
					break;
				}
				else 
				{
					// No, back up a level in the tree
					pThis->m_nLevel--;
					pThis->m_nQueuedPageCount--;
				}
			}
		}
	}

	// Make sure the "stopping, please wait" message isn't displayed

	// Make sure that the tree was correctly parsed
	// nNodeCount should always be 0
	pThis->m_nLevel = pThis->m_nQueuedPageCount;

	// Should we fix up the links for browsing??
	if(pThis->m_Options.bFixupLinks)
	{
		// Yes, but the user has canceled the download so ask if they want
		// to fix up anyway
		if(false&&pView->ShowPrompt(IDS_FIXUP_PAGES,MB_ICONQUESTION|MB_YESNO) != IDYES)
			;
		else

		{
			// Everything was normal so just fixup the links
			pThis->FixupLinks();
		}

	}

	// Close the log file
	pThis->m_fileLog.Close();

	// Are we doing the TOC??
	if(pThis->m_Options.bContents)
	{
		// Yep...write the bottom of it and close the file
		CString strText;
		strText = "</UL>\r\n</BODY>\r\n</HTML>\r\n";
		pThis->m_fileContents.Write(strText,strText.GetLength());
		pThis->m_fileContents.Close();
	}

	// Let the user know that we've finished
	pView->SetSnagging(FALSE);

	// Finished with the maps, so free the memory
	pThis->ClearCacheMaps();

	// Make sure the "stopping, please wait" message isn't displayed

	// Close the main window if we're in automatic mode
	if(pThis->m_bAutoMode)
		AfxGetMainWnd()->PostMessage(WM_CLOSE);

	// Shutdown the WinInet Session
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

// Removes all of the entries in the page and file download lists.  These are
// CMap's so it must navigate the linked list and delete each of the CObject
// entries.
void CMyDoc::ClearCacheMaps()
{
	MAP_FILES *pMapEntry;
	CString strPage;
	POSITION pos;
	int i;

	// Zap the page list
	pos = m_arrPagesDone.GetStartPosition();
	for(i = 0; i < m_arrPagesDone.GetCount(); i++)
	{
		m_arrPagesDone.GetNextAssoc(pos,strPage,(CObject *&) pMapEntry);
		delete pMapEntry;
	}
	m_arrPagesDone.RemoveAll();

	// Zap the multimedia list
	pos = m_arrMediaDone.GetStartPosition();
	for(i = 0; i < m_arrMediaDone.GetCount(); i++)
	{
		m_arrMediaDone.GetNextAssoc(pos,strPage,(CObject *&) pMapEntry);
		delete pMapEntry;
	}
	m_arrMediaDone.RemoveAll();
}

// Adds a new entry to the retrieved page list
void CMyDoc::SetPageCacheEntry(LPCTSTR lpszPage, LPCTSTR lpszFileName, int nLevel)
{
	CString strTempPage = lpszPage;
	MAP_FILES *pMapEntry = new MAP_FILES();

	// The page name should always be saved as lowercase
	pMapEntry->strFileName = lpszFileName;
	pMapEntry->nMaxLevel = nLevel;
	strTempPage.MakeLower();
	strTempPage = strTempPage.SpanExcluding("#");

	// Assimilate it into the collective
	m_arrPagesDone.SetAt(strTempPage,(CObject *&) pMapEntry);

	// Check for a generated default page name...if we have one then add some
	// other default names so we don't break during link fixup
	if(pMapEntry->strFileName == "index.htm" && strTempPage.Find("index.htm") == -1)
	{
		// Common default page names, for links such as "www.mycompany.com/"
		char *aszDefPages[4] = { "index.htm", "index.html", "default.htm", 
										"default.html"};

		MAP_FILES *pTempMapEntry;

		CString strServer, strObject, strUser, strPassword;
		INTERNET_PORT nPort;
		DWORD dwServiceType;
		AfxParseURLEx(strTempPage,dwServiceType,strServer,strObject,nPort,
									strUser,strPassword,
									ICU_NO_ENCODE);

		// Add the default names
		if(!strServer.IsEmpty())
		{
			strServer = "http://"+strServer+"/";

			for(int i = 0; i < 4; i++)
			{
				pTempMapEntry = new MAP_FILES();
				pTempMapEntry->strFileName = pMapEntry->strFileName;
				pTempMapEntry->nMaxLevel = pMapEntry->nMaxLevel;
				m_arrPagesDone.SetAt(strServer+aszDefPages[i],(CObject *&) pTempMapEntry);
			}
		}
	}	
}

// Adds a new entry to the retrieved file list
void CMyDoc::SetMediaCacheEntry(LPCTSTR lpszMedia, LPCTSTR lpszFileName)
{
	CString strTempMedia = lpszMedia;
	MAP_FILES *pMapEntry = new MAP_FILES();

	// The file name should always be saved as lowercase
	pMapEntry->strFileName = lpszFileName;
	pMapEntry->nMaxLevel = -1;
	strTempMedia.MakeLower();
	strTempMedia = strTempMedia.SpanExcluding("#");

	// Assimilate it into the collective
	m_arrMediaDone.SetAt(strTempMedia,(CObject *&) pMapEntry);
}

// Sets up a snagging session and calls AfxBeginThread() to start the
// real download thread routine.  This routine returns immediately once that
// thread is started, freeing up the UI.
void CMyDoc::RecursiveDownload(LPCTSTR lpszURL)
{
	// Initialize the statistics
	m_nGottenPageCount = 0;
	m_nGottenFileCount = 0;
	m_nQueuedPageCount = 0;
	m_nTotalBytes = 0;
	m_strStartPage = lpszURL;
	ClearCacheMaps();

	// Initialize the statistics window display
	POSITION pos = GetFirstViewPosition();
	CMyView* pView = (CMyView *) GetNextView(pos);

	// Start the download thread
	CWinThread *pThread = AfxBeginThread(DownloadThread,this);
}

// Fixs up each of the downloaded pages to allow local browsing.  It
// basically goes through the downloaded page list and opens each 
// file specified then parses the page and fixes up each of the 
// links found.
void CMyDoc::FixupLinks()
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

	// Update the statistics window to indicate the number of files
	// we have left to do

	// Iterate through the downloaded page list
	POSITION pos = m_arrPagesDone.GetStartPosition();
	for(int i = 0; i < m_arrPagesDone.GetCount(); i++)
	{
		m_arrPagesDone.GetNextAssoc(pos,strPage,(CObject *&) pMapEntry);

		if(pMapEntry->strFileName == "index.htm" || 
						pMapEntry->strFileName == "index.html")
		{
			if(bIndexDone)
			{
				// Decrement the queued file count in the stastics window
				continue;
			}	
			else bIndexDone = TRUE;
		}

		// Display the current file in that statistics window
		strFileName = m_strDirectory+pMapEntry->strFileName;

		// Open the local file for create 
		if(fileIn.Open(strFileName.SpanExcluding("#"),CFile::modeRead,&ex))
		{
			pbyBuffer = m_byBuffer;

			// Read the file into memory
			nLen = fileIn.Read(pbyBuffer,MAX_INET_BUFFER);
			fileIn.Close();

			// Parse it (tell the parser to look for only links, i.e. fixup mode)
			CHTMLFileParser Parser;
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

			// Open a temporary workfile
			if(fileOut.Open(strTempName,CFile::modeCreate|CFile::modeWrite,&ex))
			{
				CString strURL;

				nIndex = 0;
				nBytesWritten = 0;
				nOffset = 0;

				// Write the file and perform fix-ups (if necessary)
				for(int j = 0; j < Parser.GetFixups().arrURL.GetSize(); j++)
				{
					strURL = Parser.GetFixups().arrURL.GetAt(j);

					// Text page fixup??
					if(Parser.GetFixups().arrTextPage.GetAt(j))
					{
						// Did we download this page??
						if(!ShouldGetPage(strURL,pMapEntry))
						{
							// Yep...get it's local filename
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
					else // Media fixup
					{
						// Did we download this multimedia file??
						if(!ShouldGetMedia(strURL,pMapEntry))
						{
							// Yep...get it's local filename
							strFixupFileName = pMapEntry->strFileName;
							bFixup = TRUE;
						}
						else bFixup = FALSE;
					}

					// Write the data leading up to the reference that we are
					// going to fix up
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

					// Do we have an entry to fix up??
					if(bFixup)
					{
						// Yes, write the local filename
						try
						{
							fileOut.Write(" \"",2);
							fileOut.Write(strFixupFileName,strFixupFileName.GetLength());
						}
						catch(...)
						{
						}

						// Find the start of the old filename
						while(*pbyBuffer == ' ') 
						{
							pbyBuffer++;
							nBytesWritten++;
							nOffset++;
						}

						// Is the filename in quotes?
						if(*pbyBuffer == '"') 
						{
							// Yep, look for the end
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

					// Save the location where this fixup started
					nIndex = Parser.GetFixups().arrIndex.GetAt(j);
				}

				// Finished fixing up, so write the rest of the data left
				// in the file buffer
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

		// Replace the original page with this new fixed up page
		remove(strFileName.SpanExcluding("#"));
		rename(strTempName,strFileName.SpanExcluding("#"));

		// Decrement the queued file count in the stastics window
	}

	// Get rid if any leftover work files (this might happen if the
	// user aborted in the middle)
	remove(strTempName);
}

/////////////////////////////////////////////////////////////////////////////
// CMyDoc diagnostics

#ifdef _DEBUG

// Check validity of object
void CMyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

// Dump the object
void CMyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

