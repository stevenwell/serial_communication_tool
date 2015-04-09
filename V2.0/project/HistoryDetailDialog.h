#if !defined(AFX_HISTORYDETAILDIALOG_H__878EC2C5_E200_410F_B369_1EA0D8C5379B__INCLUDED_)
#define AFX_HISTORYDETAILDIALOG_H__878EC2C5_E200_410F_B369_1EA0D8C5379B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistoryDetailDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistoryDetailDialog dialog

class CHistoryDetailDialog : public CDialog
{
// Construction
public:
	CHistoryDetailDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHistoryDetailDialog)
	enum { IDD = IDD_HISTORY_DETAIL };
	int		m_iHistoryNum;
	CString	m_strMsgContent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoryDetailDialog)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistoryDetailDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYDETAILDIALOG_H__878EC2C5_E200_410F_B369_1EA0D8C5379B__INCLUDED_)
