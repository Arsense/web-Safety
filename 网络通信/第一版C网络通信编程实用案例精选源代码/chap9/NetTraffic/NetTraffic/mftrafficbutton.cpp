#include "stdafx.h"
#include "MFTrafficButton.h"
#include "MemDC.h"
#include <math.h>

#ifndef _WIN32_IE
	#define _WIN32_IE 0x0500
#endif



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// MFTrafficButton

MFTrafficButton::MFTrafficButton()
{
	brushInitalized = FALSE;
	interfaceCallBack = NULL;
	useAdaptiveScale = FALSE;
	gridxstartpos = 0;
	gridystartpos = 0;
	gridxresolution		=	GRIDXRESOLUTION;
	gridyresolution		=	GRIDYRESOLUTION;
	gridscrollxspeed	=	GRIDSCROLLXSPEED;
	gridscrollyspeed	=	GRIDSCROLLYSPEED; 
	plotgranularity		=	PLOTGRANULATRITY;
	netupdatespeed		=	NETUPDATESPEED;
	gridupdatespeed		=	GRIDUPDATESPEED;
	ToggleStatusText	=	"局部最大值";
}

MFTrafficButton::~MFTrafficButton()
{

}


BEGIN_MESSAGE_MAP(MFTrafficButton, CButton)
	//{{AFX_MSG_MAP(MFTrafficButton)
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MFTrafficButton message handlers



void MFTrafficButton::PreSubclassWindow() 
{

	CButton::PreSubclassWindow();
	// 这个函数确保这个button是用来绘制的而不是接收按下的消息的
	ModifyStyle( 0 , BS_OWNERDRAW );

	this->GetWindowRect(&TrafficDrawRectangle);
	this->GetWindowRect(&TrafficDrawUpdateRectangle);
	ScreenToClient(&TrafficDrawUpdateRectangle);
	ScreenToClient(&TrafficDrawRectangle);

	TGSize.cx = TrafficDrawRectangle.right - TrafficDrawRectangle.left;
	TGSize.cy = TrafficDrawRectangle.bottom - TrafficDrawRectangle.top;

	initalized = FALSE;
	MaxTrafficAmount = 0.0;
	SelectedInterface = 0;
	
	smallFont.CreateFont(-10,0,0,0,FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, "Times New Roman");

	red		= RGB(255,125,125);
	green	= RGB(125,255,125);
	cyan	= RGB(0,255,255);
	darkblue= RGB(0,0,75);
//	darkblue= RGB(255,255,255);
	darkgray= RGB(50,50,50);
	white	= RGB(255,255,255);
	black	= RGB(0,0,0);
	lightgreen	= RGB(156,255,156);
	darkgreen	= RGB(32,64,32);
//	darkgreen    =0;
	greenbrush.CreateSolidBrush(green);
	redbrush.CreateSolidBrush(red);
	GridPen.CreatePen(PS_SOLID ,1 , darkgreen);
	GetWindowRect(&TrafficDrawRectangle);
	ScreenToClient(&TrafficDrawRectangle);

	
	TGSize.cx = TrafficDrawRectangle.right - TrafficDrawRectangle.left;
	TGSize.cy = TrafficDrawRectangle.bottom - TrafficDrawRectangle.top;
	
	
	TrafficEntries = TGSize.cx / plotgranularity;
	TrafficStats = new TRAFFICENTRY[TrafficEntries+1];
	
	for(DWORD x=0; x<TrafficEntries; x++)
	{
		TrafficStats[x].connected = TRUE;//FALSE;
		TrafficStats[x].value	= 0.0;//(float)x * (float)TrafficEntries*0.05;//;10.0 + 10.0*(sin(5.0*(float)x*3.14/180.0));
	}
	
	MaxTrafficAmount = 0.0;
	SelectedInterface = 0;

	CRgn rectRgn, ellRgn, finalRgn;
	rectRgn.CreateRectRgn(0,0,TGSize.cx, TGSize.cy);
	ShapeWNDRegion.CreateRectRgn(0,0,TGSize.cx, TGSize.cy);;
	ShapeDCRegion.CreateRectRgn(0,0,TGSize.cx, TGSize.cy);;

	int x1,x2,y1,y2,xe,ye,xs,ys;
	int xof, yof;
	int r;
	xs = TGSize.cx;
	ys = TGSize.cy;
	x1 = 0;
	y1 = 0;
	x2 = xs;
	y2 = ys;
	xe = 5;//Radius of edge
	ye = 5;//Radius of edge
	xof = (int)( (float)xs*0.0);
	yof = (int)( (float)ys*0.0);
	r = ellRgn.CreateRoundRectRgn(x1,y1,x2,y2,xe,ye);
	r = ellRgn.OffsetRgn(-xof, -yof);
	r = ShapeWNDRegion.CombineRgn(&rectRgn, &ellRgn,RGN_AND );
	r = ShapeDCRegion.CombineRgn(&rectRgn, &ellRgn,RGN_AND );

	this->SetWindowRgn((HRGN)ShapeWNDRegion, TRUE);
	initalized = TRUE;

	SetTimer(GRIDTIMER,	gridupdatespeed,	0);
	SetTimer(NETTIMER,	netupdatespeed,		0);

}


void MFTrafficButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
//	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);

	ASSERT( lpDrawItemStruct != NULL );

	CDC * pDC   = CDC::FromHandle( lpDrawItemStruct -> hDC );
	int erg = pDC->SelectClipRgn(&ShapeDCRegion);

	CRect rect = lpDrawItemStruct -> rcItem;
	UINT state = lpDrawItemStruct -> itemState;
	UINT nStyle = GetStyle( );

	int nSavedDC = pDC -> SaveDC( );

	// Create the brush for the color bar
	if(brushInitalized == FALSE)
	{
		CBitmap bmp;
		CMemDC *memDC = new CMemDC(pDC);
		
		RECT clipRect;
		memDC->GetClipBox(&clipRect);

		if(clipRect.right - clipRect.left > 1)
		{
			bmp.CreateCompatibleBitmap(memDC,plotgranularity, TGSize.cy);
			CBitmap *pOld = memDC->SelectObject(&bmp);
			
			CSize bmps = bmp.GetBitmapDimension();		
			
			double factor = 255.0 / (float)TGSize.cy;
			BYTE r,g,b;
			for(int x = 0; x<TGSize.cy; x++)
			{
				g = (BYTE)(255-factor*x);
				r = (BYTE)(factor*x);
				b = (BYTE)64;
				memDC->SetPixelV(0,x,RGB(r,g,b));
				memDC->SetPixelV(1,x,RGB(r,g,b));
			}
			memDC->SelectObject(pOld);

			colorbrush.CreatePatternBrush(&bmp);	
			brushInitalized = TRUE;
		}
	}
	if(initalized == TRUE)
	{
		COLORREF backcolor = GetSysColor(COLOR_BTNFACE);
		
		CBrush brush;
		CMemDC *memDC = new CMemDC(pDC);
		
		RECT clipRect;
		memDC->GetClipBox(&clipRect);
		memDC->FillSolidRect(&clipRect,backcolor);
		
		CFont *oldFont;
		int xp, yp, xx, yy;
		orgBrushOrigin = memDC->GetBrushOrg();
		
		oldFont = memDC->SelectObject(&smallFont);
		
		double scale = (double)TGSize.cy / (double)MaxTrafficAmount;
		
		yp = TrafficDrawRectangle.bottom;
		xp = TrafficDrawRectangle.left;
		
		RECT fillrect;
		
		CString tmp;
		
		// 填充背景
		back = memDC->GetBkColor();
		brush.CreateSolidBrush(darkblue);
		memDC->FillRect(&TrafficDrawRectangle, &brush);

		// 画网格
		int xgridlines, ygridlines;
		
		xgridlines = TGSize.cx / gridxresolution;
		ygridlines = TGSize.cy / gridyresolution;
		CPen* oldPen = memDC->SelectObject(&GridPen);
		// 创建垂直线
		for (int x=0; x<= xgridlines; x++)
		{
			memDC->MoveTo(x*gridxresolution + gridxstartpos, 0			);
			memDC->LineTo(x*gridxresolution + gridxstartpos, TGSize.cy	);
		}
		// 添加水平线
		for (int y=0; y<= ygridlines; y++)
		{
			memDC->MoveTo(0			, gridystartpos + TGSize.cy - y*gridyresolution - 2);
			memDC->LineTo(TGSize.cx	, gridystartpos + TGSize.cy - y*gridyresolution - 2);
		}

		gridxstartpos += gridscrollxspeed;
		gridystartpos += gridscrollyspeed;
		if(gridxstartpos < 0				) gridxstartpos = gridxresolution;
		if(gridxstartpos > gridxresolution	) gridxstartpos = 0;
		if(gridystartpos < 0				) gridystartpos = gridyresolution;
		if(gridystartpos > gridyresolution	) gridystartpos = 0;


		memDC->SelectObject(oldPen );

		for(DWORD cnt=0; cnt<TrafficEntries; cnt++)
		{
			xx = xp + cnt*plotgranularity;
			double traffic = (double)TrafficStats[cnt].value; 
			yy = yp - (int)((double)TrafficStats[cnt].value * scale);
			
			// 网络处在连接状态才绘制
			if(TrafficStats[cnt].connected == TRUE)
			{
				fillrect.bottom = yp;
				fillrect.top	= yy;
				fillrect.left	= xx;
				fillrect.right	= xx+plotgranularity;
				memDC->SetBrushOrg(xx,yp);
				if(TrafficStats[cnt].value > 0.0) 
				{
					memDC->FillRect(&fillrect, &colorbrush);
					memDC->SetPixelV(xx, yy, cyan);
				}
			}
		}
		
		tmp.Format("%8.1f",TrafficStats[TrafficEntries-1].value);
		COLORREF textcolor = memDC->GetTextColor();
		int bkmode = memDC->GetBkMode();
		memDC->SetBkMode(TRANSPARENT);
		memDC->SetTextColor(darkblue);
		memDC->TextOut(6,5,AllTraffic);
		memDC->SetTextColor(cyan);
//		memDC->SetTextColor(darkblue);
		memDC->TextOut(5,5,AllTraffic); 
		memDC->SetTextColor(textcolor);
		memDC->SetBkMode(bkmode);
		
		memDC->SelectObject(oldFont);
		memDC->SetBrushOrg(orgBrushOrigin.x, orgBrushOrigin.y);
	
		delete memDC;
	}	
	pDC -> RestoreDC( nSavedDC );

}

void MFTrafficButton::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == NETTIMER)
	{
		
#ifdef _I_HAVE_PLATFORM_SDK_INSTALLED_		

		DWORD flag, reserved;
		BOOL erg;
		flag = 0;//INTERNET_CONNECTION_OFFLINE ;
		reserved = 0;
		TCHAR connectionname[1024];
		erg =  InternetGetConnectedStateEx(
			&flag,	//OUT LPDWORD lpdwFlags,
			(LPTSTR)&connectionname,//OUT LPTSTR lpszConnectionName,
			1024,//IN DWORD dwNameLen,
			0//IN DWORD dwReserved
			);
		
		isOnline = erg;
#else
		isOnline = TRUE;
#endif		

		// 得到当前的流量
		double traffic = m_cTrafficClass.GetTraffic(SelectedInterface);
		DWORD  totaltraffic = m_cTrafficClass.GetInterfaceTotalTraffic(SelectedInterface);
		
		double delta1;
		double divisor = (1000.0/(double)NETUPDATESPEED);
		delta1 = (double)(traffic * divisor) / 1024.0;
		CurrentTraffic.Format("当前流量: %.1f KB/sec",delta1);
		
		
		// Should we recalculate the local maximum per session or per display?
		if(useAdaptiveScale==TRUE)
		{
			MaxTrafficAmount = 0.0;
		}
		// Shift whole array 1 step to left and calculate local maximum
		for(DWORD x=0; x<TrafficEntries; x++)
		{
			TrafficStats[x].connected = TrafficStats[x+1].connected;
			TrafficStats[x].value	= TrafficStats[x+1].value;
			if(TrafficStats[x].value > MaxTrafficAmount)
				MaxTrafficAmount = TrafficStats[x].value;
		}
		if(isOnline == TRUE)
		{
			TrafficStats[TrafficEntries].connected = TRUE;
			TrafficStats[TrafficEntries].value = traffic;
			if(TrafficStats[TrafficEntries].value > MaxTrafficAmount)
				MaxTrafficAmount = TrafficStats[TrafficEntries].value;
		}
		else
		{
			TrafficStats[TrafficEntries].connected = FALSE;
			TrafficStats[TrafficEntries].value = traffic;
			if(TrafficStats[TrafficEntries].value > MaxTrafficAmount)
				MaxTrafficAmount = TrafficStats[TrafficEntries].value;
		}
		
		double delta2;
		divisor = (1000.0/(double)NETUPDATESPEED);
		delta2 = (double)(MaxTrafficAmount * divisor) / 1024.0;
		MaximalTraffic.Format("最大流量:%.1f KB/sec",delta2);
		AllTraffic.Format("总流量为: %.1f / %.1f KB/sec",delta1, delta2);
		
	}

	// 绘制
	Invalidate(FALSE);

	CButton::OnTimer(nIDEvent);
}

//创建右键按下后产生的一个pop-up菜单
void MFTrafficButton::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
#define STARTINDEX	2000
#define ENDINDEX	2999
#define SET_SCALING	3000

	HMENU hMenu , hPopup;
	long retId = 0;
	int InterfaceIndex = STARTINDEX;
	POINT pt;
	
	hMenu = CreatePopupMenu( );			  
	if( hMenu )
	{	
		//  If the menu was created, add the item text
		CString tmp, tmp2;
		double tottraff = (double)m_cTrafficClass.GetInterfaceTotalTraffic(SelectedInterface) / (1024.0*1024.0);
		m_cTrafficClass.GetNetworkInterfaceName(&tmp, SelectedInterface);
		tmp2.Format("接口名称:%s : %.1f MB", tmp, tottraff);
		
		AppendMenu( hMenu , MF_STRING , 1001 , tmp2 );
		AppendMenu( hMenu , MF_SEPARATOR , 1000 , NULL );
		// Submenu
		hPopup = CreatePopupMenu( );			  //  Create a new popup menu
		if( hPopup )
		{
			CString tmp;
			int cnt;
			for(cnt=0; cnt<=m_cTrafficClass.GetNetworkInterfacesCount(); cnt++)
			{
				CString tmp2;
				double tottraff = (double)m_cTrafficClass.GetInterfaceTotalTraffic(cnt) / (1024.0*1024.0);
				m_cTrafficClass.GetNetworkInterfaceName(&tmp, cnt);
				
				tmp2.Format("%s : %.1f MB", tmp, tottraff);
				AppendMenu( hPopup , MF_STRING , InterfaceIndex++ , tmp2 );
			}
			AppendMenu( hMenu , MF_POPUP , ( UINT )hPopup , "选择一个接口" );
			AppendMenu( hMenu , MF_SEPARATOR , 1000 , NULL );
			AppendMenu( hMenu , MF_STRING , SET_SCALING , "锁定的缩放比例:" + 	ToggleStatusText);
		}
	}
	
	GetCursorPos( & pt );			
	//显示菜单
	retId = TrackPopupMenu( hMenu , TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD ,  pt.x , pt.y , 0 , this -> m_hWnd , NULL );  //  start the menu
	DestroyMenu( hMenu );			
	if( retId == 0 )
	{
		return;
	}
	else
	{
		if(retId == SET_SCALING)
		{
			if(useAdaptiveScale == TRUE)
			{
				useAdaptiveScale = FALSE;
				ToggleStatusText = "全局最大值";
			}
			else
			{
				useAdaptiveScale = TRUE;
				ToggleStatusText = "局部最大值";
			}
		}
		if((retId>=STARTINDEX) && (retId<ENDINDEX) )
		{
			SelectedInterface = retId-STARTINDEX;
			InterfaceHasChanged();
			ReInit(SelectedInterface);
		}
	}
	
	CButton::OnRButtonDown(nFlags, point);
}

void MFTrafficButton::ReInit(RECT newSize)
{
	KillTimer(GRIDTIMER);
	KillTimer(NETTIMER);

	delete [] TrafficStats;

	TGSize.cx = newSize.right - newSize.left;
	TGSize.cy = newSize.bottom - newSize.top;

	TrafficEntries = TGSize.cx / plotgranularity;
	TrafficStats = new TRAFFICENTRY[TrafficEntries+1];
	
	MaxTrafficAmount = 0.0;
	SelectedInterface = 0;
	
	for(DWORD x=0; x<TrafficEntries; x++)
	{
		TrafficStats[x].connected = TRUE;
		TrafficStats[x].value	= 0.0;
	}
	m_cTrafficClass.GetTraffic(SelectedInterface);
	m_cTrafficClass.GetTraffic(SelectedInterface);
	MaxTrafficAmount = 0.0;
	SetTimer(GRIDTIMER,	gridupdatespeed,	0);
	SetTimer(NETTIMER,	netupdatespeed,		0);
	
}

//监视其他接口
void MFTrafficButton::ReInit(int newInterface)
{
	KillTimer(GRIDTIMER);
	KillTimer(NETTIMER);

	SelectedInterface = newInterface;

	for(DWORD x=0; x<TrafficEntries; x++)
	{
		TrafficStats[x].connected = TRUE;
		TrafficStats[x].value	= 0.0;
	}
	m_cTrafficClass.GetTraffic(SelectedInterface);
	m_cTrafficClass.GetTraffic(SelectedInterface);
	MaxTrafficAmount = 0.0;
	SetTimer(GRIDTIMER,	gridupdatespeed,	0);
	SetTimer(NETTIMER,	netupdatespeed,		0);
	
}

//设置更新速度
void MFTrafficButton::SetUpdateSpeed(UINT netspeed, UINT gridspeed)
{
	gridupdatespeed	= gridspeed;
	netupdatespeed	= netspeed;
	KillTimer(GRIDTIMER);
	KillTimer(NETTIMER);
	SetTimer(GRIDTIMER,	gridupdatespeed,	0);
	SetTimer(NETTIMER,	netupdatespeed,		0);
}


//回调函数
void MFTrafficButton::InterfaceHasChanged()
{
	if(interfaceCallBack !=NULL)
	{
		interfaceCallBack(SelectedInterface);
	}
}

void MFTrafficButton::SetInterfaceNumberNotificationFunction(INTERFACECHANCEDPROC callfunct)
{
	interfaceCallBack = callfunct;	
}

void MFTrafficButton::SelectTrafficType(int trafficType)
{
	switch(trafficType)
	{
	case Traffic_Incoming: 	m_cTrafficClass.SetTrafficType(MFNetTraffic::IncomingTraffic); break;
	case Traffic_Outgoing:	m_cTrafficClass.SetTrafficType(MFNetTraffic::OutGoingTraffic); break;
	case Traffic_Total:		m_cTrafficClass.SetTrafficType(MFNetTraffic::AllTraffic); break;
	default:				m_cTrafficClass.SetTrafficType(MFNetTraffic::AllTraffic);
	}
}
void MFTrafficButton::SetInterfaceNumber(int interfacenumber)
{
	SelectedInterface = interfacenumber;
	ReInit(SelectedInterface);
}

void MFTrafficButton::SetAdaptiveScaling(BOOL adaptive)
{
	useAdaptiveScale = adaptive;
}

