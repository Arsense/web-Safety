/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Mar 26 14:17:45 2003
 */
/* Compiler settings for IEHelper.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __IEHelper_h__
#define __IEHelper_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IIEHlprObj_FWD_DEFINED__
#define __IIEHlprObj_FWD_DEFINED__
typedef interface IIEHlprObj IIEHlprObj;
#endif 	/* __IIEHlprObj_FWD_DEFINED__ */


#ifndef __IEHlprObj_FWD_DEFINED__
#define __IEHlprObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class IEHlprObj IEHlprObj;
#else
typedef struct IEHlprObj IEHlprObj;
#endif /* __cplusplus */

#endif 	/* __IEHlprObj_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IIEHlprObj_INTERFACE_DEFINED__
#define __IIEHlprObj_INTERFACE_DEFINED__

/* interface IIEHlprObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IIEHlprObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CE7C3CEF-4B15-11D1-ABED-709549C10000")
    IIEHlprObj : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IIEHlprObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IIEHlprObj __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IIEHlprObj __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IIEHlprObj __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IIEHlprObj __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IIEHlprObj __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IIEHlprObj __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IIEHlprObj __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IIEHlprObjVtbl;

    interface IIEHlprObj
    {
        CONST_VTBL struct IIEHlprObjVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIEHlprObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IIEHlprObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IIEHlprObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IIEHlprObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IIEHlprObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IIEHlprObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IIEHlprObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IIEHlprObj_INTERFACE_DEFINED__ */



#ifndef __IEHELPERLib_LIBRARY_DEFINED__
#define __IEHELPERLib_LIBRARY_DEFINED__

/* library IEHELPERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_IEHELPERLib;

EXTERN_C const CLSID CLSID_IEHlprObj;

#ifdef __cplusplus

class DECLSPEC_UUID("CE7C3CF0-4B15-11D1-ABED-709549C10000")
IEHlprObj;
#endif
#endif /* __IEHELPERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
