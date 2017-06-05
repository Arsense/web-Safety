#include "stdafx.h"
#include "MyTreeCtrl.h"
#include "InternetDownload.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl

// Constructor
CMyTreeCtrl::CMyTreeCtrl()
{
}

// Destructor
CMyTreeCtrl::~CMyTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CMyTreeCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Adds an item to the root of the tree
HTREEITEM CMyTreeCtrl::AddRootItem(LPCSTR lpszTitle, int nImage)
{
	return(InsertItem(lpszTitle,nImage,nImage,TVI_ROOT,TVI_LAST));
}

// Adds a child item under the specified parent node
HTREEITEM CMyTreeCtrl::AddChildItem(LPCTSTR lpszTitle, HTREEITEM htreeParent, int nImage)
{
	return(InsertItem(lpszTitle,nImage,nImage,htreeParent,TVI_LAST));
}

// Returns the proper multimedia node type based on the provide filename
CMyTreeCtrl::NODE_TYPE CMyTreeCtrl::GetMediaType(CString& strFileName)
{
	NODE_TYPE nodeType = CMyTreeCtrl::IMAGE_NODE;

	// Get the extension of the file
	CString strExt = CInternetDownload::SplitFileName(strFileName,CInternetDownload::EXT);
	strExt.MakeLower();

	CString strTest = strFileName;

	// Is it mail link?
 	if(strTest.Left(7) == "mailto:")      
 		nodeType = CMyTreeCtrl::EMAIL_NODE;
	else 
	{
		// Sound file?
		if(strExt == ".wav" || strExt == ".au" || strExt == ".mid" || strExt == ".midi")
			nodeType = CMyTreeCtrl::WAVE_NODE;
		else if(strExt == ".txt" || strExt == ".pdf")
			nodeType = CMyTreeCtrl::DOC_NODE;
		else if(strExt == ".class")
			nodeType = CMyTreeCtrl::JAVA_NODE;
	}

	// Return the type
	return(nodeType);
}

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl message handlers
