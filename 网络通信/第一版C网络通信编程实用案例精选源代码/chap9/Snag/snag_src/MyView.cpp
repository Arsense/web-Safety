#include "stdafx.h"

#include <afxadv.h>

#include <io.h>
#include <direct.h>

#include "SiteDownload.h"
#include "MyDocument.h"
#include "MyView.h"
#include "InputURLDlg.h"
#include "ConfigureSetting.h"
#include "InputProjectDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const CString g_strProjDir = "Projects\\";

/////////////////////////////////////////////////////////////////////////////
// CMyView

IMPLEMENT_DYNCREATE(CMyView, CView)

BEGIN_MESSAGE_MAP(CMyView, CView)
	//{{AFX_MSG_MAP(CMyView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	ON_COMMAND(ID_FILE_OPTIONS, OnFileOptions)
	ON_COMMAND(ID_SNAG, OnSnag)
	ON_UPDATE_COMMAND_UI(ID_SNAG, OnUpdateSnag)
	ON_COMMAND(ID_STOP, OnStop)
	ON_UPDATE_COMMAND_UI(ID_STOP, OnUpdateStop)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPTIONS, OnUpdateFileOptions)
	ON_COMMAND(ID_FILE_REMOVEALL, OnFileRemoveall)
	ON_UPDATE_COMMAND_UI(ID_FILE_REMOVEALL, OnUpdateFileRemoveall)
	ON_UPDATE_COMMAND_UI(ID_APP_EXIT, OnUpdateAppExit)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_RENAME, OnFileRename)
	ON_UPDATE_COMMAND_UI(ID_FILE_RENAME, OnUpdateFileRename)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_SHOW_PROMPT,OnShowPrompt)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, OnUpdateFileMRU)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyView construction/destruction

// Constructor
CMyView::CMyView()
{
	m_bSnagging = FALSE;
}

// Destructor
CMyView::~CMyView()
{
}

// Called to modify window creation characteristics
BOOL CMyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyView drawing

// Draws the view -- not used
void CMyView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CMyView diagnostics

#ifdef _DEBUG
void CMyView::AssertValid() const
{
	CView::AssertValid();
}

void CMyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyDoc* CMyView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyDoc)));
	return (CMyDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyView message handlers

// Called when a new view is created -- for SDI applications this is called
// multiple times.  Therefore, we set a static flag and create the tree
// control the first time the view is initialized.
void CMyView::OnInitialUpdate() 
{
	static BOOL bFirstTime = TRUE;

	CView::OnInitialUpdate();

	// Is this the first time?
	if(bFirstTime)
	{
		// Yes, create the tree control
		CRect rc;
		GetClientRect(&rc);
		m_Tree.Create(TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT|WS_VISIBLE,
							rc,this,IDC_TREE);
		bFirstTime = FALSE;

		// Cyan is the transparent color in this bitmap
		m_ImageList.Create(IDB_TREE,16,1,RGB(0,128,128));
		m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);

		// Create the child progress window too
	}
}

//初始化树形控件的显示

//    根(站点名称) |
//                 |-----文件目录
//                 |-----多媒体/APPLET文件
//                 |-----网页/asp/文本文件
void CMyView::InitTree(CString& strSite)
{
	CMyDoc* pDoc = GetDocument();
	m_htreeRoot = m_htreeMedia = m_htreePages = 0;
	m_Tree.DeleteAllItems();
	m_htreeRoot = m_Tree.AddRootItem(strSite,0);
	if(pDoc->GetOptions()->bContents)
		m_htreeContents = m_Tree.AddChildItem("文件目录",
						m_htreeRoot,CMyTreeCtrl::TOC_NODE);
	if(pDoc->GetOptions()->bMultimedia)
		m_htreeMedia = m_Tree.AddChildItem("多媒体/APPLET文件",m_htreeRoot,
						CMyTreeCtrl::MEDIA_NODE);
	m_htreePages = m_Tree.AddChildItem("网页/asp/文本文件",m_htreeRoot,
						CMyTreeCtrl::PAGES_NODE);
}

//遍历树形控件项htreeParent的子项，并把结果放入加入arrstrEntries数组中
void CMyView::GetChildren(HTREEITEM htreeParent, CStringArray& arrstrEntries)
{
	arrstrEntries.RemoveAll();
	if(htreeParent == NULL) 
		return;
	HTREEITEM htreeItem = m_Tree.GetNextItem(htreeParent,TVGN_CHILD);
	while(htreeItem)
	{
		arrstrEntries.Add(m_Tree.GetItemText(htreeItem));
		htreeItem = m_Tree.GetNextSiblingItem(htreeItem);
	}
}

// 加入子项(由arrstrEntries这个字符串数组保存的)到特定的树形控件项中htreeParent
void CMyView::AddChildren(HTREEITEM htreeParent, CStringArray& arrstrEntries,int nPageType)
{
	if(htreeParent == NULL) 
		return;
	BOOL bPageAdjust = (nPageType == CMyTreeCtrl::PAGE_NODE);
	for(int i = 0; i < arrstrEntries.GetSize(); i++)
	{
		if(bPageAdjust)
		{
			if(arrstrEntries[i].Left(4) == "http")
				nPageType = CMyTreeCtrl::OFFSITE_PAGE_NODE;
			else 
				nPageType = CMyTreeCtrl::PAGE_NODE;
		}
		else 
			nPageType = CMyTreeCtrl::GetMediaType(arrstrEntries[i]);

		m_Tree.AddChildItem(arrstrEntries[i],htreeParent,nPageType);
	}
}

//清空树形控件的所有内容
void CMyView::ClearTree() 
{ 
	if(::IsWindow(m_Tree.GetSafeHwnd()))
		m_Tree.DeleteAllItems(); 

	m_htreeRoot =  NULL;
	m_htreeContents =  NULL;
	m_htreeMedia =  NULL;
	m_htreePages = NULL;
}

//处理树形控件的存储和读取
void CMyView::SerializeTree(CArchive& ar)
{
	CString strSite;
	CMyDoc* pDoc = (CMyDoc *) ar.m_pDocument;
	CStringArray arrstrEntries;

	if(ar.IsStoring())
	{
		strSite = pDoc->GetStartPage();
		ar << strSite;

		GetChildren(m_htreeContents,arrstrEntries);
		arrstrEntries.Serialize(ar);

		GetChildren(m_htreeMedia,arrstrEntries);
		arrstrEntries.Serialize(ar);

		GetChildren(m_htreePages,arrstrEntries);
		arrstrEntries.Serialize(ar);
	}
	else
	{
		ar >> strSite;
		pDoc->SetStartPage(strSite);

		if(!strSite.IsEmpty())
			InitTree(strSite);

		arrstrEntries.Serialize(ar);
		AddChildren(m_htreeContents,arrstrEntries,CMyTreeCtrl::TOC_PAGE_NODE);

		arrstrEntries.Serialize(ar);
		AddChildren(m_htreeMedia,arrstrEntries,CMyTreeCtrl::IMAGE_NODE);

		arrstrEntries.Serialize(ar);
		AddChildren(m_htreePages,arrstrEntries,CMyTreeCtrl::PAGE_NODE);
	}
}

//加入多媒体资源到树形控件的多媒体子项
HTREEITEM CMyView::AddTreeMedia(CString strMedia, int nMediaType)
{
	return m_Tree.AddChildItem(strMedia,m_htreeMedia,nMediaType);
}

//加入网页/asp/文本文件到树形控件的网页/asp/文本子项
HTREEITEM CMyView::AddTreePage(CString& strPage, BOOL bOffsite)
{
	int nPageType = bOffsite?CMyTreeCtrl::OFFSITE_PAGE_NODE: CMyTreeCtrl::PAGE_NODE;
	return m_Tree.AddChildItem(strPage,m_htreePages,nPageType);
}

//加入文件目录到树形控件的文件目录子项
HTREEITEM CMyView::AddTreeContent(CString& strPage)
{
	return m_Tree.AddChildItem(strPage,m_htreeContents,CMyTreeCtrl::TOC_PAGE_NODE);
}

void CMyView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
}

//处理树形控件的双击消息
void CMyView::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPoint pt;
	UINT uFlags = TVHT_ONITEMLABEL; 
	CString strText;

	GetCursorPos(&pt);
	m_Tree.ScreenToClient(&pt);

	HTREEITEM htreeItem = m_Tree.HitTest(pt,&uFlags);

	// Make sure it's not just a category entry that the user clicked on
	if(htreeItem && htreeItem != m_htreeRoot && htreeItem != m_htreeContents && 
				htreeItem != m_htreeMedia && htreeItem != m_htreePages)
	{
		// It's a valid file so get its name
		strText = m_Tree.GetItemText(htreeItem);
		int nIndex = strText.ReverseFind('(');
		if(nIndex >= 0)
		{
			strText = strText.Mid(nIndex+1);
			strText = strText.Left(strText.GetLength()-1);
		}

		CMyDoc *pDoc = GetDocument();

		// Don't pre-pend the path for an EMAIL entry
		if(strText.Left(7) != "mailto:")
			strText = pDoc->GetDirectory()+strText;

		// Let the shell invoke the proper handler for this file
		ShellExecute(NULL,"open",strText,NULL,NULL,SW_SHOWNORMAL);

		*pResult = 1;
	}
	else 
		*pResult = 0;
}

//更改网站下载的配置信息
void CMyView::OnFileOptions() 
{
	CMyDoc* pDoc = GetDocument();

	CConfigureDlg cOptions(pDoc->GetOptions());

	if(cOptions.DoModal() == IDOK)
	{
		pDoc->SetModifiedFlag(TRUE);
	}
}

//更改网站下载的配置信息
void CMyView::OnUpdateFileOptions(CCmdUI* pCmdUI) 
{
	CMyDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->ProjectLoaded() && !m_bSnagging);
}

void CMyView::OnSnag() 
{
	CMyDoc *pDoc = GetDocument();

	if(!TreeIsEmpty())
	{
		if(AfxMessageBox("当前项目中已经有一个网站，继续将会覆盖原来的下载.\nContinue?",MB_YESNO|MB_ICONQUESTION) != IDYES)
			return;
		CString strPath = CInternetDownload::SplitFileName(pDoc->GetPathName(),CInternetDownload::DRIVE|CInternetDownload::PATH|CInternetDownload::FNAME)+"\\";
		ClearProject(strPath);
		pDoc->SetModifiedFlag(TRUE);
	}

	CURLDlg dlgURL;
	dlgURL.SetMaxLevels(pDoc->GetOptions()->nMaxDepth);
	dlgURL.SetMaxPages(pDoc->GetOptions()->nMaxPages);
	dlgURL.SetGetMultimedia(pDoc->GetOptions()->bMultimedia);
	dlgURL.SetURL(pDoc->GetStartPage());

	if(dlgURL.DoModal() == IDOK)
	{
		CString strURL = dlgURL.GetURL();

		InitTree(strURL);
		pDoc->SetModifiedFlag(TRUE);

		pDoc->RecursiveDownload(strURL);
		m_bSnagging = TRUE;
	}
}

void CMyView::OnUpdateSnag(CCmdUI* pCmdUI) 
{
	CMyDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->ProjectLoaded() && !m_bSnagging);
}

void CMyView::OnStop() 
{
	CMyDoc* pDoc = GetDocument();
	pDoc->SetAutoMode(FALSE);

}

void CMyView::OnUpdateStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bSnagging);
}

void CMyView::ClearProject(LPCTSTR lpszDir)
{
	CString strTemp = lpszDir+CString("*.*");

	WIN32_FIND_DATA findData;

	HANDLE hFind = FindFirstFile(strTemp,&findData);

	if(hFind)
	{
		while(TRUE)
		{
			strTemp = CString(lpszDir)+findData.cFileName;
			remove(strTemp);

			if(!FindNextFile(hFind,&findData))
				break;
		}

		FindClose(hFind);
	}
}

LRESULT CMyView::OnShowPrompt(WPARAM wParam, LPARAM lParam)
{
	return AfxMessageBox(wParam,lParam);
}

int CMyView::ShowPrompt(int nID, int nOptions)
{
	return(SendMessage(UM_SHOW_PROMPT,nID,nOptions));
}

void CMyView::OnFileRemoveall() 
{
	if(AfxMessageBox("You have chosen to remove all the files for this project.\nAre you sure?",
				MB_ICONQUESTION|MB_YESNOCANCEL) == IDYES)
	{
		CMyDoc* pDoc = GetDocument();
		CString strPath = CInternetDownload::SplitFileName(pDoc->GetPathName(),
						CInternetDownload::DRIVE|CInternetDownload::PATH|CInternetDownload::FNAME)+"\\";
		ClearProject(strPath);

		rmdir(strPath);

		ClearTree();

		strPath = CInternetDownload::SplitFileName(pDoc->GetPathName(),
				CInternetDownload::DRIVE|CInternetDownload::PATH|CInternetDownload::FNAME|CInternetDownload::EXT);

		pDoc->Reset();

		remove(strPath);

		// Remove the project's entry in the MRU list
		CSiteDownloadApp *pApp = (CSiteDownloadApp *) AfxGetApp();
		CRecentFileList *pMRUList = pApp->GetMRUList();
		pMRUList->Remove(0);

	}
}

void CMyView::OnUpdateFileRemoveall(CCmdUI* pCmdUI) 
{
	CMyDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->ProjectLoaded() && !m_bSnagging);
}

void CMyView::OnUpdateAppExit(CCmdUI* pCmdUI) 
{
	CMyDoc* pDoc = GetDocument();
	pCmdUI->Enable(!m_bSnagging);
}

void CMyView::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	CMyDoc* pDoc = GetDocument();
	pCmdUI->Enable(!m_bSnagging);
}

void CMyView::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	CMyDoc* pDoc = GetDocument();
	pCmdUI->Enable(!m_bSnagging);
}

void CMyView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	CMyDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->ProjectLoaded() && !m_bSnagging);
}

void CMyView::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	CMyDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->ProjectLoaded() && !m_bSnagging);
}

void CMyView::OnFileNew() 
{
	CMyDoc* pDoc = GetDocument();

	// Display the Create Project dialogbox
	CInputProjectDlg dlgProject;

	// Did the user press OK?
	if(dlgProject.DoModal() == IDOK)
	{
		CString strTestName = g_strProjDir+dlgProject.GetProjectName()+CString(".sng");

		// Does this project already exist?
		if(access(strTestName,0) == 0)
		{
			// Yes, ask the user whether it's okay to replace it
			if(AfxMessageBox("A project already exists with this name do you want to replace it?",
						MB_ICONQUESTION|MB_YESNOCANCEL) != IDYES)
				return;
		}

		// Make sure we save the current project
		pDoc->SaveModified();

		// Get the new project name and create the project file and the 
		// new subdirectory
		CString strPathName = g_strProjDir+dlgProject.GetProjectName()+CString(".sng");
		mkdir(g_strProjDir+dlgProject.GetProjectName());

		// Initialize the tree
		ClearTree();

		// Create the new document
		pDoc->OnNewDocument();

		// Save the new document title
		pDoc->SetPathName(strPathName);
		pDoc->SetTitle(dlgProject.GetProjectName());
		pDoc->SetModifiedFlag(TRUE);

		pDoc->DoSave(strPathName);
		pDoc->SetDirectory(g_strProjDir+dlgProject.GetProjectName()+
									CString("\\"));
	}
}

void CMyView::OnFileOpen() 
{
	if(m_bSnagging) return;

	CFileDialog fileOpen(TRUE,".sng",NULL, 
		OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|
		OFN_NONETWORKBUTTON,"SiteSnagger projects (*.sng)|*.sng||",this);

	fileOpen.m_ofn.lpstrInitialDir = g_strProjDir;

	if(fileOpen.DoModal() == IDOK)
	{
		CMyDoc* pDoc = GetDocument();

		// Open the new document
		pDoc->OnOpenDocument(fileOpen.GetPathName());
	}
}

void CMyView::OnFileRename() 
{
	CMyDoc* pDoc = GetDocument();

	// Display the Create Project dialogbox
	CInputProjectDlg dlgProject;

	CString strCurrProject = CInternetDownload::SplitFileName(pDoc->GetPathName(),
							CInternetDownload::DRIVE|CInternetDownload::PATH|CInternetDownload::FNAME);

	// Set the proper title
	dlgProject.SetTitle("Renaming "+pDoc->GetTitle());

	// Did the user press OK?
	if(dlgProject.DoModal() == IDOK)
	{
		CString strNewProject = g_strProjDir+dlgProject.GetProjectName();

		// Does this directory or project already exist?
		if(access(strNewProject,0) == 0 || access(strNewProject+".sng",0) == 0)
		{
			// Yes, don't allow the rename
			AfxMessageBox("Sorry, a project already exists with this name.",
								MB_ICONWARNING|MB_OK);
		}
		else
		{
			// Remove the project's entry in the MRU list
			CSiteDownloadApp *pApp = (CSiteDownloadApp *) AfxGetApp();
			CRecentFileList *pMRUList = pApp->GetMRUList();
			pMRUList->Remove(0);

			// Rename the in-memory project
			pDoc->SetTitle(dlgProject.GetProjectName()+".sng");
			pDoc->SetPathName(strNewProject+".sng");

			// Rename the old project on disk
			rename(strCurrProject+".sng",strNewProject+".sng");

			// Rename the directory
			rename(strCurrProject,strNewProject);

			// Make sure the new project gets saved
			pDoc->SetModifiedFlag(TRUE);
		}
	}
}

void CMyView::OnUpdateFileRename(CCmdUI* pCmdUI) 
{
	CMyDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->ProjectLoaded() && !m_bSnagging);
}

void CMyView::OnUpdateFileMRU(CCmdUI* pCmdUI) 
{
	// Load the most recently used file list
	CSiteDownloadApp *pApp = (CSiteDownloadApp *) AfxGetApp();
	pApp->UpdateMRU(pCmdUI);

	// Disable the MRU entries if necessary
	if(m_bSnagging)
	{
		for(int i = ID_FILE_MRU_FIRST; i <= ID_FILE_MRU_LAST; i++)
			pCmdUI->m_pMenu->EnableMenuItem(i,
							MF_DISABLED|MF_GRAYED|MF_BYCOMMAND);
	}
}

void CMyView::OnFileClose() 
{
	CMyDoc* pDoc = GetDocument();
	pDoc->SaveModified();
	pDoc->Reset();
	ClearTree();
}

void CMyView::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	CMyDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->ProjectLoaded() && !m_bSnagging);
}

