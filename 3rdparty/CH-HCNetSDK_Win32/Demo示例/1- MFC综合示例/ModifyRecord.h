#ifndef MODIFYRECORD_H
#define MODIFYRECORD_H

//����������ʽ
typedef bool (*ConditionCmp)(const void *ItemF, const void *ItemS); 

//������Դ����¼�޸�
//Update ��ˢ��ĳһ�� �����޸ļ�¼ɾ��
//Add ��ӱ����޸���  ������ ���޸�
//Modify �޸ı������е��� 
//Del  ɾ��ĳһ��  
//Clear ��������޸ļ�¼  
class ModRecord
{
public:
	ModRecord();   
	~ModRecord();
//operate function 
	BOOL Init(void *p, int iNum, int iItemSize);            //��ʼ����Դ���޸ļ�¼��ͬʱ��ɾ����¼����
	int Update(const void *pItem , ConditionCmp pFunc = NULL);   //����������һ�� ConditionCmp����Ϊtrueʱ  ����
	int Update(const void *pItem, int iIndex);   //����������һ�� ��iIndex�±�Ľṹ�� ����ͬ��
	int  Add(const void *pItem, ConditionCmp pFunc = NULL);                     //��ӻ���� ĳһ�� ����޸ļ�¼
	BOOL Add(const void *pItem, int iIndex);         //��ӻ���� ���±���� ����ͬ�� ����޸ļ�¼
	int  Modify(const void *pItem, ConditionCmp pFunc);                       //�޸�������
	int  Modify(const void *pItem, int iIndex);                       //���±꣬ �޸������� ����ͬ��
	BOOL Del(const void *pItem , ConditionCmp pFunc);            //������ɾ��ĳ��
	BOOL Del(int iIndex);            //���±�ɾ��ĳ��
	BOOL Clear();                            //�����¼

//get infomation function
	int  GetMod(void * *p);                  //��ȡ�޸���Դ
	int  GetUsedNum();                      //��ȡʹ����Ŀ
	void* GetSrc();                          //��ȡԭʼ��Դ��ַ
    int  GetItemSize();                      //��ȡÿһ��Ĵ�С
protected:
	int _Cmpadd(const void *pItem, ConditionCmp pFunc = NULL);     
	int _add(const void *pItem, int iIndex);   //
	int _del(int iIndex); 
	BOOL ReAllocSource();                //����������Դ  ԭ����Դ������
private:
	int m_iMaxNum;                        //��ʷ�����Դ��
	int m_iAvailableNum;                   //������Դ��
	int m_iItemSize;                     //������Դ��С
	int m_iUsedNum;                       //ʹ����Դ��
	int m_iModNum;					      //�޸���Դ��

	char *m_pSrc;                         //��Դ
	char *m_pModSrc;                      //�޸���Դ
	BYTE *m_pRecord;                      //��¼
};


//������ʹ�ã�һ��Ϊ����ƥ����������һ��Ϊ�������ݰ����±����
class TwoItemBM
{
public:
    BOOL Init(void * lpIndex, int iFSize,  void * lpSrc, int iSSize,  DWORD max);             //��ʼ����Դ
    int  GetModify(void **lpIndex, void **lpModifySrc);        //��ȡ�޸Ĺ���Դ, ���ظ���
    BOOL Clear();                           //����޸ļ�¼
    int  GetNum();                         //��ȡ����
    BOOL GetSrc(void **lpIndex, void **lpModifySrc);    //��ȡԭ�����ڴ�

    BOOL Update(DWORD dwNum, const void *struIndex, const void* ItemCfg, ConditionCmp pFunc=NULL);    //����ˢ����
    BOOL Update(const void *IndexInfo, const void* ItemCfg, ConditionCmp pFunc);   //ˢ��ĳһ�� 
    BOOL Modify(const void *IndexInfo, const void* ItemCfg, ConditionCmp pFunc);  //�޸�������
    BOOL Add(const void *IndexInfo, const void* ItemCfg, ConditionCmp pFunc);  //�����޸�ĳһ��   
    BOOL Del(const void *IndexInfo,  ConditionCmp pFunc);       //ɾ��ĳһ��
protected:            
        

private:
    ModRecord m_RecordInfo; 
    ModRecord m_RecordCfg;
};


#endif