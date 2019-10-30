#include "StdAfx.h"
#include "ModifyRecord.h"

ModRecord::ModRecord():
	m_iMaxNum(0),
	m_iModNum(0),
	m_iUsedNum(0),
	m_pRecord (NULL),
	m_iItemSize(0),
	m_pSrc(NULL),
	m_pModSrc(NULL)
{
	
};

ModRecord::~ModRecord()
{
	if ( m_pRecord)
	{
		delete [] m_pRecord; 
		m_pRecord = NULL; 
	}
	if ( m_pModSrc)
	{
		delete [] m_pModSrc	; 
		m_pModSrc = NULL; 
	}
}

//��ʼ����Դ���޸ļ�¼��ͬʱ��ɾ����¼����
BOOL ModRecord::Init(void *p, int iNum ,int iItemSize)
{
	if ( p == NULL || iNum == 0 || iItemSize == 0)
	{
		return FALSE; 
	}
	m_pSrc = (char *)p ; 
	m_iAvailableNum = iNum; 
	m_iItemSize = iItemSize ;  
	m_iModNum = 0; 
	m_iUsedNum = 0;
	if ( m_iAvailableNum > m_iMaxNum)
	{
		ReAllocSource(); 
		m_iMaxNum = m_iAvailableNum; 
	}
	return TRUE; 
}

//����������Դ  ԭ����Դ������
BOOL ModRecord::ReAllocSource()
{
	//�ͷŵ�ԭ��
	if ( m_pRecord )
	{
		delete [] m_pRecord; 
		m_pRecord = NULL; 
	}

	//�����µ�
	try
	{
		m_pRecord = new BYTE[m_iAvailableNum]; 
		memset(m_pRecord, 0, sizeof(BYTE)*m_iAvailableNum); 
		m_pModSrc = new char[m_iAvailableNum * m_iItemSize];
	}
	catch (...)
	{
		m_pRecord = NULL;
		return FALSE; 
	}
	return TRUE; 
}

//���������� pFunc����Ϊtrueʱ ���� ��pFunc ΪNULL�� ֱ����ĩβ��� (ɾ�����ؼ�¼)
//�����±�  
int ModRecord::Update(const void *Item , ConditionCmp pFunc)
{
	if ( !m_pSrc || !m_pRecord || !Item )
	{
		return -1; 
	} 
	if ( !pFunc )
	{
		//ֱ����ĩβ���
		if ( _add( Item, m_iUsedNum) != -1)
		{
			m_pRecord[m_iUsedNum] = 0;          //ɾ����¼
			return m_iUsedNum++; 
		}
		return -1; 
	}
	//���ж����� �������ж�
	for (int i=0; i<m_iUsedNum; i++)
	{
		//��������Ϊ�� �滻��ԭ���� ɾ���޸ļ�¼ 
		if ( pFunc( m_pSrc+i*m_iItemSize, Item) )
		{
			memcpy( m_pSrc+i*m_iItemSize, Item, m_iItemSize); 
			m_pRecord[i] = 0;          //ɾ����¼
			return i; 
		}	
	}
	return -1; 
}
//����������һ�� ��iIndex�±�Ľṹ�壬�� ConditionCmp����Ϊtrueʱ
BOOL ModRecord::Update(const void *pItem, int iIndex)
{ 
	if ( !m_pSrc || !m_pRecord || iIndex > m_iUsedNum)
	{
		return FALSE; 
	} 
	try
	{
		memcpy( m_pSrc+iIndex*m_iItemSize, pItem, m_iItemSize); 
		m_pRecord[iIndex] = 0;          //ɾ����¼
		if ( iIndex == m_iUsedNum)
		{
			m_iUsedNum ++; 
		}
	}
	catch (...)
	{
		return FALSE; 
	}
	return TRUE; 
}

int ModRecord::_add(const void *pItem, int iIndex)
{
	if ( !m_pSrc || !m_pRecord || !pItem  || iIndex > m_iUsedNum )
	{
		return -1; 
	}
	try
	{
		void *pAdd = m_pSrc + (iIndex)*m_iItemSize; 
		memcpy( pAdd, pItem, m_iItemSize);
		m_pRecord[iIndex] = 1; 
		return iIndex; 
	}
	catch (...)
	{
		return -1; 
	}
}

//��ԭ�нṹ��ԱȲ��ң�����Ϊ��  ���ǵ�
int ModRecord::_Cmpadd(const void *pItem, ConditionCmp pFunc ) 
{
	if ( !m_pSrc || !m_pRecord || !pItem )
	{
		return  -1; 
	}
	if ( pFunc )
	{
		for (int i=0; i<m_iUsedNum; i++)
		{
			//��������Ϊ��  ����ԭ�� 
			if ( pFunc( m_pSrc+i*m_iItemSize, pItem) )
			{
				if ( _add(pItem, i) != -1)
				{
					return i; 
				}
				return -1;  
			}
		}
	}	
	return -1; 
}
//��ӻ��޸�ĳһ�� 
//����ֵ -1Ϊʧ��  ����Ϊ�����±� 
//û�жԱ�������ֱ����ĩβ���
//�еĳ��ԶԱȣ��ԱȲ��ɹ�����ĩβ���
int ModRecord::Add(const void *pItem , ConditionCmp pFunc )
{
	int iIndex = -1; 
	if ( (iIndex = _Cmpadd( pItem,  pFunc)) != -1)
	{
		return iIndex; 
	}
	
	//ԭ��û�з��������� ĩβ����µ�
	if ( _add(pItem, m_iUsedNum) != -1)
	{
        
		return m_iUsedNum++; 
	}
	return -1; 
}
//���±���� ����ͬ�� 
BOOL ModRecord::Add(const void *pItem, int iIndex)
{
	if ( _add(pItem, iIndex) != -1 )
	{
		if ( iIndex == m_iUsedNum)
		{
			m_iUsedNum ++; 
		}
		return TRUE; 
	}
	return FALSE; 
}

//ɾ���±�iIndex����
int ModRecord::_del(int iIndex)
{
	if ( !m_pSrc || !m_pRecord ||  iIndex >= m_iUsedNum)
	{
		return -1; 
	}
	for (int j=iIndex; j<(m_iUsedNum-1); j++)           
	{
		memcpy(m_pSrc+j*m_iItemSize, m_pSrc+(j+1)*m_iItemSize, m_iItemSize); 
		m_pRecord[j] = m_pRecord[j+1];
	}
	return iIndex; 
}
//������ɾ��ĳ��
BOOL ModRecord::Del( const void *Item ,ConditionCmp pFunc)
{
	if ( !m_pSrc || !m_pRecord || !Item || !pFunc )
	{
		return FALSE; 
	}
	for (int i=0; i<m_iUsedNum; i++)
	{
		//��������Ϊ�� �滻��ԭ���� ɾ���޸ļ�¼ 
		if ( pFunc( m_pSrc+i*m_iItemSize, Item) && _del(i) != -1)
		{
			m_iUsedNum --; 
			return TRUE; 
		}	
	}
	return FALSE; 
}
//���±�ɾ��ĳ��
BOOL ModRecord::Del(int iIndex)
{
	return (_del(iIndex) != -1)?TRUE:FALSE; 
}


//��ȡ�޸���Դ
int ModRecord::GetMod(void * *p)
{
	if ( !m_pSrc || !m_pRecord || !m_pModSrc )
	{
		return -1; 
	} 
	int j = 0; 
	for (int i=0; i<m_iUsedNum; i++)
	{
		if ( m_pRecord[i] == 1)
		{
			memcpy(m_pModSrc+j*m_iItemSize, m_pSrc+i*m_iItemSize, m_iItemSize);
			j++; 
		}
	}
	m_iModNum = j; 
	*p = m_pModSrc; 
	return m_iModNum; 
}

//����޸ļ�¼
BOOL ModRecord::Clear()
{
	m_iUsedNum = 0; 
	m_iModNum = 0;
	memset(m_pRecord , 0 , sizeof(BYTE)*m_iAvailableNum); 
	return TRUE; 
}
//��ȡʹ����Ŀ
int  ModRecord::GetUsedNum()
{
	return m_iUsedNum; 
}

//�޸�������
int  ModRecord::Modify(const void *pItem, ConditionCmp pFunc)
{	
	return _Cmpadd( pItem,  pFunc); 
}
//���±꣬ �޸�������
int  ModRecord::Modify(const void *pItem, int iIndex)
{
	return Add( pItem, iIndex); 
}

 //��ȡԭʼ��Դ��ַ
void* ModRecord::GetSrc()
{
	return m_pSrc; 
}

//��ȡÿһ��Ĵ�С
int  ModRecord::GetItemSize()
{
    return m_iItemSize; 
}                     

//��ʼ����Դ
BOOL TwoItemBM::Init(void * lpIndex, int iFSize,  void * lpSrc, int iSSize,  DWORD max)
{
    return (m_RecordInfo.Init( lpIndex, max, iFSize) && m_RecordCfg.Init( lpSrc, max, iSSize));
}   

//��ȡ�޸Ĺ���Դ, ���ظ���
int TwoItemBM::GetModify(void **lpIndex, void **lpModifySrc)
{
    int iRetNumCfg = 0;
    int iRetNumInfo = 0; 
    iRetNumInfo	 = m_RecordInfo.GetMod(lpIndex);
    iRetNumCfg	 = m_RecordCfg.GetMod(lpModifySrc);
    return (iRetNumCfg == iRetNumInfo)?iRetNumInfo:-1; 
}   

//��ȡ����
int  TwoItemBM::GetNum()  
{
    return (m_RecordCfg.GetUsedNum() == m_RecordInfo.GetUsedNum()) ? m_RecordCfg.GetUsedNum() : -1; 
}   

//��ȡԭʼ��Դ��ַ
BOOL TwoItemBM::GetSrc(void **lpIndex, void **lpModifySrc)
{
    *lpIndex = m_RecordInfo.GetSrc(); 
    *lpModifySrc =  m_RecordCfg.GetSrc();   
    return TRUE;   
}

//����޸ļ�¼                    
BOOL TwoItemBM::Clear()
{
    m_RecordInfo.Clear(); 
    m_RecordCfg.Clear(); 
    return TRUE; 
}


//����
BOOL TwoItemBM::Update(DWORD dwNum, const void *ItemInfo, const void* ItemCfg, ConditionCmp pFunc)    
{
    m_RecordInfo.Clear(); 
    m_RecordCfg.Clear(); 
    int iIndex = -1; 
    const BYTE * byItemInfo = (BYTE *)ItemInfo; 
    const BYTE * byItemCfg = (BYTE *)ItemCfg;
    int iInfoSize = m_RecordInfo.GetItemSize(); 
    int iCfgSize = m_RecordCfg.GetItemSize(); 
    try
    {
        
        for (DWORD i=0 ; i<dwNum; i++)
        {
            Update(byItemInfo+i*iInfoSize, byItemCfg+i*iCfgSize, pFunc); 
        }
    }
    catch (...)
    {
        return FALSE; 
    }
    return TRUE; 
}


//ˢ��ĳһ�� 
BOOL TwoItemBM::Update(const void *IndexInfo, const void* ItemCfg, ConditionCmp pFunc)
{
    int iIndex; 
    if (( iIndex = m_RecordInfo.Update(IndexInfo , pFunc)) != -1)
    {
        return  m_RecordCfg.Update(ItemCfg, iIndex); 
    }
    return FALSE; 
}

//�������ĳһ��
BOOL TwoItemBM::Add(const void *IndexInfo, const void* ItemCfg, ConditionCmp pFunc)
{
    int iIndex; 
    if (( iIndex = m_RecordInfo.Add(IndexInfo, pFunc)) != -1)
    {
        return m_RecordCfg.Add(ItemCfg, iIndex); 
    }
    return FALSE; 
}  

////�޸�������
BOOL TwoItemBM::Modify(const void *IndexInfo, const void* ItemCfg, ConditionCmp pFunc)
{
    int iIndex; 
    if (( iIndex = m_RecordInfo.Modify(IndexInfo, pFunc)) != -1)
    {
        return m_RecordCfg.Modify(ItemCfg, iIndex); 
    }
    return FALSE; 
}  

//ɾ��ĳһ���
BOOL TwoItemBM::Del(const void *ItemInfo,  ConditionCmp pFunc)
{
    int iIndex = -1; 
    if ((iIndex = m_RecordCfg.Del(ItemInfo, pFunc)) != -1)
    {
        m_RecordInfo.Del(iIndex);
    }
    return TRUE; 
}