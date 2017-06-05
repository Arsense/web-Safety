#include <windows.h>
#include <stdio.h>

void main()
{
	
	int a = 2356;

	char a1 = ((char*)&a)[0];
	
	char a2 =((char*)&a)[1];
	
	int b1 = a1;
	
	int b2 = a2;
	
	b2<<=8;
	
	b1+=b2;
	
	printf("%d",b1);


	//这里我用的方法不是很好没有用算法好好体会

}