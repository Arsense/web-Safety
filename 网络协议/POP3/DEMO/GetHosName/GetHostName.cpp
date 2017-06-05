#include <winsock2.h>
#include <windows.h>
#include <stdio.h>


#pragma comment(lib,"Ws2_32")
void main()
{
	char Name[20]={0};

	WSADATA wsadata;
	
	if (WSAStartup(MAKEWORD(2,2),&wsadata)!=0)
	{
		return;
	}

	gethostname(Name,20);

	

	printf("%s\n",Name);
	HOSTENT* host = gethostbyname(Name);
	
	
	if (host==NULL)
	{
		printf("Can Not Resolve Host Name\n");
		
		return;
	}
	
	printf("%s",inet_ntoa(*(struct in_addr*)host->h_addr)) ;

	

}