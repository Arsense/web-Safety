// TraceRoute.cpp: implementation of the CTraceRoute class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyTraceRoute.h"
#include "TraceRoute.h"
#include "MyTraceRouteDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CTraceRoute::CTraceRoute(CMyTraceRouteDlg* dlg)
{
	m_dlg=dlg;
	m_nTTL = 1;
	m_nMaxhops = MAX_HOPS;
	m_sockRaw = INVALID_SOCKET;
	m_RcvBuffer = NULL;
	m_IcmpData  = NULL;
	m_nTimeout  = 1000;
	m_bDone     = FALSE;
  
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		AfxMessageBox("Sorry, socket cannot load dll!");
	}
}

CTraceRoute::~CTraceRoute()
{
    
}

void CTraceRoute::ConnectToHost(char* strHost)
{
    m_sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP,
                          NULL, 0,WSA_FLAG_OVERLAPPED);	                     
    if (m_sockRaw == INVALID_SOCKET) 
    {   
		AfxMessageBox("socket创建失败!");
		return;
    }
    
	int ret = setsockopt(m_sockRaw, SOL_SOCKET, SO_RCVTIMEO, 
                        (char *)&m_nTimeout, sizeof(m_nTimeout));
    if (ret == SOCKET_ERROR)
    {
		AfxMessageBox("设置socket接收超时参数失败!");
        return ;
    }
    
	ret = setsockopt(m_sockRaw, SOL_SOCKET, SO_SNDTIMEO, 
        (char *)&m_nTimeout, sizeof(m_nTimeout));
    if (ret == SOCKET_ERROR)
    {   
		AfxMessageBox("设置socket发送超时参数失败!");
        return ;   
    }

	m_addrDest.sin_family = AF_INET;
    if ((m_addrDest.sin_addr.s_addr = inet_addr(strHost)) == INADDR_NONE)
    {   
		HOSTENT* hp;
        hp = gethostbyname(strHost);
        if (hp)
            memcpy(&(m_addrDest.sin_addr), hp->h_addr, hp->h_length);
        else
        {
			AfxMessageBox("不正确的主机名!");
            return ;    
        }
    }

	int bOpt = TRUE;
    if (setsockopt(m_sockRaw, SOL_SOCKET, SO_DONTROUTE, (char *)&bOpt, 
            sizeof(BOOL)) == SOCKET_ERROR)
	{
			AfxMessageBox("设置socket参数失败!");
            return ;
	}
    m_nDatasize = DEF_PACKET_SIZE;
	m_nDatasize += sizeof(IcmpHeader);  
    //
    // Allocate the sending and receiving buffers for ICMP packets
    //
    m_IcmpData = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PACKET);
    m_RcvBuffer = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PACKET);

   if ((!m_IcmpData) || (!m_RcvBuffer))
    {
		AfxMessageBox("堆分配失败!");
        return ;
    }
    // Set the socket to bypass the standard routing mechanisms 
    //  i.e. use the local protocol stack to the appropriate network
    //       interface
    //
    
    //  
    // Here we are creating and filling in an ICMP header that is the 
    // core of trace route.
    //
   
     memset(m_IcmpData, 0, MAX_PACKET);
	 CString temp;
	 temp.Format("Tracing route to %s over a maximum of %d hops: \r\n",strHost, m_nMaxhops);
	 m_dlg->m_result+=temp;
	 m_dlg->SetDlgItemText(IDC_RESULT,m_dlg->m_result);
    fill_icmp_data(m_IcmpData, m_nDatasize);
	
	for(m_nTTL = 1; ((m_nTTL < m_nMaxhops) && (!m_bDone)); m_nTTL++)
    {
        int bwrote;

        // Set the time to live option on the socket
        //
        set_ttl(m_sockRaw, m_nTTL);

        //
        // Fill in some more data in the ICMP header
        //
        ((IcmpHeader*)m_IcmpData)->i_cksum = 0;
        ((IcmpHeader*)m_IcmpData)->timestamp = GetTickCount();

        ((IcmpHeader*)m_IcmpData)->i_seq = m_nSeqno++;
        ((IcmpHeader*)m_IcmpData)->i_cksum = checksum((USHORT*)m_IcmpData, 
            m_nDatasize);
        //
        // Send the ICMP packet to the destination
        //
        bwrote = sendto(m_sockRaw, m_IcmpData, m_nDatasize, 0, 
                    (SOCKADDR *)&m_addrDest, sizeof(m_addrDest));
        if (bwrote == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) 
            {
				temp.Format("%2d  Send request timed out. \r\n", m_nTTL);
				m_dlg->m_result+=temp;
				m_dlg->SetDlgItemText(IDC_RESULT,m_dlg->m_result);
                continue;
            }
			temp.Format("发送数据报函数调用失败!\r\n");
			m_dlg->m_result+=temp;
			m_dlg->SetDlgItemText(IDC_RESULT,m_dlg->m_result);
            return ;
        }
        // Read a packet back from the destination or a router along 
        // the way.
        //
		int fromlen;
		fromlen = sizeof(SOCKADDR);
        ret = recvfrom(m_sockRaw, m_RcvBuffer, MAX_PACKET, 0, 
            (struct sockaddr*)&m_addrFrom, &fromlen);
        if (ret == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) 
            {
				temp.Format("%2d  Receive Request timed out. \r\n", m_nTTL);
				m_dlg->m_result+=temp;
				m_dlg->SetDlgItemText(IDC_RESULT,m_dlg->m_result);
                continue;
            }
			temp.Format("recvfrom 函数调用失败!\n");
			m_dlg->m_result+=temp;
			m_dlg->SetDlgItemText(IDC_RESULT,m_dlg->m_result);
            return ;
        }
        //
        // Decode the response to see if the ICMP response is from a 
        // router along the way or whether it has reached the destination.
        //
        m_bDone = decode_resp(m_RcvBuffer, ret, &m_addrFrom, m_nTTL);
        Sleep(1000);
    }   

 }

void CTraceRoute::fill_icmp_data(char * icmp_data, int datasize)
{
     IcmpHeader *icmp_hdr;
    char       *datapart;

    icmp_hdr = (IcmpHeader*)icmp_data;

    icmp_hdr->i_type = ICMP_ECHO;
    icmp_hdr->i_code = 0;
    icmp_hdr->i_id   = (USHORT)GetCurrentProcessId();
    icmp_hdr->i_cksum = 0;
    icmp_hdr->i_seq = 0;
  
    datapart = icmp_data + sizeof(IcmpHeader);
    //
    // Place some junk in the buffer. Don't care about the data...
    //
    memset(datapart,'E', datasize - sizeof(IcmpHeader));
}



int CTraceRoute::set_ttl(SOCKET s, int nTimeToLive)
{
    int     nRet;
    
    nRet = setsockopt(s, IPPROTO_IP, IP_TTL, (LPSTR)&nTimeToLive,
                sizeof(int));
    if (nRet == SOCKET_ERROR)
    {
		AfxMessageBox("设置socket选项IP_TTL失败!");
        return 0;
    }
    return 1;
}

USHORT CTraceRoute::checksum(USHORT *buffer, int size)
{
    unsigned long cksum=0;
	
    while(size > 1) 
    {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if(size )
        cksum += *(UCHAR*)buffer;
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
	
    return (USHORT)(~cksum);
}

void CTraceRoute::Cleanup()
{
    HeapFree(GetProcessHeap(), 0, m_RcvBuffer);
    HeapFree(GetProcessHeap(), 0, m_IcmpData);
	if (m_sockRaw != NULL)
		closesocket(m_sockRaw);
//	WSACleanup();
}

int CTraceRoute::decode_resp(char *buf, int bytes, SOCKADDR_IN *from, int ttl)
{
    IpHeader       *iphdr = NULL;
    IcmpHeader     *icmphdr = NULL;
    unsigned short  iphdrlen;
    struct hostent *lpHostent = NULL;
    struct in_addr  inaddr = from->sin_addr;

    iphdr = (IpHeader *)buf;
    // Number of 32-bit words * 4 = bytes
	iphdrlen = iphdr->h_len * 4; 

	CString temp;
    if (bytes < iphdrlen + ICMP_MIN) 
	{
		temp.Format("Too few bytes from %s \r\n",inet_ntoa(from->sin_addr));
		m_dlg->m_result+=temp;
		m_dlg->SetDlgItemText(IDC_RESULT,m_dlg->m_result);
	}
    icmphdr = (IcmpHeader*)(buf + iphdrlen);

    switch (icmphdr->i_type)
    {
        case ICMP_ECHOREPLY:     // Response from destination
            lpHostent = gethostbyaddr((const char *)&from->sin_addr, 
                AF_INET, sizeof(struct in_addr));
            if (lpHostent != NULL)
			{
				temp.Format("%2d  %s (%s) \r\n", ttl, lpHostent->h_name,inet_ntoa(inaddr));
				m_dlg->m_result+=temp;
				m_dlg->SetDlgItemText(IDC_RESULT,m_dlg->m_result);
			}
            return 1;
            break;
        case ICMP_TIMEOUT:      // Response from router along the way
			temp.Format("%2d  %s \r\n", ttl, inet_ntoa(inaddr));
			m_dlg->m_result+=temp;
			m_dlg->SetDlgItemText(IDC_RESULT,m_dlg->m_result);
            return 0;
            break;
        case ICMP_DESTUNREACH:  // Can't reach the destination at all
			temp.Format("%2d  %s  reports: Host is unreachable \r\n", ttl,inet_ntoa(inaddr));
			m_dlg->m_result+=temp;
			m_dlg->SetDlgItemText(IDC_RESULT,m_dlg->m_result);
            return 1;
            break;
        default:
			temp.Format("non-echo type %d recvd\n", icmphdr->i_type);
			m_dlg->m_result+=temp;
			m_dlg->SetDlgItemText(IDC_RESULT,m_dlg->m_result);
            return 1;
            break;
    }
    return 0;
}
