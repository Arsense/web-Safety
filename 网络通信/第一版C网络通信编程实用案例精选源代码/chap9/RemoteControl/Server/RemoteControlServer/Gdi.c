#include "Gdi.h"
#include "Server.h"
#include <windows.h>

// 添加一个元素到GDI链表
struct GdiList *Add_Gdi(struct GdiList *pNode,struct GdiDS Gdi)
{
	// 添加到链表的末尾
	if (pNode->pNext = (struct GdiList *)malloc(sizeof(struct GdiList)))
	{
		pNode = pNode->pNext;

		// 添加网格坐标
		pNode->Gdi.iGridX = Gdi.iGridX;
		pNode->Gdi.iGridY = Gdi.iGridY;

		// 设置区域的矩形坐标
		pNode->Gdi.iWidth1 = Gdi.iWidth1;
		pNode->Gdi.iWidth2 = Gdi.iWidth2;
		pNode->Gdi.iHeight1 = Gdi.iHeight1;
		pNode->Gdi.iHeight2 = Gdi.iHeight2;

		// 设置DIB颜色表的颜色数
		pNode->Gdi.nColors = Gdi.nColors;

		// 设置DIB信息头的字节数
		pNode->Gdi.dwBitMapHeader = Gdi.dwBitMapHeader;

		// 设置位图长度和起始坐标
		pNode->Gdi.dwLen = Gdi.dwLen;
		pNode->Gdi.dwCompress = Gdi.dwCompress;
		pNode->Gdi.iStartPos = Gdi.iStartPos;

		//设置DIB
		pNode->Gdi.DIBitmap = Gdi.DIBitmap;

		// 设置DIB信息头
		pNode->Gdi.BMIH = Gdi.BMIH;

		// 设置DIB信息头的指针
		pNode->Gdi.lpBMIH = Gdi.lpBMIH;

		// 设置区域的装置设备句柄
		pNode->Gdi.hMemDC = Gdi.hMemDC;

		// 设置区域的位图句柄
		pNode->Gdi.hDIBitmap = Gdi.hDIBitmap;

		// 区域无压缩DIB的指针
		pNode->Gdi.pDIB = Gdi.pDIB;

		//设置指向区域DIB变化的部分的指针
		pNode->Gdi.pDIBChange = Gdi.pDIBChange;

		//设置指向压缩区域的DIB的指针
		pNode->Gdi.pDIBCompress = Gdi.pDIBCompress;

		//设置指向全局区域位图指针
		pNode->Gdi.pDIBitmap = Gdi.pDIBitmap;

		// 区域位图标志
		pNode->Gdi.fDIBitmap = Gdi.fDIBitmap;
		pNode->Gdi.fChange = Gdi.fChange;

		pNode->pNext = NULL;
		return pNode;
	}
	return NULL;
}

// 完全清楚GDI链表
void Clear_Gdi(struct GdiList *pStart)
{
	struct	GdiList	*pPrev;
	struct	GdiList	*pNode;
	while (pNode = pStart->pNext)
	{
		pPrev = pStart;
		pPrev->pNext = pNode->pNext;
		DeleteDC(pNode->Gdi.hMemDC);
		DeleteObject(pNode->Gdi.hDIBitmap);
		if (pNode->Gdi.fDIBitmap)
		{
			free(pNode->Gdi.pDIBitmap);
			free(pNode->Gdi.pDIB);
			free(pNode->Gdi.pDIBChangeStart);
		}
		free(pNode);
	}
}