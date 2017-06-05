#include <windows.h>
#include <stdio.h>


void main()
{
	WIN32_FIND_DATA Data;

	HANDLE hFile;


	//文件的路径

	char Buffer[50] ={0};


	GetCurrentDirectory(50,Buffer);


	strcat(Buffer,"\\*.*");
	
	hFile = FindFirstFile(Buffer,&Data);

	int i = 0 ;

	if (hFile!=INVALID_HANDLE_VALUE)
	{
		
		do 
		{
		   if (Data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		   {
			   continue;
		   }
			
		   i++;

		   printf("%s\n",Data.cFileName);
		   
		
		} while (FindNextFile(hFile,&Data));
	}


}