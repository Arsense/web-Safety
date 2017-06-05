// Ping.cpp: implementation of the CPing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyPing.h"
#include "Ping.h"
#include "MyPingDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPing::CPing(CMyPingDlg * dlg)
{
	m_dlg=dlg;
	icmp_data = NULL;
	seq_no = 0;
	recvbuf = NULL;
	m_bRecordRout = FALSE;
	lpdest = NULL;
	datasize = DEF_PACKET_SIZE;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		AfxMessageBox("Sorry, you cannot load socket dll!");
		return ;
	}
	m_hSocket = INVALID_SOCKET;

}

CPing::~CPing()
{
	if(lpdest)
		delete []lpdest;
}

  

void CPing::Ping(int timeout)
{   
	 m_hSocket = WSASocket (AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0,
                           WSA_FLAG_OVERLAPPED);

    if (m_hSocket == INVALID_SOCKET) 
    {
        AfxMessageBox("socket 创建失败!");
        return ;
    }

    if (m_bRecordRout)
    {
        // Setup the IP option header to go out on every ICMP packet
        //
        ZeroMemory(&m_ipopt, sizeof(m_ipopt));
        m_ipopt.code = IP_RECORD_ROUTE; // Record route option
        m_ipopt.ptr  = 4;               // Point to the first addr offset
        m_ipopt.len  = 39;              // Length of option header
  
        int ret = setsockopt(m_hSocket, IPPROTO_IP, IP_OPTIONS, 
            (char *)&m_ipopt, sizeof(m_ipopt));
        if (ret == SOCKET_ERROR)
        {
            AfxMessageBox("设置socket协议选项错误!");
        }
    }

    // Set the send/recv timeout values
    //
    int bread = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, 
                (char*)&timeout, sizeof(timeout));
    if(bread == SOCKET_ERROR) 
    {
        AfxMessageBox("设置socket接收超时选项错误!");
        return ;
    }
    timeout = 1000;
    bread = setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, 
                (char*)&timeout, sizeof(timeout));
    if (bread == SOCKET_ERROR) 
    {
        AfxMessageBox("设置socket发送超时选项错误!");
        return ;
    }
    memset(&m_addrDest, 0, sizeof(m_addrDest));
    //
    // Resolve the endpoint's name if necessary
    //
    m_addrDest.sin_family = AF_INET;
    if ((m_addrDest.sin_addr.s_addr = inet_addr(lpdest)) == INADDR_NONE)
    {   
		 struct hostent *hp = NULL;

        if ((hp = gethostbyname(lpdest)) != NULL)
        {
            memcpy(&(m_addrDest.sin_addr), hp->h_addr, hp->h_length);
            m_addrDest.sin_family = hp->h_addrtype;
//            printf("m_addrDest.sin_addr = %s\n", inet_ntoa(m_addrDest.sin_addr));
        }
        else
        {
//            printf("gethostbyname() failed: %d\n", 
 //               WSAGetLastError());
			AfxMessageBox("输入的主机不存在!");
            return ;
        }
    }        

    // 
    // Create the ICMP packet
    //       
    datasize += sizeof(IcmpHeader);  

    icmp_data =(char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                  MAX_PACKET);
    recvbuf =(char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                  MAX_PACKET);
    if (!icmp_data) 
    {
        AfxMessageBox("堆分配错误!");
        return ;
    }
    memset(icmp_data,0,MAX_PACKET);
    FillICMPData(icmp_data,datasize);
    //
    // Start sending/receiving ICMP packets
    //
	//static int nCount = 0;
	int nCount=0;
    while(1) 
    {
        int        bwrote;
                
        if (nCount++ == 4) 
            break;
                
        ((IcmpHeader*)icmp_data)->i_cksum = 0;
        ((IcmpHeader*)icmp_data)->timestamp = GetTickCount();
        ((IcmpHeader*)icmp_data)->i_seq = seq_no++;
        ((IcmpHeader*)icmp_data)->i_cksum = 
            checksum((USHORT*)icmp_data, datasize);

        bwrote = sendto(m_hSocket, icmp_data, datasize, 0, 
                     (struct sockaddr*)&m_addrDest, sizeof(m_addrDest));
        if (bwrote == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) 
            {
				m_dlg->m_result+="Timed out ! \r\n";
                m_dlg->SetDlgItemText(IDC_EDIT2,m_dlg->m_result);
                continue;
            }
			AfxMessageBox("发送数据函数调用错误!");
            return ;
        }
        if (bwrote < datasize) 
        {
			CString temp;
			temp.Format("Wrote %d bytes \r\n", bwrote);
			m_dlg->m_result+=temp;
            m_dlg->SetDlgItemText(IDC_EDIT2,m_dlg->m_result);
        }

		int fromlen = sizeof(m_addrFrom);
        bread = recvfrom(m_hSocket, recvbuf, MAX_PACKET, 0, 
                    (struct sockaddr*)&m_addrFrom, &fromlen);
        if (bread == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) 
            {
				m_dlg->m_result+="Timed out !\r\n";
                m_dlg->SetDlgItemText(IDC_EDIT2,m_dlg->m_result);
                continue;
            }
			AfxMessageBox("接收数据函数调用错误!");
            return ;
        }
        DecodeICMPHeader(recvbuf, bread, &m_addrFrom);
        
    }
}

void CPing::Cleanup()
{
  if (m_hSocket != INVALID_SOCKET) 
        closesocket(m_hSocket);
    HeapFree(GetProcessHeap(), 0, recvbuf);
    HeapFree(GetProcessHeap(), 0, icmp_data);

//    WSACleanup();
    return ;
}

void CPing::FillICMPData(char *icmp_data, int datasize)
{
   IcmpHeader *icmp_hdr = NULL;
    char       *datapart = NULL;

    icmp_hdr = (IcmpHeader*)icmp_data;
    icmp_hdr->i_type = ICMP_ECHO;        // Request an ICMP echo
    icmp_hdr->i_code = 0;
    icmp_hdr->i_id = (USHORT)GetCurrentProcessId();
    icmp_hdr->i_cksum = 0;
    icmp_hdr->i_seq = 0;
  
    datapart = icmp_data + sizeof(IcmpHeader);
}

void CPing::DecodeIPOptions(char *buf, int bytes)
{
     IpOptionHeader *ipopt = NULL;
    IN_ADDR         inaddr;
    int             i;
    HOSTENT        *host = NULL;

    ipopt = (IpOptionHeader *)(buf + 20);

	m_dlg->m_result+="Ping 结果:   \r\n";
    m_dlg->SetDlgItemText(IDC_EDIT2,m_dlg->m_result);
    for(i = 0; i < (ipopt->ptr / 4) - 1; i++)
    {
        inaddr.S_un.S_addr = ipopt->addr[i];
        if (i != 0)
		{
//			m_dlg->m_result+="  ";
			m_dlg->SetDlgItemText(IDC_EDIT2,m_dlg->m_result);
		}
        host = gethostbyaddr((char *)&inaddr.S_un.S_addr,
                    sizeof(inaddr.S_un.S_addr), AF_INET);

		CString temp;
        if (host)
		{
			temp.Format("(%-15s) %s \r\n", inet_ntoa(inaddr), host->h_name);
			m_dlg->m_result+=temp;
			m_dlg->SetDlgItemText(IDC_EDIT2,m_dlg->m_result);
		}
        else
		{
			temp.Format("(%-15s)\r\n", inet_ntoa(inaddr));
			m_dlg->m_result+=temp;
			m_dlg->SetDlgItemText(IDC_EDIT2,m_dlg->m_result);
		}
    }
    return;
}

USHORT CPing::checksum(USHORT *buffer, int size)
{
   unsigned long cksum=0;

    while (size > 1) 
    {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if (size) 
    {
        cksum += *(UCHAR*)buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    return (USHORT)(~cksum);
}

void CPing::SetConfigure(char * host,BOOL recordrout,int size)
{
	if(lpdest)
	{
		delete[] lpdest;
		lpdest=NULL;
	}
    m_bRecordRout = recordrout;
    datasize = size;
    lpdest = new char [strlen(host)+1];
	strcpy(lpdest,host);
}

void CPing::DecodeICMPHeader(char *buf, int bytes, SOCKADDR_IN *from)
{
   IpHeader       *iphdr = NULL;
    IcmpHeader     *icmphdr = NULL;
    unsigned short  iphdrlen;
    DWORD           tick;
    static   int    icmpcount = 0;

    iphdr = (IpHeader *)buf;
	// Number of 32-bit words * 4 = bytes
    iphdrlen = iphdr->h_len * 4;
    tick = GetTickCount();

    if ((iphdrlen == MAX_IP_HDR_SIZE) && (!icmpcount))
        DecodeIPOptions(buf, bytes);

	CString temp;
    if (bytes  < iphdrlen + ICMP_MIN) 
    {
		temp.Format("Too few bytes from %s \r\n",inet_ntoa(from->sin_addr));
		m_dlg->m_result+=temp;
		m_dlg->SetDlgItemText(IDC_EDIT2,m_dlg->m_result);
    }
    icmphdr = (IcmpHeader*)(buf + iphdrlen);

    if (icmphdr->i_type != ICMP_ECHOREPLY) 
    {
		temp.Format("nonecho type %d recvd \r\n", icmphdr->i_type);
		m_dlg->m_result+=temp;
		m_dlg->SetDlgItemText(IDC_EDIT2,m_dlg->m_result);
        return;
    }
    // Make sure this is an ICMP reply to something we sent!
    //
    if (icmphdr->i_id != (USHORT)GetCurrentProcessId()) 
    {
		temp.Format("someone else's packet! \r\n");
		m_dlg->m_result+=temp;
		m_dlg->SetDlgItemText(IDC_EDIT2,m_dlg->m_result);
        return ;
    }

	temp.Format("%d bytes from %s: \r\n", bytes, inet_ntoa(from->sin_addr));
	m_dlg->m_result+=temp;
	temp.Format(" icmp_seq = %d. \r\n", icmphdr->i_seq);
	m_dlg->m_result+=temp;
	temp.Format(" time: %d ms \r\n", tick - icmphdr->timestamp);
	m_dlg->m_result+=temp;

	m_dlg->SetDlgItemText(IDC_EDIT2,m_dlg->m_result);

    icmpcount++;
    return;
}

