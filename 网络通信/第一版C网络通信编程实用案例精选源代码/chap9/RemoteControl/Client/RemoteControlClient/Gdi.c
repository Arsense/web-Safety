#include "Gdi.h"
#include "Client.h"
#include <windows.h>

struct GdiList *Add_Gdi(struct GdiList *pNode,struct GdiDS Gdi)
{
	if (pNode->pNext = (struct GdiList *)malloc(sizeof(struct GdiList)))
	{
		pNode = pNode->pNext;
		pNode->Gdi.pDIBitmap = Gdi.pDIBitmap;

		pNode->Gdi.pDIB = NULL;
		pNode->Gdi.pStartDIB = NULL;

		pNode->Gdi.iGridX = Gdi.iGridX;
		pNode->Gdi.iGridY = Gdi.iGridY;

		pNode->Gdi.iWidth1 = Gdi.iWidth1;
		pNode->Gdi.iWidth2 = Gdi.iWidth2;
		pNode->Gdi.iHeight1 = Gdi.iHeight1;
		pNode->Gdi.iHeight2 = Gdi.iHeight2;
		pNode->Gdi.lpBitmapIH = Gdi.lpBitmapIH;
		pNode->Gdi.fDIBitmap = Gdi.fDIBitmap;
		pNode->pNext = NULL;
		return pNode;
	}
	return NULL;
}

void Clear_Gdi(struct GdiList *pStart)
{
	struct	GdiList	*pPrev;
	struct	GdiList	*pNode;
	while (pNode = pStart->pNext)
	{
		pPrev = pStart;
		pPrev->pNext = pNode->pNext;
		if (pNode->Gdi.fDIBitmap)
		{
			free(pNode->Gdi.pDIBitmap);
			free(pNode->Gdi.pDIB);
		}
		free(pNode);
	}
}