#if !defined(AFX_COLORBUTTON_H__47E63433_C638_4E2C_90F6_3F860699B8E8__INCLUDED_)
#define AFX_COLORBUTTON_H__47E63433_C638_4E2C_90F6_3F860699B8E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorButton.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CPTZButton window

class CPTZButton : public CButton
{
// Construction
public:
	CPTZButton();

// Attributes
public:
	int m_iSubBtnIndex;
	LONG m_lPlayHandle;//Player Handle
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTZButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPTZButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPTZButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBUTTON_H__47E63433_C638_4E2C_90F6_3F860699B8E8__INCLUDED_)
