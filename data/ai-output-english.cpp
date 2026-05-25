/**
 * CDSA<T_ITEM> - Dynamic Structure Array wrapper
 */
template <typename T_ITEM>
class CDSA
{
protected:
    HDSA m_hDSA;

public:
    CDSA() : m_hDSA(NULL) {}

    virtual ~CDSA()
    {
        DeleteAllItems();
    }

    INT GetItemCount()
    {
        if (!m_hDSA)
            return 0;
        return DSA_GetItemCount(m_hDSA);
    }

    virtual BOOL DeleteItem(INT iItem)
    {
        return DSA_DeleteItem(m_hDSA, iItem);
    }

    virtual BOOL DeleteAllItems()
    {
        return DSA_DeleteAllItems(m_hDSA);
    }

    BOOL Create(INT cItemGrow)
    {
        m_hDSA = DSA_Create(sizeof(T_ITEM), cItemGrow);
        return !!m_hDSA;
    }

    T_ITEM* GetItemPtr(INT iItem)
    {
        return (T_ITEM*)DSA_GetItemPtr(m_hDSA, iItem);
    }

    INT AppendItem(T_ITEM* pItem)
    {
        return DSA_InsertItem(m_hDSA, DSA_APPEND, pItem);
    }

    void DestroyCallback(PFNDAENUMCALLBACK pfnCB, PVOID pData)
    {
        DSA_DestroyCallback(m_hDSA, pfnCB, pData);
        m_hDSA = NULL;
    }
};

template <typename T_ITEM>
inline CDSA<T_ITEM>* CDSA_Create(INT cItemGrow)
{
    CDSA<T_ITEM> *pDSA = new CDSA<T_ITEM>();
    if (pDSA && !pDSA->Create(cItemGrow))
    {
        delete pDSA;
        pDSA = NULL;
    }
    return pDSA;
}

/*****************************************************************************
 * IEnumAssociationElements interface
 *
 * @see https://www.geoffchappell.com/studies/windows/shell/shell32/interfaces/ienumassociationelements.htm
 */
#define INTERFACE IEnumAssociationElements
DECLARE_INTERFACE_(IEnumAssociationElements, IUnknown) // {A6B0FB57-7523-4439-9425-EBE99823B828}
{
    /*** IUnknown ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID, PVOID*) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /*** IEnumAssociationElements ***/
    STDMETHOD(Next)(THIS_ ULONG celt, IAssociationElement *pElement, ULONG *pceltFetched) PURE;
    STDMETHOD(Skip)(THIS_ ULONG celt) PURE;
    STDMETHOD(Reset)(THIS) PURE;
    STDMETHOD(Clone)(THIS_ IEnumAssociationElements **ppNew) PURE;
};
#undef INTERFACE

#ifdef COBJMACROS
#define IEnumAssociationElements_QueryInterface(T,a,b)  (T)->lpVtbl->QueryInterface(T,a,b)
#define IEnumAssociationElements_AddRef(T)              (T)->lpVtbl->AddRef(T)
#define IEnumAssociationElements_Release(T)             (T)->lpVtbl->Release(T)
#define IEnumAssociationElements_Next(T,a,b,c)          (T)->lpVtbl->Next(T,a,b,c)
#define IEnumAssociationElements_Skip(T,a)              (T)->lpVtbl->Skip(T,a)
#define IEnumAssociationElements_Reset(T)               (T)->lpVtbl->Reset(T)
#define IEnumAssociationElements_Clone(T,a)             (T)->lpVtbl->Clone(T,a)
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
    STDMETHOD(QueryInterface)(THIS_ REFIID, PVOID*) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /*** IAssociationElementOld ***/
    STDMETHOD(QueryString)(THIS_ ASSOCQUERY query, PCWSTR key, PWSTR *ppszValue) PURE;
    STDMETHOD(QueryDword)(THIS_ ASSOCQUERY query, PCWSTR key, DWORD *pdwValue) PURE;
    STDMETHOD(QueryExists)(THIS_ ASSOCQUERY query, PCWSTR key) PURE;
    STDMETHOD(QueryDirect)(THIS_ ASSOCQUERY query, PCWSTR key, FLAGGED_BYTE_BLOB **ppBlob) PURE;
    STDMETHOD(QueryObject)(THIS_ ASSOCQUERY query, PCWSTR key, REFIID riid, PVOID *ppvObj) PURE;
};
#undef INTERFACE

#ifdef COBJMACROS
#define IAssociationElementOld_QueryInterface(T,a,b)    (T)->lpVtbl->QueryInterface(T,a,b)
#define IAssociationElementOld_AddRef(T)                (T)->lpVtbl->AddRef(T)
#define IAssociationElementOld_Release(T)               (T)->lpVtbl->Release(T)
#define IAssociationElementOld_QueryString(T,a,b,c)     (T)->lpVtbl->QueryString(T,a,b,c)
#define IAssociationElementOld_QueryDword(T,a,b,c)      (T)->lpVtbl->QueryDword(T,a,b,c)
#define IAssociationElementOld_QueryExists(T,a,b)       (T)->lpVtbl->QueryExists(T,a,b)
#define IAssociationElementOld_QueryDirect(T,a,b,c)     (T)->lpVtbl->QueryDirect(T,a,b,c)
#define IAssociationElementOld_QueryObject(T,a,b,c,d)   (T)->lpVtbl->QueryObject(T,a,b,c,d)
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
    STDMETHOD(QueryInterface)(THIS_ REFIID, PVOID*) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /*** IAssociationArrayOld ***/
    STDMETHOD(EnumElements)(THIS_ ULONG flags, IEnumAssociationElements **ppElement) PURE;
    STDMETHOD(QueryString)(THIS_ ULONG flags, ASSOCQUERY query, PCWSTR key, PWSTR *ppszValue) PURE;
    STDMETHOD(QueryDword)(THIS_ ULONG flags, ASSOCQUERY query, PCWSTR key, DWORD *pdwValue) PURE;
    STDMETHOD(QueryExists)(THIS_ ULONG flags, ASSOCQUERY query, PCWSTR key) PURE;
    STDMETHOD(QueryDirect)(THIS_ ULONG flags, ASSOCQUERY query, PCWSTR key, FLAGGED_BYTE_BLOB **ppBlob) PURE;
    STDMETHOD(QueryObject)(THIS_ ULONG flags, ASSOCQUERY query, PCWSTR key, REFIID riid, PVOID *ppvObj) PURE;
};
#undef INTERFACE

#ifdef COBJMACROS
#define IAssociationArrayOld_QueryInterface(T,a,b)      (T)->lpVtbl->QueryInterface(T,a,b)
#define IAssociationArrayOld_AddRef(T)                  (T)->lpVtbl->AddRef(T)
#define IAssociationArrayOld_Release(T)                 (T)->lpVtbl->Release(T)
#define IAssociationArrayOld_EnumElements(T,a,b)        (T)->lpVtbl->EnumElements(T,a,b)
#define IAssociationArrayOld_QueryString(T,a,b,c,d)     (T)->lpVtbl->QueryString(T,a,b,c,d)
#define IAssociationArrayOld_QueryDword(T,a,b,c,d)      (T)->lpVtbl->QueryDword(T,a,b,c,d)
#define IAssociationArrayOld_QueryExists(T,a,b,c)       (T)->lpVtbl->QueryExists(T,a,b,c)
#define IAssociationArrayOld_QueryDirect(T,a,b,c,d)     (T)->lpVtbl->QueryDirect(T,a,b,c,d)
#define IAssociationArrayOld_QueryObject(T,a,b,c,d,e)   (T)->lpVtbl->QueryObject(T,a,b,c,d,e)
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
    STDMETHOD(QueryInterface)(THIS_ REFIID, PVOID*) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;
    /*** IAssociationArrayInitialize ***/
    STDMETHOD(InitClassElements)(THIS_ ULONG flags, PCWSTR pszClass) PURE;
    STDMETHOD(InsertElements)(THIS_ ULONG flags, IEnumAssociationElements *pElements) PURE;
    STDMETHOD(FilterElements)(THIS_ ULONG dwFilter) PURE;
};
#undef INTERFACE

#ifdef COBJMACROS
#define IAssociationArrayInitialize_QueryInterface(T,a,b)   (T)->lpVtbl->QueryInterface(T,a,b)
#define IAssociationArrayInitialize_AddRef(T)               (T)->lpVtbl->AddRef(T)
#define IAssociationArrayInitialize_Release(T)              (T)->lpVtbl->Release(T)
#define IAssociationArrayInitialize_InitClassElements(T,a,b)(T)->lpVtbl->InitClassElements(T,a,b)
#define IAssociationArrayInitialize_InsertElements(T,a,b)   (T)->lpVtbl->InsertElements(T,a,b)
#define IAssociationArrayInitialize_FilterElements(T,a)     (T)->lpVtbl->FilterElements(T,a)
#endif

/*****************************************************************************
 * _Query* helper thunks
 *
 * IAssociationArrayOld::Query* takes (flags, query, key, ...) but the
 * corresponding IAssociationElementOld methods have no flags parameter.
 * These thunks adapt the signature so a single function-pointer type can be
 * passed to _QueryElementAny, discarding the unused extra argument.
 */
static HRESULT WINAPI _QueryString(
        IAssociationElementOld *pElem,
        ASSOCQUERY query,
        PCWSTR key,
        PWSTR *ppszValue,
        int /*unused*/)
{
    return pElem->lpVtbl->QueryString(pElem, query, key, ppszValue);
}

static HRESULT WINAPI _QueryDword(
        IAssociationElementOld *pElem,
        ASSOCQUERY query,
        PCWSTR key,
        DWORD *pdwValue,
        int /*unused*/)
{
    return pElem->lpVtbl->QueryDword(pElem, query, key, pdwValue);
}

static HRESULT WINAPI _QueryExists(
        IAssociationElementOld *pElem,
        ASSOCQUERY query,
        PCWSTR key,
        int /*unused*/,
        int /*unused*/)
{
    return pElem->lpVtbl->QueryExists(pElem, query, key);
}

static HRESULT WINAPI _QueryDirect(
        IAssociationElementOld *pElem,
        ASSOCQUERY query,
        PCWSTR key,
        FLAGGED_BYTE_BLOB **ppBlob,
        int /*unused*/)
{
    return pElem->lpVtbl->QueryDirect(pElem, query, key, ppBlob);
}

static HRESULT WINAPI _QueryObject(
        IAssociationElementOld *pElem,
        ASSOCQUERY query,
        PCWSTR key,
        REFIID riid,
        PVOID *ppvObj)
{
    return pElem->lpVtbl->QueryObject(pElem, query, key, riid, ppvObj);
}

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
    PFNASSOCELEM_CREATE     pfnCreate;  // typedef HRESULT (WINAPI*)(const GUID*, PCWSTR, IAssociationElementOld**)
    IAssociationElementOld *pElement;
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
    DWORD                       m_dwUnknown30;          // stores ASSOCF_OPEN_BYEXENAME flag
    CDSA<AEINFO>                m_DSA;
    IEnumAssociationElements   *m_pElement1;
    DWORD                       m_dwFlags1;
    IEnumAssociationElements   *m_pElement2;
    DWORD                       m_dwFlags2;
    IUnknown                   *m_pUnk3;                // cached verb element
    LPWSTR                      m_pszVerb;              // cached verb name
    DWORD                       m_dwUnknown31_5;        // cached flags mask
    IUnknown                   *m_pUnknown13;           // cached verb parent element

    // Internal helpers
    void    _Reset();
    BOOL    _FirstElement(DWORD dwFlags, IAssociationElementOld **ppElem);
    INT     _GetElement(INT iItem, DWORD dwFlags, IAssociationElementOld **ppElem);
    HRESULT _GetCachedVerbElement(DWORD dwFlags, PCWSTR pszVerb,
                                  IUnknown **ppElem, IUnknown **ppParent);
    void    _SetCachedVerbElement(DWORD dwFlags, PCWSTR pszVerb,
                                  IUnknown *pElem, IUnknown *pParent);
    HRESULT _GetVerbElement(DWORD dwFlags, PCWSTR pszVerb,
                            IAssociationElementOld **ppElem,
                            IAssociationElementOld **ppParent);
    void    _InitDelayedElements(INT iInsertPos, DWORD dwFlags);
    HRESULT _InsertSingleElement(IUnknown *pUnk);

    template <typename T_OUT>
    HRESULT _QueryElementAny(
        HRESULT (WINAPI *pfnQuery)(IAssociationElementOld*, ASSOCQUERY, PCWSTR, T_OUT, int),
        DWORD   dwFlags,
        ASSOCQUERY query,
        PCWSTR  pszKey,
        T_OUT   pOut,
        int     nExtra);

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

CAssocArray::CAssocArray()
    : m_cRefs(1)
    , m_hr(E_UNEXPECTED)
    , m_pszClass(NULL)
    , m_dwFilter(0)
    , m_dwUnknown30(0)
    , m_pElement1(NULL)
    , m_dwFlags1(0)
    , m_pElement2(NULL)
    , m_dwFlags2(0)
    , m_pUnk3(NULL)
    , m_pszVerb(NULL)
    , m_dwUnknown31_5(0)
    , m_pUnknown13(NULL)
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
    INT hr;
    do
    {
        hr = _GetElement(i++, dwFlags, ppElem);
    }
    while (hr == 0);    // 0 = flags mismatch (skip), 1 = success, -1/-2 = error/end
    return (hr == 1);
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
        IUnknown **ppElem,
        IUnknown **ppParent)
{
    if (m_pUnk3 &&
        dwFlags == m_dwUnknown31_5 &&
        _IsSameVerb(pszVerb, m_pszVerb))
    {
        *ppElem = m_pUnk3;
        m_pUnk3->AddRef();
        if (ppParent)
        {
            *ppParent = m_pUnknown13;
            m_pUnknown13->AddRef();
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
INT CAssocArray::_GetElement(INT iItem, DWORD dwFlags, IAssociationElementOld **ppElem)
{
    INT nResult = -1;

    AEINFO *pInfo = m_DSA.GetItemPtr(iItem);
    if (!pInfo)
        return -1;

    if (!(dwFlags & pInfo->dwFlags))
        return 0;

    // Create the element on first access if it has not been instantiated yet
    if (!pInfo->pElement)
    {
        PCWSTR pszClass = pInfo->pClsid ? m_pszClass : NULL;
        if (!pszClass)
            pszClass = m_pszClass;

        HRESULT hr = pInfo->pfnCreate(
            pInfo->pClsid,
            pszClass,
            &pInfo->pElement);

        if (FAILED(hr))
        {
            m_DSA.DeleteItem(iItem);
            return -2;
        }
    }

    if (pInfo->pElement)
    {
        *ppElem = pInfo->pElement;
        pInfo->pElement->AddRef();
        return 1;
    }

    return nResult;
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
    HRESULT hr = _GetCachedVerbElement(
        dwFlags, pszVerb,
        reinterpret_cast<IUnknown**>(ppElem),
        reinterpret_cast<IUnknown**>(ppParent));

    if (SUCCEEDED(hr))
        return hr;

    IAssociationElementOld *pCurrent = NULL;
    for (INT i = 0; _GetElement(i, dwFlags, &pCurrent) >= 0; ++i)
    {
        if (!pCurrent)
            continue;

        IAssociationElementOld *pVerbElem = NULL;
        hr = pCurrent->lpVtbl->QueryObject(
            pCurrent,
            static_cast<ASSOCQUERY>(0x2200001),
            pszVerb,
            IID_IAssociationElement,
            reinterpret_cast<PVOID*>(&pVerbElem));

        if (SUCCEEDED(hr))
        {
            _SetCachedVerbElement(dwFlags, pszVerb,
                reinterpret_cast<IUnknown*>(pVerbElem),
                reinterpret_cast<IUnknown*>(pCurrent));
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
            while (SUCCEEDED(m_pElement1->lpVtbl->Next(m_pElement1, 1, (void**)&pElem, NULL)))
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
        while (SUCCEEDED(m_pElement2->lpVtbl->Next(m_pElement2, 1, (void**)&pElem, NULL)))
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
 * _QueryElementAny - dispatch a query function across all matching elements
 *
 * Iterates elements filtered by dwFlags and calls pfnQuery on each until one
 * succeeds.  When the upper bit of ASSOCQUERY (0x2000000) is set the query
 * targets a specific verb element, which is resolved via _GetVerbElement
 * instead of a full scan.
 */
template <typename T_OUT>
HRESULT CAssocArray::_QueryElementAny(
        HRESULT (WINAPI *pfnQuery)(IAssociationElementOld*, ASSOCQUERY, PCWSTR, T_OUT, int),
        DWORD    dwFlags,
        ASSOCQUERY query,
        PCWSTR   pszKey,
        T_OUT    pOut,
        int      nExtra)
{
    DWORD dwEffective = m_dwFilter & dwFlags;

    HRESULT hr = E_FAIL;

    // Determine whether to use a full scan or the verb-element shortcut.
    // The 0x2000000 bit in the query value signals "verb-targeted" queries,
    // unless m_dwUnknown30 (ASSOCF_OPEN_BYEXENAME) is set and the query is
    // one of the known override values.
    if (!(static_cast<DWORD>(query) & 0x2000000) ||
        (m_dwUnknown30 &&
         (query == static_cast<ASSOCQUERY>(0x2010007) ||
          query == static_cast<ASSOCQUERY>(0x2170008) ||
          query == static_cast<ASSOCQUERY>(0x2200001))))
    {
        // Normal linear scan
        IAssociationElementOld *pElem = NULL;
        for (INT i = 0; _GetElement(i, dwEffective, &pElem) >= 0; ++i)
        {
            if (!pElem)
                continue;

            hr = pfnQuery(pElem, query, pszKey, pOut, nExtra);
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
            if (query == static_cast<ASSOCQUERY>(0x2170008))
            {
                // Route ASSOCQUERY 0x2170008 to the parent element
                hr = pfnQuery(pParentElem, static_cast<ASSOCQUERY>(0x2170008),
                              pszKey, pOut, nExtra);
            }
            else
            {
                hr = pfnQuery(pVerbElem, query, NULL, pOut, nExtra);
            }

            pVerbElem->Release();
            if (pParentElem)
                pParentElem->Release();
        }
    }

    return hr;
}

/**
 * _SetCachedVerbElement - update the verb element cache
 *
 * Releases any existing cached values before storing the new ones.
 */
void CAssocArray::_SetCachedVerbElement(
        DWORD    dwFlags,
        PCWSTR   pszVerb,
        IUnknown *pElem,
        IUnknown *pParent)
{
    if (m_pUnk3)
    {
        IUnknown_SafeReleaseAndNullPtr(&m_pUnk3);
        IUnknown_SafeReleaseAndNullPtr(&m_pUnknown13);
        if (m_pszVerb)
        {
            LocalFree(m_pszVerb);
            m_pszVerb = NULL;
        }
    }

    m_pszVerb = pszVerb ? StrDupW(pszVerb) : NULL;

    if (m_pszVerb || !pszVerb)
    {
        m_pUnk3 = pElem;
        pElem->AddRef();
        m_pUnknown13 = pParent;
        pParent->AddRef();
        m_dwUnknown31_5 = dwFlags;
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
    IUnknown_SafeReleaseAndNullPtr(&m_pUnk3);

    if (m_pszVerb)
    {
        LocalFree(m_pszVerb);
        m_pszVerb = NULL;
    }

    IUnknown_SafeReleaseAndNullPtr(&m_pUnknown13);
    m_dwUnknown30 = 0;
    m_hr = E_UNEXPECTED;
}

// @implemented
STDMETHODIMP CAssocArray::EnumElements(ULONG flags, IEnumAssociationElements **ppElement)
{
    *ppElement = new CEnumAssocElems(this, m_dwFilter & flags);
    return *ppElement ? S_OK : E_OUTOFMEMORY;
}

// @implemented
STDMETHODIMP CAssocArray::QueryString(
        ULONG flags, ASSOCQUERY query, PCWSTR key, PWSTR *ppszValue)
{
    return _QueryElementAny(_QueryString, flags, query, key, ppszValue, 0);
}

// @implemented
STDMETHODIMP CAssocArray::QueryDword(
        ULONG flags, ASSOCQUERY query, PCWSTR key, DWORD *pdwValue)
{
    return _QueryElementAny(_QueryDword, flags, query, key, pdwValue, 0);
}

// @implemented
STDMETHODIMP CAssocArray::QueryExists(ULONG flags, ASSOCQUERY query, PCWSTR key)
{
    return _QueryElementAny(
        reinterpret_cast<HRESULT (WINAPI*)(IAssociationElementOld*, ASSOCQUERY, PCWSTR, int, int)>(_QueryExists),
        flags, query, key, 0, 0);
}

// @implemented
STDMETHODIMP CAssocArray::QueryDirect(
        ULONG flags, ASSOCQUERY query, PCWSTR key, FLAGGED_BYTE_BLOB **ppBlob)
{
    return _QueryElementAny(_QueryDirect, flags, query, key, ppBlob, 0);
}

// @implemented
STDMETHODIMP CAssocArray::QueryObject(
        ULONG flags, ASSOCQUERY query, PCWSTR key, REFIID riid, PVOID *ppvObj)
{
    // _QueryObject swaps the riid/ppvObj arguments, so a dedicated thunk is used
    return _QueryElementAny(
        reinterpret_cast<HRESULT (WINAPI*)(IAssociationElementOld*, ASSOCQUERY, PCWSTR, REFIID, PVOID*)>(_QueryObject),
        flags, query, key, riid, reinterpret_cast<int>(ppvObj));
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

    m_dwUnknown30 = flags & ASSOCF_OPEN_BYEXENAME;

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
                         pszExtra, IID_IAssociationElement,
                         reinterpret_cast<PVOID*>(&pElem));
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
    STDMETHODIMP Next(ULONG celt, IAssociationElement *pElement, ULONG *pceltFetched) override;
    STDMETHODIMP Skip(ULONG celt) override;
    STDMETHODIMP Reset() override;
    STDMETHODIMP Clone(IEnumAssociationElements **ppNew) override;

private:
    HRESULT _Next(IAssociationElementOld **ppElem);
};

// @implemented
CEnumAssocElems::CEnumAssocElems(CAssocArray *pAssocArray, DWORD dwFlags)
    : m_cRefs(1)
    , m_iItem(0)
    , m_dwFlags(dwFlags)
    , m_pAssocArray(pAssocArray)
{
    m_pAssocArray->AddRef();
}

// @implemented
CEnumAssocElems::~CEnumAssocElems()
{
    if (m_pAssocArray)
        m_pAssocArray->Release();
}

/**
 * _Next - fetch the next valid element and advance m_iItem
 *
 * Skips entries for which _GetElement returns 0 (flags mismatch) and stops
 * when it returns 1 (success) or a negative value (end / error).
 */
HRESULT CEnumAssocElems::_Next(IAssociationElementOld **ppElem)
{
    INT i = 0;
    INT hr;
    do
    {
        hr = m_pAssocArray->_GetElement(m_iItem + i, m_dwFlags, ppElem);
        ++i;
    }
    while (hr == 0);

    m_iItem += i - 1;
    return (hr == 1) ? S_OK : S_FALSE;
}
