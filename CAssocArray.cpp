#include <windows.h>
#include <shlobj.h>
#include "CAssocArray.h"

inline void IUnknown_SafeReleaseAndNullPtr(IUnknown **ppUnk)
{
    if (!ppUnk || !*ppUnk)
        return;
    (*ppUnk)->Release();
    *ppUnk = NULL;
}

inline BOOL _IsSameVerb(PCWSTR psz1, PCWSTR psz2)
{
    if (!psz1)
        return !psz2;
    if (!psz2)
        return FALSE;
    return StrCmpIW(psz1, psz2) == 0;
}

static DWORD _MaskFromFlags(DWORD dwFlags)
{
    DWORD mask = 0xFFFF;
    if (dwFlags & ASSOCF_IGNOREBASECLASS)
        mask = 6;
    if (dwFlags & ASSOCF_NOUSERSETTINGS)
        mask &= ~2u;
    return mask;
}

static HRESULT
_CopyOut(BOOL bStrict, LPCWSTR lpString, PWSTR pszDest, PDWORD cchDest)
{
    INT cch = IS_INTRESOURCE(cchDest) ? PtrToUlong(cchDest) : *cchDest;
    const UINT cchString = (UINT)lstrlenW(lpString);

    HRESULT hr;
    if (pszDest)
    {
        if (bStrict && cch <= cchString)
        {
            hr = E_POINTER;
        }
        else
        {
            StrCpyNW(pszDest, lpString, cch);
            hr = S_OK;
        }
    }
    else
    {
        hr = S_FALSE;
    }

    if (!IS_INTRESOURCE(cchDest))
        *cchDest = SUCCEEDED(hr) ? (lstrlenW(pszDest) + 1) : (cchString + 1);

    return hr;
}

/*****************************************************************************
 * _Query* helper thunks
 *
 * IAssociationArrayOld::Query* takes (flags, query, key, ...) but the
 * corresponding IAssociationElementOld methods have no flags parameter.
 * These thunks adapt the signature so a single function-pointer type can be
 * passed to _QueryElementAny, discarding the unused extra argument.
 */
static HRESULT CALLBACK _QueryString(
    IAssociationElementOld *pElem,
    ASSOCQUERY query,
    PCWSTR key,
    PVOID pOut,
    PVOID* ppvObj)
{
    UNREFERENCED_PARAMETER(ppvObj);
    return pElem->QueryString(query, key, (PWSTR *)pOut);
}

static HRESULT CALLBACK _QueryDword(
    IAssociationElementOld *pElem,
    ASSOCQUERY query,
    PCWSTR key,
    PVOID pOut,
    PVOID* ppvObj)
{
    UNREFERENCED_PARAMETER(ppvObj);
    return pElem->QueryDword(query, key, (DWORD*)pOut);
}

static HRESULT CALLBACK _QueryExists(
    IAssociationElementOld *pElem,
    ASSOCQUERY query,
    PCWSTR key,
    PVOID pOut,
    PVOID* ppvObj)
{
    UNREFERENCED_PARAMETER(pOut);
    UNREFERENCED_PARAMETER(ppvObj);
    return pElem->QueryExists(query, key);
}

static HRESULT CALLBACK _QueryDirect(
    IAssociationElementOld *pElem,
    ASSOCQUERY query,
    PCWSTR key,
    PVOID pOut,
    PVOID* ppvObj)
{
    UNREFERENCED_PARAMETER(ppvObj);
    return pElem->QueryDirect(query, key, (FLAGGED_BYTE_BLOB **)pOut);
}

static HRESULT CALLBACK _QueryObject(
    IAssociationElementOld *pElem,
    ASSOCQUERY query,
    PCWSTR key,
    PVOID pOut,
    PVOID *ppvObj)
{
    return pElem->QueryObject(query, key, *(const IID*)pOut, ppvObj);
}

/*****************************************************************************
 * CEnumAssocElems - implementation of IEnumAssociationElements
 */

// @implemented
HRESULT CEnumAssocElems::Next(
    ULONG celt,
    IAssociationElementOld *pElement,
    ULONG *pceltFetched)
{
    ULONG cGot = 0;
    ZeroMemory(pElement, 4 * celt);
    if (celt)
    {
        do
        {
            if (!_Next(pElement))
                break;
            ++m_iItem;
            ++cGot;
            ++pElement;
        } while (cGot < celt);
    }

    if (pceltFetched)
        *pceltFetched = cGot;

    return (celt == cGot) ? S_OK : S_FALSE;
}

// @implemented
STDMETHODIMP CEnumAssocElems::Skip(ULONG celt)
{
    return E_NOTIMPL;
}

// @implemented
STDMETHODIMP CEnumAssocElems::Reset()
{
    return E_NOTIMPL;
}

// @implemented
STDMETHODIMP CEnumAssocElems::Clone(IEnumAssociationElements **ppNew)
{
    return E_NOTIMPL;
}

/*****************************************************************************
 * CAssocArray
 */
CAssocArray::CAssocArray()
    : m_cRefs(1)
    , m_hr(E_UNEXPECTED)
    , m_pszClass(NULL)
    , m_dwFilter(0)
    , m_bOpenByExeName(0)
    , m_pElement1(NULL)
    , m_dwFlags1(0)
    , m_pElement2(NULL)
    , m_dwFlags2(0)
    , m_pElement3(NULL)
    , m_pszVerb(NULL)
    , m_dwFlags3(0)
    , m_pParent(NULL)
{
}

// @implemented
CAssocArray::~CAssocArray()
{
    _Reset();
}

/**
 * _FirstElement - return the first element that matches the given flags
 *
 * Iterates _GetElement starting at i=0 and returns as soon as a valid element
 * is found.
 * Returns TRUE if an element was found, FALSE otherwise.
 */
BOOL CAssocArray::_FirstElement(DWORD dwFlags, IAssociationElementOld **ppElem)
{
    INT i = 0;
    GETELEMENT result;
    do
    {
        result = _GetElement(i++, dwFlags, ppElem);
    } while (result == GETELEMENT_FLAGSMISMATCH);
    return result == GETELEMENT_SUCCESSS;
}

/**
 * _GetCachedVerbElement - look up the verb element cache
 *
 * If dwFlags and pszVerb match the current cache, returns the cached element
 * pointers with an extra AddRef on each.
 */
HRESULT CAssocArray::_GetCachedVerbElement(
    DWORD    dwFlags,
    PCWSTR   pszVerb,
    IAssociationElementOld **ppElem,
    IAssociationElementOld **ppParent)
{
    if (m_pElement3 &&
        dwFlags == m_dwFlags3 &&
        _IsSameVerb(pszVerb, m_pszVerb))
    {
        *ppElem = m_pElement3;
        m_pElement3->AddRef();
        if (ppParent)
        {
            *ppParent = m_pParent;
            m_pParent->AddRef();
        }
        return S_OK;
    }
    else
    {
        *ppElem = NULL;
        if (ppParent)
            *ppParent = NULL;
        return E_FAIL;
    }
}

/**
 * _GetElement - retrieve the i-th DSA entry, creating it on demand if needed
 *
 * Return values:
 *   1  = success (*ppElem is valid and has been AddRef'd)
 *   0  = flags mismatch (caller should skip this entry)
 *  -1  = iItem is out of range (end of array)
 *  -2  = on-demand creation failed (entry has been removed from the DSA)
 */
GETELEMENT CAssocArray::_GetElement(INT iItem, DWORD dwFlags, IAssociationElementOld **ppElem)
{
    AEINFO *pInfo = m_DSA.GetItemPtr(iItem);
    if (!pInfo)
        return GETELEMENT_OUTOFRANGE;

    if (!(dwFlags & pInfo->dwFlags))
        return GETELEMENT_FLAGSMISMATCH;

    // Create the element on first access if it has not been instantiated yet
    if (!pInfo->pElement)
    {
        PCWSTR pszClass = pInfo->pClsid ? m_pszClass : NULL;
        if (!pszClass)
            pszClass = m_pszClass;

        HRESULT hr = pInfo->pfnCreate(pInfo->pClsid, pszClass, &pInfo->pElement);
        if (FAILED(hr))
        {
            m_DSA.DeleteItem(iItem);
            return GETELEMENT_CREATIONFAILED;
        }
    }

    if (pInfo->pElement)
    {
        *ppElem = pInfo->pElement;
        pInfo->pElement->AddRef();
        return GETELEMENT_SUCCESSS;
    }

    return GETELEMENT_OUTOFRANGE;
}

/**
 * _GetVerbElement - find the IAssociationElement associated with a verb
 *
 * Checks the cache first.  On a miss, walks all elements and queries each one
 * with ASSOCQUERY_OBJECT (0x2200000|...).  A successful result is stored in
 * the cache for subsequent calls.
 */
HRESULT CAssocArray::_GetVerbElement(
    DWORD                    dwFlags,
    PCWSTR                   pszVerb,
    IAssociationElementOld **ppElem,
    IAssociationElementOld **ppParent)
{
    HRESULT hr = _GetCachedVerbElement(dwFlags, pszVerb, ppElem, ppParent));
    if (SUCCEEDED(hr))
        return hr;

    IAssociationElementOld *pCurrent = NULL;
    for (INT i = 0;
         _GetElement(i, dwFlags, &pCurrent) >= GETELEMENT_FLAGSMISMATCH;
         ++i)
    {
        if (!pCurrent)
            continue;

        IAssociationElementOld *pVerbElem = NULL;
        hr = pCurrent->QueryObject(0x2200001, pszVerb, IID_IAssociationElement, (PVOID*)&pVerbElem);

        if (SUCCEEDED(hr))
        {
            _SetCachedVerbElement(dwFlags, pszVerb, pVerbElem, pCurrent);

            *ppElem = pVerbElem;
            if (ppParent)
            {
                pCurrent->AddRef();
                *ppParent = pCurrent;
            }
        }

        pCurrent->Release();

        if (SUCCEEDED(hr))
            break;
    }

    return hr;
}

/**
 * _InitDelayedElements - flush the deferred-insertion queues into the DSA
 *
 * Processes both the front-insertion queue (m_pElement1) and the
 * back-insertion queue (m_pElement2).
 *   iInsertPos : starting index for front insertion
 *   dwFlags    : filter mask
 */
void CAssocArray::_InitDelayedElements(INT iInsertPos, DWORD dwFlags)
{
    // Front-insertion queue (m_pElement1)
    if (!iInsertPos && m_pElement1)
    {
        if ((dwFlags & m_dwFlags1) == m_dwFlags1)
        {
            IAssociationElementOld *pElem = NULL;
            INT i = 0;
            while (SUCCEEDED(m_pElement1->Next(1, (void**)&pElem, NULL)))
            {
                AEINFO info = {};
                info.dwFlags  = 1;
                info.pElement = pElem;

                if (DSA_InsertItem(m_DSA.m_hDSA, i++, &info) != -1)
                    pElem->AddRef();

                pElem->Release();
            }
            IUnknown_SafeReleaseAndNullPtr(&m_pElement1);
        }
    }

    // Back-insertion queue (m_pElement2)
    if (m_pElement2 &&
        (dwFlags & 0x10000) &&
        iInsertPos == m_DSA.GetItemCount())
    {
        IAssociationElementOld *pElem = NULL;
        while (SUCCEEDED(m_pElement2->Next(1, (void**)&pElem, NULL)))
        {
            AEINFO info = {};
            info.dwFlags  = 0x10000;
            info.pElement = pElem;

            if (m_DSA.AppendItem(&info) != -1)
                pElem->AddRef();

            pElem->Release();
        }
        IUnknown_SafeReleaseAndNullPtr(&m_pElement2);
    }
}

/**
 * _InsertSingleElement - add a single IUnknown as a standalone entry in the DSA
 */
HRESULT CAssocArray::_InsertSingleElement(IUnknown *pUnk)
{
    if (!m_DSA.m_hDSA && !m_DSA.Create(1))
        return E_OUTOFMEMORY;

    AEINFO info = {};
    info.dwFlags  = 4;
    info.pElement = static_cast<IAssociationElementOld*>(pUnk);

    if (m_DSA.AppendItem(&info) == -1)
        return E_OUTOFMEMORY;

    pUnk->AddRef();
    return S_OK;
}

/**
 * _SetCachedVerbElement - update the verb element cache
 *
 * Releases any existing cached values before storing the new ones.
 */
void CAssocArray::_SetCachedVerbElement(
    DWORD    dwFlags,
    PCWSTR   pszVerb,
    IAssociationElementOld *pElem,
    IAssociationElementOld *pParent)
{
    if (m_pElement3)
    {
        IUnknown_SafeReleaseAndNullPtr(&m_pElement3);
        IUnknown_SafeReleaseAndNullPtr(&m_pParent);
        if (m_pszVerb)
        {
            LocalFree(m_pszVerb);
            m_pszVerb = NULL;
        }
    }

    m_pszVerb = pszVerb ? StrDupW(pszVerb) : NULL;

    if (m_pszVerb || !pszVerb)
    {
        m_pElement3 = pElem;
        pElem->AddRef();
        m_pParent = pParent;
        pParent->AddRef();
        m_dwFlags3 = dwFlags;
    }
}

/**
 * _Reset - tear down all state and return the object to its pre-init condition
 */
void CAssocArray::_Reset()
{
    if (m_hr == E_UNEXPECTED)   // already uninitialized; nothing to do
        return;

    if (m_DSA.m_hDSA)
        m_DSA.DestroyCallback(_AeinfoDelete, NULL);

    if (m_pszClass)
    {
        CoTaskMemFree(m_pszClass);
        m_pszClass = NULL;
    }

    IUnknown_SafeReleaseAndNullPtr(&m_pElement1);
    IUnknown_SafeReleaseAndNullPtr(&m_pElement2);
    IUnknown_SafeReleaseAndNullPtr(&m_pElement3);

    if (m_pszVerb)
    {
        LocalFree(m_pszVerb);
        m_pszVerb = NULL;
    }

    IUnknown_SafeReleaseAndNullPtr(&m_pParent);
    m_bOpenByExeName = 0;
    m_hr = E_UNEXPECTED;
}

// @implemented
STDMETHODIMP CAssocArray::EnumElements(ULONG flags, IEnumAssociationElements **ppElement)
{
    *ppElement = new CEnumAssocElems(this, m_dwFilter & flags);
    return *ppElement ? S_OK : E_OUTOFMEMORY;
}

// @implemented
STDMETHODIMP CAssocArray::QueryString(ULONG flags, ASSOCQUERY query, PCWSTR key, PWSTR *ppszValue)
{
    return _QueryElementAny(_QueryString, flags, query, key, ppszValue, NULL);
}

// @implemented
STDMETHODIMP CAssocArray::QueryDword(ULONG flags, ASSOCQUERY query, PCWSTR key, DWORD *pdwValue)
{
    return _QueryElementAny(_QueryDword, flags, query, key, pdwValue, NULL);
}

// @implemented
STDMETHODIMP CAssocArray::QueryExists(ULONG flags, ASSOCQUERY query, PCWSTR key)
{
    return _QueryElementAny(_QueryExists, flags, query, key, NULL, NULL);
}

// @implemented
STDMETHODIMP CAssocArray::QueryDirect(ULONG flags, ASSOCQUERY query, PCWSTR key, FLAGGED_BYTE_BLOB **ppBlob)
{
    return _QueryElementAny(_QueryDirect, flags, query, key, ppBlob, 0);
}

// @implemented
STDMETHODIMP CAssocArray::QueryObject(
        ULONG flags, ASSOCQUERY query, PCWSTR key, REFIID riid, PVOID *ppvObj)
{
    return _QueryElementAny(_QueryObject, flags, query, key, riid, ppvObj);
}

/**
 * InitClassElements - initialize the standard element set for a class name
 *
 * Selects the CLSID table based on the first character of pszClass:
 *   '.'      -> file extension  -> 4-entry User/ProgID/System/Perceived table
 *   '{'      -> CLSID string    -> ClsidElement table
 *   (other)  -> ProgID string   -> ProgidElement table
 *
 * If bit 0x100 of flags is set, a FolderElement entry is appended.
 * If bit 0x200 of flags is set, a StarElement entry is appended.
 */
STDMETHODIMP CAssocArray::InitClassElements(ULONG flags, PCWSTR pszClass)
{
    _Reset();

    HRESULT hr = SHStrDupW(pszClass, &m_pszClass);
    if (FAILED(hr))
    {
        m_hr = hr;
        return hr;
    }

    hr = m_DSA.Create(6) ? S_OK : E_OUTOFMEMORY;
    if (FAILED(hr))
    {
        m_hr = hr;
        return hr;
    }

    // Choose the entry table based on the leading character (reconstructed
    // from the static data in the assembly listing)
    const AEINFO *pTable;
    INT nEntries;

    if (*m_pszClass == L'.')
    {
        // File extension: User / ProgID / System / Perceived (4 entries)
        pTable   = s_aeinfoExtension;   // corresponds to dword_7C916CF8
        nEntries = 4;
    }
    else if (*m_pszClass == L'{')
    {
        // CLSID
        pTable   = s_aeinfoCLSID;       // corresponds to dword_7C912488
        nEntries = 1;
    }
    else
    {
        // ProgID
        pTable   = s_aeinfoProgid;      // corresponds to dword_7C912470
        nEntries = 1;
    }

    for (INT i = 0; i < nEntries; ++i)
        m_DSA.AppendItem(&pTable[i]);

    // Optional elements
    if (flags & 0x100)  // equivalent to ASSOCF_INIT_DEFAULTTOFOLDER
        m_DSA.AppendItem(&s_aeinfoFolder);  // corresponds to dword_7C913DAC

    if (flags & 0x200)  // equivalent to ASSOCF_INIT_DEFAULTTOSTAR
        m_DSA.AppendItem(&s_aeinfoStar);    // corresponds to dword_7C916E10

    // Verify that at least the first element can be instantiated
    IAssociationElementOld *pElem = NULL;
    hr = _FirstElement(6, &pElem) ? S_OK : S_FALSE;
    if (pElem)
        pElem->Release();

    m_hr = hr;
    return hr;
}

/**
 * InsertElements - register an enumerator in the deferred-insertion queue
 *
 * flags & 0x00001: register in the front-insertion queue (m_pElement1)
 * flags & 0x10000: register in the back-insertion queue  (m_pElement2)
 */
STDMETHODIMP CAssocArray::InsertElements(ULONG flags, IEnumAssociationElements *pElements)
{
    HRESULT hr = E_UNEXPECTED;

    if (!m_pElement1 && (flags & 0x00001))
    {
        m_pElement1 = pElements;
        pElements->AddRef();
        m_dwFlags1 = flags;
        hr = S_OK;
    }

    if (!m_pElement2 && (flags & 0x10000))
    {
        m_pElement2 = pElements;
        pElements->AddRef();
        m_dwFlags2 = flags;
        return S_OK;
    }

    return hr;
}

// @implemented
STDMETHODIMP CAssocArray::FilterElements(ULONG dwFilter)
{
    m_dwFilter = dwFilter;
    return S_OK;
}

/**
 * Init - IQueryAssociations initialization
 *
 * Creates a single AssocElement from pszAssoc or hkProgid and inserts it into
 * the DSA to complete initialization.
 *
 * flags & ASSOCF_OPEN_BYEXENAME    : treat pszAssoc as an executable name
 * flags & ASSOCF_INIT_DEFAULTTOFOLDER : include folder element
 * flags & ASSOCF_INIT_DEFAULTTOSTAR   : include wildcard element
 */
STDMETHODIMP CAssocArray::Init(
    _In_     ASSOCF  flags,
    _In_opt_ LPCWSTR pszAssoc,
    _In_opt_ HKEY    hkProgid,
    _In_opt_ HWND    /*hwnd*/)
{
    if (!pszAssoc && !hkProgid)
    {
        m_hr = E_INVALIDARG;
        return E_INVALIDARG;
    }

    _Reset();

    m_bOpenByExeName = (flags & ASSOCF_OPEN_BYEXENAME);

    HRESULT hr;
    IAssociationElementOld *pElem = NULL;

    if (!hkProgid)
    {
        if (!(flags & ASSOCF_OPEN_BYEXENAME))
        {
            // Extension-based initialization: delegate to InitClassElements
            DWORD dwInitFlags = 0;
            if (flags & ASSOCF_INIT_DEFAULTTOFOLDER) dwInitFlags |= 0x100;
            if (flags & ASSOCF_INIT_DEFAULTTOSTAR)   dwInitFlags |= 0x200;

            LPCWSTR pszExt = pszAssoc;
            if (StrChrW(pszExt, L'\\'))
                pszExt = PathFindExtensionW(pszExt);

            if (*pszExt)
                hr = InitClassElements(dwInitFlags, pszExt);
            else
                hr = E_INVALIDARG;

            m_hr = hr;
            return hr;
        }

        // Executable name without a registry key
        hr = AssocElemCreateForClass(
            &CLSID_AssocApplicationElement,
            pszAssoc,
            &pElem);
    }
    else
    {
        const GUID *pClsid;
        if (flags & ASSOCF_OPEN_BYEXENAME)
            pClsid = &CLSID_AssocApplicationElement;
        else if (flags & 1)
            pClsid = &CLSID_AssocClsidElement;
        else
            pClsid = &CLSID_AssocProgidElement;

        hr = AssocElemCreateForKey(pClsid, hkProgid, &pElem);
    }

    if (SUCCEEDED(hr))
    {
        hr = _InsertSingleElement(pElem);
        pElem->Release();
    }

    m_hr = hr;
    return hr;
}

/**
 * GetString - IQueryAssociations::GetString implementation
 *
 * Looks up the ASSOCQUERY and optional default key name from the static table
 * (s_assocstrTable, corresponding to dword_7C915F84), then calls QueryString.
 */
STDMETHODIMP CAssocArray::GetString(
    _In_                       ASSOCF  flags,
    _In_                       ASSOCSTR str,
    _In_opt_                   LPCWSTR pszExtra,
    _Out_writes_opt_(*pcchOut) LPWSTR  pszOut,
    _Inout_                    DWORD  *pcchOut)
{
    if (!str || str >= ASSOCSTR_MAX || !pcchOut || (!pszOut && !*pcchOut))
        return E_UNEXPECTED;

    const ASSOCSTR_ENTRY &entry = s_assocstrTable[str];
    if (!entry.query)
        return E_UNEXPECTED;

    LPCWSTR pszKey = pszExtra ? pszExtra : entry.pszDefaultKey;

    DWORD dwMask = _MaskFromFlags(flags);
    LPWSTR pszResult = NULL;

    HRESULT hr = QueryString(m_dwFilter & dwMask, entry.query, pszKey, &pszResult);
    if (SUCCEEDED(hr))
    {
        hr = _CopyOut(flags & 0x20, pszResult, pszOut, pcchOut);
        CoTaskMemFree(pszResult);
    }

    return hr;
}

/**
 * GetKey - IQueryAssociations::GetKey implementation
 *
 * Selects the target registry key based on the ASSOCKEY value:
 *   ASSOCKEY_APP (1)       : registry key of the verb element's parent
 *   ASSOCKEY_CLASS (2)     : registry key of the first filter-matching element
 *   ASSOCKEY_BASECLASS (3) : QueryObject-based lookup (exact semantics TBD)
 *   ASSOCKEY_SHELLEXEC (4) : registry key for the folder/star filter
 */
STDMETHODIMP CAssocArray::GetKey(
    _In_     ASSOCF   flags,
    _In_     ASSOCKEY key,
    _In_opt_ LPCWSTR  pszExtra,
    _Out_    HKEY    *phkeyOut)
{
    *phkeyOut = NULL;

    if (!key || key >= 5)
        return E_INVALIDARG;

    HRESULT hr = E_INVALIDARG;
    IAssociationElementOld *pElem = NULL;

    switch (key)
    {
        case ASSOCKEY_APP:  // 1: parent key of the verb element
        {
            DWORD dwMask = _MaskFromFlags(flags);
            IAssociationElementOld *pParent = NULL;
            hr = _GetVerbElement(m_dwFilter & dwMask, pszExtra, &pElem, &pParent);
            if (SUCCEEDED(hr))
                pParent->Release();
            break;
        }
        case ASSOCKEY_CLASS:  // 2: key for the first element matching the filter
        {
            DWORD dwMask = _MaskFromFlags(flags);
            hr = _FirstElement(m_dwFilter & dwMask, &pElem) ? S_OK : E_FAIL;
            break;
        }
        case ASSOCKEY_BASECLASS:  // 3: via QueryObject
        {
            hr = QueryObject(0xFFFF, static_cast<ASSOCQUERY>(0x2200001),
                             pszExtra, IID_IAssociationElement, (PVOID*)&pElem);
            break;
        }
        case ASSOCKEY_SHELLEXEC:  // 4: folder / star filter key
        {
            hr = _FirstElement(m_dwFilter & 0x300, &pElem) ? S_OK : E_FAIL;
            break;
        }
    }

    if (SUCCEEDED(hr) && pElem)
    {
        hr = AssocKeyFromElement(reinterpret_cast<INT>(pElem),
                                 reinterpret_cast<INT>(phkeyOut));
        pElem->Release();
    }

    return hr;
}

/**
 * GetData - IQueryAssociations::GetData implementation
 *
 * Looks up the ASSOCQUERY from the static table and retrieves the data via
 * QueryDirect (when pcbOut is provided) or QueryExists (existence check only).
 */
STDMETHODIMP CAssocArray::GetData(
    _In_                            ASSOCF    flags,
    _In_                            ASSOCDATA data,
    _In_opt_                        LPCWSTR   pszExtra,
    _Out_writes_bytes_opt_(*pcbOut) LPVOID    pvOut,
    _Inout_opt_                     DWORD    *pcbOut)
{
    if (!data || data >= ASSOCDATA_MAX)
        return E_INVALIDARG;

    const ASSOCDATA_ENTRY &entry = s_assocdataTable[data]; // corresponds to locret_7C91762C

    LPCWSTR pszKey = pszExtra ? pszExtra : entry.pszDefaultKey;

    if (entry.query)
    {
        DWORD dwMask = _MaskFromFlags(flags);

        if (pcbOut)
        {
            FLAGGED_BYTE_BLOB *pBlob = NULL;
            HRESULT hr = QueryDirect(m_dwFilter & dwMask, entry.query, pszKey, &pBlob);
            if (SUCCEEDED(hr))
            {
                hr = _CopyDataOut(flags & 0x20, pBlob,
                                  static_cast<BYTE*>(pvOut), pcbOut);
                CoTaskMemFree(pBlob);
            }
            return hr;
        }
        else
        {
            // Existence check only
            return QueryExists(m_dwFilter & dwMask, entry.query, pszKey);
        }
    }
    else if (data == ASSOCDATA_HASPERUSERASSOC)
    {
        IAssociationElementOld *pElem = NULL;
        if (_FirstElement(2, &pElem))
        {
            pElem->Release();
            return S_OK;    // per-user association exists
        }
        return S_FALSE;     // no per-user association
    }

    return E_INVALIDARG;
}

// @implemented
STDMETHODIMP CAssocArray::GetEnum(
    _In_     ASSOCF    /*flags*/,
    _In_     ASSOCENUM /*assocenum*/,
    _In_opt_ LPCWSTR   /*pszExtra*/,
    _In_     REFIID    /*riid*/,
    _Outptr_ LPVOID   */*ppvOut*/)
{
    return E_NOTIMPL;
}
