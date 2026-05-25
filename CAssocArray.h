#pragma once

#include "CDSA.h"

class CAssocArray;

HRESULT (CALLBACK* PFNASSOCELEM_CREATE)(const GUID*, PCWSTR, IAssociationElementOld**);

typedef HRESULT (CALLBACK *FNQUERYELEMENT)(IAssociationElementOld*, ASSOCQUERY, PCWSTR, PVOID, PVOID*),

/*****************************************************************************
 * AEINFO - element entry stored in the DSA
 *
 * Layout reconstructed from the static data in the assembly listing:
 *   offset  0: DWORD  dwFlags   (e.g. 4, 0x10, 0x20, 0x100, 0x200, 0x10000)
 *   offset  4: GUID*  pClsid    (e.g. &CLSID_AssocProgidElement)
 *   offset  8: DWORD  reserved  (always 0)
 *   offset 12: PFUNC  pfnCreate (e.g. AssocElemCreateForClass)
 *   offset 16: IAssociationElementOld* pElement  (filled in on demand)
 */
struct AEINFO
{
    DWORD                   dwFlags;
    const GUID             *pClsid;
    DWORD                   dwReserved;
    PFNASSOCELEM_CREATE     pfnCreate;
    IAssociationElementOld *pElement;
};

/*****************************************************************************
 * CEnumAssocElems - implementation of IEnumAssociationElements
 */
class CEnumAssocElems : public IEnumAssociationElements
{
protected:
    LONG         m_cRefs;
    INT          m_iItem;
    DWORD        m_dwFlags;         // filter mask captured at EnumElements time
    CAssocArray *m_pAssocArray;

public:
    CEnumAssocElems(CAssocArray *pAssocArray, DWORD dwFlags);
    ~CEnumAssocElems();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, PVOID *ppv) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

    // IEnumAssociationElements
    STDMETHODIMP Next(ULONG celt, IAssociationElementOld *pElement, ULONG *pceltFetched) override;
    STDMETHODIMP Skip(ULONG celt) override;
    STDMETHODIMP Reset() override;
    STDMETHODIMP Clone(IEnumAssociationElements **ppNew) override;

private:
    HRESULT _Next(IAssociationElementOld **ppElem);
};

enum GETELEMENT
{
    GETELEMENT_SUCCESSS = 1,
    GETELEMENT_FLAGSMISMATCH = 0,
    GETELEMENT_OUTOFRANGE = -1,
    GETELEMENT_CREATIONFAILED = -2,
};

/*****************************************************************************
 * CAssocArray
 *
 * @see IAssociationArray
 *   https://www.geoffchappell.com/studies/windows/shell/shell32/interfaces/iassociationarray.htm
 * @see IQueryAssociations
 *   https://learn.microsoft.com/en-us/windows/win32/api/shlwapi/nn-shlwapi-iqueryassociations
 */
class CAssocArray
    : public IAssociationArrayOld
    , public IAssociationArrayInitialize
    , public IQueryAssociations
{
protected:
    LONG                        m_cRefs;
    HRESULT                     m_hr;
    LPWSTR                      m_pszClass;
    DWORD                       m_dwFilter;
    BOOL                        m_bOpenByExeName;       // stores ASSOCF_OPEN_BYEXENAME flag
    CDSA<AEINFO>                m_DSA;
    IEnumAssociationElements   *m_pElement1;
    DWORD                       m_dwFlags1;
    IEnumAssociationElements   *m_pElement2;
    DWORD                       m_dwFlags2;
    IAssociationElementOld     *m_pElement3;
    LPWSTR                      m_pszVerb;           // cached verb name
    DWORD                       m_dwFlags3;          // cached flags mask
    IAssociationElementOld     *m_pParent;           // cached verb parent element

    // Internal helpers
    void    _Reset();
    BOOL    _FirstElement(DWORD dwFlags, IAssociationElementOld **ppElem);
    GETELEMENT _GetElement(INT iItem, DWORD dwFlags, IAssociationElementOld **ppElem);
    HRESULT _GetCachedVerbElement(DWORD dwFlags, PCWSTR pszVerb,
                                  IAssociationElementOld **ppElem,
                                  IAssociationElementOld **ppParent);
    void    _SetCachedVerbElement(DWORD dwFlags, PCWSTR pszVerb,
                                  IAssociationElementOld *pElem,
                                  IAssociationElementOld *pParent);
    HRESULT _GetVerbElement(DWORD dwFlags, PCWSTR pszVerb,
                            IAssociationElementOld **ppElem,
                            IAssociationElementOld **ppParent);
    void    _InitDelayedElements(INT iInsertPos, DWORD dwFlags);
    HRESULT _InsertSingleElement(IUnknown *pUnk);

    template <typename T_OUT>
    HRESULT _QueryElementAny(
        FNQUERYELEMENT pfnQuery,
        DWORD   dwFlags,
        ASSOCQUERY query,
        PCWSTR  pszKey,
        T_OUT   pOut,
        PVOID*  ppvObj);

public:
    CAssocArray();
    ~CAssocArray();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, PVOID *ppv) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

    // IAssociationArrayOld
    STDMETHODIMP EnumElements(ULONG flags, IEnumAssociationElements **ppElement) override;
    STDMETHODIMP QueryString(ULONG flags, ASSOCQUERY query, PCWSTR key, PWSTR *ppszValue) override;
    STDMETHODIMP QueryDword(ULONG flags, ASSOCQUERY query, PCWSTR key, DWORD *pdwValue) override;
    STDMETHODIMP QueryExists(ULONG flags, ASSOCQUERY query, PCWSTR key) override;
    STDMETHODIMP QueryDirect(ULONG flags, ASSOCQUERY query, PCWSTR key, FLAGGED_BYTE_BLOB **ppBlob) override;
    STDMETHODIMP QueryObject(ULONG flags, ASSOCQUERY query, PCWSTR key, REFIID riid, PVOID *ppvObj) override;

    // IAssociationArrayInitialize
    STDMETHODIMP InitClassElements(ULONG flags, PCWSTR pszClass) override;
    STDMETHODIMP InsertElements(ULONG flags, IEnumAssociationElements *pElements) override;
    STDMETHODIMP FilterElements(ULONG dwFilter) override;

    // IQueryAssociations
    STDMETHODIMP Init(
        _In_     ASSOCF    flags,
        _In_opt_ LPCWSTR   pszAssoc,
        _In_opt_ HKEY      hkProgid,
        _In_opt_ HWND      hwnd) override;
    STDMETHODIMP GetString(
        _In_                          ASSOCF   flags,
        _In_                          ASSOCSTR str,
        _In_opt_                      LPCWSTR  pszExtra,
        _Out_writes_opt_(*pcchOut)    LPWSTR   pszOut,
        _Inout_                       DWORD   *pcchOut) override;
    STDMETHODIMP GetKey(
        _In_     ASSOCF   flags,
        _In_     ASSOCKEY key,
        _In_opt_ LPCWSTR  pszExtra,
        _Out_    HKEY    *phkeyOut) override;
    STDMETHODIMP GetData(
        _In_                            ASSOCF    flags,
        _In_                            ASSOCDATA data,
        _In_opt_                        LPCWSTR   pszExtra,
        _Out_writes_bytes_opt_(*pcbOut) LPVOID    pvOut,
        _Inout_opt_                     DWORD    *pcbOut) override;
    STDMETHODIMP GetEnum(
        _In_     ASSOCF    flags,
        _In_     ASSOCENUM assocenum,
        _In_opt_ LPCWSTR   pszExtra,
        _In_     REFIID    riid,
        _Outptr_ LPVOID   *ppvOut) override;
};

/**
 * _QueryElementAny - dispatch a query function across all matching elements
 *
 * Iterates elements filtered by dwFlags and calls pfnQuery on each until one
 * succeeds.  When the upper bit of ASSOCQUERY (0x2000000) is set the query
 * targets a specific verb element, which is resolved via _GetVerbElement
 * instead of a full scan.
 */
template <typename T_OUT>
HRESULT CAssocArray::_QueryElementAny(
    FNQUERYELEMENT pfnQuery,
    DWORD    dwFlags,
    ASSOCQUERY query,
    PCWSTR   pszKey,
    T_OUT    pOut,
    PVOID*   ppvObj)
{
    DWORD dwEffective = m_dwFilter & dwFlags;

    HRESULT hr = E_FAIL;

    // Determine whether to use a full scan or the verb-element shortcut.
    // The 0x2000000 bit in the query value signals "verb-targeted" queries,
    // unless m_dwUnknown30 (ASSOCF_OPEN_BYEXENAME) is set and the query is
    // one of the known override values.
    if (!(query & 0x2000000) ||
        (m_dwUnknown30 && (query == 0x2010007 || query == 0x2170008 || query == 0x2200001)))
    {
        // Normal linear scan
        IAssociationElementOld *pElem = NULL;
        for (INT i = 0; _GetElement(i, dwEffective, &pElem) >= 0; ++i)
        {
            if (!pElem)
                continue;

            hr = pfnQuery(pElem, query, pszKey, pOut, ppvObj);
            pElem->Release();

            if (SUCCEEDED(hr))
                break;
        }
    }
    else
    {
        // Direct query against the verb element
        IAssociationElementOld *pVerbElem   = NULL;
        IAssociationElementOld *pParentElem = NULL;

        hr = _GetVerbElement(dwEffective, pszKey, &pVerbElem, &pParentElem);
        if (SUCCEEDED(hr))
        {
            if (query == 0x2170008)
                hr = pfnQuery(pParentElem, 0x2170008, pszKey, pOut, ppvObj);
            else
                hr = pfnQuery(pVerbElem, query, NULL, pOut, ppvObj);

            pVerbElem->Release();
            if (pParentElem)
                pParentElem->Release();
        }
    }

    return hr;
}
