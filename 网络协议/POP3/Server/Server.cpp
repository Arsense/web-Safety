#include <winsock2.h>
#pragma comment(lib, "WS2_32")

#include <stdio.h>
#include <windows.h>

//测试线程
//DWORD WINAPI RecvProc(LPVOID lpParam);
//DWORD WINAPI SendProc(LPVOID lpParam);



//创建个人用户在邮件服务器中的文件夹
void CreateUserDirectory(char* UserName);  

//确认文件夹中文件文件个数
//对与我们的文件名字的传送需要算法或者数据库因为我们无法确定传送的字节数
void  CheckFile(char* UserDirect,int* FileNum,char* FileName,char* Name);



//查找文件并返回信息
void FindFileByIndex(char a,char* Length,char* pName);



//接受到代理端发送过来的用户姓名和Pass进行验证
DWORD WINAPI RecvProc(LPVOID lpParam);




typedef struct _SOCKETLIST_ 
{
	SOCKET sClient;

	_SOCKETLIST_* pNext;

}SocketList,*pSocketList;


pSocketList pSocketHead = NULL;
pSocketList pSocketTail = NULL;


//记录用户信息的结构
typedef struct _USERINFOR_
{
	char NameBuffer[20];
	char PassBuffer[20];
	char UserDirec[50];

	_USERINFOR_* pNext;

}UserInfor,*pUserInfor;

pUserInfor pUserHead = NULL;
pUserInfor pUserTail = NULL;

//用户文件夹中文件的信息

typedef struct _PERSON_ 
{
	char NameBuffer[20];
	char Index;
	char FileName[100];			//这里不能记录大型文件的大小
	char FileLength;
	_PERSON_* pPersonNext;

}Person,*pPerson;




typedef struct _FILEINFOR_
{
     pPerson pPersonHead;
	 pPerson pPersonTail;

	_FILEINFOR_* pFileInforNext;

}FileInfor,*pFileInfor;


pFileInfor pFileHead = NULL;
pFileInfor pFileTail = NULL;




pPerson CreatePerson(char i,char* FileName,char* Name);
pFileInfor CreateFileInfor();
bool CheckInsert(pFileInfor pFileInforTemp,pPerson pPersonTemp);
void InsertPersonToFileInfor(pPerson pPersonTemp);
void LinkFileInfor(pFileInfor pFileInforTemp);

void CreateSocketList(SOCKET& sClient);

void CreateUserList(char* UserBuffer,char* PassBuffer);

void main()
{
	
	//我在这里就直接将其设死了

	char UserBuffer[20] = "shine";
	char PassBuffer[20] = "1234";

	CreateUserList(UserBuffer,PassBuffer);


	memset(UserBuffer,0,20);
	memset(PassBuffer,0,20);

	strcpy(UserBuffer,"wangxue");
	strcpy(PassBuffer,"1234");

	CreateUserList(UserBuffer,PassBuffer);


	pFileInfor pFileInforTemp = CreateFileInfor();

	LinkFileInfor(pFileInforTemp);
	
	
	
	
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2,2);
	if(WSAStartup(sockVersion, &wsaData) != 0)
	{
		exit(0);
	}


	SOCKET sListen;
	
	
	sListen= socket(AF_INET,SOCK_STREAM,0);
	
	if (sListen==INVALID_SOCKET)
	{
		printf("Create Listen Socket Error\n");
		
		return;
	}
	
	//初始化网卡
	sockaddr_in ProxyAddr;
	
	ProxyAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	ProxyAddr.sin_family = AF_INET;
	ProxyAddr.sin_port = htons(110);
	
	//绑定
	int Ret = bind(sListen,(sockaddr*)&ProxyAddr,sizeof(ProxyAddr));
	
	if (Ret==SOCKET_ERROR)
	{
		printf("Bind Error\n");
		
		closesocket(sListen);
		
		
		return;
	}
	
	//监听
	
	listen(sListen,5);
	
	//创建一个与客户通信的套接字
	
	SOCKET sClient;
	
	
	sockaddr_in ClientAddr;
	
	int nLen = sizeof(ClientAddr);

	HANDLE hThread[2];


	bool bOK = false;
	while (1)
	{
		sClient = accept(sListen,(sockaddr*)&ClientAddr,&nLen);

		CreateSocketList(sClient);


		if (!bOK)
		{
	
//			hThread[1] = CreateThread(NULL,0,SendProc,(LPVOID)pSocketHead,0,NULL);

		

			bOK = true;
		}
		
//		hThread[0] = CreateThread(NULL,0,RecvProc,(LPVOID)pSocketTail,0,NULL);


		hThread[0] = CreateThread(NULL,0,RecvProc,(LPVOID)pSocketTail,0,NULL);

	}



	Sleep(INFINITE);
	//WaitForMultipleObjects(1,hThread[0],false,INFINITE);

}

DWORD WINAPI RecvProc(LPVOID lpParam)
{
	SOCKET s = ((pSocketList)lpParam)->sClient;

	int  FileNum = 0;                 //个人文件夹中的文件个数
	char FileName[4096] = {0};       //个人文件夹中各个文件名字并且以\r\n为区分文件名字的标记   
	
	char Buffer[512]={0};

	char Name[20] = {0};
    char Pass[20] = {0};


	pUserInfor pUserTemp = pUserHead; 
	
	while (1)
	{
		int Ret = recv(s,Buffer,512,0);    //这里只接受一次就可以了
		
		Buffer[Ret]='\0';
		
		//测试
	    // printf("%s\n",Buffer);


		//我们在这里得到代理端传输来的用户名和密码我们要在这里进行验证


		char* pTemp = Buffer;

	
		char i = 0;

		if (strstr(Buffer,"USER "))
		{
			pTemp = pTemp+strlen("USER ");

			while (*pTemp!='\r'&&*(pTemp+1)!='\n')
			{

				//这里最好就是计算一下姓名的长度动态申请内存，我在这里就用数组了

				Name[i] = *pTemp;

				pTemp++;

				i++;
			}

			Name[i]='\0';

	
			//接受密码就不需要我们再次调用recv函数因为我们的客户端程序执行的很快

	        i=0;
		  //memset(Buffer,0,512);
		  //Ret = recv(s,Buffer,512,0);


		  //Buffer[Ret]='\0';

		  //以上的注释要好好体会
			
		    pTemp = Buffer;

			if (strstr(Buffer,"PASS "))
			{
			
				_asm
				{
					mov pTemp,eax     //投机了
				}

				pTemp= pTemp+strlen("PASS ");
				
				while (*pTemp!='\r'&&*(pTemp+1)!='\n')
				{
					
					//这里最好就是计算一下姓名的长度动态申请内存，我在这里就用数组了
					
					Pass[i] = *pTemp;
					
					pTemp++;
					
					i++;
				}
				
				Pass[i]='\0';

				while (pUserTemp!=NULL)
				{
					if (strcmp(pUserTemp->NameBuffer,Name)==0&&strcmp(pUserTemp->PassBuffer,Pass)==0)
					{

						printf("User:%s Comming\n",Name);
						memset(Buffer,0,512);

					
						strcat(Buffer,"welcome ");

						strcat(Buffer,Name);
						send(s,Buffer,strlen(Buffer),0);      
	
					    CheckFile(pUserTemp->UserDirec,&FileNum,FileName,Name);
						break;
					
					}
					
					else
					{
						//错误处理要使用send函数
					}

					pUserTemp = pUserTemp->pNext;
				}
				
			}

			else
			{
				//最好在这里设置一个错误处理要使用一个send函数
			}
		
		}

		if (strstr(Buffer,"STAT"))
		{
			
			
			sprintf(Buffer,"+OK %d ",FileNum);        //从邮件服务端正确返回的指令是+OK 错误的返回指令是-ERR

			strcat(Buffer,FileName);
			
			send(s,Buffer,strlen(Buffer),0);

		}

		if (strstr(Buffer,"LIST"))
		{
		
			
			pTemp = Buffer+strlen("LIST ");

			char a = *pTemp;

			char Length = '0';

			FindFileByIndex(a,&Length,Name);

			int j = 0;

			if (Length=='0')
			{
				j = sprintf(Buffer,"-ERR 0 ");

				sprintf(Buffer+j,"0");

				send(s,Buffer,strlen(Buffer),0);
			}

			else
			{
				a = a-'0';
				j = sprintf(Buffer,"+OK %d ",a);
				
				sprintf(Buffer+j,"%d",Length);
				
				send(s,Buffer,strlen(Buffer),0);

			}

		
		}
		
		memset(Buffer,0,512);
	}
	
	
	return 0;
}





/*这里的两个线程与Client中的测试线程对应
DWORD WINAPI RecvProc(LPVOID lpParam)
{
	SOCKET s = ((pSocketList)lpParam)->sClient;

	char Buffer[512]={0};

	while (1)
	{
		int Ret = recv(s,Buffer,512,0);

		Buffer[Ret]='\0';

		printf("%s\n",Buffer);


		memset(Buffer,0,512);
	}


	return 0;
}


DWORD WINAPI SendProc(LPVOID lpParam)
{
	pSocketList pTempHead  = (pSocketList)lpParam;
	
	char Buffer[512]={0};
	
	int Num = 0;


	while (1)
	{
		pSocketList pTemp = pTempHead;

		
		
		Num = 0;
		scanf("%d",&Num);

		Num = Num-1;
		while (Num--)
		{
			pTemp = pTemp->pNext;
		}
		
		scanf("%s",Buffer);
		
		int Ret = send(pTemp->sClient,Buffer,strlen(Buffer),0);	
		
		memset(Buffer,0,512);
	}


	return 0;
}

*/

//辅助函数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateSocketList(SOCKET& sClient)
{
	pSocketList pSocketTemp;

	pSocketTemp = (pSocketList)malloc(sizeof(SOCKET));

	if (pSocketTemp==NULL)
	{
		printf("No Memory\n");

		return;
	}

	else
	{
		pSocketTemp->sClient = sClient;

		pSocketTemp->pNext = NULL;

		if (pSocketHead==NULL)
		{
			pSocketHead = pSocketTail = pSocketTemp;
		}
		else
		{
			pSocketTail->pNext = pSocketTemp;
			pSocketTail = pSocketTail->pNext;
		}
	}
}


void CreateUserList(char* UserBuffer,char* PassBuffer)
{
	pUserInfor pUserTemp;

	char CurrentDirec[50]={0};
	
	pUserTemp = (pUserInfor)malloc(sizeof(UserInfor));
	
	if (pUserTemp==NULL)
	{
		printf("No Memory\n");
		
		return;
	}
	
	else
	{
		//保存用户在邮件服务器中的个人文件夹的路径并建立文件夹

		
		GetCurrentDirectory(50,CurrentDirec);

		strcpy(pUserTemp->UserDirec,CurrentDirec);

		strcat(pUserTemp->UserDirec,"\\");
		strcat(pUserTemp->UserDirec,UserBuffer);


		CreateUserDirectory(UserBuffer);




		//设置用户在邮件服务器中的个人姓名与Password
		strcpy(pUserTemp->NameBuffer,UserBuffer);
		strcpy(pUserTemp->PassBuffer,PassBuffer);
		
		pUserTemp->pNext = NULL;
		
		if (pUserHead==NULL)
		{
			pUserHead = pUserTail = pUserTemp;
		}
		else
		{
			pUserTail->pNext = pUserTemp;
			pUserTail = pUserTemp->pNext;
		}
	}
}




void CreateUserDirectory(char* UserName)
{
	//在这里我们可以使用文件查找的几个相关函数
	
	WIN32_FIND_DATA FileData;   
	
	HANDLE hSearch;
	
	char Temp[20] = {0};
	
	strcpy(Temp,".\\");
	
	strcat(Temp,UserName);
	
	hSearch = FindFirstFile(Temp,&FileData);
	
	if (hSearch==INVALID_HANDLE_VALUE||FileData.dwFileAttributes!=FILE_ATTRIBUTE_DIRECTORY)
	{
		//如果目录中没有该用户的目录我们进行创建这和QQ中创建文件目录的方法一样是一个经常用到的知识点
		
		//在这里我们最好进行密码的验证要用到数据库或是数据结构保存密码和用户名。。。(当第二次进行操作时)
		
		CreateDirectory(Temp,NULL);
	}
	
	//最后要关闭文件句柄
	
	FindClose(hSearch);
}


void  CheckFile(char* UserDirect,int* FileNum,char* FileName,char* Name)     //最后的两个参数都是传出值
{
	   
	WIN32_FIND_DATA Data;         //文件信息结构体
	
	HANDLE hFile;

	char FileRoad[50]={0};

	char Temp[50] = {0};

	pPerson pPerTemp = NULL;                     

	
	strcpy(FileRoad,UserDirect);	
	strcat(UserDirect,"\\*.*");
	strcat(FileRoad,"\\");
	
	hFile = FindFirstFile(UserDirect,&Data);
	
	char i = 0 ;
	
	if (hFile!=INVALID_HANDLE_VALUE)
	{
		
		do 
		{
			if (Data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				continue;
			}
			
			memset(Temp,0,50);
		
			//记录文件的信息并存入链表中

			strcpy(Temp,FileRoad);

			strcat(Temp,Data.cFileName);
	
			
			
		    strcat(FileName,Data.cFileName);

			
			pPerson pPersonTemp = CreatePerson(i,Temp,Name);         //i = 0     Temp = c:\shine\3.doc    name =shine
			
			InsertPersonToFileInfor(pPersonTemp);
			
			i++;

			strcat(FileName,"\r\n");


			
			
		} while (FindNextFile(hFile,&Data));
	}



	*FileNum = i;

}


pFileInfor CreateFileInfor()
{
	pFileInfor pFileInforTemp = (pFileInfor)malloc(sizeof(FileInfor));
	if (pFileInforTemp == NULL)
	{
		printf("No Memory\n");
		
		return NULL;
	}
	
	pFileInforTemp->pPersonHead = pFileInforTemp->pPersonTail = NULL;

	pFileInforTemp->pFileInforNext = NULL;
	
	return pFileInforTemp;
}


void InsertPersonToFileInfor(pPerson pPersonTemp)
{
	pFileInfor pFileInforTemp = pFileHead;			

	while (pFileInforTemp != NULL)
	{
		//	如果插入节点成功，跳出
		if (CheckInsert(pFileInforTemp,pPersonTemp) == true)
		{
			return ;
		}
		
		//	如果插入节点不成功的话
		
		pFileInforTemp = pFileInforTemp->pFileInforNext;
	}
	
	if (pFileInforTemp == NULL)
	{
		pFileInforTemp = CreateFileInfor();
		
		LinkFileInfor(pFileInforTemp);
	}
	
	InsertPersonToFileInfor(pPersonTemp);

}

bool CheckInsert(pFileInfor pFileInforTemp,pPerson pPersonTemp)
{

	if (pFileInforTemp->pPersonHead == NULL)				
	{
		pFileInforTemp->pPersonHead = pFileInforTemp->pPersonTail = pPersonTemp;
	}
	else
	{
	
	    if (strcmp(pFileInforTemp->pPersonHead->NameBuffer,pPersonTemp->NameBuffer)==0)
	    {
			pFileInforTemp->pPersonTail->pPersonNext = pPersonTemp;
			pFileInforTemp->pPersonTail = pPersonTemp;
	    }

		else
		{
			return false;
		}
	
	}
		
	return true;
}


pPerson CreatePerson(char i,char* FileName,char* Name)
{
	//	申请一个新的节点内存空间
	pPerson pPersonTemp = (pPerson)malloc(sizeof(Person));
	
	if (pPersonTemp == NULL)
	{
		printf("No Memory\n");
		
		return NULL;
	}
	
    FILE* fp;


	pPersonTemp->pPersonNext = NULL;
	
	fp = fopen(FileName,"rb");
	
	//计算文件大小
	
	if (fp==NULL)
	{
		return NULL;
	}
	
	fseek(fp,0,SEEK_END);
	
	long nSize = ftell(fp); 
	
	fclose(fp);
	
	pPersonTemp->FileLength = (char)nSize;
	
	strcpy(pPersonTemp->NameBuffer,Name);
	
	pPersonTemp->Index = i;
	
	strcpy(pPersonTemp->FileName,FileName);
	
	return pPersonTemp;
}

void LinkFileInfor(pFileInfor pFileInforTemp)
{
	if (pFileHead == NULL)
	{
		pFileHead = pFileTail = pFileInforTemp;
	}
	else
	{
		pFileTail->pFileInforNext = pFileInforTemp;
		
		pFileTail = pFileInforTemp;
	}
}


void FindFileByIndex(char a,char* Length,char* pName)
{
   pFileInfor  pFileTemp = pFileHead;

   pPerson     pPersonTemp = NULL;
   
   a = a-'0';
   
   while (pFileTemp!=NULL)
   {	   
	  pPersonTemp = pFileTemp->pPersonHead;
	   
	   while (pPersonTemp!=NULL)
	   {
		   if ((pPersonTemp->Index==a)&&(strcmp(pPersonTemp->NameBuffer,pName)==0))
		   {
			   *Length = pPersonTemp->FileLength;
			   
			   break;
		   }

		   pPersonTemp = pPersonTemp->pPersonNext;
	   }	   

	   pFileTemp = pFileTemp->pFileInforNext;
   } 
}