
#ifndef __INFORMATIONRELEASESYSTEM__PARSEXMLUTIL_H__
#define __INFORMATIONRELEASESYSTEM__PARSEXMLUTIL_H__


#include "ParseXMLHelper.h"
#include <libxml/parser.h>


using namespace Base;

namespace InformationReleaseSystem
{

    /** @fn void InformationReleaseSystem::TParseXmlDoc(T& tXmlParser, const string& strXmlDoc, const string& strRootNodeName)
     *  @brief ����XML�ĵ�ģ�巽��
     *  @param (OUT)T& tXmlParser xml�����ࣨ����ʵ��void Parse(xmlNodePtr pNode)��������������ʧ�ܣ�
     *  @param (IN)const string& strXmlDoc xml�ַ���
     *  @param (IN)const string& strRootNodeName ���ڵ�����
     *  @return void
     */
    template<class T>
    void TParseXmlDoc(T& tXmlParser, const string& strXmlDoc, const string& strRootNodeName)
    {
        if (strXmlDoc.empty())
        {
            return;
        }

        string strXmlDocUtf8 = GB2UTF(strXmlDoc.c_str());

        if (g_hDllInst == NULL)
        {
            return;
        }
        dllxmlParseDoc xmlParseDoc = (dllxmlParseDoc)GetProcAddress(g_hDllInst, "xmlParseDoc");
        dllxmlGetLastError xmlGetLastError = (dllxmlGetLastError)GetProcAddress(g_hDllInst, "xmlGetLastError");
        dllxmlDocGetRootElement xmlDocGetRootElement = (dllxmlDocGetRootElement)GetProcAddress(g_hDllInst, "xmlDocGetRootElement");
        dllxmlFreeDoc xmlFreeDoc = (dllxmlFreeDoc)GetProcAddress(g_hDllInst, "xmlFreeDoc");

        //��ȡxml�ļ�--------
        xmlDocPtr pDoc = xmlParseDoc(BAD_CAST strXmlDocUtf8.c_str()); // �����ļ�
        if (NULL == pDoc)
        {
            xmlErrorPtr struXmlError = xmlGetLastError();
            int d =struXmlError->line ;
            string s=struXmlError->message;
           // IVMS_LOG_ERROR("TParseXmlDoc", "Cann't parse the XML, The error is at line[%d], the error message is [%s].", struXmlError->line, struXmlError->message);
            return;
        }

        //��ȡ���ڵ�--------
        xmlNodePtr rootNode = xmlDocGetRootElement(pDoc);
        if (NULL == rootNode)
        {
            xmlErrorPtr struXmlError = xmlGetLastError();
            xmlFreeDoc(pDoc);
            return;
        }

        //��ʼ�ڵ����--------
        if (!FindElem(rootNode, strRootNodeName.c_str()))
        {
           
            xmlFreeDoc(pDoc);
            return;
        }

        //����ʵ�ʽ�������
        tXmlParser.Parse(rootNode);

        //�ͷ�xml������������Դ
        xmlFreeDoc(pDoc);
    }

    /** @fn std::string InformationReleaseSystem::TGenXmlDoc(const T& tXmlParser, const string& strVersion)
     *  @brief ����XML�ĵ�ģ�巽��
     *  @param (IN)const T& tXmlParser xml�����ࣨ����ʵ��xmlNodePtr CreateXmlNodePtr()��������������ʧ�ܣ�
     *  @param (IN)const string& strVersion �汾��
     *  @return XML�ĵ�
     */
    template<class T>
    string TGenXmlDoc(T& tXmlParser, const string& strVersion)
    {
        string strXmlDoc;
        xmlDocPtr pXmlDoc = NULL;
        xmlNodePtr pNodeRoot = NULL;

        if (g_hDllInst == NULL)
        {
            return strXmlDoc;
        }
        dllxmlNewProp xmlNewProp = (dllxmlNewProp)GetProcAddress(g_hDllInst, "xmlNewProp");
        dllxmlNewDoc xmlNewDoc = (dllxmlNewDoc)GetProcAddress(g_hDllInst, "xmlNewDoc");
        dllxmlFreeDoc xmlFreeDoc = (dllxmlFreeDoc)GetProcAddress(g_hDllInst, "xmlFreeDoc");
        dllxmlDocSetRootElement xmlDocSetRootElement = (dllxmlDocSetRootElement)GetProcAddress(g_hDllInst, "xmlDocSetRootElement");
        dllxmlDocDumpFormatMemoryEnc xmlDocDumpFormatMemoryEnc = (dllxmlDocDumpFormatMemoryEnc)GetProcAddress(g_hDllInst, "xmlDocDumpFormatMemoryEnc");

        pXmlDoc = xmlNewDoc(BAD_CAST"1.0");
        if (NULL == pXmlDoc)
        {
            return strXmlDoc;
        }

        pNodeRoot = tXmlParser.CreateXmlNodePtr();
        if (NULL == pNodeRoot)
        {
            return strXmlDoc;
        }

        if (!strVersion.empty())
        {
            //���Ӹ��ڵ�����
            xmlNewProp(pNodeRoot, BAD_CAST"version", (xmlChar *)strVersion.c_str());
        }

        //�����ڵ���ӵ��ĵ���
        xmlDocSetRootElement(pXmlDoc, pNodeRoot);

        //���ĵ����浽�ڴ��У�����utf8�����ʽ����
        xmlChar* pOutbuf = NULL;
        //��������ַ�����
        int iOutbufLen = 0;
        xmlDocDumpFormatMemoryEnc(pXmlDoc, &pOutbuf, &iOutbufLen, "UTF-8", 1);

        if (NULL != pOutbuf)
        {
            strXmlDoc = string((char*)pOutbuf);
            xmlFreeFunc(pOutbuf);
        }

        //�ͷ�xml������������Դ
        xmlFreeDoc(pXmlDoc);

        return strXmlDoc;
    }
}
#endif // __INFORMATIONRELEASESYSTEM__PARSEXMLUTIL_H__

