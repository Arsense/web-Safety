#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main()
{
	char IP[20]={0};


	scanf("%s",IP);


	char* p = IP;

	char Temp[20]={0};


	char a[4]={0};
	char b[2]={0};
	
	int i = 0;

	int j = 0;

	while(p!=NULL)
	{
	  p = strstr(IP,".");
  
     if (p!=NULL)
     {
		 strcpy(Temp,p+1);
		 
		 
		 *p='\0';


		 i = atoi(IP);
		 
		 strcpy(IP,Temp);
		 
	 
		 a[j] = char(i);
	
     }
		
	 
       else
	   {
		   i = atoi(Temp);

		   a[j] = char(i);
	   }
		
	 
	 	j++;
	}



	for(i=0;i<4;i++)
	{
		printf("%x",a[i]);
	}


	printf("\n");
	

	char Port[20]={0};
	
	scanf("%s",Port);
	
	
	i = 0;
	
	i = atoi(Port);

	b[0] = (char)(i/256);

	b[1] = (char)(i-b[0]*256);




	printf("%x",b[0]);

	printf("%x",b[1]);





}

	




	













