

#include "stdafx.h"
#include "XMLParamsConvert.h"


/** @fn          static string UTF82A(const char* cont)
 *  @brief       UTF8ת���ֽ�
 *  @param (in)  const char* cont UTF8�����ַ���
 *  @return      ���ֽ��ַ���
 */
static string UTF82A(const char* cont)
{
	if (NULL == cont)
    {
        return string("");
    }
    int num = MultiByteToWideChar(CP_UTF8, NULL, cont, -1, NULL, NULL);
    wchar_t* buffw = new wchar_t[(unsigned int)num];
    MultiByteToWideChar(CP_UTF8, NULL, cont, -1, buffw, num);
    int len = WideCharToMultiByte(CP_ACP, 0, buffw, num - 1, NULL, NULL, NULL, NULL); 
    char* lpsz = new char[(unsigned int)len + 1]; 
    WideCharToMultiByte(CP_ACP, 0, buffw, num - 1, lpsz, len, NULL, NULL);
    lpsz[len]='\0';
    delete[] buffw;
    string rtn(lpsz);
    delete[] lpsz;
    return rtn ;
}

/** @fn          static string A2UTF8(const char* cont)
 *  @brief       ���ֽ�תUTF8
 *  @param (in)  const char* cont ���ֽ��ַ���
 *  @return      UTF8������ַ���
 */
static string A2UTF8(const char* cont)
{
    if (NULL == cont)
    {
        return string("");
    }
#ifdef OS_WINDOWS
    int num = MultiByteToWideChar(CP_ACP, NULL, cont, -1, NULL, NULL);
    wchar_t* buffw = new wchar_t[(unsigned int)num];
    MultiByteToWideChar(CP_ACP, NULL, cont, -1, buffw, num);
    int len = WideCharToMultiByte(CP_UTF8, 0, buffw, num - 1, NULL, NULL, NULL, NULL); 
    char* lpsz = new char[(unsigned int)len + 1]; 
    WideCharToMultiByte(CP_UTF8, 0, buffw, num - 1, lpsz, len, NULL, NULL);
    lpsz[len]='\0';
    delete[] buffw;
    string rtn(lpsz);
    delete[] lpsz;
    return rtn;
#elif (defined(OS_POSIX) || defined(OS_MAC) || defined(OS_IPHONE)) && !defined OS_ANDROID
    char* outbuf = 0;
    string str;
    if (cont)
    {
        iconv_t cd = iconv_open("UTF-8", "EUC-CN");
        if (cd)
        {
            size_t contlen = strlen(cont);
            size_t outbuflen = contlen * 3 + 1;
            outbuf = new char[outbuflen];
            memset(outbuf, 0, outbuflen);
            char* inptr = const_cast<char*>(cont);
            char* outptr = outbuf;   
            size_t inlen = contlen;
            size_t outlen = outbuflen;
            if (iconv(cd, &inptr, &inlen, &outptr, &outlen) == (size_t)(-1))
            {
                outbuf = 0;
            }
            iconv_close(cd);
            str = outbuf;
            delete[] outbuf;
        }
    }
    return str;
#else
    return string(cont);
#endif
}

/** @fn          BOOL ConvertSingleNodeData(void *pStructVale, CXmlBase &struXml, const char* pNodeName, BYTE byDataType, DWORD nArrayLen)
 *  @brief       ���������͵ĵ����ڵ�����ת��
 *  @param (in)  void *pStructVale �������͵�����ָ�룬�����Ͳ�ͬ������Ϊ�������
 *  @param (in)  CXmlBase &struXml ��������󣬰����Ͳ�ͬ������Ϊ�������
 *  @param (in)  const char* pNodeName Ҫת���ĵ�ǰxml�ڵ�����
 *  @param (in)  BYTE byDataType ת�����ͣ�����μ�XML_NODE_TYPE
 *  @param (in)  DWORD nArrayLen string to arrayʱ�������С�������жϱ������
 *  @param (in)  BOOL bReqNode �ڵ��Ƿ�Ϊ����
 *  @return      BOOL
 */
BOOL ConvertSingleNodeData(void *pStructVale, CXmlBase &struXml, const char* pNodeName, BYTE byDataType, DWORD nArrayLen, BOOL bReqNode)
{
    if (byDataType < NODE_TYPE_REVERSE)
    {
        if (struXml.FindElem(pNodeName))
        {
            //string to
            if (byDataType == NODE_STRING_TO_BOOL) //bool����
            {
                if (struXml.GetData().compare("true") == 0)
                {
                    *(BYTE*)pStructVale = 1;
                    return TRUE;
                }
                else if(struXml.GetData().compare("false") == 0)
                {
                    *(BYTE*)pStructVale = 0;
                    return TRUE;
                }            
            }
            else if (byDataType == NODE_STRING_TO_INT) //int����
            {
                if (struXml.GetData() != "")
                {
                    *(DWORD*)pStructVale = (DWORD)atoi(struXml.GetData().c_str());
                    return TRUE;
                }                       
            }
            else if (byDataType == NODE_STRING_TO_ARRAY)
            {
                string strTmp = UTF82A(struXml.GetData().c_str());
                if (strTmp != "")
                {
                    int nLen = (int)strTmp.length();
                    if (nLen > (int)nArrayLen)
                    {
                        nLen = nArrayLen;
                    }
                    memcpy((char*)pStructVale, strTmp.c_str(), (DWORD)nLen);
                    return TRUE;
                }
            }
            else if (byDataType == NODE_STRING_TO_BYTE)
            {
                if (struXml.GetData() != "")
                {
                    *(BYTE*)pStructVale = (BYTE)atoi(struXml.GetData().c_str());
                    return TRUE;
                }
            }
            else if (byDataType == NODE_STRING_TO_WORD)
            {
                if (struXml.GetData() != "")
                {
                    *(WORD*)pStructVale = (WORD)atoi(struXml.GetData().c_str());
                    return TRUE;
                }
            }
            else if(byDataType == NODE_STRING_TO_FLOAT)
            {
                if (struXml.GetData() != "")
                {
                    *(float*)pStructVale = (float)atof(struXml.GetData().c_str());
                    return TRUE;
                }
            }
        }
        else
        {
			if (bReqNode)
			{
				return FALSE;
			}
			else
			{
				return TRUE; //û�ҵ��ڵ���Ϊ�豸��֧�֣�Ҳ������ȷ
			}
        }
    }
    else if(byDataType > NODE_TYPE_REVERSE)
    {
        if (byDataType == NODE_BOOL_TO_STRING)
        {
            if (*(BYTE*)pStructVale == 0)
            {
                if (struXml.AddNode(pNodeName, "false"))
                {
                    struXml.OutOfElem();
                    return TRUE;
                }
            }
            else if (*(BYTE*)pStructVale == 1)
            {
                if(struXml.AddNode(pNodeName, "true"))
                {
                    struXml.OutOfElem();
                    return TRUE;
                }                
            }
        }
        else if (byDataType == NODE_INT_TO_STRING)
        {
            char szBuf[16] = {0};
			sprintf((char*)szBuf, "%d", *(int*)pStructVale);
            if (struXml.AddNode(pNodeName, szBuf))
            {
                struXml.OutOfElem();
                return TRUE;
            }
        }
        else if (byDataType == NODE_ARRAY_TO_STRING)
        {
            string strValue = "";
            if (nArrayLen == 0)
            {
                strValue = A2UTF8((char*)pStructVale);
            }
            else 
            {
                int nValueLen = (int)strlen((char*)pStructVale);
                if (nValueLen < (int)nArrayLen) //����û�б�����,�н�����
                {
                    strValue = A2UTF8((char*)pStructVale);
                }
                else    
                {
                    char *lpTmp = NULL;
                    lpTmp =  new char[nArrayLen + 1];
                    if (lpTmp == NULL)
                    {
                        return FALSE;
                    }
                    memset(lpTmp,0, nArrayLen + 1);
                    memcpy(lpTmp, (char*)pStructVale, (DWORD)nArrayLen); //����һ��������
                    strValue = A2UTF8(lpTmp);
                    if (lpTmp != NULL)
                    {
						delete[] lpTmp;
						lpTmp = NULL;
                    }   
                }
            }
            if (struXml.AddNode(pNodeName, strValue.c_str()))
            {
                struXml.OutOfElem();
                return TRUE;
            }
        }
        else if (byDataType == NODE_BYTE_TO_STRING)
        {
            char szBuf[16] = {0};
			sprintf((char*)szBuf, "%d", *(int*)pStructVale);
            if (struXml.AddNode(pNodeName, szBuf))
            {
                struXml.OutOfElem();
                return TRUE;
            }
        }
        else if (byDataType == NODE_WORD_TO_STRING)
        {
            char szBuf[16] = {0};
			sprintf((char*)szBuf, "%d", (int)*(WORD*)pStructVale);
            if (struXml.AddNode(pNodeName, szBuf))
            {
                struXml.OutOfElem();
                return TRUE;
            }
        }
        else if (byDataType == NODE_DWORD_TO_STRING)
        {
            char szBuf[16] = { 0 };
            sprintf(szBuf, "%u", *(DWORD*)pStructVale);
            if (struXml.AddNode(pNodeName, szBuf))
            {
                if (!struXml.OutOfElem())
                {
                    return FALSE;
                }
            }
            else
            {
                return FALSE;
            }
            return TRUE;
        }
    }

    return FALSE;
}

/** @fn          BOOL PrintXmlToNewBuffer(char** pXmlBuf, DWORD &dwXmlLen, const CXmlBase &struXml)
 *  @brief       ���xml���ݵ�������
 *  @param (in)  char** pXmlBuf xml������
 *  @param (in)  const CXmlBase &struXml xml����
 *  @param (in)  DWORD &dwXmlLen �����xml���ݳ���
 *  @param (out) LPVOID pStruct �ṹ�����
 *  @return      BOOL
 */
BOOL PrintXmlToNewBuffer(char** pXmlBuf, DWORD &dwXmlLen, const CXmlBase &struXml)
{
    string strTmp = struXml.GetChildren();
    dwXmlLen = (DWORD)strTmp.length();
    *pXmlBuf = new CHAR[dwXmlLen+1];
    if (*pXmlBuf == NULL)
    {
        dwXmlLen = 0;
        return FALSE;
    }
    memset(*pXmlBuf, '\0', (int)dwXmlLen+1);
    memcpy(*pXmlBuf, strTmp.c_str(), dwXmlLen);
    return TRUE;
}

BOOL GenerateGUID(char* pBuffer, const DWORD dwSize)
{
    BOOL bRet = FALSE;
    if (pBuffer == NULL || dwSize < 64)
    {
        return bRet;
    }
    
    memset(pBuffer, 0, dwSize);
    
    GUID struGUID = {0};
    
    CoInitialize(NULL);  
    if(S_OK == CoCreateGuid(&struGUID))  
    {  
        _snprintf(pBuffer, 64, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",  
            struGUID.Data1,  
            struGUID.Data2,  
            struGUID.Data3,  
            struGUID.Data4[0], struGUID.Data4[1],  
            struGUID.Data4[2], struGUID.Data4[3],  
            struGUID.Data4[4], struGUID.Data4[5],  
            struGUID.Data4[6], struGUID.Data4[7]);
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }
    CoUninitialize();  
    
    return bRet;
}

/*********************************************************
������:	ConvertSingleNodeStringData
��������:	���������͵ĵ����ڵ�����ת��
�������:	pStructVale: �������͵�����ָ��,�����Ͳ�ͬ������Ϊ�������;            
            struXml:xml���������,�����Ͳ�ͬ������Ϊ�������
            pNodeName:Ҫת���ĵ�ǰxml�ڵ�����
            byDataType:ת������,����μ�XML_NODE_TYPE
            nArrayLen:string to arrayʱ�������С,�����жϱ������
�������:	            
����ֵ:	    TRUE-�ɹ�,FALSE-ʧ��
**********************************************************/
BOOL ConvertSingleNodeStringData(void *pStructVale, CXmlBase &struXml, const char* pNodeName, BYTE byDataType)
{
	string csDetectionTarget = "";
	if (NODE_STRING_TO_BYTE_DETECTION_TARGET == byDataType)
	{
		if (struXml.FindElem(pNodeName))
		{
			csDetectionTarget = struXml.GetData();
			if (csDetectionTarget.compare("all") == 0)
			{	
				*(BYTE*)pStructVale = 0;
				return TRUE;
			}
			else if (csDetectionTarget.compare("human") == 0)
			{
				*(BYTE*)pStructVale = 1;
				return TRUE;
			}
			else if (csDetectionTarget.compare("vehicle") == 0)
			{
				*(BYTE*)pStructVale = 2;
				return TRUE;
			}	
		}	
	}
	else if (NODE_BYTE_TO_STRING_DETECTION_TARGET == byDataType)
	{
		if ((*(BYTE*)pStructVale) == 0)
		{
			struXml.AddNode(pNodeName, "all");
			struXml.OutOfElem();
			return TRUE;
		}
		else if ((*(BYTE*)pStructVale) == 1)
		{
			struXml.AddNode(pNodeName, "human");
			struXml.OutOfElem();
			return TRUE;
		}
		else if ((*(BYTE*)pStructVale) == 2)
		{
			struXml.AddNode(pNodeName, "vehicle");
			struXml.OutOfElem();
			return TRUE;
		}
	}
	return FALSE;
}

