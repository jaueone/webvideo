
#ifndef _DEVICECFGFILE_H_
#define _DEVICECFGFILE_H_

#include "cjson/cJSON.h"
#include "GeneralDef.h"

/**   @class DeviceCfgFile
 *    @brief �豸�����ļ���,����Ϊ������
 */
class DeviceCfgFile
{
public:
    static DeviceCfgFile& GetInstance();
    void SaveToFile();
    void ReadFromFile();
private:
    DeviceCfgFile();
    ~DeviceCfgFile();
    DeviceCfgFile(const DeviceCfgFile&);
    DeviceCfgFile& operator=(const DeviceCfgFile&);
    
    static cJSON* WriteDeviceToNode(const STRU_DEVICE_INFO& device, int nIndex);
    static BOOL ReadDeviceFromNode(const cJSON* pNode, PLOCAL_DEVICE_INFO pDevice, int* pIndex);

    const static char* c_strNode_Devices;               //���ڵ���

    const static char* c_strNode_DeviceID;              //�豸ID�ڵ�
    const static char* c_strNode_SerialNo;              //�豸serial number�ڵ�
    const static char* c_strNode_ChanNum;               //�豸channel number�ڵ�
    const static char* c_strNode_StartChan;             //�豸start channel number�ڵ�
    const static char* c_strNode_AlarmInNum;            //�豸alarm in number�ڵ�
    const static char* c_strNode_AlarmOutNum;           //�豸alarm out number�ڵ�
    const static char* c_strNode_IPChanNum;             //�豸ip channel number�ڵ�
    const static char* c_strNode_MirrorChanNum;         //�豸����ͨ�������ڵ�
    const static char* c_strNode_StartMirrorChanNo;     //�豸��ʼ����ͨ���Žڵ�
    const static char* c_strNode_LocalNodeName;         //�豸local device node�ڵ�
    const static char* c_strNode_DeviceType;            //�豸device type�ڵ�
    const static char* c_strNode_DiskNum;               //�豸HD number�ڵ�
    const static char* c_strNode_LoginUserName;         //�豸login user name�ڵ�
    const static char* c_strNode_LoginUserPwd;          //�豸login user password�ڵ�
    const static char* c_strNode_DeviceIP;              //�豸device ip�ڵ�
    const static char* c_strNode_DeviceMultiIP;         //�豸multi-cast group address�ڵ�
    const static char* c_strNode_DevicePort;            //�豸port number�ڵ�
    const static char* c_strNode_DeviceName;            //�豸device name�ڵ�
    const static char* c_strNode_Chans;                 //�豸channels�ڵ�
    const static char* c_strNode_LoginMode;             //�豸LoginMode�ڵ�
    const static char* c_strNode_Https;                 //�豸Https�ڵ�

    const static char* c_strNode_ChanID;                //�豸channel id�ڵ�
    const static char* c_strNode_DeviceIndex;           //�豸device name�ڵ�
    const static char* c_strNode_ChanIndex;             //�豸channel index�ڵ�
    const static char* c_strNode_ChanName;              //�豸channel name�ڵ�
    const static char* c_strNode_Protocol;              //�豸network protocol�ڵ�
    const static char* c_strNode_PicResolution;         //�豸pic resolution�ڵ�
    const static char* c_strNode_PicQuality;            //�豸pic quality�ڵ�
    const static char* c_strNode_Enable;                //�豸channel enable�ڵ�
    const static char* c_strPreviewMode;                //�豸priview mode�ڵ�
    const static char* c_strPreviewProtocolType;        //�豸priview protocol type�ڵ�
    const static char* c_strVideoCodingType;            //�豸video coding type�ڵ�
    const static char* c_strStreamType;                 //�豸�������ͽڵ�
    const static char* c_strLinkMode;                   //�豸�������ӷ�ʽ�ڵ�
};

BOOL GetNodeVal_JSON(const cJSON* pFather, const char* strName, int* nVal);
BOOL GetNodeVal_JSON(const cJSON* pFather, const char* strName, char* strVal, int nLen);
#endif
