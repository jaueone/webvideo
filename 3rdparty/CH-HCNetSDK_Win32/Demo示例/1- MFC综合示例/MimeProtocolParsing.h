
#include "stdafx.h"
#include "string.h"
using namespace std;
typedef unsigned long           DWORD;
#define TRUE  1
#define FALSE 0
#define  BOOL  int

#define UTILS_SPACE               " "         //�ո���ʵ�������������Է���һ�����Ƕ���һ����
#define UTILS_CRLF                "\r\n"      //�س����У�HTTP�ﾭ�����ֻس����У����������
#define UTILS_LINE                "\n"        //���з�
#define DOUBLE_UTILS_CRLF         "\r\n\r\n"  //˫�س����У�httpͷ������־

char *get_boundary(char *mfd);
char* mutipart_form_data(char *mfd, char *boundary, char **type, char **content, int mfdSize, int& contentLen, int& offsetLen);