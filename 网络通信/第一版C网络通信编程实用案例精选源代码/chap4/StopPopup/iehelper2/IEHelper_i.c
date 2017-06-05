/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Mar 26 14:17:45 2003
 */
/* Compiler settings for IEHelper.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IIEHlprObj = {0xCE7C3CEF,0x4B15,0x11D1,{0xAB,0xED,0x70,0x95,0x49,0xC1,0x00,0x00}};


const IID LIBID_IEHELPERLib = {0xCE7C3CE2,0x4B15,0x11D1,{0xAB,0xED,0x70,0x95,0x49,0xC1,0x00,0x00}};


const CLSID CLSID_IEHlprObj = {0xCE7C3CF0,0x4B15,0x11D1,{0xAB,0xED,0x70,0x95,0x49,0xC1,0x00,0x00}};


#ifdef __cplusplus
}
#endif

