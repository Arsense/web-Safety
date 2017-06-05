// IEHlprObj.cpp : Implementation of CIEHlprObj
#include "stdafx.h"
#include "IEHelper.h"
#include "IEHlprObj.h"
#include "ExDispID.h"
#include <strstrea.h>
#include <stdio.h>

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
		break;
		
	case DISPID_NAVIGATECOMPLETE2:
		
		break;

	case DISPID_DOCUMENTCOMPLETE:
		break;
		
	case DISPID_DOWNLOADBEGIN:
		break;
		
	case DISPID_DOWNLOADCOMPLETE:
		break;
		
	case DISPID_NEWWINDOW2:
		//设置是否允许弹出窗口
		READYSTATE m_ReadyState;
        m_spWebBrowser2->get_ReadyState(&m_ReadyState);
        if (m_ReadyState!=READYSTATE_COMPLETE)
        {
            *pDispParams->rgvarg[0].pboolVal = TRUE;
            return S_OK;
        }
		else
		{
            *pDispParams->rgvarg[0].pboolVal = FALSE;
            return S_OK;
		}
	   	break;
		
		// The parameters for this DISPID:
		// [0]: Address of cancel flag - VT_BYREF|VT_BOOL
		//
	case DISPID_QUIT:
		
		break;
		
	default:
		
		break;
	}
	
	return S_OK;
}
