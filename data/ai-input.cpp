清書してください。CAssocArrayShift4、CAssocArrayShift8などは、元の構造体CAssocArrayからshiftされた構造です。
負の参照は、shiftする前の構造体を参照してください。

```cxx
template <typename T_ITEM>
class CDSA
{
protected:
    HDSA m_hDSA;

public:
    CDSA()
    {
        m_hDSA = NULL;
    }

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

    BOOL AppendItem(T_ITEM* pItem)
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
    CDSA<T_ITEM> *pDSA = new CDSA();
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
 * IAssociationElementOld interface
 *
 * @see IAssociationElementOld
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
    STDMETHOD(EnumElements)(THIS_ ULONG flags, IEnumAssociationElements **ppElement) PURE;
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
    STDMETHOD(InitClassElements)(ULONG, PCWSTR);
    STDMETHOD(InsertElements)(ULONG, IEnumAssociationElements *);
    STDMETHOD(FilterElements)(ULONG);
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

HRESULT __stdcall _QueryString(IAssociationElementOld *a1, ASSOCQUERY a2, const WCHAR *a3, PWSTR *a4, int a5)
{
  return a1->lpVtbl->QueryString(a1, a2, a3, a4);
}

int __stdcall _QueryDword(IAssociationElementOld *a1, ASSOCQUERY a2, const WCHAR *a3, DWORD *a4, int a5)
{
  return a1->lpVtbl->QueryDword(a1, a2, a3, a4);
}

HRESULT __stdcall _QueryExists(IAssociationElementOld *a1, ASSOCQUERY a2, const WCHAR *a3, int a4, int a5)
{
  return a1->lpVtbl->QueryExists(a1, a2, a3);
}

HRESULT __stdcall _QueryDirect(
        IAssociationElementOld *a1,
        ASSOCQUERY a2,
        const WCHAR *a3,
        FLAGGED_BYTE_BLOB **a4,
        int a5)
{
  return a1->lpVtbl->QueryDirect(a1, a2, a3, a4);
}

// @see IAssociationArray https://www.geoffchappell.com/studies/windows/shell/shell32/interfaces/iassociationarray.htm
// @see IQueryAssociations https://learn.microsoft.com/en-us/windows/win32/api/shlwapi/nn-shlwapi-iqueryassociations
class CAssocArray
    : IAssociationArrayOld
    , IAssociationArrayInitialize
    , IQueryAssociations
{
protected:
    LONG m_cRefs;
    HRESULT m_hr;
    LPWSTR m_pszClass;
    DWORD m_dwFilter;
    DWORD m_dwUnknown30;
    CDSA m_DSA;
    IEnumAssociationElements *m_pElement1;
    DWORD m_dwFlags1;
    IEnumAssociationElements *m_pElement2;
    DWORD m_dwFlags2;
    IUnknown *m_pUnk3;
    LPWSTR m_pszVerb;
    DWORD m_dwUnknown31_5;
    IUnknown *m_pUnknown13;

    void _Reset();

public:
    CAssocArray();
    ~CAssocArray();

    // IAssociationArrayOld methods
    STDMETHODIMP EnumElements(ULONG flags, IEnumAssociationElements **ppElement) override;
    STDMETHODIMP QueryString(ULONG flags, ASSOCQUERY query, PCWSTR key, PWSTR *ppszValue) override;
    STDMETHODIMP QueryDword(ULONG flags, ASSOCQUERY query, PCWSTR key, DWORD *pdwValue) override;
    STDMETHODIMP QueryExists(ULONG flags, ASSOCQUERY query, PCWSTR key) override;
    STDMETHODIMP QueryDirect(ULONG flags, ASSOCQUERY query, PCWSTR key, FLAGGED_BYTE_BLOB **ppBlob) override;
    STDMETHODIMP QueryObject(ULONG flags, ASSOCQUERY query, PCWSTR key, REFIID riid, PVOID *ppvObj) override;

    // IAssociationArrayInitialize methods
    STDMETHODIMP InitClassElements(ULONG, PCWSTR) override;
    STDMETHODIMP InsertElements(ULONG, IEnumAssociationElements *) override;
    STDMETHODIMP FilterElements(ULONG dwFilter) override;

    // IQueryAssociations methods
    STDMETHODIMP Init(_In_ ASSOCF flags, _In_opt_ LPCWSTR pszAssoc, _In_opt_ HKEY hkProgid, _In_opt_ HWND hwnd) override;
    STDMETHODIMP GetString(_In_ ASSOCF flags, _In_ ASSOCSTR str, _In_opt_ LPCWSTR pszExtra, _Out_writes_opt_(*pcchOut) LPWSTR pszOut, _Inout_ DWORD *pcchOut) override;
    STDMETHODIMP GetKey(_In_ ASSOCF flags, _In_ ASSOCKEY key, _In_opt_ LPCWSTR pszExtra, _Out_ HKEY *phkeyOut) override;
    STDMETHODIMP GetData(_In_ ASSOCF flags, _In_ ASSOCDATA data, _In_opt_ LPCWSTR pszExtra, _Out_writes_bytes_opt_(*pcbOut) LPVOID pvOut, _Inout_opt_ DWORD *pcbOut) override;
    STDMETHODIMP GetEnum(_In_ ASSOCF flags, _In_ ASSOCENUM assocenum, _In_opt_ LPCWSTR pszExtra, _In_ REFIID riid, _Outptr_ LPVOID *ppvOut) override;
};

CAssocArray::CAssocArray()
{
}

// @implemented
CAssocArray::~CAssocArray()
{
    _Reset();
}

BOOL __thiscall CAssocArray::_FirstElement(CAssocArray *this, DWORD dwCount, struct IAssociationElementOld **a3)
{
  INT v4; // esi
  int Element; // eax

  v4 = 0;
  do
    Element = CAssocArray::GetElement(this, v4++, dwCount, a3);
  while ( !Element );
  return Element == 1;
}

int __thiscall CAssocArray::_GetCachedVerbElement(
        CAssocArray *this,
        unsigned int a2,
        PCWSTR pszVerb,
        IUnknown **a4,
        IUnknown **a5)
{
  if ( this->m_pUnk3 && a2 == this->m_dwUnknown31_5 && _IsSameVerb(pszVerb, this->m_pszVerb) )
  {
    *a4 = this->m_pUnk3;
    this->m_pUnk3->lpVtbl->AddRef(this->m_pUnk3);
    if ( a5 )
    {
      *a5 = this->m_pUnknown13;
      this->m_pUnknown13->lpVtbl->AddRef(this->m_pUnknown13);
    }
    return 0;
  }
  else
  {
    *a4 = 0;
    if ( a5 )
      *a5 = 0;
    return E_FAIL;
  }
}

int __thiscall CAssocArray::_GetElement(CAssocArray *this, int i, int a3, _DWORD *a4)
{
  CDSA *p_m_DSA; // ebx
  char *ItemPtr; // eax
  _DWORD *v7; // esi
  LPWSTR m_pszClass; // ecx
  int v10; // [esp+Ch] [ebp-4h]

  v10 = -1;
  p_m_DSA = &this->m_DSA;
  ItemPtr = (char *)CDSA<_AEINFO>::GetItemPtr(&this->m_DSA, i);
  if ( ItemPtr )
  {
    if ( (a3 & *(_DWORD *)ItemPtr) != 0 )
    {
      v7 = ItemPtr + 16;
      if ( *((_DWORD *)ItemPtr + 4) )
        goto LABEL_9;
      m_pszClass = (LPWSTR)*((_DWORD *)ItemPtr + 2);
      if ( !m_pszClass )
        m_pszClass = this->m_pszClass;
      if ( (*((int (__stdcall **)(_DWORD, LPWSTR, char *))ItemPtr + 3))(
             *((_DWORD *)ItemPtr + 1),
             m_pszClass,
             ItemPtr + 16) < 0 )
      {
        ((void (__thiscall *)(CDSA *, int))p_m_DSA->lpVtbl->DeleteAllItems)(p_m_DSA, i);
        v10 = -2;
      }
      if ( *v7 )
      {
LABEL_9:
        *a4 = *v7;
        (*(void (__stdcall **)(_DWORD))(*(_DWORD *)*v7 + 4))(*v7);
        return 1;
      }
    }
    else
    {
      return 0;
    }
  }
  return v10;
}

HRESULT __thiscall CAssocArray::_GetVerbElement(
        CAssocArray *this,
        unsigned int dwCount,
        PCWSTR psz1,
        struct IAssociationElementOld **a4,
        struct IAssociationElementOld **a5)
{
  IUnknown *v5; // esi
  int CachedVerbElement; // eax
  HRESULT v8; // ebx
  IAssociationElementOld *v9; // esi
  IUnknown *v11; // [esp-4h] [ebp-14h]
  struct IAssociationElementOld *v12; // [esp+8h] [ebp-8h] BYREF
  CAssocArray *v13; // [esp+Ch] [ebp-4h]
  struct IAssociationElementOld **dwIndexa; // [esp+20h] [ebp+10h]

  v13 = this;
  CachedVerbElement = CAssocArray::_GetCachedVerbElement(this, dwCount, psz1, (IUnknown **)a4, (IUnknown **)a5);
  dwIndexa = 0;
  v8 = CachedVerbElement;
  if ( CachedVerbElement < 0 )
  {
    v11 = v5;
    do
    {
      if ( CAssocArray::GetElement(v13, (INT)dwIndexa, dwCount, &v12) < 0 )
        break;
      v9 = v12;
      if ( v12 )
      {
        v8 = ((int (__stdcall *)(struct IAssociationElementOld *, int, PCWSTR, GUID *, struct IAssociationElementOld **, IUnknown *))v12->lpVtbl->QueryObject)(
               v12,
               35651584,
               psz1,
               &IID_IAssociationElement,
               a4,
               v11);
        if ( v8 >= 0 )
        {
          CAssocArray::_SetCachedVerbElement(v13, dwCount, psz1, (IUnknown *)*a4, (IUnknown *)v9);
          if ( a5 )
          {
            v9->lpVtbl->AddRef((IUnknown *)v9);
            *a5 = v9;
          }
        }
        v11 = (IUnknown *)v9;
        ((void (*)(void))v9->lpVtbl->Release)();
      }
      dwIndexa = (struct IAssociationElementOld **)((char *)dwIndexa + 1);
    }
    while ( v8 < 0 );
  }
  return v8;
}

void __thiscall CAssocArray::_InitDelayedElements(CAssocArray *this, int a2, unsigned int a3)
{
  CDSA *p_m_DSA; // esi
  IEnumAssociationElements **p_m_pElement1; // edi
  IEnumAssociationElements **p_m_pElement2; // edi
  int inserted; // eax
  struct _DSA *m_hDSA; // [esp-Ch] [ebp-38h]
  int pitem; // [esp+Ch] [ebp-20h] BYREF
  int v10; // [esp+10h] [ebp-1Ch]
  int v11; // [esp+14h] [ebp-18h]
  int v12; // [esp+18h] [ebp-14h]
  int v13; // [esp+1Ch] [ebp-10h]
  char v14[4]; // [esp+20h] [ebp-Ch] BYREF
  int i; // [esp+24h] [ebp-8h]
  int v16; // [esp+28h] [ebp-4h] BYREF

  if ( !a2 )
  {
    p_m_pElement1 = &this->m_pElement1;
    if ( this->m_pElement1 )
    {
      if ( (a3 & this->m_dwFlags1) == this->m_dwFlags1 )
      {
        i = 0;
        while ( !(*p_m_pElement1)->lpVtbl->Next(*p_m_pElement1, 1, (void **)&v16, (ulong *)v14) )
        {
          v13 = v16;
          pitem = 1;
          m_hDSA = this->m_DSA.m_hDSA;
          v10 = 0;
          v11 = 0;
          v12 = 0;
          inserted = DSA_InsertItem(m_hDSA, i++, &pitem);
          if ( inserted != -1 )
            (*(void (__stdcall **)(int))(*(_DWORD *)v16 + 4))(v16);
          (*(void (__stdcall **)(int))(*(_DWORD *)v16 + 8))(v16);
        }
        IUnknown_SafeReleaseAndNullPtr<IDeskBand>((IUnknown **)&this->m_pElement1);
      }
    }
  }
  p_m_pElement2 = &this->m_pElement2;
  if ( this->m_pElement2 && (a3 & 0x10000) != 0 && a2 == *(_DWORD *)this->m_DSA.m_hDSA )
  {
    if ( !(*p_m_pElement2)->lpVtbl->Next(*p_m_pElement2, 1, (void **)&v16, (ulong *)v14) )
    {
      p_m_DSA = &this->m_DSA;
      do
      {
        v13 = v16;
        pitem = 0x10000;
        v10 = 0;
        v11 = 0;
        v12 = 0;
        if ( CDSA<_DKAITEM>::AppendItem(p_m_DSA, &pitem) != -1 )
          (*(void (__stdcall **)(int))(*(_DWORD *)v16 + 4))(v16);
        (*(void (__stdcall **)(int))(*(_DWORD *)v16 + 8))(v16);
      }
      while ( !(*p_m_pElement2)->lpVtbl->Next(*p_m_pElement2, 1, (void **)&v16, (ulong *)v14) );
    }
    IUnknown_SafeReleaseAndNullPtr<IDeskBand>((IUnknown **)p_m_pElement2);
  }
}

int __thiscall CAssocArray::_InsertSingleElement(CAssocArray *this, IUnknown *a2)
{
  CDSA *p_m_DSA; // esi
  int pitem[5]; // [esp+8h] [ebp-14h] BYREF

  p_m_DSA = &this->m_DSA;
  pitem[0] = 4;
  memset(&pitem[1], 0, 12);
  pitem[4] = (int)a2;
  if ( !CDSA<LOCATION_ENTRY>::Create(&this->m_DSA, 1) || CDSA<_DKAITEM>::AppendItem(p_m_DSA, pitem) == -1 )
    return E_OUTOFMEMORY;
  a2->lpVtbl->AddRef(a2);
  return 0;
}

HRESULT __thiscall CAssocArray::_QueryElementAny<unsigned short * *>(
        CAssocArray *this,
        int (__stdcall *a2)(struct IAssociationElement *, int, PCWSTR, int, int),
        DWORD dwCount,
        int a4,
        PCWSTR psz1,
        int a6,
        int a7)
{
  unsigned int v8; // ecx
  HRESULT hr; // edi
  INT i; // esi
  DWORD v12; // ebx
  int v13; // eax
  struct IAssociationElement *v14; // [esp+Ch] [ebp-4h] BYREF

  v8 = this->m_dwFilter & dwCount;
  dwCount = v8;
  hr = E_FAIL;
  if ( (a4 & 0x2000000) == 0 || this->m_dwUnknown30 && (a4 == 0x2010007 || a4 == 0x2170008 || a4 == 0x2200001) )
  {
    for ( i = 0; CAssocArray::GetElement(this, i, dwCount, &v14) >= 0; ++i )
    {
      if ( v14 )
      {
        hr = a2(v14, a4, psz1, a6, a7);
        v14->lpVtbl->Release((IUnknown *)v14);
        if ( hr >= 0 )
          break;
      }
    }
  }
  else
  {
    hr = CAssocArray::_GetVerbElement(this, v8, psz1, &v14, (struct IAssociationElement **)&dwCount);
    if ( hr >= 0 )
    {
      v12 = dwCount;
      if ( a4 == 0x2170008 )
        v13 = a2((struct IAssociationElement *)dwCount, 0x2170008, psz1, a6, a7);
      else
        v13 = a2(v14, a4, 0, a6, a7);
      hr = v13;
      v14->lpVtbl->Release((IUnknown *)v14);
      (*(void (__stdcall **)(DWORD))(*(_DWORD *)v12 + 8))(v12);
    }
  }
  return hr;
}

void __thiscall CAssocArray::_SetCachedVerbElement(
        CAssocArray *this,
        unsigned int a2,
        PCWSTR pszSrch,
        IUnknown *a4,
        IUnknown *a5)
{
  IUnknown **p_m_pUnk3; // edi

  p_m_pUnk3 = &this->m_pUnk3;
  if ( this->m_pUnk3 )
  {
    IUnknown_SafeReleaseAndNullPtr<IDeskBand>(&this->m_pUnk3);
    IUnknown_SafeReleaseAndNullPtr<IDeskBand>(&this->m_pUnknown13);
    if ( this->m_pszVerb )
      LocalFree(this->m_pszVerb);
  }
  if ( pszSrch )
    this->m_pszVerb = StrDupW(pszSrch);
  else
    this->m_pszVerb = 0;
  if ( this->m_pszVerb || !pszSrch )
  {
    *p_m_pUnk3 = a4;
    a4->lpVtbl->AddRef(a4);
    this->m_pUnknown13 = a5;
    a5->lpVtbl->AddRef(a5);
    this->m_dwUnknown31_5 = a2;
  }
}

void __thiscall CAssocArray::_Reset(CAssocArray *this)
{
  if ( this->m_hr != -1 )
  {
    if ( this->m_DSA.m_hDSA )
      CDSA<PINENTRY>::DestroyCallback(&this->m_DSA, (PFNDAENUMCALLBACK)_AeinfoDelete, 0);
    if ( this->m_pszClass )
    {
      CoTaskMemFree(this->m_pszClass);
      this->m_pszClass = 0;
    }
    IUnknown_SafeReleaseAndNullPtr<IDeskBand>(&this->m_pElement1);
    IUnknown_SafeReleaseAndNullPtr<IDeskBand>(&this->m_pElement2);
    IUnknown_SafeReleaseAndNullPtr<IDeskBand>(&this->m_pUnk3);
    if ( this->m_pszVerb )
      LocalFree(this->m_pszVerb);
    IUnknown_SafeReleaseAndNullPtr<IDeskBand>(&this->m_pUnknown13);
    this->m_dwUnknown30 = 0;
    this->m_hr = -1;
  }
}

// @implemented
HRESULT CAssocArray::EnumElements(ULONG flags, IEnumAssociationElements **ppElement)
{
    *ppElement = = CEnumAssocElems(this, m_dwFilter & flags);
    return *ppElement ? S_OK : E_OUTOFMEMORY;
}

int __stdcall CAssocArray::QueryString(
        CAssocArray *a1,
        struct IAssociationElementOld *a2,
        int a3,
        const WCHAR *psz1,
        int a5)
{
  return CAssocArray::_QueryElementAny<unsigned short * *>(a1, (int)&_QueryString, a2, a3, psz1, a5, 0);
}

int __stdcall CAssocArray::QueryDword(
        CAssocArray *a1,
        struct IAssociationElementOld *a2,
        int a3,
        const WCHAR *psz1,
        int a5)
{
  return CAssocArray::_QueryElementAny<unsigned short * *>(a1, (int)&_QueryDword, a2, a3, psz1, a5, 0);
}

int __stdcall CAssocArray::QueryExists(CAssocArray *a1, struct IAssociationElementOld *a2, int a3, const WCHAR *psz1)
{
  return CAssocArray::_QueryElementAny<unsigned short * *>(
           a1,
           (int (__stdcall *)(struct IAssociationElementOld *, int, PCWSTR, int, int))&_QueryExists,
           (signed int)a2,
           a3,
           psz1,
           0,
           0);
}

int __stdcall CAssocArray::QueryDirect(
        CAssocArray *a1,
        struct IAssociationElementOld *a2,
        int a3,
        const WCHAR *psz1,
        int a5)
{
  return CAssocArray::_QueryElementAny<unsigned short * *>(
           a1,
           (int (__stdcall *)(struct IAssociationElementOld *, int, PCWSTR, int, int))_QueryDirect,
           (signed int)a2,
           a3,
           psz1,
           a5,
           0);
}

int __stdcall CAssocArray::QueryObject(
        CAssocArray *a1,
        struct IAssociationElementOld *a2,
        int a3,
        const WCHAR *psz1,
        int a5,
        int a6)
{
  return CAssocArray::_QueryElementAny<unsigned short * *>(
           a1,
           (int (__stdcall *)(struct IAssociationElementOld *, int, PCWSTR, int, int))&_QueryObject,
           (signed int)a2,
           a3,
           psz1,
           a6,
           a5);
}

int __stdcall _QueryObject(IAssociationElementOld *a1, int a2, int a3, int a4, int a5)
{
  return ((int (__stdcall *)(IAssociationElementOld *, int, int, int, int))a1->lpVtbl->QueryDirect)(a1, a2, a3, a5, a4);
}

LONG __stdcall CAssocArray::InitClassElements(CAssocArrayShift4 *this, IAssociationElementOld *pElement, LPCWSTR psz)
{
  LONG hr; // eax
  WCHAR v4; // ax
  int *v5; // esi
  int v6; // eax
  int v7; // ebx

  CAssocArray::_Reset((CAssocArray *)&this[-1].m_pUnknown13);
  hr = SHStrDupW(psz, &this->m_pszClass);
  if ( hr >= 0 )
  {
    hr = CDSA<LOCATION_ENTRY>::Create(&this->m_DSA, 6) ? 0 : E_OUTOFMEMORY;
    if ( hr >= 0 )
    {
      v4 = *this->m_pszClass;
      if ( v4 == '.' )
      {
        v5 = dword_7C916CF8;
        v6 = 4;
      }
      else
      {
        v5 = &dword_7C912488;
        if ( v4 != '{' )
          v5 = &dword_7C912470;
        v6 = 1;
      }
      v7 = v6;
      do
      {
        CDSA<_DKAITEM>::AppendItem(&this->m_DSA, v5);
        v5 += 5;
        --v7;
      }
      while ( v7 );
      if ( (BYTE1(pElement) & 1) != 0 )
        CDSA<_DKAITEM>::AppendItem(&this->m_DSA, &dword_7C913DAC);
      if ( (BYTE1(pElement) & 2) != 0 )
        CDSA<_DKAITEM>::AppendItem(&this->m_DSA, &dword_7C916E10);
      if ( CAssocArray::_FirstElement(
             (CAssocArray *)&this[-1].m_pUnknown13,
             6u,
             (struct IAssociationElementOld **)&pElement) )
      {
        pElement->lpVtbl->Release((IUnknown *)pElement);
        hr = 0;
      }
      else
      {
        hr = 1;
      }
    }
  }
  this->m_hr = hr;
  return hr;
}

HRESULT __stdcall CAssocArray::InsertElements(
        CAssocArrayShift4 *this,
        DWORD dwFlags,
        struct IEnumAssociationElements *pElements)
{
  HRESULT hr; // eax

  hr = E_UNEXPECTED;
  if ( !this->m_pElement1 && (dwFlags & 1) != 0 )
  {
    this->m_pElement1 = pElements;
    pElements->lpVtbl->AddRef(pElements);
    this->m_dwFlags1 = dwFlags;
    hr = 0;
  }
  if ( !this->m_pElement2 && (dwFlags & 0x10000) != 0 )
  {
    this->m_pElement2 = pElements;
    pElements->lpVtbl->AddRef(pElements);
    this->m_dwFlags2 = dwFlags;
    return 0;
  }
  return hr;
}

// @implemented
HRESULT CAssocArray::FilterElements(ULONG dwFilter)
{
    m_dwFilter = dwFilter;
    return S_OK;
}

HRESULT __stdcall CAssocArray::Init(CAssocArrayShift8 *this, char flags, LPCWSTR pszAssoc, HKEY hkProgid, HWND hwnd)
{
  GUID *v5; // eax
  int v6; // eax
  const WCHAR *ExtensionW; // esi
  bool bNoProgId; // zf
  int v9; // ebx
  HRESULT inserted; // esi

  ExtensionW = pszAssoc;
  if ( !pszAssoc && !hkProgid )
  {
    inserted = E_INVALIDARG;
    goto LABEL_12;
  }
  CAssocArray::_Reset((CAssocArray *)&this[-1].m_dwUnknown31_5);
  bNoProgId = hkProgid == 0;
  this->m_dwUnknown30 = flags & ASSOCF_OPEN_BYEXENAME;
  if ( bNoProgId )
  {
    if ( (flags & ASSOCF_OPEN_BYEXENAME) == 0 )
    {
      v9 = 0;
      if ( (flags & ASSOCF_INIT_DEFAULTTOFOLDER) != 0 )
        v9 = 256;
      if ( (flags & ASSOCF_INIT_DEFAULTTOSTAR) != 0 )
        v9 |= 0x200u;
      if ( StrChrW(ExtensionW, '\\') )
        ExtensionW = PathFindExtensionW(ExtensionW);
      if ( *ExtensionW )
        inserted = ((int (__stdcall *)(IUnknown **, int, const WCHAR *))this[-1].m_pUnknown13[3].lpVtbl)(
                     &this[-1].m_pUnknown13,
                     v9,
                     ExtensionW);
      else
        inserted = E_INVALIDARG;
      goto LABEL_12;
    }
    v6 = AssocElemCreateForClass(&CLSID_AssocApplicationElement, (int)ExtensionW, (int)&pszAssoc);
  }
  else
  {
    if ( (flags & ASSOCF_OPEN_BYEXENAME) != 0 )
    {
      v5 = &CLSID_AssocApplicationElement;
    }
    else
    {
      v5 = &CLSID_AssocClsidElement;
      if ( (flags & 1) == 0 )
        v5 = &CLSID_AssocProgidElement;
    }
    v6 = AssocElemCreateForKey(v5, (int)hkProgid, (int)&pszAssoc);
  }
  inserted = v6;
  if ( v6 >= 0 )
  {
    inserted = CAssocArray::_InsertSingleElement(
                 (CAssocArray *)&this[-1].m_dwUnknown31_5,
                 (struct IAssociationElementOld *)pszAssoc);
    (*(void (__stdcall **)(LPCWSTR))(*(_DWORD *)pszAssoc + 8))(pszAssoc);
  }
LABEL_12:
  this->m_hr = inserted;
  return inserted;
}

int __stdcall CAssocArray::GetString(
        CAssocArrayShift8 *this,
        __int16 a2,
        enum ASSOCSTR pv,
        const unsigned __int16 *a4,
        PWSTR pszDst,
        unsigned int *cchMax)
{
  int v6; // edi
  const unsigned __int16 *v7; // edx
  DWORD v8; // edi
  unsigned int v9; // eax
  int v10; // ecx
  int v12; // [esp-10h] [ebp-14h]
  LPWSTR v13; // [esp-Ch] [ebp-10h]

  v6 = E_UNEXPECTED;
  if ( pv && pv < ASSOCSTR_DROPTARGET && cchMax && (pszDst || HIWORD(cchMax)) )
  {
    v7 = a4;
    if ( !a4 )
      v7 = (const unsigned __int16 *)dword_7C915F84[2 * pv];
    if ( *((_DWORD *)&loc_7C915F80 + 2 * pv) )
    {
      v13 = (LPWSTR)v7;
      v12 = *((_DWORD *)&loc_7C915F80 + 2 * pv);
      v8 = this[-1].m_dwUnknown31_5;
      v9 = _MaskFromFlags(a2);
      v6 = (*(int (__stdcall **)(int, unsigned int, int, LPWSTR, enum ASSOCSTR *))(v8 + 16))(v10, v9, v12, v13, &pv);
      if ( v6 >= 0 )
      {
        v6 = _CopyOut(a2 & 0x20, (LPCWSTR)pv, pszDst, cchMax);
        CoTaskMemFree((LPVOID)pv);
      }
    }
  }
  return v6;
}

HRESULT __stdcall CAssocArray::GetKey(CAssocArrayShift8 *this, __int16 a2, int a3, PCWSTR psz1, HKEY *a5)
{
  int v5; // eax
  HKEY *v6; // edi
  HRESULT VerbElement; // esi
  int v8; // eax
  int v9; // eax
  int v10; // eax
  unsigned int v11; // ecx
  BOOL Element; // eax
  unsigned int v14; // eax
  const WCHAR *v15; // [esp-Ch] [ebp-14h]

  v5 = a3;
  v6 = a5;
  *a5 = 0;
  VerbElement = E_INVALIDARG;
  if ( v5 && v5 < 5 )
  {
    v8 = v5 - 1;
    if ( v8 )
    {
      v9 = v8 - 1;
      if ( v9 )
      {
        v10 = v9 - 1;
        if ( v10 )
        {
          if ( v10 != 1 )
            return VerbElement;
          Element = CAssocArray::_FirstElement(
                      (CAssocArray *)&this[-1].m_dwUnknown31_5,
                      this->m_dwFilter & 0x300,
                      (struct IAssociationElementOld **)&this);
        }
        else
        {
          v11 = _MaskFromFlags(a2);
          Element = CAssocArray::_FirstElement(
                      (CAssocArray *)&this[-1].m_dwUnknown31_5,
                      this->m_dwFilter & v11,
                      (struct IAssociationElementOld **)&this);
        }
        VerbElement = !Element ? E_FAIL : 0;
      }
      else
      {
        VerbElement = (*(int (__stdcall **)(DWORD *, int, int, PCWSTR, GUID *, CAssocArrayShift8 **))(this[-1].m_dwUnknown31_5 + 32))(
                        &this[-1].m_dwUnknown31_5,
                        0xFFFF,
                        0x2200001,
                        psz1,
                        &IID_IAssociationElement,
                        &this);
      }
    }
    else
    {
      v15 = psz1;
      v14 = _MaskFromFlags(a2);
      VerbElement = CAssocArray::_GetVerbElement(
                      (CAssocArray *)&this[-1].m_dwUnknown31_5,
                      this->m_dwFilter & v14,
                      v15,
                      (struct IAssociationElementOld **)&psz1,
                      (struct IAssociationElementOld **)&this);
      if ( VerbElement < 0 )
        return VerbElement;
      (*(void (__stdcall **)(PCWSTR))(*(_DWORD *)psz1 + 8))(psz1);
    }
    if ( VerbElement >= 0 )
    {
      VerbElement = AssocKeyFromElement((int)this, (int)v6);
      this->lpVtbl->Release(this);
    }
  }
  return VerbElement;
}

int __stdcall CAssocArray::GetData(
        CAssocArrayShift8 *this,
        __int16 a2,
        enum ASSOCDATA a3,
        const unsigned __int16 *a4,
        byte *a5,
        unsigned int *a6)
{
  int v6; // edi
  const unsigned __int16 *v7; // edx
  DWORD *p_m_dwUnknown31_5; // ecx
  DWORD v9; // edi
  char v10; // si
  unsigned int v11; // eax
  int v12; // ecx
  DWORD v14; // esi
  unsigned int v15; // eax
  int v16; // ecx
  int v17; // [esp-Ch] [ebp-14h]
  const unsigned __int16 *v18; // [esp-8h] [ebp-10h]
  int v19; // [esp-8h] [ebp-10h]
  const unsigned __int16 *v20; // [esp-4h] [ebp-Ch]

  v6 = E_INVALIDARG;
  if ( a3 && a3 < ASSOCDATA_MAX )
  {
    v7 = a4;
    if ( !a4 )
      v7 = (const unsigned __int16 *)*((_DWORD *)&locret_7C91762C + 2 * a3);
    if ( *(_DWORD *)((char *)&loc_7C917627 + 8 * a3 + 1) )
    {
      p_m_dwUnknown31_5 = &this[-1].m_dwUnknown31_5;
      if ( a6 )
      {
        v9 = *p_m_dwUnknown31_5;
        v10 = a2;
        v18 = v7;
        v17 = *(_DWORD *)((char *)&loc_7C917627 + 8 * a3 + 1);
        v11 = _MaskFromFlags(a2);
        v6 = (*(int (__stdcall **)(int, unsigned int, int, const unsigned __int16 *, CAssocArrayShift8 **))(v9 + 28))(
               v12,
               v11,
               v17,
               v18,
               &this);
        if ( v6 >= 0 )
        {
          v6 = _CopyDataOut(v10 & 0x20, (struct _FLAGGED_BYTE_BLOB *)this, a5, a6);
          CoTaskMemFree(this);
        }
      }
      else
      {
        v14 = *p_m_dwUnknown31_5;
        v20 = v7;
        v19 = *(_DWORD *)((char *)&loc_7C917627 + 8 * a3 + 1);
        v15 = _MaskFromFlags(a2);
        return (*(int (__stdcall **)(int, unsigned int, int, const unsigned __int16 *))(v14 + 24))(v16, v15, v19, v20);
      }
    }
    else if ( a3 == ASSOCDATA_HASPERUSERASSOC )
    {
      if ( CAssocArray::_FirstElement(
             (CAssocArray *)&this[-1].m_dwUnknown31_5,
             2u,
             (struct IAssociationElementOld **)&this) )
      {
        this->lpVtbl->Release(this);
        return 0;
      }
      else
      {
        return 1;
      }
    }
  }
  return v6;
}

// @implemented
STDMETHODIMP CAssocArray::GetEnum(_In_ ASSOCF flags, _In_ ASSOCENUM assocenum, _In_opt_ LPCWSTR pszExtra, _In_ REFIID riid, _Outptr_ LPVOID *ppvOut)
{
    return E_NOTIMPL;
}

class CEnumAssocElems : IEnumAssociationElements
{
protected:
    LONG m_cRefs
    INT m_iItem
    INT m_cItems;
    CAssocArray *m_pAssocArray;

public:
    CEnumAssocElems(CAssocArray *pAssocArray, UINT cItems);
    ~CEnumAssocElems();

    HRESULT _Next(IAssociationElementOld **ppEnum)
};

// @implemented
CEnumAssocElems::CEnumAssocElems(CAssocArray *pAssocArray, UINT cItems)
    : m_cRefs(1)
    , m_iItem(0)
    , m_cItems(cItems)
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

HRESULT __thiscall CEnumAssocElems::_Next(CEnumAssocElems *this, struct IAssociationElement **a2)
{
  int iItem; // edi
  int hr; // eax

  iItem = 0;
  do
  {
    hr = CAssocArray::GetElement(this->m_pAssocArray, iItem + this->m_iItem, this->m_cItems, a2);
    ++iItem;
  }
  while ( !hr );
  this->m_iItem += iItem - 1;
  return hr == 1;
}
```

```asm
.text:7C916CF8 dword_7C916CF8  dd 2, 2 dup(0)          ; DATA XREF: CAssocArray::InitClassElements(ulong,ushort const *)+4935↑o
.text:7C916D04                 dd offset ?AssocElemCreateForUser@@YGJPBU_GUID@@PBGPAPAUIAssociationElement@@@Z ; AssocElemCreateForUser(_GUID const *,ushort const *,IAssociationElement * *)
.text:7C916D08                 dd 0
.text:7C916D0C                 dd 4
.text:7C916D10                 dd offset _CLSID_AssocProgidElement
.text:7C916D14                 align 8
.text:7C916D18                 dd offset _AssocElemCreateForClass@12 ; AssocElemCreateForClass(x,x,x)
.text:7C916D1C                 align 10h
.text:7C916D20                 dd 10h
.text:7C916D24                 dd offset _CLSID_AssocSystemElement
.text:7C916D28                 dd 0
.text:7C916D2C                 dd offset _AssocElemCreateForClass@12 ; AssocElemCreateForClass(x,x,x)
.text:7C916D30                 dd 0
.text:7C916D34                 dd 20h
.text:7C916D38                 dd offset _CLSID_AssocPerceivedElement
.text:7C916D3C                 align 10h
.text:7C916D40                 dd offset _AssocElemCreateForClass@12 ; AssocElemCreateForClass(x,x,x)
.text:7C916D44                 dd 0
.text:7C916D48                 db 5 dup(90h)

.text:7C912470 dword_7C912470  dd 4                    ; DATA XREF: CAssocArray::InitClassElements(ulong,ushort const *)+5B↑o
.text:7C912474                 dd offset _CLSID_AssocProgidElement
.text:7C912478                 dd 0
.text:7C91247C                 dd offset _AssocElemCreateForClass@12 ; AssocElemCreateForClass(x,x,x)
.text:7C912480                 dd 2 dup(0)
.text:7C912488 dword_7C912488  dd 4                    ; DATA XREF: CAssocArray::InitClassElements(ulong,ushort const *)+54↑o
.text:7C91248C                 dd offset _CLSID_AssocClsidElement
.text:7C912490                 dd 0
.text:7C912494                 dd offset _AssocElemCreateForClass@12 ; AssocElemCreateForClass(x,x,x)
.text:7C912498                 dd 0
.text:7C91249C                 db 5 dup(90h)

.text:7C913DAC dword_7C913DAC  dd 100h                 ; DATA XREF: CAssocArray::InitClassElements(ulong,ushort const *):loc_7C913D98↑o
.text:7C913DB0                 dd offset _CLSID_AssocFolderElement
.text:7C913DB4                 align 8
.text:7C913DB8                 dd offset _AssocElemCreateForClass@12 ; AssocElemCreateForClass(x,x,x)

.text:7C916E10 dword_7C916E10  dd 200h                 ; DATA XREF: CAssocArray::InitClassElements(ulong,ushort const *):loc_7C916DFE↑o
.text:7C916E14                 dd offset _CLSID_AssocStarElement
.text:7C916E18                 dd 0
.text:7C916E1C                 dd offset _AssocElemCreateForClass@12 ; AssocElemCreateForClass(x,x,x)
.text:7C916E20                 dd 2 dup(0)

.text:7C915F84 dword_7C915F84  dd 90909090h            ; DATA XREF: CAssocArray::GetString(ulong,ASSOCSTR,ushort const *,ushort *,ulong *)+2E↑r
.text:7C915F88                 dd 2070000h, 0
.text:7C915F90                 dd 2010007h, 0
.text:7C915F98                 dd 170000h, 0
.text:7C915FA0                 dd 2170008h, 0
.text:7C915FA8                 dd 1170001h
.text:7C915FAC                 dd offset aNoopen       ; "NoOpen"
.text:7C915FB0                 dd 2 dup(0)
.text:7C915FB8                 dd 2070001h, 0
.text:7C915FC0                 dd 2070002h, 0
.text:7C915FC8                 dd 2070003h, 0
.text:7C915FD0                 dd 2070004h, 0
.text:7C915FD8                 dd 1170001h
.text:7C915FDC                 dd offset aInfotip_1    ; "InfoTip"
.text:7C915FE0                 dd 1170001h
.text:7C915FE4                 dd offset aQuicktip     ; "QuickTip"
.text:7C915FE8                 dd 1170001h
.text:7C915FEC                 dd offset aTileinfo     ; "TileInfo"
.text:7C915FF0                 dd 80070002h, 0
.text:7C915FF8                 dd 70001h, 0
.text:7C916000                 dd 81070002h, 0
```
