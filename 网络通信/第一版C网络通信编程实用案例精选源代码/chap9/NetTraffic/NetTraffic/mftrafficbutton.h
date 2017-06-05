#if !defined(AFX_MFTRAFFICBUTTON_H__9F2EABAA_E8C7_48B4_94A5_EC279905F50C__INCLUDED_)
#define AFX_MFTRAFFICBUTTON_H__9F2EABAA_E8C7_48B4_94A5_EC279905F50C__INCLUDED_

#include "MFNetTraffic.h"	// Added by ClassView
#include "Globals.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// MFTrafficButton window

//VOID CALLBACK InterfaceChanged(int newInterface);

//typedef FARPROC INTERFACECHANCEDPROC;
typedef VOID (CALLBACK* INTERFACECHANCEDPROC)(int);

class MFTrafficButton : public CButton
{
public:
	MFTrafficButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MFTrafficButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetAdaptiveScaling(BOOL adaptive);
	void SetInterfaceNumber(int interfacenumber);
	enum traffictype
	{
		Traffic_Total		= 0,
		Traffic_Incoming	= 1,
		Traffic_Outgoing	= 2
	};

	void SelectTrafficType(int trafficType);
	void SetInterfaceNumberNotificationFunction(INTERFACECHANCEDPROC callfunct);
	void SetUpdateSpeed(UINT netspeed, UINT gridspeed);
	void ReInit(int newInterface);
	void ReInit(RECT newSize);
	virtual ~MFTrafficButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(MFTrafficButton)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void InterfaceHasChanged();

	INTERFACECHANCEDPROC interfaceCallBack;

	MFNetTraffic m_cTrafficClass;

	CFont	smallFont;
	CBrush	colorbrush;

	COLORREF red, green, back, cyan, darkblue, darkgray, white, black, lightgreen, darkgreen;
	CBitmap	colorbrushbmp;
	CPen	GridPen;
	CSize	TGSize;

	CBrush	greenbrush;
	CBrush	redbrush;
	CPoint	orgBrushOrigin; 

	RECT	TrafficDrawRectangle;
	RECT	TrafficDrawUpdateRectangle;

	CString CurrentTraffic;
	CString MaximalTraffic;
	CString AllTraffic;

	DWORD	TrafficEntries;

	BOOL	initalized;
	BOOL	isOnline;
	BOOL	brushInitalized;

	CRgn	ShapeWNDRegion;
	CRgn	ShapeDCRegion;

	double	MaxTrafficAmount;
	CString	ToggleStatusText;

	int SelectedInterface;

	BOOL useAdaptiveScale;

	TRAFFICENTRY* TrafficStats;

	int gridxstartpos;		
	int gridystartpos;
	int plotgranularity;		

public:
	int gridxresolution;		// 网格大小
	int gridyresolution;
	int gridscrollxspeed;		// 网格滚动速度
	int gridscrollyspeed; 
	int netupdatespeed;			// 网络更新速度
	int gridupdatespeed;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFTRAFFICBUTTON_H__9F2EABAA_E8C7_48B4_94A5_EC279905F50C__INCLUDED_)
