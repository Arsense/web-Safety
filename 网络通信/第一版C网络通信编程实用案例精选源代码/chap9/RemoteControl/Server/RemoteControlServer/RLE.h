#if !defined(RLE_H)
#define RLE_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" 
{
#endif

//Run Length ±àÂë/½âÂëº¯Êý¼¯
DWORD	RunLength(BYTE *pInput,DWORD dwCount);
DWORD	RunLengthEncode(BYTE *pInput,DWORD dwCount,BYTE *pOutput);
DWORD	RunLengthDecode(BYTE *pInput,BYTE *pOutput);

#ifdef __cplusplus
}
#endif
#endif