
#pragma once

/*****************************************************************************
 * IQuerySourceOld interface
 *
 * @see https://www.geoffchappell.com/studies/windows/shell/shlwapi/interfaces/iquerysource.htm
 */
#define INTERFACE IQuerySourceOld
DECLARE_INTERFACE_(IQuerySourceOld, IUnknown) // {C7478486-7583-49E7-A6C2-FAF8F02BC30E}
{
    /*** IUnknown ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;
    /*** IQuerySourceOld ***/
    STDMETHOD(EnumValues)(THIS_ IEnumString **ppEnum);
    STDMETHOD(EnumSources)(THIS_ IEnumString **ppEnum);
    STDMETHOD(QueryValueString)(THIS_ PCWSTR keyName, PCWSTR valueName, PWSTR *ppszValue);
    STDMETHOD(QueryValueDword)(THIS_ PCWSTR keyName, PCWSTR valueName, DWORD *pdwValue);
    STDMETHOD(QueryValueExists)(THIS_ PCWSTR keyName, PCWSTR valueName);
    STDMETHOD(QueryValueDirect)(THIS_ PCWSTR keyName, PCWSTR valueName, FLAGGED_BYTE_BLOB **ppBlob);
    STDMETHOD(OpenSource)(THIS_ PCWSTR keyName, BOOL, IQuerySourceOld **ppSource);
    STDMETHOD(SetValueDirect)(THIS_ PCWSTR keyName, PCWSTR valueName, DWORD, DWORD, PBYTE);
};
#undef INTERFACE

#ifdef COBJMACROS
#define IQuerySourceOld_QueryInterface(T,a,b) (T)->lpVtbl->QueryInterface(T,a,b)
#define IQuerySourceOld_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IQuerySourceOld_Release(T) (T)->lpVtbl->Release(T)
#define IQuerySourceOld_EnumValues(T,a) (T)->lpVtbl->EnumValues(T,a)
#define IQuerySourceOld_EnumSources(T,a) (T)->lpVtbl->EnumSources(T,a)
#define IQuerySourceOld_QueryValueString(T,a,b,c) (T)->lpVtbl->QueryValueString(T,a,b,c)
#define IQuerySourceOld_QueryValueDword(T,a,b,c) (T)->lpVtbl->QueryValueDword(T,a,b,c)
#define IQuerySourceOld_QueryValueExists(T,a,b) (T)->lpVtbl->QueryValueExists(T,a,b)
#define IQuerySourceOld_QueryValueDirect(T,a,b,c) (T)->lpVtbl->QueryValueDirect(T,a,b,c)
#define IQuerySourceOld_OpenSource(T,a,b,c) (T)->lpVtbl->OpenSource(T,a,b,c)
#define IQuerySourceOld_SetValueDirect(T,a,b,c,d,e) (T)->lpVtbl->SetValueDirect(T,a,b,c,d,e)
#endif

/*****************************************************************************
 * IQuerySource interface (new version)
 *
 * @see https://www.geoffchappell.com/studies/windows/shell/shlwapi/interfaces/iquerysource.htm
 */
#define INTERFACE IQuerySource
DECLARE_INTERFACE_(IQuerySource, IUnknown) // {7BC28AC2-0D9C-4941-BB9A-72BECB184FAC}
{
    /*** IUnknown ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;
    /*** IQuerySource ***/
    STDMETHOD(EnumValues)(THIS_ IEnumString **ppEnum);
    STDMETHOD(QueryValueString)(THIS_ PCWSTR keyName, PCWSTR valueName, PWSTR *ppszValue);
    STDMETHOD(QueryValueDword)(THIS_ PCWSTR keyName, PCWSTR valueName, DWORD *pdwValue);
    STDMETHOD(QueryValueGuid)(THIS_ PCWSTR keyName, PCWSTR valueName, GUID *guid);
    STDMETHOD(QueryValueExists)(THIS_ PCWSTR keyName, PCWSTR valueName);
    STDMETHOD(QueryValueDirect)(THIS_ PCWSTR keyName, PCWSTR valueName, FLAGGED_BYTE_BLOB **ppBlob);
    STDMETHOD(EnumSources)(THIS_ IEnumString **ppEnum);
    STDMETHOD(OpenSource)(THIS_ PCWSTR keyName, IQuerySource **ppSource);
};
#undef INTERFACE

#ifdef COBJMACROS
#define IQuerySource_QueryInterface(T,a,b) (T)->lpVtbl->QueryInterface(T,a,b)
#define IQuerySource_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IQuerySource_Release(T) (T)->lpVtbl->Release(T)
#define IQuerySource_EnumValues(T,a) (T)->lpVtbl->EnumValues(T,a)
#define IQuerySource_QueryValueString(T,a,b,c) (T)->lpVtbl->QueryValueString(T,a,b,c)
#define IQuerySource_QueryValueDword(T,a,b,c) (T)->lpVtbl->QueryValueDword(T,a,b,c)
#define IQuerySource_QueryValueGuid(T,a,b,c) (T)->lpVtbl->QueryValueGuid(T,a,b,c)
#define IQuerySource_QueryValueExists(T,a,b) (T)->lpVtbl->QueryValueExists(T,a,b)
#define IQuerySource_QueryValueDirect(T,a,b,c) (T)->lpVtbl->QueryValueDirect(T,a,b,c)
#define IQuerySource_EnumSources(T,a) (T)->lpVtbl->EnumSources(T,a)
#define IQuerySource_OpenSource(T,a,b) (T)->lpVtbl->OpenSource(T,a,b)
#endif

/*****************************************************************************
 * IObjectWithQuerySource interface
 *
 * @see https://www.geoffchappell.com/studies/windows/shell/shell32/interfaces/iobjectwithquerysource.htm
 */
#define INTERFACE IObjectWithQuerySource
DECLARE_INTERFACE_(IObjectWithQuerySource, IUnknown) // {B3DCB623-4280-4EB1-84B3-8D07E84F299A}
{
    /*** IUnknown ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;
    /*** IObjectWithQuerySource ***/
    STDMETHOD(SetSource)(THIS_ IQuerySource *pSource);
    STDMETHOD(GetSource)(THIS_ REFIID riid, PVOID *ppSource);
};
#undef INTERFACE

#ifdef COBJMACROS
#define IObjectWithQuerySource_QueryInterface(T,a,b) (T)->lpVtbl->QueryInterface(T,a,b)
#define IObjectWithQuerySource_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IObjectWithQuerySource_Release(T) (T)->lpVtbl->Release(T)
#define IObjectWithQuerySource_SetSource(T,a) (T)->lpVtbl->SetSource(T,a)
#define IObjectWithQuerySource_GetSource(T,a,b) (T)->lpVtbl->GetSource(T,a,b)
#endif

/*****************************************************************************
 * IAssociationElementOld interface
 *
 * @see IAssociationElement
 * @see https://www.geoffchappell.com/studies/windows/shell/shlwapi/interfaces/iassociationelement.htm
 */
#define INTERFACE IAssociationElementOld
DECLARE_INTERFACE_(IAssociationElementOld, IUnknown) // {E58B1ABF-9596-4DBA-8997-89DCDEF46992}
{
    /*** IUnknown ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;
    /*** IAssociationElementOld ***/
    STDMETHOD(QueryString)(THIS_ ASSOCQUERY query, PCWSTR key, PWSTR *ppszValue) PURE;
    STDMETHOD(QueryDword)(THIS_ ASSOCQUERY query, PCWSTR key, DWORD *pdwValue) PURE;
    STDMETHOD(QueryExists)(THIS_ ASSOCQUERY query, PCWSTR key) PURE;
    STDMETHOD(QueryDirect)(THIS_ ASSOCQUERY query, PCWSTR key, FLAGGED_BYTE_BLOB **ppBlob) PURE;
    STDMETHOD(QueryObject)(THIS_ ASSOCQUERY query, PCWSTR key, REFIID riid, PVOID *ppvObj) PURE;
};
#undef INTERFACE

#ifdef COBJMACROS
#define IAssociationElementOld_QueryInterface(T,a,b) (T)->lpVtbl->QueryInterface(T,a,b)
#define IAssociationElementOld_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IAssociationElementOld_Release(T) (T)->lpVtbl->Release(T)
#define IAssociationElementOld_QueryString(T,a,b,c) (T)->lpVtbl->QueryString(T,a,b,c)
#define IAssociationElementOld_QueryDword(T,a,b,c) (T)->lpVtbl->QueryDword(T,a,b,c)
#define IAssociationElementOld_QueryExists(T,a,b) (T)->lpVtbl->QueryExists(T,a,b)
#define IAssociationElementOld_QueryDirect(T,a,b,c) (T)->lpVtbl->QueryDirect(T,a,b,c)
#define IAssociationElementOld_QueryObject(T,a,b,c,d) (T)->lpVtbl->QueryObject(T,a,b,c,d)
#endif

/*****************************************************************************
 * IAssociationElement interface (new version)
 *
 * @see IAssociationElementOld
 * @see https://www.geoffchappell.com/studies/windows/shell/shlwapi/interfaces/iassociationelement.htm
 */
#define INTERFACE IAssociationElement
DECLARE_INTERFACE_(IAssociationElement, IUnknown) // {D8F6AD5B-B44F-4BCC-88FD-EB3473DB7502}
{
    /*** IUnknown ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;
    /*** IAssociationElement ***/
    STDMETHOD(QueryString)(THIS_ ASSOCQUERY query, PCWSTR key, PWSTR *ppszValue) PURE;
    STDMETHOD(QueryDword)(THIS_ ASSOCQUERY query, PCWSTR key, DWORD *pdwValue) PURE;
    STDMETHOD(QueryGuid)(THIS_ ASSOCQUERY query, PCWSTR key, GUID *guid) PURE;
    STDMETHOD(QueryExists)(THIS_ ASSOCQUERY query, PCWSTR key) PURE;
    STDMETHOD(QueryDirect)(THIS_ ASSOCQUERY query, PCWSTR key, FLAGGED_BYTE_BLOB **ppBlob) PURE;
    STDMETHOD(QueryObject)(THIS_ ASSOCQUERY query, PCWSTR key, REFIID riid, PVOID *ppvObj) PURE;
};
#undef INTERFACE

#ifdef COBJMACROS
#define IAssociationElement_QueryInterface(T,a,b) (T)->lpVtbl->QueryInterface(T,a,b)
#define IAssociationElement_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IAssociationElement_Release(T) (T)->lpVtbl->Release(T)
#define IAssociationElement_QueryString(T,a,b,c) (T)->lpVtbl->QueryString(T,a,b,c)
#define IAssociationElement_QueryDword(T,a,b,c) (T)->lpVtbl->QueryDword(T,a,b,c)
#define IAssociationElement_QueryGuid(T,a,b,c) (T)->lpVtbl->QueryGuid(T,a,b,c)
#define IAssociationElement_QueryExists(T,a,b) (T)->lpVtbl->QueryExists(T,a,b)
#define IAssociationElement_QueryDirect(T,a,b,c) (T)->lpVtbl->QueryDirect(T,a,b,c)
#define IAssociationElement_QueryObject(T,a,b,c,d) (T)->lpVtbl->QueryObject(T,a,b,c,d)
#endif

/*****************************************************************************
 * IEnumAssociationElements interface
 *
 * @see https://www.geoffchappell.com/studies/windows/shell/shell32/interfaces/ienumassociationelements.htm
 */
#define INTERFACE IEnumAssociationElements
DECLARE_INTERFACE_(IEnumAssociationElements, IUnknown) // {A6B0FB57-7523-4439-9425-EBE99823B828}
{
    /*** IUnknown ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;
    /*** IEnumAssociationElements ***/
    STDMETHOD(Next)(THIS_ ULONG celt, IAssociationElement *pElement, ULONG *pceltFetched) PURE;
    STDMETHOD(Skip)(THIS_ ULONG celt) PURE;
    STDMETHOD(Reset)(THIS) PURE;
    STDMETHOD(Clone)(THIS_ IEnumAssociationElements **ppNew) PURE;
};
#undef INTERFACE

#ifdef COBJMACROS
#define IEnumAssociationElements_QueryInterface(T,a,b) (T)->lpVtbl->QueryInterface(T,a,b)
#define IEnumAssociationElements_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IEnumAssociationElements_Release(T) (T)->lpVtbl->Release(T)
#define IEnumAssociationElements_Next(T,a,b,c) (T)->lpVtbl->Next(T,a,b,c)
#define IEnumAssociationElements_Skip(T,a) (T)->lpVtbl->Skip(T,a)
#define IEnumAssociationElements_Reset(T) (T)->lpVtbl->Reset(T)
#define IEnumAssociationElements_Clone(T,a) (T)->lpVtbl->Clone(T,a)
#endif

/*****************************************************************************
 * IAssociationArrayOld interface
 *
 * @see IAssociationArray
 * @see https://www.geoffchappell.com/studies/windows/shell/shell32/interfaces/iassociationarray.htm
 */
#define INTERFACE IAssociationArrayOld
DECLARE_INTERFACE_(IAssociationArrayOld, IUnknown) // {3B877E3C-67DE-4F9A-B29B-17D0A1521C6A}
{
    /*** IUnknown ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;
    /*** IAssociationArrayOld ***/
    STDMETHOD(EnumElements)(THIS_ ULONG flags, IEnumAssociationElements **ppEnum) PURE;
    STDMETHOD(QueryString)(THIS_ ULONG flags, ASSOCQUERY query, PCWSTR key, PWSTR *ppszValue) PURE;
    STDMETHOD(QueryDword)(THIS_ ULONG flags, ASSOCQUERY query, PCWSTR key, DWORD *pdwValue) PURE;
    STDMETHOD(QueryExists)(THIS_ ULONG flags, ASSOCQUERY query, PCWSTR key) PURE;
    STDMETHOD(QueryDirect)(THIS_ ULONG flags, ASSOCQUERY query, PCWSTR key, FLAGGED_BYTE_BLOB **ppBlob) PURE;
    STDMETHOD(QueryObject)(THIS_ ULONG flags, ASSOCQUERY query, PCWSTR key, REFIID riid, PVOID *ppvObj) PURE;
};
#undef INTERFACE

#ifdef COBJMACROS
#define IAssociationArrayOld_QueryInterface(T,a,b) (T)->lpVtbl->QueryInterface(T,a,b)
#define IAssociationArrayOld_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IAssociationArrayOld_Release(T) (T)->lpVtbl->Release(T)
#define IAssociationArrayOld_EnumElements(T,a,b) (T)->lpVtbl->EnumElements(T,a,b)
#define IAssociationArrayOld_QueryString(T,a,b,c,d) (T)->lpVtbl->QueryString(T,a,b,c,d)
#define IAssociationArrayOld_QueryDword(T,a,b,c,d) (T)->lpVtbl->QueryDword(T,a,b,c,d)
#define IAssociationArrayOld_QueryExists(T,a,b,c) (T)->lpVtbl->QueryExists(T,a,b,c)
#define IAssociationArrayOld_QueryDirect(T,a,b,c,d) (T)->lpVtbl->QueryDirect(T,a,b,c,d)
#define IAssociationArrayOld_QueryObject(T,a,b,c,d,e) (T)->lpVtbl->QueryObject(T,a,b,c,d,e)
#endif

/*****************************************************************************
 * IAssociationArray interface
 *
 * @see IAssociationArrayOld
 * @see https://www.geoffchappell.com/studies/windows/shell/shell32/interfaces/iassociationarray.htm
 */
#define INTERFACE IAssociationArray
DECLARE_INTERFACE_(IAssociationArray, IUnknown) // {19ADBAFD-1C5F-4FC7-94EE-846702DFB58B}
{
    /*** IUnknown ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;
    /*** IAssociationArray ***/
    STDMETHOD(QueryString)(THIS_ ASSOCQUERY query, PCWSTR key, PWSTR *ppszValue) PURE;
    STDMETHOD(QueryDword)(THIS_ ASSOCQUERY query, PCWSTR key, DWORD *pdwValue) PURE;
    STDMETHOD(QueryGuid)(THIS_ ASSOCQUERY query, PCWSTR key, GUID *pguid) PURE;
    STDMETHOD(QueryExists)(THIS_ ASSOCQUERY query, PCWSTR key) PURE;
    STDMETHOD(QueryDirect)(THIS_ ASSOCQUERY query, PCWSTR key, FLAGGED_BYTE_BLOB **ppBlob) PURE;
    STDMETHOD(QueryObject)(THIS_ ASSOCQUERY query, PCWSTR key, REFIID riid, PVOID *ppvObj) PURE;
};
#undef INTERFACE

#ifdef COBJMACROS
#define IAssociationArray_QueryInterface(T,a,b) (T)->lpVtbl->QueryInterface(T,a,b)
#define IAssociationArray_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IAssociationArray_Release(T) (T)->lpVtbl->Release(T)
#define IAssociationArray_QueryString(T,a,b,c) (T)->lpVtbl->QueryString(T,a,b,c)
#define IAssociationArray_QueryDword(T,a,b,c) (T)->lpVtbl->QueryDword(T,a,b,c)
#define IAssociationArray_QueryGuid(T,a,b,c) (T)->lpVtbl->QueryGuid(T,a,b,c)
#define IAssociationArray_QueryExists(T,a,b) (T)->lpVtbl->QueryExists(T,a,b)
#define IAssociationArray_QueryDirect(T,a,b,c) (T)->lpVtbl->QueryDirect(T,a,b,c)
#define IAssociationArray_QueryObject(T,a,b,c,d) (T)->lpVtbl->QueryObject(T,a,b,c,d)
#endif

/*****************************************************************************
 * IAssociationArrayInitialize interface
 *
 * @see https://www.geoffchappell.com/studies/windows/shell/shell32/interfaces/iassociationarrayinitialize.htm
 */
#define INTERFACE IAssociationArrayInitialize
DECLARE_INTERFACE_(IAssociationArrayInitialize, IUnknown) // {EE9165BF-A4D9-474B-8236-6735CB7E28B6}
{
    /*** IUnknown ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;
    /*** IAssociationArrayInitialize ***/
    STDMETHOD(InitClassElements)(ULONG flags, PCWSTR pszClass);
    STDMETHOD(InsertElements)(ULONG flags, IEnumAssociationElements *pEnum);
    STDMETHOD(FilterElements)(ULONG filter);
};
#undef INTERFACE

#ifdef COBJMACROS
#define IAssociationArrayInitialize_QueryInterface(T,a,b) (T)->lpVtbl->QueryInterface(T,a,b)
#define IAssociationArrayInitialize_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IAssociationArrayInitialize_Release(T) (T)->lpVtbl->Release(T)
#define IAssociationArrayInitialize_InitClassElements(T,a,b) (T)->lpVtbl->InitClassElements(T,a,b)
#define IAssociationArrayInitialize_InsertElements(T,a,b) (T)->lpVtbl->InsertElements(T,a,b)
#define IAssociationArrayInitialize_FilterElements(T,a) (T)->lpVtbl->FilterElements(T,a)
#endif
