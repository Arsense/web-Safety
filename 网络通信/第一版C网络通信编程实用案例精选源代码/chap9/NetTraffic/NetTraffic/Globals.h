#ifndef __GOBALS_H__
#define __GOBALS_H__

typedef struct _TRAFFIC_ENTRY_
{
	double value;
	BOOL connected;
} TRAFFICENTRY;

//定义图中的矩形宽度
#define PLOTGRANULATRITY 2		

//数据定时器的更新速率
#define NETUPDATESPEED	1000	
//网格更新速率
#define GRIDUPDATESPEED 50		

//网格定时器的ID
#define GRIDTIMER		1		
//数据定时器的ID
#define NETTIMER		2		

//在x方向的网格距离	
#define GRIDXRESOLUTION	10		
//在y方向的网格距离
#define GRIDYRESOLUTION	10		

//在x方向的网格滚动条的滚动速度
#define GRIDSCROLLXSPEED -1		
//在y方向的网格滚动条的滚动速度
#define GRIDSCROLLYSPEED 0		

#endif