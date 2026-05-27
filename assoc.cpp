// @implemented 
HRESULT WINAPI
AssocQueryKeyW(
    ASSOCF flags,
    ASSOCKEY key,
    LPCWSTR pszAssoc,
    LPCWSTR pszExtra,
    HKEY *phkeyOut)
{
    IQueryAssociations *pQA;
    HRESULT hr = AssocCreate(CLSID_QueryAssociations, IID_IQueryAssociations, (void **)&pQA);
    if (FAILED(hr))
        return hr;

    const ASSOCF mask = (ASSOCF_INIT_NOREMAPCLSID | ASSOCF_INIT_BYEXENAME |
                         ASSOCF_INIT_DEFAULTTOSTAR | ASSOCF_INIT_DEFAULTTOFOLDER);
    hr = pQA->Init((flags & mask), pszAssoc, NULL, NULL);
    if (SUCCEEDED(hr))
        hr = pQA->GetKey(flags, key, pszExtra, phkeyOut);

    pQA->Release();
    return hr;
}

HRESULT WINAPI AssocQueryStringW(
    ASSOCF cfFlags,
    ASSOCSTR str,
    LPCWSTR pszAssoc,
    LPCWSTR pszExtra,
    LPWSTR pszOut,
    DWORD *pcchOut)
{
    IQueryAssociations *pQA;
    HRESULT hr = AssocCreate(CLSID_QueryAssociations, IID_IQueryAssociations, (void **)&pQA);
    if (FAILED(hr))
        return hr;

    const ASSOCF mask = (ASSOCF_INIT_NOREMAPCLSID | ASSOCF_INIT_BYEXENAME |
                         ASSOCF_INIT_DEFAULTTOSTAR | ASSOCF_INIT_DEFAULTTOFOLDER);
    hr = pQA->Init((cfFlags & mask), pszAssoc, NULL, NULL);
    if (SUCCEEDED(hr))
        hr = pQA->GetString(pQA, cfFlags, str, pszExtra, pszOut, pcchOut);

    pQA->Release();
    return hr;
}

HRESULT WINAPI AssocQueryStringByKeyW(
    ASSOCF flags,
    ASSOCSTR str,
    HKEY hkAssoc,
    LPCWSTR pszExtra,
    LPWSTR pszOut,
    DWORD *pcchOut)
{
    IQueryAssociations *pQA;
    HRESULT hr = AssocCreate(CLSID_QueryAssociations, IID_IQueryAssociations, (void **)&pQA);
    if (FAILED(hr))
        return hr;

    const ASSOCF mask = (ASSOCF_INIT_NOREMAPCLSID | ASSOCF_INIT_BYEXENAME |
                         ASSOCF_INIT_DEFAULTTOSTAR | ASSOCF_INIT_DEFAULTTOFOLDER);
    hr = pQA->Init((flags & mask), NULL, hkAssoc, NULL);
    if (SUCCEEDED(hr))
        hr = pQA->GetString(flags, str, pszExtra, pszOut, pcchOut);

    pQA->Release();
    return hr;
}

class CWideString
{
protected:
    LPWSTR m_pszWide;
    INT m_cchWide;
    WCHAR m_szBuff[128];

    CWideString() : m_pszWide(m_szBuff), m_cchWide(1)
    {
        m_szBuff[0] = UNICODE_NULL;
    }

    ~CWideString()
    {
        Reset();
    }

    operator LPCWSTR() const { return m_pszWide; }

    void Reset()
    {
        if (m_pszWide != m_szBuff)
            CoTaskMemFree(m_pszWide);
        m_pszWide = m_szBuff;
        m_szBuff[0] = UNICODE_NULL;
        m_cchWide = 1;
    }

    INT GetLength() const
    {
        return m_cchWide - 1;
    }

    INT GetSize() const
    {
        return m_cchWide;
    }

    HRESULT SetLength(INT cchLength)
    {
        return SetSize(cchLength + 1);
    }

    HRESULT SetSize(INT cchWide)
    {
        if (cchWide <= _countof(m_szBuff))
        {
            m_cchWide = cchWide;
            return S_OK;
        }

        LPWSTR pszWide = CoTaskMemAlloc(cchWide * sizeof(WCHAR));
        if (!pszWide)
            return E_OUTOFMEMORY;

        CoTaskMemFree(m_pszWide);
        m_pszWide = pszWide;
        m_pszWide[0] = m_szBuff[0] = UNICODE_NULL;
        m_cchWide = cchWide;
        return S_OK;
    }

    HRESULT SetStrA(LPCSTR pszAnsi, INT cchAnsi = -1)
    {
        Reset();

        if (!pszAnsi || !cchAnsi)
            return S_FALSE;

        if (cchAnsi == -1)
            cchAnsi = lstrlenA(pszAnsi) + 1;

        if (!cchAnsi)
            return S_FALSE;

        INT cchWide = MultiByteToWideChar(CP_ACP, 0, pszAnsi, cchAnsi, NULL, 0);
        if (!cchWide)
            return S_FALSE;

        HRESULT hr = SetSize(cchWide);
        if (FAILED(hr))
            return hr;

        if (!MultiByteToWideChar(CP_ACP, 0, pszAnsi, cchAnsi, m_pszWide, m_cchWide))
        {
            Reset();
            return S_FALSE;
        }

        return S_OK;
    }
};

HRESULT WINAPI AssocQueryKeyA(ASSOCF flags, ASSOCKEY key, LPCSTR pszAssoc, LPCSTR pszExtra, HKEY *phkeyOut)
{
    CWideString pszAssocW, pszExtraW;
    if (FAILED(pszAssocW.SetStrA(pszAssoc)) || FAILED(pszExtraW.SetStrA(pszExtra)))
        return E_OUTOFMEMORY;
    return AssocQueryKeyW(flags, key, pszAssocW, pszExtraW, phkeyOut);
}

HRESULT WINAPI AssocQueryStringA(
    ASSOCF flags,
    ASSOCSTR str,
    LPCSTR pszAssoc,
    LPCSTR pszExtra,
    LPSTR pszOut,
    DWORD *pcchOut)
{
    if (!pcchOut)
        return E_INVALIDARG;

    CWideString pszAssocW, pszExtraW;
    if (FAILED(pszAssocW.SetStrA(pszAssoc)) || FAILED(pszExtraW.SetStrA(pszExtra)))
        return E_OUTOFMEMORY;

    DWORD cchOut = IS_INTRESOURCE(pcchOut) ? PtrToUlong(pcchOut) : *pcchOut;

    HRESULT hr;
    LPWSTR pszOutW = NULL;
    INT cchOutW = 0;
    CWideString pszOutBuffW
    if (pszOut)
    {
        hr = pszOutBuffW.SetSize(cchOut);
        if (FAILED(hr))
            return hr;
        pszOutW = pszOutBuffW;
        cchOutW = cchOut;
    }

    hr = AssocQueryStringW(flags, str, pszAssocW, pszExtraW, pszOutW, &cchOutW);
    if (FAILED(hr))
        return hr;

    INT cchOutA = SHUnicodeToAnsi(pszOutBuffW, pszOut, cchOut);
    if (!IS_INTRESOURCE(pcchOut))
        *pcchOut = cchOutA;

    return hr;
}

HRESULT WINAPI AssocQueryStringByKeyA(
    ASSOCF flags,
    ASSOCSTR str,
    HKEY hkAssoc,
    LPCSTR pszExtra,
    LPSTR pszOut,
    DWORD *pcchOut)
{
    if (!pcchOut)
        return E_INVALIDARG;

    HRESULT hr = E_OUTOFMEMORY;
    CWideString pszExtraW;
    if (FAILED(pszExtraW.SetStr(pszExtra)))
        return E_OUTOFMEMORY;

    DWORD cchOut = IS_INTRESOURCE(pcchOut) ? PtrToUlong(pcchOut) : *pcchOut;

    HRESULT hr;
    LPWSTR pszOutW = NULL;
    INT cchOutW = 0;
    CWideString pszOutBuffW;
    if (pszOut)
    {
        hr = pszOutBuffW.SetSize(cchOut);
        if (FAILED(hr))
            return hr;
        pszOutW = pszOutBuffW;
        cchOutW = cchOut;
    }

    hr = AssocQueryStringByKeyW(flags, str, hkAssoc, pszExtraW, pszOutW, &cchOutW);
    if (FAILED(hr))
        return hr;

    INT cchOutA = SHUnicodeToAnsi(pszOutW, pszOut, cchOut);
    if (!IS_INTRESOURCE(pcchOut))
        *pcchOut = cchOutA;

    return hr;
}
