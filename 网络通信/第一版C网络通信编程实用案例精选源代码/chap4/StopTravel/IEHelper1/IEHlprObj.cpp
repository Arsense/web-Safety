// IEHlprObj.cpp : Implementation of CIEHlprObj
#include "stdafx.h"
#include "IEHelper.h"
#include "IEHlprObj.h"
#include "ExDispID.h"
#include <strstrea.h>
#include <stdio.h>
#include "fstream.h"



const char* const pszAppName = "IEHelper";

/////////////////////////////////////////////////////////////////////////////
// CIEHlprObj

//
// CIEHlprObj Methods
//

CIEHlprObj::~CIEHlprObj()
{
}

BOOL CIEHlprObj::ManageConnection(enum ConnectType eConnectType)
{
	if (!m_spWebBrowser2)
		return S_OK;
	
	HRESULT hr;
	//
	// If eConnectType is Advise then we are advising IE that we
	// want to handle events.  If eConnectType is Unadvise, we are
	// telling IE that we no longer want to handle events.
	//
	CComQIPtr<IConnectionPointContainer,
		&IID_IConnectionPointContainer> spCPContainer(m_spWebBrowser2);
	
	if (spCPContainer != NULL)
	{
		CComPtr<IConnectionPoint> spConnectionPoint;
		
		hr = spCPContainer->FindConnectionPoint(DIID_DWebBrowserEvents2, &spConnectionPoint);
		if (SUCCEEDED(hr))
		{
			if (eConnectType == Advise)
			{
				//
				// Advise the client site of our desire to be handle events
				//
				hr = spConnectionPoint->Advise((IDispatch*)this, &m_dwCookie);
				if (FAILED(hr))
					ATLTRACE("\n%s: ManageConnection(): Failed to Advise\n\n", pszAppName);
			}
			else
			{
				// Remove us from the list of people interested...
				hr = spConnectionPoint->Unadvise(m_dwCookie);
				if (FAILED(hr))
					ATLTRACE("\npszAppName: ManageConnection(): Failed to Unadvise\n\n", pszAppName);
			}
		}
	}
	
	return (SUCCEEDED(hr));
}

//
// IOleObjectWithSite Methods
//
STDMETHODIMP CIEHlprObj::SetSite(IUnknown *pUnkSite)
{
	USES_CONVERSION;
	
	if (!pUnkSite)
		ATLTRACE("\nSetSite(): pUnkSite is NULL\n\n");
	else
	{
		// Query pUnkSite for the IWebBrowser2 interface.
		m_spWebBrowser2 = pUnkSite;
		
		if (m_spWebBrowser2)
		{
			// Connect to the browser in order to handle events.
			if (!ManageConnection(Advise))
				::MessageBox(NULL, _T("Failure sinking events from IWebBrowser2"), pszAppName, MB_OK);
		}
	}
	
	return S_OK;
}


//
// IDispatch Methods
//
STDMETHODIMP CIEHlprObj::Invoke(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags,
                                DISPPARAMS* pDispParams, VARIANT* pvarResult,
                                EXCEPINFO*  pExcepInfo,  UINT* puArgErr)
{
	USES_CONVERSION;
	
	if (!pDispParams)
		return E_INVALIDARG;
	
	//
	// Get the current URL
	//  
	LPOLESTR lpURL = NULL;
	m_spWebBrowser2->get_LocationURL(&lpURL);

	char sitelist[100][1024];	//所有站点名称(1024)
	char site[1024];		//站点名称

	ifstream in("C:\\winnt\\SiteList.txt",ios::in);
//	ofstream out("C:dingding.txt",ios::out);
	if(in.fail())
	{
		return E_INVALIDARG;
	}
	int i=0;
	int count;
	while(1)
	{
		in>>site;
		if(site==NULL||strlen(site)==0)
			break;
//		sitelist[i]=new char [strlen(site)+1];
		strcpy(sitelist[i],site);
//		out<<site<<endl;
		i++;
		site[0]='\0';
		if(i>=1024)
		{
			break;
		}
	}
	in.close();
//	out.close();
	count=i;
	switch (dispidMember)
	{
		//
		// The parameters for this DISPID are as follows:
		// [0]: Cancel flag  - VT_BYREF|VT_BOOL
		// [1]: HTTP headers - VT_BYREF|VT_VARIANT
		// [2]: Address of HTTP POST data  - VT_BYREF|VT_VARIANT 
		// [3]: Target frame name - VT_BYREF|VT_VARIANT 
		// [4]: Option flags - VT_BYREF|VT_VARIANT
		// [5]: URL to navigate to - VT_BYREF|VT_VARIANT
		// [6]: An object that evaluates to the top-level or frame
		//      WebBrowser object corresponding to the event. 
		//
	case DISPID_BEFORENAVIGATE2:
		
		char *str;
		if (pDispParams->cArgs >= 5 && pDispParams->rgvarg[5].vt == (VT_BYREF|VT_VARIANT))
		{
            CComVariant varURL(*pDispParams->rgvarg[5].pvarVal);
            varURL.ChangeType(VT_BSTR);
			//转化要访问的网址为char *型
			str = OLE2A(varURL.bstrVal);
		}
		//如果正要访问的网址为要被拦截的，则stop
		for(i=0;i<count;i++)
		{
			if(strstr(str,sitelist[i])!=NULL)
			{
				*pDispParams->rgvarg[0].pboolVal = TRUE;
				MessageBox(NULL,"当前系统禁止浏览该页","警告",MB_ICONSTOP);
				return S_OK;
			}
		}
		break;
		
		//
		// The parameters for this DISPID:
		// [0]: URL navigated to - VT_BYREF|VT_VARIANT
		// [1]: An object that evaluates to the top-level or frame
		//      WebBrowser object corresponding to the event. 
		//
	case DISPID_NAVIGATECOMPLETE2:
		
		break;

	case DISPID_DOCUMENTCOMPLETE:
		break;
		
	case DISPID_DOWNLOADBEGIN:
		break;
		
	case DISPID_DOWNLOADCOMPLETE:
		break;
		
	case DISPID_NEWWINDOW2:
	   	break;
		
		// The parameters for this DISPID:
		// [0]: Address of cancel flag - VT_BYREF|VT_BOOL
		//
	case DISPID_QUIT:
		
		break;
		
	default:
		
		break;
	}
	/*
	for(i=0;i<count;i++)
	{
		delete []sitelist[i];
	}
	*/
	return S_OK;
}
