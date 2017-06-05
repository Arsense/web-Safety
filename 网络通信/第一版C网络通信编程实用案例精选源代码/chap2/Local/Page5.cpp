// Page5.cpp : 实现文件
//

#include "stdafx.h"
#include "Chapter5.h"
#include "Page5.h"
#include ".\page5.h"
#include <winsock2.h>
#include <rpc.h>


// CPage5 对话框

IMPLEMENT_DYNAMIC(CPage5, CDialog)
CPage5::CPage5(CWnd* pParent /*=NULL*/)
	: CDialog(CPage5::IDD, pParent)
	, m_Protocols(_T(""))
{
}

CPage5::~CPage5()
{
}

void CPage5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_Protocols);
}


BEGIN_MESSAGE_MAP(CPage5, CDialog)
END_MESSAGE_MAP()


// 获取计算机信息

int CPage5::GetInfo(void)
{
    WSADATA WSAData;
    int i, nRet;
    DWORD dwErr;
    WSAPROTOCOL_INFO *lpProtocolBuf = NULL;
    DWORD dwBufLen = 0;

    if (WSAStartup(MAKEWORD(2,2), &WSAData))
        m_Protocols.AppendFormat("WSAStartup %d", WSAGetLastError());
    else
        {
		//方法：WSAEnumProtocols获得计算机安装的协议
        // First, have WSAEnumProtocols tell you how big a buffer you need.
        nRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
        if (SOCKET_ERROR != nRet)
            m_Protocols.AppendFormat("WSAEnumProtocols: should not have succeeded\r\n");
        else if (WSAENOBUFS != (dwErr = WSAGetLastError()))
            // WSAEnumProtocols failed for some reason not relating to buffer size - also odd.
            m_Protocols.AppendFormat("WSAEnumProtocols(1): %d\r\n", WSAGetLastError());
        else
            {
            // WSAEnumProtocols failed for the "expected" reason.
            // Now you need to allocate a buffer that is the right size.
            lpProtocolBuf = (WSAPROTOCOL_INFO *)malloc(dwBufLen);
            if (lpProtocolBuf)
                {
                // Now you can call WSAEnumProtocols again with the
                // expectation that it will succeed
                // because you have allocated a big enough buffer.
                nRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
                if (SOCKET_ERROR == nRet)
                    m_Protocols.AppendFormat("WSAEnumProtocols(3): %d\r\n", WSAGetLastError());
                else
                    {
                    // Enumerate the protocols.
                    for (i=0; i<nRet; i++)
                        PrintBufEntry(&lpProtocolBuf[i]);
                    }
                free(lpProtocolBuf);
                }
            }
        }
	

    WSACleanup();
    return(0);
    }


BOOL CPage5::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	GetInfo();
	UpdateData(False);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// 易读格式打印出WSAPROTOCOL_INFO结构的内容
void CPage5::PrintBufEntry(WSAPROTOCOL_INFO *pProtocolBuf)
       {
       unsigned char *pszUuid;
       int j;

       m_Protocols.AppendFormat("协议 <%s>\r\n", pProtocolBuf->szProtocol);

       // A guid is the same as a uuid.
       UuidToString(&pProtocolBuf->ProviderId, &pszUuid);
       m_Protocols.AppendFormat("  ProviderId {%s}\r\n", pszUuid);
       RpcStringFree(&pszUuid);

       if (!pProtocolBuf->dwServiceFlags1)
           m_Protocols.AppendFormat("  dwServiceFlags1: 0\r\n");
       else
           m_Protocols.AppendFormat("  dwServiceFlags1: 0x%08X\r\n",
                     pProtocolBuf->dwServiceFlags1);

       // Check which bit flags are set.
       if (pProtocolBuf->dwServiceFlags1 & XP1_CONNECTIONLESS)
           m_Protocols.AppendFormat("    XP1_CONNECTIONLESS\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_GUARANTEED_DELIVERY)
           m_Protocols.AppendFormat("    XP1_GUARANTEED_DELIVERY\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_GUARANTEED_ORDER)
           m_Protocols.AppendFormat("    XP1_GUARANTEED_ORDER\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_MESSAGE_ORIENTED)
           m_Protocols.AppendFormat("    XP1_MESSAGE_ORIENTED\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_PSEUDO_STREAM)
           m_Protocols.AppendFormat("    XP1_PSEUDO_STREAM\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_GRACEFUL_CLOSE)
           m_Protocols.AppendFormat("    XP1_GRACEFUL_CLOSE\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_EXPEDITED_DATA)
           m_Protocols.AppendFormat("    XP1_EXPEDITED_DATA\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_CONNECT_DATA)
           m_Protocols.AppendFormat("    XP1_CONNECT_DATA\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_DISCONNECT_DATA)
           m_Protocols.AppendFormat("    XP1_DISCONNECT_DATA\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_SUPPORT_BROADCAST)
           m_Protocols.AppendFormat("    XP1_SUPPORT_BROADCAST\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_SUPPORT_MULTIPOINT)
           m_Protocols.AppendFormat("    XP1_SUPPORT_MULTIPOINT\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_MULTIPOINT_CONTROL_PLANE)
           m_Protocols.AppendFormat("    XP1_MULTIPOINT_CONTROL_PLANE\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_MULTIPOINT_DATA_PLANE)
           m_Protocols.AppendFormat("    XP1_MULTIPOINT_DATA_PLANE\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_QOS_SUPPORTED)
           m_Protocols.AppendFormat("    XP1_QOS_SUPPORTED\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_INTERRUPT)
           m_Protocols.AppendFormat("    XP1_INTERRUPT\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_UNI_SEND)
           m_Protocols.AppendFormat("    XP1_UNI_SEND\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_UNI_RECV)
           m_Protocols.AppendFormat("    XP1_UNI_RECV\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_IFS_HANDLES)
           m_Protocols.AppendFormat("    XP1_IFS_HANDLES\r\n");
       if (pProtocolBuf->dwServiceFlags1 & XP1_PARTIAL_MESSAGE)
           m_Protocols.AppendFormat("    XP1_PARTIAL_MESSAGE\r\n");

       m_Protocols.AppendFormat("  dwServiceFlags2: reserved\r\n");

       m_Protocols.AppendFormat("  dwServiceFlags3: reserved\r\n");

       m_Protocols.AppendFormat("  dwServiceFlags4: reserved\r\n");

       m_Protocols.AppendFormat("  dwProviderFlags:\r\n");
       if (pProtocolBuf->dwProviderFlags & PFL_MULTIPLE_PROTO_ENTRIES)
           m_Protocols.AppendFormat("    PFL_MULTIPLE_PROTO_ENTRIES\r\n");
       if (pProtocolBuf->dwProviderFlags & PFL_RECOMMENDED_PROTO_ENTRY)
           m_Protocols.AppendFormat("    PFL_RECOMMENDED_PROTO_ENTRY\r\n");
       if (pProtocolBuf->dwProviderFlags & PFL_HIDDEN)
           m_Protocols.AppendFormat("    PFL_HIDDEN\r\n");
       if (pProtocolBuf->dwProviderFlags & PFL_MATCHES_PROTOCOL_ZERO)
           m_Protocols.AppendFormat("    PFL_MATCHES_PROTOCOL_ZERO\r\n");

       m_Protocols.AppendFormat("  dwCatalogEntryId = %u\r\n", pProtocolBuf->dwCatalogEntryId);

       m_Protocols.AppendFormat("  ProtocolChain.ChainLen = %d ",
              pProtocolBuf->ProtocolChain.ChainLen);
       if (1 == pProtocolBuf->ProtocolChain.ChainLen)
           m_Protocols.AppendFormat("  ==> this is a base service provider\r\n");
       else if (pProtocolBuf->ProtocolChain.ChainLen > 1)
           {
           m_Protocols.AppendFormat("  ==> ProtocolChain layered to base protocol\r\n");
           for (j=0; j<pProtocolBuf->ProtocolChain.ChainLen; j++)
               m_Protocols.AppendFormat("    Chain Catalog Entry Id = %u\r\n",
                      pProtocolBuf->ProtocolChain.ChainEntries[j]);
           }
       else if (0 == pProtocolBuf->ProtocolChain.ChainLen)
           m_Protocols.AppendFormat("  ==> this is a layered service provider\r\n");
       else
           m_Protocols.AppendFormat("  Invalid\r\n");

       m_Protocols.AppendFormat("  iVersion = %d\r\n", pProtocolBuf->iVersion);

       m_Protocols.AppendFormat("  iAddressFamily = %d\r\n", pProtocolBuf->iAddressFamily);

       m_Protocols.AppendFormat("  iMaxSockAddr = %d\r\n", pProtocolBuf->iMaxSockAddr);

       m_Protocols.AppendFormat("  iMinSockAddr = %d\r\n", pProtocolBuf->iMinSockAddr);

       // iProtocols returns a negative number for Microsoft NetBIOS
       // service providers corresponding to the lana number * -1 (for
       // example, -2 implies lana 2), except for lana 0 which is equal to
       // 0x80000000 because protocol 0 is reserved for special use.
       m_Protocols.AppendFormat("  iProtocol = %d\r\n", pProtocolBuf->iProtocol);

       m_Protocols.AppendFormat("  iProtocolMaxOffset = %d\r\n",
              pProtocolBuf->iProtocolMaxOffset);

       m_Protocols.AppendFormat("  iNetworkByteOrder = %s\r\n",
           ((pProtocolBuf->iNetworkByteOrder == LITTLEENDIAN) ?
             "LITTLEENDIAN" : "BIGENDIAN"));

       m_Protocols.AppendFormat("  iSecurityScheme = %d\r\n", pProtocolBuf->iSecurityScheme);

       m_Protocols.AppendFormat("  dwMessageSize = %u\r\n", pProtocolBuf->dwMessageSize);

       m_Protocols.AppendFormat("  dwProviderReserved = reserved\r\n");

       return;
       }
