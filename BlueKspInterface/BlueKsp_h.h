

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 03:14:07 2038
 */
/* Compiler settings for ..\KeyStorageProvider\PhoneKSP\BlueKsp.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __BlueKsp_h_h__
#define __BlueKsp_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IFoo_FWD_DEFINED__
#define __IFoo_FWD_DEFINED__
typedef interface IFoo IFoo;

#endif 	/* __IFoo_FWD_DEFINED__ */


#ifndef __IBlueKey_FWD_DEFINED__
#define __IBlueKey_FWD_DEFINED__
typedef interface IBlueKey IBlueKey;

#endif 	/* __IBlueKey_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_BlueKsp_0000_0000 */
/* [local] */ 

typedef struct NCryptKeyName
    {
    LPWSTR pszName;
    LPWSTR pszAlgid;
    DWORD dwLegacyKeySpec;
    DWORD dwFlags;
    } 	NCryptKeyName;



extern RPC_IF_HANDLE __MIDL_itf_BlueKsp_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_BlueKsp_0000_0000_v0_0_s_ifspec;


#ifndef __MyLibrary_LIBRARY_DEFINED__
#define __MyLibrary_LIBRARY_DEFINED__

/* library MyLibrary */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_MyLibrary;

#ifndef __IFoo_INTERFACE_DEFINED__
#define __IFoo_INTERFACE_DEFINED__

/* interface IFoo */
/* [uuid][object] */ 


EXTERN_C const IID IID_IFoo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("400075B9-4BD6-45A5-B8B7-9DA0CF7B9B13")
    IFoo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE DoFoo( 
            /* [in] */ long arg,
            /* [retval][out] */ long *result) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IFooVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFoo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFoo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFoo * This);
        
        HRESULT ( STDMETHODCALLTYPE *DoFoo )( 
            IFoo * This,
            /* [in] */ long arg,
            /* [retval][out] */ long *result);
        
        END_INTERFACE
    } IFooVtbl;

    interface IFoo
    {
        CONST_VTBL struct IFooVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFoo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFoo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFoo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFoo_DoFoo(This,arg,result)	\
    ( (This)->lpVtbl -> DoFoo(This,arg,result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFoo_INTERFACE_DEFINED__ */


#ifndef __IBlueKey_INTERFACE_DEFINED__
#define __IBlueKey_INTERFACE_DEFINED__

/* interface IBlueKey */
/* [uuid][object][local] */ 


EXTERN_C const IID IID_IBlueKey;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c0338938-2f32-412a-8dd2-3027ea61a732")
    IBlueKey : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCryptKeyName( 
            /* [annotation][out] */ 
            _Out_  NCryptKeyName *keyName) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBlueKeyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBlueKey * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBlueKey * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBlueKey * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCryptKeyName )( 
            IBlueKey * This,
            /* [annotation][out] */ 
            _Out_  NCryptKeyName *keyName);
        
        END_INTERFACE
    } IBlueKeyVtbl;

    interface IBlueKey
    {
        CONST_VTBL struct IBlueKeyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBlueKey_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBlueKey_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBlueKey_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBlueKey_GetCryptKeyName(This,keyName)	\
    ( (This)->lpVtbl -> GetCryptKeyName(This,keyName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBlueKey_INTERFACE_DEFINED__ */

#endif /* __MyLibrary_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


