#include <windows.h>
#include <shlobj.h>
#include "shstr.h"

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

HRESULT WINAPI AssocQueryKeyA(ASSOCF flags, ASSOCKEY key, LPCSTR pszAssoc, LPCSTR pszExtra, HKEY *phkeyOut)
{
    CShStrW pszAssocW, pszExtraW;
    if (FAILED(pszAssocW.SetStr(pszAssoc)) || FAILED(pszExtraW.SetStr(pszExtra)))
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

    CShStrW pszAssocW, pszExtraW;
    if (FAILED(pszAssocW.SetStr(pszAssoc)) || FAILED(pszExtraW.SetStr(pszExtra)))
        return E_OUTOFMEMORY;

    DWORD cchOut = IS_INTRESOURCE(pcchOut) ? PtrToUlong(pcchOut) : *pcchOut;

    HRESULT hr;
    LPWSTR pszOutW = NULL;
    INT cchOutW = 0;
    CShStrW pszOutBuffW
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
    CShStrW pszExtraW;
    if (FAILED(pszExtraW.SetStr(pszExtra)))
        return E_OUTOFMEMORY;

    DWORD cchOut = IS_INTRESOURCE(pcchOut) ? PtrToUlong(pcchOut) : *pcchOut;

    HRESULT hr;
    LPWSTR pszOutW = NULL;
    INT cchOutW = 0;
    CShStrW pszOutBuffW;
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
