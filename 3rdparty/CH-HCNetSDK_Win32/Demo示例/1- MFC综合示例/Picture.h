// CPicture.h: interface for the CPicture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTURE_H__081D008A_5029_4C56_B80C_37793E323DF6__INCLUDED_)
#define AFX_PICTURE_H__081D008A_5029_4C56_B80C_37793E323DF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <afxctl.h>
//------------------------------------------

typedef enum
{
	OFM_WRITE        = 0,
	OFM_READ         = 1
}FILE_OPENMODE;

#define HIMETRIC_INCH 2540

//=-----------------------------
class CFileProcess
{
private :
	FILE  *m_File;

public:
	CFileProcess();
   ~CFileProcess();
    BOOL	Open        (char * FileName , FILE_OPENMODE Open_Mode);
	BOOL	Write       (VOID * zBuffer, DWORD cSize);
    BOOL	Read        (VOID * zBuffer, DWORD cSize);
	LONG    GetSize     (VOID);
	VOID    Close       (VOID);
};

//-------------------------------------------------------------


//-----------------------------------------
class CPicture  
{
public:
	CPicture();
	virtual ~CPicture();
	
private:
	IPicture *m_pic;
	HGLOBAL  hGlobal;
	LPPICTURE m_pPict;

public:

	HBITMAP LoadPicture(char * FileName);
	HBITMAP LoadPicture(char * FileName, HDC hdc);
	HBITMAP LoadPicture(char *FileName, HDC hdc, long cx, long cy);
	VOID    FreePicture( VOID );
    HDC     _GetDC     ( VOID );
	HBITMAP _GetHandle ( VOID );
	DWORD   _GetWidth  ( VOID );
	DWORD   _GetHeight ( VOID );
	BOOL    DrawPicture(HDC hdc,long x,long y,long cx,long cy);
	void Render(CDC* pDC, LPRECT pDrawRect/*Ŀ����Σ���λ���߼����굥λ*/, LPRECT 
		
		
		
		pSrcRect=NULL/*��Դ���Σ���λ��0.01����,���Ϊ�գ�����������ͼ��Ŀ�����*/,LPCRECT prcWBounds=NULL/*ͼԪ�ļ�ר�ã��󶨾���*/);//�ڸ�����DC�ϻ�ͼ��


};

class CPicture_Ex:public CPictureHolder  
{
public:
	BOOL Load(CString sFilePathName);//���ļ���ȡͼ��
	BOOL Load(HINSTANCE hInstance,LPCTSTR lpszResourceName, LPCSTR ResourceType);//����Դ��ȡͼ��
	BOOL LoadPictureData(BYTE* pBuffer, int nSize);//���ڴ��ȡͼ��
	BOOL SaveAsBitmap(CString sFilePathName);//д�뵽BMP�ļ�
	void Render(CDC* pDC, LPRECT pDrawRect/*Ŀ����Σ���λ���߼����굥λ*/, LPRECT 
		
		
		
		pSrcRect=NULL/*��Դ���Σ���λ��0.01����,���Ϊ�գ�����������ͼ��Ŀ�����*/,LPCRECT prcWBounds=NULL/*ͼԪ�ļ�ר�ã��󶨾���*/);//�ڸ�����DC�ϻ�ͼ��
	CPicture_Ex();
	virtual ~CPicture_Ex();
	void  UnloadPicture();//�ͷ�ͼ������ͬCPictureHolder::~CPictureHolder()
public:
	LONG      get_Height(); // ��0.01����Ϊ��λ��ͼ��߶�
	LONG      get_Width();  // ��0.01����Ϊ��λ��ͼ����
};

#endif // !defined(AFX_PICTURE_H__081D008A_5029_4C56_B80C_37793E323DF6__INCLUDED_)
