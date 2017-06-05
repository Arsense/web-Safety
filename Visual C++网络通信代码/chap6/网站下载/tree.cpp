/*
	Tree.cpp : implementation of the CTree class

	Implements the tree class used to list downloaded pages, table of contents
	and multimedia files.

	Author: Steven E. Sipe
*/

#include "stdafx.h"
#include "tree.h"
#include "inet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTree

// Constructor
CTree::CTree()
{
}

// Destructor
CTree::~CTree()
{
}

BEGIN_MESSAGE_MAP(CTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CTree)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Adds an item to the root of the tree
HTREEITEM CTree::AddRootItem(LPCSTR lpszTitle, int nImage)
{
	return(InsertItem(lpszTitle,nImage,nImage,TVI_ROOT,TVI_LAST));
}

// Adds a child item under the specified parent node
HTREEITEM CTree::AddChildItem(LPCTSTR lpszTitle, HTREEITEM htreeParent, int nImage)
{
	return(InsertItem(lpszTitle,nImage,nImage,htreeParent,TVI_LAST));
}

// Returns the proper multimedia node type based on the provide filename
CTree::NODE_TYPE CTree::GetMediaType(CString& strFileName)
{
	NODE_TYPE nodeType = CTree::IMAGE_NODE;

	// Get the extension of the file
	CString strExt = CInet::SplitFileName(strFileName,CInet::EXT);
	strExt.MakeLower();

	CString strTest = strFileName;

	// Is it mail link?
 	if(strTest.Left(7) == "mailto:")      
 		nodeType = CTree::EMAIL_NODE;
	else 
	{
		// Sound file?
		if(strExt == ".wav" || strExt == ".au" || strExt == ".mid" || strExt == ".midi")
			nodeType = CTree::WAVE_NODE;
		else if(strExt == ".txt" || strExt == ".pdf")
			nodeType = CTree::DOC_NODE;
		else if(strExt == ".class")
			nodeType = CTree::JAVA_NODE;
	}

	// Return the type
	return(nodeType);
}

/////////////////////////////////////////////////////////////////////////////
// CTree message handlers
