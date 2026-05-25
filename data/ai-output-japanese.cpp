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
 * IAssociationArrayOld の Query* メソッドは (flags, query, key, ...) を受け取るが、
 * IAssociationElementOld の対応メソッドは flags を持たない。
 * _QueryElementAny に渡す関数ポインタの型を統一するため、
 * 余分な引数を無視するサンク関数を介して呼び出す。
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
 * AEINFO - DSA に格納される要素エントリ
 *
 * アセンブリコードの静的データから復元したレイアウト:
 *   offset  0: DWORD  dwFlags   (例: 4, 0x10, 0x20, 0x100, 0x200, 0x10000)
 *   offset  4: GUID*  pClsid    (例: &CLSID_AssocProgidElement)
 *   offset  8: DWORD  reserved  (常に 0)
 *   offset 12: PFUNC  pfnCreate (例: AssocElemCreateForClass)
 *   offset 16: IAssociationElementOld* pElement  (遅延生成後に格納)
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
    DWORD                       m_dwUnknown30;          // ASSOCF_OPEN_BYEXENAME フラグ保持
    CDSA<AEINFO>                m_DSA;
    IEnumAssociationElements   *m_pElement1;
    DWORD                       m_dwFlags1;
    IEnumAssociationElements   *m_pElement2;
    DWORD                       m_dwFlags2;
    IUnknown                   *m_pUnk3;                // キャッシュ済みverb要素
    LPWSTR                      m_pszVerb;              // キャッシュ済みverb名
    DWORD                       m_dwUnknown31_5;        // キャッシュ済みflagsマスク
    IUnknown                   *m_pUnknown13;           // キャッシュ済みverb親要素

    // 内部ヘルパー
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
        _In_                       ASSOCF    flags,
        _In_                       ASSOCDATA data,
        _In_opt_                   LPCWSTR   pszExtra,
        _Out_writes_bytes_opt_(*pcbOut) LPVOID pvOut,
        _Inout_opt_                DWORD    *pcbOut) override;
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
 * _FirstElement - フィルタに合致する最初の要素を返す
 *
 * GetElement を i=0 から順に呼び、最初に有効な要素が見つかった時点で返す。
 * 戻り値: TRUE = 要素あり、FALSE = なし
 */
BOOL CAssocArray::_FirstElement(DWORD dwFlags, IAssociationElementOld **ppElem)
{
    INT i = 0;
    INT hr;
    do
    {
        hr = _GetElement(i++, dwFlags, ppElem);
    }
    while (hr == 0);            // 0 = フラグ不一致でスキップ、1 = 成功、-1/-2 = エラー
    return (hr == 1);
}

/**
 * _GetCachedVerbElement - verb要素キャッシュを参照する
 *
 * dwFlags と pszVerb が現在のキャッシュと一致する場合、
 * キャッシュ済みの要素ポインタを AddRef して返す。
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
 * _GetElement - DSA から i 番目のエントリを取得し、必要なら遅延生成する
 *
 * 戻り値:
 *   1  = 成功 (*ppElem に有効なポインタを格納、AddRef 済み)
 *   0  = dwFlags 不一致 (スキップ)
 *  -1  = iItem 範囲外 (終端)
 *  -2  = 遅延生成に失敗 (エントリは DSA から削除済み)
 */
INT CAssocArray::_GetElement(INT iItem, DWORD dwFlags, IAssociationElementOld **ppElem)
{
    INT nResult = -1;

    AEINFO *pInfo = m_DSA.GetItemPtr(iItem);
    if (!pInfo)
        return -1;

    if (!(dwFlags & pInfo->dwFlags))
        return 0;

    // pElement が未生成なら遅延生成する
    if (!pInfo->pElement)
    {
        PCWSTR pszClass = pInfo->pClsid ? m_pszClass : NULL; // CLSID なし = クラス名不要
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
 * _GetVerbElement - verb に対応する IAssociationElement を検索する
 *
 * まずキャッシュを確認し、ヒットしなければ全要素を走査して
 * ASSOCQUERY_OBJECT (0x2200000 | ...) で問い合わせる。
 * 成功した結果はキャッシュに保存される。
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
 * _InitDelayedElements - 遅延挿入キューから要素を DSA に追加する
 *
 * m_pElement1 (前挿入) と m_pElement2 (後挿入) の両キューを処理する。
 * iInsertPos: 前挿入の開始インデックス
 * dwFlags:    フィルタマスク
 */
void CAssocArray::_InitDelayedElements(INT iInsertPos, DWORD dwFlags)
{
    // 前挿入キュー (m_pElement1)
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

    // 後挿入キュー (m_pElement2)
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
 * _InsertSingleElement - IUnknown を DSA に単体要素として追加する
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
 * _QueryElementAny - 全要素に対してクエリ関数 pfnQuery を順に試みる
 *
 * dwFlags で絞り込んだ要素を先頭から走査し、pfnQuery が S_OK を返した時点で
 * ループを打ち切る。ASSOCQUERY の上位ビットが verb 要素指定を示す場合は
 * _GetVerbElement 経由で該当要素のみに問い合わせる。
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

    // verb 要素指定クエリかどうか判定 (上位ビット 0x2000000 が立っている場合)
    if (!(static_cast<DWORD>(query) & 0x2000000) ||
        (m_dwUnknown30 &&
         (query == static_cast<ASSOCQUERY>(0x2010007) ||
          query == static_cast<ASSOCQUERY>(0x2170008) ||
          query == static_cast<ASSOCQUERY>(0x2200001))))
    {
        // 通常走査
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
        // verb 要素への直接クエリ
        IAssociationElementOld *pVerbElem  = NULL;
        IAssociationElementOld *pParentElem = NULL;

        hr = _GetVerbElement(dwEffective, pszKey,
                             &pVerbElem, &pParentElem);
        if (SUCCEEDED(hr))
        {
            if (query == static_cast<ASSOCQUERY>(0x2170008))
            {
                // 親要素に ASSOCQUERY 0x2170008 を投げる
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
 * _SetCachedVerbElement - verb要素キャッシュを更新する
 *
 * 既存キャッシュがあれば解放してから新しい値を格納する。
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
 * _Reset - すべての状態をリセットして初期化前の状態に戻す
 */
void CAssocArray::_Reset()
{
    if (m_hr == E_UNEXPECTED)   // 未初期化なら何もしない
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
    // _QueryObject は riid/ppvObj を逆順に渡すため専用サンクを使用
    return _QueryElementAny(
        reinterpret_cast<HRESULT (WINAPI*)(IAssociationElementOld*, ASSOCQUERY, PCWSTR, REFIID, PVOID*)>(_QueryObject),
        flags, query, key, riid, reinterpret_cast<int>(ppvObj));
}

/**
 * InitClassElements - クラス名に基づいて標準要素セットを初期化する
 *
 * pszClass の先頭文字によって使用する CLSID テーブルを切り替える:
 *   '.'  → 拡張子 → 4 エントリのユーザー/ProgID/システム/Perceived テーブル
 *   '{'  → CLSID  → ClsidElement テーブル
 *   その他 → ProgID → ProgidElement テーブル
 *
 * flags の bit1 が立っていれば FolderElement を、
 * bit2 が立っていれば StarElement を追加する。
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

    // 先頭文字でテーブルを選択 (アセンブリの静的データから復元)
    const AEINFO *pTable;
    INT nEntries;

    if (*m_pszClass == L'.')
    {
        // 拡張子: User/Progid/System/Perceived の 4 エントリ
        pTable   = s_aeinfoExtension;   // dword_7C916CF8 相当
        nEntries = 4;
    }
    else if (*m_pszClass == L'{')
    {
        // CLSID
        pTable   = s_aeinfoCLSID;       // dword_7C912488 相当
        nEntries = 1;
    }
    else
    {
        // ProgID
        pTable   = s_aeinfoProgid;      // dword_7C912470 相当
        nEntries = 1;
    }

    for (INT i = 0; i < nEntries; ++i)
        m_DSA.AppendItem(&pTable[i]);

    // オプション要素
    if (flags & 0x100)  // ASSOCF_INIT_DEFAULTTOFOLDER 相当
        m_DSA.AppendItem(&s_aeinfoFolder);  // dword_7C913DAC 相当

    if (flags & 0x200)  // ASSOCF_INIT_DEFAULTTOSTAR 相当
        m_DSA.AppendItem(&s_aeinfoStar);    // dword_7C916E10 相当

    // 最初の要素が生成できるか確認
    IAssociationElementOld *pElem = NULL;
    hr = _FirstElement(6, &pElem) ? S_OK : S_FALSE;
    if (pElem)
        pElem->Release();

    m_hr = hr;
    return hr;
}

/**
 * InsertElements - 遅延挿入キューに列挙子を登録する
 *
 * flags & 0x00001: 前挿入キュー (m_pElement1) に登録
 * flags & 0x10000: 後挿入キュー (m_pElement2) に登録
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
 * Init - IQueryAssociations の初期化メソッド
 *
 * pszAssoc または hkProgid から単一の AssocElement を生成し、
 * DSA に挿入して初期化完了とする。
 *
 * flags & ASSOCF_OPEN_BYEXENAME: 実行ファイル名として処理
 * flags & ASSOCF_INIT_DEFAULTTOFOLDER: フォルダ要素を含める
 * flags & ASSOCF_INIT_DEFAULTTOSTAR: ワイルドカード要素を含める
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
            // 拡張子による初期化 → InitClassElements に委譲
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

        // 実行ファイル名 (キーなし)
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
 * GetString - IQueryAssociations::GetString の実装
 *
 * ASSOCSTR の種類に応じて静的テーブル (s_assocstrTable) から
 * ASSOCQUERY と省略時キー名を引き出し、QueryString を呼ぶ。
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

    const ASSOCSTR_ENTRY &entry = s_assocstrTable[str]; // dword_7C915F84 相当
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
 * GetKey - IQueryAssociations::GetKey の実装
 *
 * ASSOCKEY の値に応じて取得対象キーを切り替える:
 *   ASSOCKEY_APP (1)   : verb 要素の親レジストリキー
 *   ASSOCKEY_CLASS (2) : フィルタ指定要素のレジストリキー
 *   ASSOCKEY_BASECLASS (3): ... (未確認)
 *   ASSOCKEY_SHELLEXEC (4): フォルダ/Star フィルタのキー
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
    case ASSOCKEY_APP:  // 1: verb 要素の親キー
    {
        DWORD dwMask = _MaskFromFlags(flags);
        IAssociationElementOld *pParent = NULL;
        hr = _GetVerbElement(m_dwFilter & dwMask, pszExtra, &pElem, &pParent);
        if (SUCCEEDED(hr))
        {
            pParent->Release();
        }
        break;
    }
    case ASSOCKEY_CLASS:  // 2: クエリ指定フィルタの要素
    {
        DWORD dwMask = _MaskFromFlags(flags);
        hr = _FirstElement(m_dwFilter & dwMask, &pElem) ? S_OK : E_FAIL;
        break;
    }
    case ASSOCKEY_BASECLASS:  // 3: QueryObject 経由
    {
        hr = QueryObject(0xFFFF, static_cast<ASSOCQUERY>(0x2200001),
                         pszExtra, IID_IAssociationElement,
                         reinterpret_cast<PVOID*>(&pElem));
        break;
    }
    case ASSOCKEY_SHELLEXEC:  // 4: フォルダ/Star フィルタ
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
 * GetData - IQueryAssociations::GetData の実装
 *
 * ASSOCDATA の種類に応じて静的テーブルから ASSOCQUERY を引き出し、
 * QueryDirect / QueryExists を使ってデータを取得する。
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

    const ASSOCDATA_ENTRY &entry = s_assocdataTable[data]; // locret_7C91762C 相当

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
            // 存在チェックのみ
            return QueryExists(m_dwFilter & dwMask, entry.query, pszKey);
        }
    }
    else if (data == ASSOCDATA_HASPERUSERASSOC)
    {
        IAssociationElementOld *pElem = NULL;
        if (_FirstElement(2, &pElem))
        {
            pElem->Release();
            return S_OK;        // ユーザー関連付けあり
        }
        return S_FALSE;         // なし
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
 * CEnumAssocElems - IEnumAssociationElements の実装
 */
class CEnumAssocElems : public IEnumAssociationElements
{
protected:
    LONG         m_cRefs;
    INT          m_iItem;
    DWORD        m_dwFlags;         // フィルタマスク (EnumElements 時点の値)
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
 * _Next - 次の有効な要素を 1 つ取得して m_iItem を進める
 *
 * _GetElement が 0 (フラグ不一致) を返す間はスキップし、
 * 1 (成功) または負値 (終端/エラー) で停止する。
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
