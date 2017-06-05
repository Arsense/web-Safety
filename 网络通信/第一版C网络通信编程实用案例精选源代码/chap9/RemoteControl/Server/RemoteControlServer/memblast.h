//#if !defined(MEMBLAST_H)
//#define MEMBLAST_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" 
{
#endif

__inline void memblast(void* dest,void* src,DWORD count);

#ifdef __cplusplus
}
#endif
//#endif