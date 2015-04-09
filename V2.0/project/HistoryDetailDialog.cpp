// HistoryDetailDialog.cpp : implementation file
//

#include "stdafx.h"
#include "serialcomm.h"
#include "HistoryDetailDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistoryDetailDialog dialog


CHistoryDetailDialog::CHistoryDetailDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CHistoryDetailDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistoryDetailDialog)
	m_iHistoryNum = 0;
	m_strMsgContent = _T("");
	//}}AFX_DATA_INIT
}


void CHistoryDetailDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistoryDetailDialog)
	DDX_Text(pDX, IDC_EDIT_NUM, m_iHistoryNum);
	DDX_Text(pDX, IDC_EDIT_HISTORY_DETAIL, m_strMsgContent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistoryDetailDialog, CDialog)
	//{{AFX_MSG_MAP(CHistoryDetailDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistoryDetailDialog message handlers

int CHistoryDetailDialog::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DoModal();
}

void CHistoryDetailDialog::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::CalcWindowRect(lpClientRect, nAdjustType);
}
