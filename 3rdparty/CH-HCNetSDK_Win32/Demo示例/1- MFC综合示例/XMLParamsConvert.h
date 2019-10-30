

#ifndef _XML_PARAMS_CONVERT_H_
#define _XML_PARAMS_CONVERT_H_

#include "xml/XmlBase.h"

#define REQUEST_URL_LEN       128
#define MAX_XML_ELEM_LEN      128
#define MAX_MATCH_COUNT       256

//#define ISAPI_STATUS_LEN      1024
//#define ISAPI_DATA_LEN        10*10*1024

typedef enum
{
	NODE_STRING_TO_BOOL  = 0,    //stringתbool(0,1)
		NODE_STRING_TO_INT   = 1,    //stringתint(HPR_UINT32)
		NODE_STRING_TO_ARRAY = 2,    //stringת����(HPR_UINT8[],char[])
		NODE_STRING_TO_BYTE  = 3,    //stringתHPR_UINT8,HPR_UINT8��Ϊ����ʱ
		NODE_STRING_TO_WORD  = 4,    //stringתHPR_UINT16
		NODE_STRING_TO_FLOAT = 5,    //stringתFLOAT
		
		NODE_TYPE_REVERSE    = 64,   //���ͷ�ת,��������ת������   
		NODE_BOOL_TO_STRING  = 65,   //bool(0,1)תstring
		NODE_INT_TO_STRING   = 66,   //int(HPR_UINT32)תstring
		NODE_ARRAY_TO_STRING = 67,   //����(HPR_UINT8[],char[])תstring
		NODE_BYTE_TO_STRING  = 68,   //HPR_UINT8תstring,HPR_UINT8��Ϊ����ʱ
		NODE_WORD_TO_STRING  = 69,    //HPR_UINT16תstring
        NODE_DWORD_TO_STRING = 71,  //HPR_UINT32תstring
}XML_NODE_TYPE;

typedef enum
{
	NODE_STRING_TO_BYTE_DETECTION_TARGET = 0,    //string(detectionTarget)תBYTE(byDetectionTarget)	
		NODE_BYTE_TO_STRING_DETECTION_TARGET = 100,  //BYTE(byDetectionTarget)תstring(detectionTarget)
}XML_STRING_NODE_TYPE;


BOOL ConvertSingleNodeData(void *pStructVale, CXmlBase &struXml, const char* pNodeName, BYTE byDataType, DWORD nArrayLen = 0, BOOL bReqNode = TRUE);
BOOL PrintXmlToNewBuffer(char** pXmlBuf, DWORD &dwXmlLen, const CXmlBase &struXml);

BOOL GenerateGUID(char* pBuffer, const DWORD dwSize);
BOOL ConvertSingleNodeStringData(void *pStructVale, CXmlBase &struXml, const char* pNodeName, BYTE byDataType);



#endif //_XML_PARAMS_CONVERT_H_