#pragma once

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
