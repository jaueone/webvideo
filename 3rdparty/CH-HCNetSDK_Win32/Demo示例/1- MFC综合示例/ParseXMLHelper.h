
#ifndef _PARSEXMLHELPER_H_
#define _PARSEXMLHELPER_H_


#include <libxml/parser.h>
#include "StringUtil.h"
#include <windows.h>

using namespace std;
using namespace Base;

extern HINSTANCE g_hDllInst;




namespace InformationReleaseSystem
{
    typedef xmlDocPtr(*dllxmlParseDoc)(const xmlChar *cur);
    typedef xmlErrorPtr(*dllxmlGetLastError)(void);
    typedef xmlNodePtr(*dllxmlDocGetRootElement)(xmlDocPtr doc);
    typedef void(*dllxmlFreeDoc)(xmlDocPtr cur);
    typedef xmlAttrPtr(*dllxmlNewProp)(xmlNodePtr node, const xmlChar *name, const xmlChar *value);
    typedef xmlNodePtr(*dllxmlNewNode)(xmlNsPtr ns, const xmlChar *name);
    typedef xmlDocPtr(*dllxmlNewDoc)(const xmlChar *version);
    typedef xmlNodePtr(*dllxmlNewChild)(xmlNodePtr parent, xmlNsPtr ns, const xmlChar *name, const xmlChar *content);
    typedef xmlChar*(*dllxmlNodeGetContent)(xmlNodePtr cur);
    typedef int(*dllxmlStrcmp)(const xmlChar *str1, const xmlChar *str2);
    typedef xmlNodePtr(*dllxmlDocSetRootElement)(xmlDocPtr doc, xmlNodePtr root);
    typedef void(*dllxmlDocDumpFormatMemoryEnc)(xmlDocPtr out_doc, xmlChar **doc_txt_ptr, int * doc_txt_len, const char *txt_encoding, int format);
    typedef xmlNodePtr(*dllxmlAddChild)(xmlNodePtr parent, xmlNodePtr cur);
    //Ѱ�ҽڵ�
    bool FindElem(xmlNodePtr& pNode, const char* pCStr);
    //GB2312תUTF-8
    string GB2UTF(const char* gb2312);
    //GB2312תUTF-8
    string UTF2GB(const char* utf8);
    //xml�����ַ��滻
    string SpecialCharacterReplace(string strInfo);

    //!! ����ֻ��Ҫ���֣�unsigned int; int;��
    template<typename T>
    T GetValueFormNodePtr(xmlNodePtr pNode)
    {
        if (g_hDllInst == NULL)
        {
            return NULL;
        }

        dllxmlNodeGetContent xmlNodeGetContent = (dllxmlNodeGetContent)GetProcAddress(g_hDllInst, "xmlNodeGetContent");
        xmlChar* pContent = xmlNodeGetContent(pNode);
        T tValue = StringUtil::String2T<T>(StringUtil::T2String(pContent));//xmlChar*��Ϊstring,�ٱ�ΪBool
        xmlFreeFunc((void*)pContent);

        return tValue;
    }

    //string���� ģ��������������ʹ��StringUtil::String2T���º��ո���ַ������ݶ�ʧ
    template<>
    inline string GetValueFormNodePtr(xmlNodePtr pNode)
    {
        if (g_hDllInst == NULL)
        {
            return "";
        }

        dllxmlNodeGetContent xmlNodeGetContent= (dllxmlNodeGetContent)GetProcAddress(g_hDllInst, "xmlNodeGetContent");
        xmlChar* pContent = xmlNodeGetContent(pNode);
        string strContent((char*)pContent);
        xmlFreeFunc((void*)pContent);

        return strContent;
    }

    // �ػ�boolֵ
    // �ʺ�<xxx>true</xxx> <xxx>false</xxx>
    //      <xxx>1</xxx> <xxx>0</xxx> 
    template<>
    inline bool GetValueFormNodePtr(xmlNodePtr pNode)
    {
        if (g_hDllInst == NULL)
        {
            return false;
        }

        dllxmlNodeGetContent xmlNodeGetContent=(dllxmlNodeGetContent)GetProcAddress(g_hDllInst, "xmlNodeGetContent");
        xmlChar* pContent = xmlNodeGetContent(pNode);
        string strContent = StringUtil::T2String(pContent);
        bool bValue = ("true" == strContent || "1" == strContent);
        xmlFreeFunc((void*)pContent);

        return bValue;
    }

}
#endif    //_PARSEXMLHELPER_H_