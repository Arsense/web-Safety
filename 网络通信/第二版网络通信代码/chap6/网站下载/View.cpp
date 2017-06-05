/*
	View.cpp : implementation of the CSnaggerView class

	Implements the view class which is responsible for most of the user
	interface presentation.

	Author: Steven E. Sipe
*/

#include "stdafx.h"//

#include <afxadv.h>//

#include <io.h>//
#include <direct.h>//

#include "SiteSnag.h"//
#include "Document.h"//
#include "View.h"//
#include "urldlg.h"//
#include "options.h"//
#include "progress.h"//
#include "wait.h"//
#include "project.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const CString g_strProjDir = "Projects\\";

/////////////////////////////////////////////////////////////////////////////
// CSnaggerView

IMPLEMENT_DYNCREATE(CSnaggerView, CView)

BEGIN_MESSAGE_MAP(CSnaggerView, CView)
	//{{AFX_MSG_MAP(CSnaggerView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	ON_COMMAND(ID_STOP, OnStop)
	ON_UPDATE_COMMAND_UI(ID_STOP, OnUpdateStop)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPTIONS, OnUpdateFileOptions)
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
	ON_COMMAND(ID_SITEINPUT, OnSiteinput)
	ON_COMMAND(ID_PARA, OnPara)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_COMMAND(ID_RENAME, OnRename)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_END_WAIT,OnHandleEndWait)
	ON_MESSAGE(UM_SHOW_PROMPT,OnShowPrompt)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, OnUpdateFileMRU)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSnaggerView construction/destruction

// Constructor
CSnaggerView::CSnaggerView()
{
	m_pProgress = NULL;
	m_pWait = NULL;
	m_bSnagging = FALSE;
}

// Destructor
CSnaggerView::~CSnaggerView()
{
	delete m_pProgress;
}

// Called to modify window creation characteristics
BOOL CSnaggerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSnaggerView drawing

// Draws the view -- not used
void CSnaggerView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CSnaggerView diagnostics

#ifdef _DEBUG
void CSnaggerView::AssertValid() const
{
	CView::AssertValid();
}

void CSnaggerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSnaggerDoc* CSnaggerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSnaggerDoc)));
	return (CSnaggerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSnaggerView message handlers

// Called when a new view is created -- for SDI applications this is called
// multiple times.  Therefore, we set a static flag and create the tree
// control the first time the view is initialized.
void CSnaggerView::OnInitialUpdate() 
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
		CRect rcProg;
		m_pProgress = new CProgress(this);
		m_pProgress->GetClientRect(rcProg);
		m_pProgress->MoveWindow(rc.left,rc.bottom-rcProg.Height(),
							rc.Width(),rcProg.Height());
		m_pProgress->ShowWindow(SW_SHOWNORMAL);
		m_pProgress->UpdateWindow();
	}
}

// Intializes the tree control by removing all of the items and adding
// TOC, Multimedia and page items based on the current option settings
void CSnaggerView::InitTree(CString& strSite)
{
	CSnaggerDoc* pDoc = GetDocument();

	// Initialize the tree
	m_htreeRoot = m_htreeMedia = m_htreePages = 0;
	m_Tree.DeleteAllItems();

	// Add the main node types based on the options settings
	m_htreeRoot = m_Tree.AddRootItem(strSite,0);
	if(pDoc->GetOptions()->bContents)
		m_htreeContents = m_Tree.AddChildItem("SiteSnagger Table of Contents",
						m_htreeRoot,CTree::TOC_NODE);
	if(pDoc->GetOptions()->bMultimedia)
		m_htreeMedia = m_Tree.AddChildItem("Multimedia",m_htreeRoot,
						CTree::MEDIA_NODE);
	m_htreePages = m_Tree.AddChildItem("Pages",m_htreeRoot,
						CTree::PAGES_NODE);
}

// Returns a list of all of the child items under the specified node.  This
// is used during serialization
void CSnaggerView::GetChildren(HTREEITEM htreeParent, CStringArray& arrstrEntries)
{
	int nIndex = 0;

	// Empty the list
	arrstrEntries.RemoveAll();
	if(htreeParent == NULL) return;

	// Iterate through the list of child items
	HTREEITEM htreeItem = m_Tree.GetNextItem(htreeParent,TVGN_CHILD);

	while(htreeItem)
	{
		arrstrEntries.Add(m_Tree.GetItemText(htreeItem));
		htreeItem = m_Tree.GetNextSiblingItem(htreeItem);
		nIndex++;
	}
}

// Adds the list of child items to the specified node
void CSnaggerView::AddChildren(HTREEITEM htreeParent, CStringArray& arrstrEntries,
									int nPageType)
{
	if(htreeParent == NULL) return;
	BOOL bPageAdjust = (nPageType == CTree::PAGE_NODE);

	// Add each of the items to the tree
	for(int i = 0; i < arrstrEntries.GetSize(); i++)
	{
		if(bPageAdjust)
		{
			if(arrstrEntries[i].Left(4) == "http")
				nPageType = CTree::OFFSITE_PAGE_NODE;
			else nPageType = CTree::PAGE_NODE;
		}
		else nPageType = CTree::GetMediaType(arrstrEntries[i]);

		m_Tree.AddChildItem(arrstrEntries[i],htreeParent,nPageType);
	}
}

// Removes everything from the tree control
void CSnaggerView::ClearTree() 
{ 
	if(::IsWindow(m_Tree.GetSafeHwnd()))
		m_Tree.DeleteAllItems(); 

	m_htreeRoot =  NULL;
	m_htreeContents =  NULL;
	m_htreeMedia =  NULL;
	m_htreePages = NULL;
}

// Handles storing or loading the contents of the tree control
void CSnaggerView::SerializeTree(CArchive& ar)
{
	CString strSite;
	CSnaggerDoc* pDoc = (CSnaggerDoc *) ar.m_pDocument;
	CStringArray arrstrEntries;

	// Storing??
	if(ar.IsStoring())
	{
		// Get the site name
		strSite = pDoc->GetStartPage();
		ar << strSite;

		// Contents
		GetChildren(m_htreeContents,arrstrEntries);
		arrstrEntries.Serialize(ar);

		// Media
		GetChildren(m_htreeMedia,arrstrEntries);
		arrstrEntries.Serialize(ar);

		// Pages
		GetChildren(m_htreePages,arrstrEntries);
		arrstrEntries.Serialize(ar);
	}
	else
	{
		// Get the site
		ar >> strSite;
		pDoc->SetStartPage(strSite);

		// Initialize an empty tree
		if(!strSite.IsEmpty())
			InitTree(strSite);

		// Contents
		arrstrEntries.Serialize(ar);
		AddChildren(m_htreeContents,arrstrEntries,CTree::TOC_PAGE_NODE);

		// Media
		arrstrEntries.Serialize(ar);
		AddChildren(m_htreeMedia,arrstrEntries,CTree::IMAGE_NODE);

		// Pages
		arrstrEntries.Serialize(ar);
		AddChildren(m_htreePages,arrstrEntries,CTree::PAGE_NODE);
	}
}

// Adds media to the tree
HTREEITEM CSnaggerView::AddTreeMedia(CString strMedia, int nMediaType)
{
	return m_Tree.AddChildItem(strMedia,m_htreeMedia,nMediaType);
}

// Adds a page to the tree
HTREEITEM CSnaggerView::AddTreePage(CString& strPage, BOOL bOffsite)
{
	int nPageType = bOffsite?CTree::OFFSITE_PAGE_NODE:
							 CTree::PAGE_NODE;

	return m_Tree.AddChildItem(strPage,m_htreePages,nPageType);
}

// Adds a TOC page to the tree
HTREEITEM CSnaggerView::AddTreeContent(CString& strPage)
{
	return m_Tree.AddChildItem(strPage,m_htreeContents,CTree::TOC_PAGE_NODE);
}

// Displays the please wait message box

// Handles the please wait termination message
LONG CSnaggerView::OnHandleEndWait(WPARAM,LPARAM)
{
	if(m_pWait)
	{
		m_pWait->DestroyWindow();
		delete m_pWait;
		m_pWait = NULL;
	}

	return(0L);
}

// Ends the please wait message by posting a message -- this is done because
// the please wait message is ended from another thread so we can't call the
// DestroyWindow() directly
void CSnaggerView::EndWait()
{
	SendMessage(UM_END_WAIT);
}

// Handles resize of the view -- also resizes the child statistics window
void CSnaggerView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	CRect rcProg, rc;

	// Is the stats window there yet??
	if(m_pProgress)
	{
		// Yep...resize it and the tree control
		m_pProgress->GetWindowRect(rcProg);
		m_pProgress->MoveWindow(0,cy-rcProg.Height(),cx,rcProg.Height());
		m_Tree.MoveWindow(0,0,cx,cy-rcProg.Height());
	}
}

// Handles the double-click message for the tree control -- invokes the appropriate
// handler based on the object type
void CSnaggerView::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Get the URL and open it in the web browser
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

		CSnaggerDoc *pDoc = GetDocument();

		// Don't pre-pend the path for an EMAIL entry
		if(strText.Left(7) != "mailto:")
			strText = pDoc->GetDirectory()+strText;

		// Let the shell invoke the proper handler for this file
		ShellExecute(NULL,"open",strText,NULL,NULL,SW_SHOWNORMAL);

		*pResult = 1;
	}
	else *pResult = 0;
}



void CSnaggerView::OnUpdateFileOptions(CCmdUI* pCmdUI) 
{
	CSnaggerDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->ProjectLoaded() && !m_bSnagging);
}




void CSnaggerView::OnStop() 
{
	CSnaggerDoc* pDoc = GetDocument();
	pDoc->SetAutoMode(FALSE);

//	StartWait("Stopping, Please Wait (this may take a little time).");
	m_pProgress->SetAborted(TRUE);
}

void CSnaggerView::OnUpdateStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bSnagging);
}

void CSnaggerView::ClearProject(LPCTSTR lpszDir)
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

LRESULT CSnaggerView::OnShowPrompt(WPARAM wParam, LPARAM lParam)
{
	return AfxMessageBox(wParam,lParam);
}

int CSnaggerView::ShowPrompt(int nID, int nOptions)
{
	return(SendMessage(UM_SHOW_PROMPT,nID,nOptions));
}


void CSnaggerView::OnUpdateFileRemoveall(CCmdUI* pCmdUI) 
{
	CSnaggerDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->ProjectLoaded() && !m_bSnagging);
}

void CSnaggerView::OnUpdateAppExit(CCmdUI* pCmdUI) 
{
	CSnaggerDoc* pDoc = GetDocument();
	pCmdUI->Enable(!m_bSnagging);
}

void CSnaggerView::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	CSnaggerDoc* pDoc = GetDocument();
	pCmdUI->Enable(!m_bSnagging);
}

void CSnaggerView::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	CSnaggerDoc* pDoc = GetDocument();
	pCmdUI->Enable(!m_bSnagging);
}

void CSnaggerView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	CSnaggerDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->ProjectLoaded() && !m_bSnagging);
}

void CSnaggerView::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	CSnaggerDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->ProjectLoaded() && !m_bSnagging);
}

void CSnaggerView::OnFileNew() 
{
	CSnaggerDoc* pDoc = GetDocument();

	// Display the Create Project dialogbox
	CProjectDlg dlgProject;

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

void CSnaggerView::OnFileOpen() 
{
	if(m_bSnagging) return;

	CFileDialog fileOpen(TRUE,".sng",NULL, 
		OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|
		OFN_NONETWORKBUTTON,"SiteSnagger projects (*.sng)|*.sng||",this);

	fileOpen.m_ofn.lpstrInitialDir = g_strProjDir;

	if(fileOpen.DoModal() == IDOK)
	{
		CSnaggerDoc* pDoc = GetDocument();

		// Open the new document
		pDoc->OnOpenDocument(fileOpen.GetPathName());
	}
}

void CSnaggerView::OnFileRename() 
{
	CSnaggerDoc* pDoc = GetDocument();

	// Display the Create Project dialogbox
	CProjectDlg dlgProject;

	CString strCurrProject = CInet::SplitFileName(pDoc->GetPathName(),
							CInet::DRIVE|CInet::PATH|CInet::FNAME);

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
			CSnaggerApp *pApp = (CSnaggerApp *) AfxGetApp();
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

void CSnaggerView::OnUpdateFileRename(CCmdUI* pCmdUI) 
{
	CSnaggerDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->ProjectLoaded() && !m_bSnagging);
}

void CSnaggerView::OnUpdateFileMRU(CCmdUI* pCmdUI) 
{
	// Load the most recently used file list
	CSnaggerApp *pApp = (CSnaggerApp *) AfxGetApp();
	pApp->UpdateMRU(pCmdUI);

	// Disable the MRU entries if necessary
	if(m_bSnagging)
	{
		for(int i = ID_FILE_MRU_FIRST; i <= ID_FILE_MRU_LAST; i++)
			pCmdUI->m_pMenu->EnableMenuItem(i,
							MF_DISABLED|MF_GRAYED|MF_BYCOMMAND);
	}
}

void CSnaggerView::OnFileClose() 
{
	CSnaggerDoc* pDoc = GetDocument();
	pDoc->SaveModified();
	pDoc->Reset();
	ClearTree();
}

void CSnaggerView::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	CSnaggerDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->ProjectLoaded() && !m_bSnagging);
}


void CSnaggerView::OnSiteinput() 
{
	// TODO: Add your command handler code here
		CSnaggerDoc *pDoc = GetDocument();

	if(!TreeIsEmpty())
	{
		if(AfxMessageBox("The current project already contains a site.  If you continue it will be replaced.\nContinue?",
						MB_YESNO|MB_ICONQUESTION) != IDYES)
			return;

		CString strPath = CInet::SplitFileName(pDoc->GetPathName(),
						CInet::DRIVE|CInet::PATH|CInet::FNAME)+"\\";
		ClearProject(strPath);
		pDoc->SetModifiedFlag(TRUE);
	}

	// 弹出设定对话框
	CURLDlg dlgURL;

	dlgURL.SetMaxLevels(pDoc->GetOptions()->nMaxDepth);
	dlgURL.SetMaxPages(pDoc->GetOptions()->nMaxPages);
	dlgURL.SetGetMultimedia(pDoc->GetOptions()->bMultimedia);
	dlgURL.SetURL(pDoc->GetStartPage());

	if(dlgURL.DoModal() == IDOK)
	{
		CString strURL = dlgURL.GetURL();

		// validate the URL here!!!

		InitTree(strURL);
		pDoc->SetModifiedFlag(TRUE);

		pDoc->RecursiveDownload(strURL);
		m_bSnagging = TRUE;
	}
	
}

void CSnaggerView::OnPara() 
{
	// TODO: Add your command handler code here
	CSnaggerDoc* pDoc = GetDocument();

	COptionsDlg cOptions(pDoc->GetOptions());

	if(cOptions.DoModal() == IDOK)
	{
		pDoc->SetModifiedFlag(TRUE);
	}	
}

void CSnaggerView::OnDelete() 
{
	// TODO: Add your command handler code here
		if(AfxMessageBox("You have chosen to remove all the files for this project.\nAre you sure?",
				MB_ICONQUESTION|MB_YESNOCANCEL) == IDYES)
	{
		CSnaggerDoc* pDoc = GetDocument();
		CString strPath = CInet::SplitFileName(pDoc->GetPathName(),
						CInet::DRIVE|CInet::PATH|CInet::FNAME)+"\\";
		ClearProject(strPath);

		rmdir(strPath);

		ClearTree();

		strPath = CInet::SplitFileName(pDoc->GetPathName(),
				CInet::DRIVE|CInet::PATH|CInet::FNAME|CInet::EXT);

		pDoc->Reset();

		remove(strPath);

		// Remove the project's entry in the MRU list
		CSnaggerApp *pApp = (CSnaggerApp *) AfxGetApp();
		CRecentFileList *pMRUList = pApp->GetMRUList();
		pMRUList->Remove(0);

	}
	
}

void CSnaggerView::OnRename() 
{
	CSnaggerDoc* pDoc = GetDocument();

	// Display the Create Project dialogbox
	CProjectDlg dlgProject;

	CString strCurrProject = CInet::SplitFileName(pDoc->GetPathName(),
							CInet::DRIVE|CInet::PATH|CInet::FNAME);

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
			CSnaggerApp *pApp = (CSnaggerApp *) AfxGetApp();
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
