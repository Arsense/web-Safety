// Match.cpp: implementation of the Match class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FiveChess.h"
#include "Match.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Match::Match()
{
	for(int i=0;i<LW;i++)
		for(int j=0;j<LW;j++)
		{
			chessboard[i][j]=0;
		}
}

Match::~Match()
{

}
void Match::Clear()
{
	for(int i=0;i<LW;i++)
		for(int j=0;j<LW;j++)
		{
			chessboard[i][j]=0;
		}
}
BOOL Match::CanDown(int x,int y,int who)
{
	if(x<0||x>=LW||y<0||y>=LW)
		return FALSE;
	if(chessboard[x][y]!=0)
		return FALSE;
	chessboard[x][y]=who;
	return TRUE;
}
BOOL Match::IsWin(int who,int pos[5][2])
{
	int i,j;
	for(i=0;i<LW;i++)
		for(j=0;j<LW;j++)
		{
			if(chessboard[i][j]==who)
			{
				if(j+4<LW)//水平
				{
					if(chessboard[i][j+1]==who&&chessboard[i][j+2]==who&&chessboard[i][j+3]==who&&chessboard[i][j+4]==who)
					{
						for(int k=0;k<5;k++)
						{
							pos[k][0]=i;
							pos[k][1]=j+k;
						}
						return TRUE;
					}
				}
				if(i+4<LW)//垂直
				{
					if(chessboard[i+1][j]==who&&chessboard[i+2][j]==who&&chessboard[i+3][j]==who&&chessboard[i+4][j]==who)
					{
						for(int k=0;k<5;k++)
						{
							pos[k][0]=i+k;
							pos[k][1]=j;
						}
						return TRUE;
					}
				}
				if(i+4<LW&&j+4<LW)//东南向
				{
					if(chessboard[i+1][j+1]==who&&chessboard[i+2][j+2]==who&&chessboard[i+3][j+3]==who&&chessboard[i+4][j+4]==who)
					{
						for(int k=0;k<5;k++)
						{
							pos[k][0]=i+k;
							pos[k][1]=j+k;
						}
						return TRUE;
					}
				}
				if(i-4>0&&j+4<LW)//东北
				{
					if(chessboard[i-1][j+1]==who&&chessboard[i-2][j+2]==who&&chessboard[i-3][j+3]==who&&chessboard[i-4][j+4]==who)
					{
						for(int k=0;k<5;k++)
						{
							pos[k][0]=i-k;
							pos[k][1]=j+k;
						}
						return TRUE;
					}
				}
			}
		}
	return false;
}
