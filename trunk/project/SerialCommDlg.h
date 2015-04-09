// SerialCommDlg.h : header file
//
//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES

#if !defined(AFX_SERIALCOMMDLG_H__37996DD8_CC5E_464D_ABF3_FD32D3B8D6B1__INCLUDED_)
#define AFX_SERIALCOMMDLG_H__37996DD8_CC5E_464D_ABF3_FD32D3B8D6B1__INCLUDED_

#include "mscomm.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSerialCommDlg dialog

class CSerialCommDlg : public CDialog
{
// Construction
public:
	CByteArray m_bytRecvArr;
	CSerialCommDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSerialCommDlg)
	enum { IDD = IDD_SERIALCOMM_DIALOG };
	CMSComm	m_comm;
	CString	m_strSend;
	CString	m_strRecv;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialCommDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CByteArray m_bytSendArr;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSerialCommDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtOpenport();
	afx_msg void OnCommMscomm1();
	afx_msg void OnBtSend();
	afx_msg void OnClose();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALCOMMDLG_H__37996DD8_CC5E_464D_ABF3_FD32D3B8D6B1__INCLUDED_)
 
