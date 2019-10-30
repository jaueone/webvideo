

#include "stdafx.h"
#include "ParseXMLHelper.h"
//#include "iVMSGUIToolkit/PublicControlFunc.h"


namespace InformationReleaseSystem
{
    /** @fn: InformationReleaseSystem::FindElem
    * @brief: �����ڵ���ȫ���ڵ㣬���ҵ��򷵻�true
    * @param: xmlNodePtr & pNode  �ڵ�
    * @param: const char * pCStr  ���ҵĽڵ�����
    * @return: bool
    */
    bool FindElem(xmlNodePtr& pNode, const char* pCStr)
    {
        if (g_hDllInst == NULL)
        {
            return false;
        }
        dllxmlStrcmp xmlStrcmp = (dllxmlStrcmp)GetProcAddress(g_hDllInst, "xmlStrcmp");

        xmlNodePtr pFindNode = pNode;

        while (1)
        {
            while (pFindNode != NULL && pFindNode->type != XML_ELEMENT_NODE)
            {
                pFindNode = pFindNode->next;
            }

            if (pFindNode == NULL)
            {
                break;
            }

            // ��Ϊ����Ҫ�� xmlChar* �� char* ֮���������ת�������Զ�����һ���� BAD_CAST ���䶨�����£� xmlstring.h
            //!! ԭ������˵��unsigned char��char֮�����ǿ������ת����û�������
            //!! xmlChar* == unsigned char*
            if (xmlStrcmp(pFindNode->name, BAD_CAST(pCStr)) == 0)
            {
                pNode = pFindNode;
                return true;
            }

            pFindNode = pFindNode->next;
        }

        return false;
    }

    /** @fn: InformationReleaseSystem::GB2UTF
    * @brief: GB2312תUTF-8
    * @param: const char * gb2312
    * @return: string
    */
    string GB2UTF(const char* gb2312)
    {
        int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
        wchar_t* wstr = new wchar_t[len + 1];
        memset(wstr, 0, len + 1);
        MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
        len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
        char* sztr = new char[len + 1];
        memset(sztr, 0, len + 1);
        WideCharToMultiByte(CP_UTF8, 0, wstr, -1, sztr, len, NULL, NULL);
        if (wstr) delete[] wstr;

        string str = sztr;
        delete[] sztr;
        sztr = NULL;
        return str;
    }

    /** @fn: InformationReleaseSystem::U2G
    * @brief: UTF-8 תGB2312
    * @param: const char * utf8
    * @return: string
    */
    string UTF2GB(const char* utf8)
    {
        int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
        wchar_t* wstr = new wchar_t[len + 1];
        memset(wstr, 0, len + 1);
        MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
        len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
        char* sztr = new char[len + 1];
        memset(sztr, 0, len + 1);
        WideCharToMultiByte(CP_ACP, 0, wstr, -1, sztr, len, NULL, NULL);
        if (wstr) delete[] wstr;

        string str = sztr;
        delete[] sztr;
        sztr = NULL;
        return str;
    }

    /** @fn: InformationReleaseSystem::SpecialCharacterReplace
    * @brief: xml�����ַ��滻����
    * @brief: �ַ��滻����
    * @brief: 1. " ת &quot;  2. ' ת &apos;  3. & ת &amp;  4. < ת &lt;  5. > ת &gt;
    * @param: string strInfo
    * @return: std::string
    */
    string SpecialCharacterReplace(string strInfo)
    {
        //         QString qstrInfo = iVMSGUIToolkit::ConvertToUnicode(strInfo.c_str());
        //         //����Ҫ��ת&����Ȼ�������ת���ַ��е�&�ٴ�ת��
        //         qstrInfo.replace(QString("&"), QString("&amp;"));
        //         qstrInfo.replace(QString("\""), QString("&quot;"));
        //         qstrInfo.replace(QString("'"), QString("&apos;"));
        //         qstrInfo.replace(QString("<"), QString("&lt;"));
        //         qstrInfo.replace(QString(">"), QString("&gt;"));
        // 
        //         return iVMSGUIToolkit::ConvertFromUnicode(qstrInfo).data();
        string strTemp = strInfo;
        strTemp = strTemp.replace(strTemp.begin(), strTemp.end(), "&", "&amp;");
        strTemp = strTemp.replace(strInfo.begin(), strInfo.end(), "\"", "&quot;");
        strTemp = strTemp.replace(strInfo.begin(), strInfo.end(), "'", "&apos;");
        strTemp = strTemp.replace(strInfo.begin(), strInfo.end(), "<", "&lt;");
        strTemp = strTemp.replace(strInfo.begin(), strInfo.end(), ">", "&gt;");
        return strTemp;
    }


    /** @fn bool InformationReleaseSystem::GetValueFormNodePtr(xmlNodePtr pNode)
    *  @brief �ڵ�ֵ��������
    *  @param (IN)xmlNodePtr pNode �ڵ�
    *  @return ֵ
    */
}