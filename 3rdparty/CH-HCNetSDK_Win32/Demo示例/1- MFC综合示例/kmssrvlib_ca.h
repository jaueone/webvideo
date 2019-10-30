#ifndef __CASRV_LIB__
#define __CASRV_LIB__

#ifdef   __cplusplus
extern "C"{
#endif


	typedef struct{
		char CommonName[64];
		char Province[32];
		char City[32];
		char Company[32];
		char TelNum[12];
		char Email[32];
		int  ValidYears;
	}SWCA_CERT_REQ;

	/*RSA��Կ�ṹ����*/
#define LiteRSAref_MAX_BITS    2048
#define LiteRSAref_MAX_LEN     ((LiteRSAref_MAX_BITS + 7) / 8)
#define LiteRSAref_MAX_PBITS   ((LiteRSAref_MAX_BITS + 1) / 2)
#define LiteRSAref_MAX_PLEN    ((LiteRSAref_MAX_PBITS + 7)/ 8)

	typedef struct RSArefPublicKeyLite_st
	{
		unsigned int  bits;
		unsigned char m[LiteRSAref_MAX_LEN];
		unsigned char e[LiteRSAref_MAX_LEN];
	}RSArefPublicKeyLite;

	/*ECC��Կ�ṹ����*/
#define ECCref_MAX_BITS			512 
#define ECCref_MAX_LEN			((ECCref_MAX_BITS+7) / 8)
#define ECCref_MAX_CIPHER_LEN	136

	typedef struct ECCrefPublicKey_st
	{
		unsigned int  bits;
		unsigned char x[ECCref_MAX_LEN];
		unsigned char y[ECCref_MAX_LEN];
	} ECCrefPublicKey;


	/*----------------------------------------------------------
	���ܣ���ʼ������ȡ�����ļ�
	������
	IP_CA		����		CA����IP��ַ
	Port_CA		����		CA����˿�

	����ֵ��0���ɹ�  ������ʧ�ܣ������ѯ�����룩
	----------------------------------------------------------*/
	int CA_Init(const char *IP_CA, int Port_CA);

	/*----------------------------------------------------------
	���ܣ�P10��ʽ��������֤��
	������
	iValidYear	����		֤����Ч��
	uiP10Len    ����        P10����
	ucP10		����		P10����
	uiCertLen   ����/���   ֤��洢����
	ucCert	    ���		֤��
	����ֵ��0���ɹ�  ������ʧ�ܣ������ѯ�����룩
	----------------------------------------------------------*/
	int CA_ApplyCert_P10(int iValidYear, int uiP10Len, unsigned char *ucP10, int *uiCertLen, unsigned char *ucCert);

	/*----------------------------------------------------------
	����:��������֤��
	������
	iReqType			����		�������� 1 ǩ�� 2����
	iAlgFlag			����        �㷨���� 1 RSA 2 SM2
	iModLen				����		ģ�� 1024 2048 256
	iPriType			����		������Կ�Ը�ʽ 1 ASN.1  2 SKF
	ucPubKey			����	    ��Կ ���ܽṹ
	uiPubKeyLen			����	    ��Կ����
	ucCertReqInfo		����	    �û���Ϣ
	uiCertReqInfoLen	����	    �û���Ϣ����
	ucUserCert			���        ֤��
	uiUserCertLen		����/���	֤��洢����
	ucUserKey           ���        �û�������Կ�ԣ����ģ�
	uiUserKeyLen        ����/���   �û�������Կ�Գ���
	����ֵ��0���ɹ�  ������ʧ�ܣ������ѯ�����룩
	----------------------------------------------------------*/
	int CA_GenUserCert(int iReqType, int iAlgFlag, int iModLen, int iPriType,
		unsigned char *ucPubKey, unsigned int uiPubKeyLen,
		unsigned char *ucCertReqInfo, unsigned int uiCertReqInfoLen,
		unsigned char *ucUserCert, unsigned int *uiUserCertLen,
		unsigned char *ucUserKey, unsigned int *uiUserKeyLen);

	/*----------------------------------------------------------
	����:��ȡCA��֤��
	������
	ucRootCert			���        ֤��
	uiRootCertLen		����/���	֤��洢����
	����ֵ��0���ɹ�  ������ʧ�ܣ������ѯ�����룩
	----------------------------------------------------------*/
	int CA_GetRootCert(unsigned char *ucRootCert, unsigned int *uiRootCertLen);

	/*----------------------------------------------------------
	����:��ȡKMS��֤��
	������
	ucKMSCert			���        ֤��
	uiKMSCertLen		����/���	֤��洢����
	����ֵ��0���ɹ�  ������ʧ�ܣ������ѯ�����룩
	----------------------------------------------------------*/
	int CA_GetKMSCert(unsigned char *ucKMSCert, unsigned int *uiKMSCertLen);

//�����붨��
#define KMSSRV_ERR_SRV_RETURN_BASE				0x20000000    /*�ܹܷ��񷵻ش��� ƫ��*/

#define ERR_SUCCESS						0x00    /*�ɹ�*/
#define ERR_PACKAGE						0x01    /*���ĸ�ʽ����*/
#define ERR_P10							0x02    /*p10��ʽ����*/
#define ERR_PUBKey					    0x03    /*��Կ��ʽ����*/
#define ERR_USERINFO					0x04    /*�û���Ϣ��ʽ����*/
#define ERR_GENKEY						0x05    /*���ɷǶԳ���Կ�Դ���*/

#define ERR_GENCERT						0x06    /*ǩ��֤�����*/
#define ERR_APPLYTYPE					0x07    /*�������Ͳ�֧�ִ���*/
#define ERR_ALGORITHM					0x08    /*��֧�ֵ��㷨����*/
#define ERR_LICENSE                     0x09    /*�������Ȩ����*/
#define ERR_RANDOM                      0x0A    /*��Ӧ���������Ϣ��ƥ��*/
#define ERR_CERTLEN                     0x0B    /*֤�鳤�ȴ���*/
#define ERR_CN_NULL                     0x0C    /*CNΪ��*/
#define ERR_USERINFO_ITEM_TOO_LONG      0x0D    /*�û���Ϣ����̫��*/
#define ERR_CMD                         0X0E    /*����˷��������벻��ȷ*/
#define ERR_NO_USER_KEY                 0x0F    /*�����û�з��ؼ���֤����Կ��*/
#define ERR_PRIVATE_TYPE                0x10    /*���󷵻�˽Կ��ʽ����*/
#define ERR_VALID_YEAR                  0x11    /*����֤����Ч�ڴ���1-19*/
#define ERR_UNKNOW						0x70    /*δ֪����*/


#define KMSSRV_ERR_NOINIT						0x30	/*�ӿ�δ��ʼ��*/
#define KMSSRV_ERR_TCP_CONNECT					0x31    /*�����ܹ�ʧ��*/
#define KMSSRV_ERR_TCP_SEND						0x32    /*�������ݴ���*/
#define KMSSRV_ERR_TCP_RECEIVE					0x33    /*�������ݴ���*/
#define KMSSRV_ERR_TCP_DATALEN					0x34    /*���ܱ������ݳ��ȴ���*/
#define KMSSRV_ERR_TCP_CERTBUFF					0x35    /*֤�黺�������ȴ���*/
#define KMSSRV_ERR_TCP_KEYBUFF					0x36    /*��Կ���������ȴ���*/
#define KMSSRV_ERR_TCP_UNKNOW					0x37    /*δ֪����*/

	/*��������������� ���뿨ͷ�ļ�����*/

#ifdef   __cplusplus
}
#endif
#endif