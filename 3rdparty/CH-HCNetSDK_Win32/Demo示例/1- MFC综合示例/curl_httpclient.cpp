#include "stdafx.h"
#include "curl_httpclient.h"
#include <string>
extern HINSTANCE g_hDllCurlLib;

CHttpClient::CHttpClient(void) : 
m_bDebug(false),
m_fnCurllInit(NULL),
m_fnCurlSetOpt(NULL),
m_fnCurlPerform(NULL),
m_fnCurlCleanUp(NULL)
{
    if (NULL == g_hDllCurlLib)
    {
        return;
    }
    m_fnCurllInit = (CurlLib_curl_easy_init)GetProcAddress(g_hDllCurlLib, "curl_easy_init");
    m_fnCurlSetOpt = (CurlLib_curl_easy_setopt)GetProcAddress(g_hDllCurlLib, "curl_easy_setopt");
    m_fnCurlPerform = (CurlLib_curl_easy_perform)GetProcAddress(g_hDllCurlLib, "curl_easy_perform");
    m_fnCurlCleanUp = (CurlLib_curl_easy_cleanup)GetProcAddress(g_hDllCurlLib, "curl_easy_cleanup");
}

CHttpClient::~CHttpClient(void)
{

}

static int OnDebug(CURL *, curl_infotype itype, char * pData, size_t size, void *)
{
	if(itype == CURLINFO_TEXT)
	{
		//printf("[TEXT]%s\n", pData);
	}
	else if(itype == CURLINFO_HEADER_IN)
	{
		printf("[HEADER_IN]%s\n", pData);
	}
	else if(itype == CURLINFO_HEADER_OUT)
	{
		printf("[HEADER_OUT]%s\n", pData);
	}
	else if(itype == CURLINFO_DATA_IN)
	{
		printf("[DATA_IN]%s\n", pData);
	}
	else if(itype == CURLINFO_DATA_OUT)
	{
		printf("[DATA_OUT]%s\n", pData);
	}
	return 0;
}

static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
	if( NULL == str || NULL == buffer )
	{
		return -1;
	}

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
	return nmemb;
}

int CHttpClient::Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse)
{
    if (NULL == g_hDllCurlLib)
    {
        return CURLE_FAILED_INIT;
    }
	CURLcode res;
    CURL* curl = m_fnCurllInit();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}
	if(m_bDebug)
	{
        m_fnCurlSetOpt(curl, CURLOPT_VERBOSE, 1);
        m_fnCurlSetOpt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}
    m_fnCurlSetOpt(curl, CURLOPT_URL, strUrl.c_str());
    m_fnCurlSetOpt(curl, CURLOPT_POST, 1);
    m_fnCurlSetOpt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
    m_fnCurlSetOpt(curl, CURLOPT_READFUNCTION, NULL);
    m_fnCurlSetOpt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    m_fnCurlSetOpt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    m_fnCurlSetOpt(curl, CURLOPT_NOSIGNAL, 1);
    m_fnCurlSetOpt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    m_fnCurlSetOpt(curl, CURLOPT_TIMEOUT, 5);
    res = m_fnCurlPerform(curl);
    m_fnCurlCleanUp(curl);
	return res;
}

int CHttpClient::Get(const std::string & strUrl, std::string & strResponse)
{
    if (NULL == g_hDllCurlLib)
    {
        return CURLE_FAILED_INIT;
    }
	CURLcode res;
    CURL* curl = m_fnCurllInit();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}
	if(m_bDebug)
	{
        m_fnCurlSetOpt(curl, CURLOPT_VERBOSE, 1);
        m_fnCurlSetOpt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}

    m_fnCurlSetOpt(curl, CURLOPT_URL, strUrl.c_str());
    m_fnCurlSetOpt(curl, CURLOPT_READFUNCTION, NULL);
    m_fnCurlSetOpt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    m_fnCurlSetOpt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	/**
	* ������̶߳�ʹ�ó�ʱ�����ʱ��ͬʱ���߳�����sleep����wait�Ȳ�����
	* ������������ѡ�libcurl���ᷢ�źŴ�����wait�Ӷ����³����˳���
	*/
    m_fnCurlSetOpt(curl, CURLOPT_NOSIGNAL, 1);
    m_fnCurlSetOpt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    m_fnCurlSetOpt(curl, CURLOPT_TIMEOUT, 5);
    m_fnCurlSetOpt(curl, CURLOPT_FOLLOWLOCATION, 1);
    res = m_fnCurlPerform(curl);
    m_fnCurlCleanUp(curl);
	return res;
}

int CHttpClient::Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath)
{
    if (NULL == g_hDllCurlLib)
    {
        return CURLE_FAILED_INIT;
    }
	CURLcode res;
    CURL* curl = m_fnCurllInit();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}
	if(m_bDebug)
	{
        m_fnCurlSetOpt(curl, CURLOPT_VERBOSE, 1);
        m_fnCurlSetOpt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}
    m_fnCurlSetOpt(curl, CURLOPT_URL, strUrl.c_str());
    m_fnCurlSetOpt(curl, CURLOPT_POST, 1);
    m_fnCurlSetOpt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
    m_fnCurlSetOpt(curl, CURLOPT_READFUNCTION, NULL);
    m_fnCurlSetOpt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    m_fnCurlSetOpt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    m_fnCurlSetOpt(curl, CURLOPT_NOSIGNAL, 1);
	if(NULL == pCaPath)
	{
        m_fnCurlSetOpt(curl, CURLOPT_SSL_VERIFYPEER, false);
        m_fnCurlSetOpt(curl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		//ȱʡ�������PEM�������������ã�����֧��DER
		//curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");
        m_fnCurlSetOpt(curl, CURLOPT_SSL_VERIFYPEER, true);
        m_fnCurlSetOpt(curl, CURLOPT_CAINFO, pCaPath);
	}
    m_fnCurlSetOpt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    m_fnCurlSetOpt(curl, CURLOPT_TIMEOUT, 5);
    res = m_fnCurlPerform(curl);
    m_fnCurlCleanUp(curl);
	return res;
}

int CHttpClient::Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath)
{
    if (NULL == g_hDllCurlLib)
    {
        return CURLE_FAILED_INIT;
    }
	CURLcode res;
    CURL* curl = m_fnCurllInit();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}
	if(m_bDebug)
	{
        m_fnCurlSetOpt(curl, CURLOPT_VERBOSE, 1);
        m_fnCurlSetOpt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}
    m_fnCurlSetOpt(curl, CURLOPT_URL, strUrl.c_str());
    m_fnCurlSetOpt(curl, CURLOPT_READFUNCTION, NULL);
    m_fnCurlSetOpt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    m_fnCurlSetOpt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    m_fnCurlSetOpt(curl, CURLOPT_FOLLOWLOCATION, 1);
    m_fnCurlSetOpt(curl, CURLOPT_NOSIGNAL, 1);
	if(NULL == pCaPath)
	{
        m_fnCurlSetOpt(curl, CURLOPT_SSL_VERIFYPEER, false);
        m_fnCurlSetOpt(curl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
        m_fnCurlSetOpt(curl, CURLOPT_SSL_VERIFYPEER, true);
        m_fnCurlSetOpt(curl, CURLOPT_CAINFO, pCaPath);
	}
    m_fnCurlSetOpt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    m_fnCurlSetOpt(curl, CURLOPT_TIMEOUT, 5);
    res = m_fnCurlPerform(curl);
    m_fnCurlCleanUp(curl);
	return res;
}

///////////////////////////////////////////////////////////////////////////////////////////////

void CHttpClient::SetDebug(bool bDebug)
{
	m_bDebug = bDebug;
}
