#if !defined(GDI_H)
#define GDI_H

#include <windows.h>

struct GdiDS
{
	char	*pDIBitmap;

	char	*pDIB;
	char	*pStartDIB;

	int		iGridX;
	int		iGridY;

	int		iWidth1;
	int		iWidth2;
	int		iHeight1;
	int		iHeight2;

	LPBITMAPINFOHEADER	lpBitmapIH;

	BOOL	fDIBitmap;
};

struct GdiList
{
	struct	GdiDS	Gdi;
	struct	GdiList	*pNext;
};

struct	GdiList	*Add_Gdi(struct GdiList *pNode,struct GdiDS Gdi);
void			Clear_Gdi(struct GdiList *pStart);

#endif // #define GDI_H