#if !defined(GDI_H)
#define GDI_H

#include <windows.h>

// GDI数据结构
struct GdiDS
{
	// 网格坐标
	int		iGridX;
	int		iGridY;

	// 网格矩形区域
	int		iWidth1;
	int		iWidth2;
	int		iHeight1;
	int		iHeight2;

	// 设备无关位图(DIB)颜色表中的色彩的数目
	int		nColors;

	// DIB信息头的字节数
	DWORD	dwBitMapHeader;

	// DIB 长度和起始坐标
	DWORD	dwLen;
	DWORD	dwCompress;
	DWORD	iStartPos;

	// DIB
	BITMAP	DIBitmap;

	// DIB 信息头
	BITMAPINFOHEADER	BMIH;

	// DIB信息头的指针
	LPBITMAPINFOHEADER	lpBMIH;

	// 区域的设备句柄HDC
	HDC		hMemDC;

	// 区域的位图句柄
	HBITMAP	hDIBitmap;

	// 无压缩的DIB区域指针
	char	*pDIB;

	// 指向区域DIB的变化的指针
	char	*pDIBChange;
	char	*pDIBChangeStart;

	// 指向压缩区域的DIB指针
	char	*pDIBCompress;

	// 指向全局区域的DIB
	char	*pDIBitmap;

	// DIB标志
	BOOL	fDIBitmap;
	BOOL	fChange;
};

// Gdi 链表
struct GdiList
{
	struct	GdiDS	Gdi;
	struct	GdiList	*pNext;
};

struct	GdiList	*Add_Gdi(struct GdiList *pNode,struct GdiDS Gdi);
void			Clear_Gdi(struct GdiList *pStart);

#endif 
