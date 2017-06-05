#if !defined(HUFFCOMPRESS_H)
#define HUFFCOMPRESS_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" 
{
#endif
//»ô·òÂüÑ¹Ëõ±àÂëº¯Êý¼¯
void	HuffmanInitArrays();
void	HuffmanBuildArrays();
void	HuffmanBuildByteTree();
void	HuffmanBuildCodes();
void	HuffmanQuickSortA(DWORD *pArray1,DWORD *pArray2,int iFirst,int iLast);
void	HuffmanQuickSortD(DWORD *pArray1,DWORD *pArray2,int iFirst,int iLast);
WORD	HuffmanDictionary(BYTE *pInput,DWORD dwCount,DWORD *pByteTree,DWORD *pCodes);
DWORD	HuffmanCountCompress(BYTE *pInput,DWORD dwCount,WORD iTreeSize,DWORD *pCodes);
DWORD	HuffmanCompress(BYTE *pInput,DWORD dwCount,WORD iTreeSize,DWORD *pByteTree,DWORD *pCodes,BYTE *pOutput);
DWORD	HuffmanGetSize(BYTE *pInput);
DWORD	HuffmanUnCompress(BYTE *pInput,BYTE *pOutput);
void	D2W(void* dest,void* src,DWORD count);
void	W2D(void* dest,void* src,DWORD count);

#ifdef __cplusplus
}
#endif
#endif