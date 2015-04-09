// SerialCommDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialComm.h"
#include "SerialCommDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialCommDlg dialog

CSerialCommDlg::CSerialCommDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialCommDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerialCommDlg)
	m_strSend = _T("");
	m_strRecv = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialCommDlg)
	DDX_Control(pDX, IDC_MSCOMM1, m_comm);
	DDX_Text(pDX, IDC_EDIT_SEND, m_strSend);
	DDV_MaxChars(pDX, m_strSend, 512);
	DDX_Text(pDX, IDC_EDIT_RECV, m_strRecv);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSerialCommDlg, CDialog)
	//{{AFX_MSG_MAP(CSerialCommDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_OPENPORT, OnBtOpenport)
	ON_BN_CLICKED(IDC_BTSEND, OnBtSend)
	ON_BN_CLICKED(IDC_BUTTON3, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialCommDlg message handlers

BOOL CSerialCommDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	((CComboBox *)GetDlgItem(IDC_COMBO_COM))->SetCurSel(0);
	((CComboBox *)GetDlgItem(IDC_COMBO_BRAND))->SetCurSel(0);
	((CComboBox *)GetDlgItem(IDC_COMBO_DATABIT))->SetCurSel(0);
	((CComboBox *)GetDlgItem(IDC_COMBO_ENDBIT))->SetCurSel(0);
	GetDlgItem(IDC_BTSEND)->EnableWindow(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialCommDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSerialCommDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSerialCommDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSerialCommDlg::OnBtOpenport() 
{
	if (m_comm.GetPortOpen())
	{
		m_comm.SetPortOpen(FALSE);
		GetDlgItem(IDC_BT_OPENPORT)->SetWindowText("打开串口");
		GetDlgItem(IDC_BTSEND)->EnableWindow(FALSE);
		return;
	}
	
	short iPort = ((CComboBox *)GetDlgItem(IDC_COMBO_COM))->GetCurSel() + 1;
	CString strBrand, strDataBit, strEndBit, strSetting;
	((CComboBox *)GetDlgItem(IDC_COMBO_BRAND))->GetWindowText(strBrand);
	((CComboBox *)GetDlgItem(IDC_COMBO_DATABIT))->GetWindowText(strDataBit);
	((CComboBox *)GetDlgItem(IDC_COMBO_ENDBIT))->GetWindowText(strEndBit);
	
	strSetting.Format("%s, N, %s, %s", strBrand, strDataBit, strEndBit);

	m_comm.SetCommPort(iPort);
	m_comm.SetInBufferSize(1024); //接收缓冲区
	m_comm.SetOutBufferSize(1024); //发送缓冲区
	m_comm.SetInputMode(1);     //二进制数据传输数据
	m_comm.SetRThreshold(1); //每接收517个字符就触发一次事件
	m_comm.SetSThreshold(0);   //不触发发送事件
	m_comm.SetInputLen(0);    //每次读操作从缓冲区读517字符
	m_comm.SetSettings(strSetting);

	m_comm.SetPortOpen(TRUE);
	GetDlgItem(IDC_BT_OPENPORT)->SetWindowText("关闭串口");
	GetDlgItem(IDC_BTSEND)->EnableWindow(TRUE);

	
// 	for(int j=0; j<1; j++)
// 	{
// 		m_bytSendArr.RemoveAll();
// 		unsigned char szSendBuf[517] = {0};
// 		
// 		/*HEAD AND TAIL*/
// 		unsigned char ucHead = 0xaf;
// 		unsigned char ucTail = 0xfa;
// 		unsigned char ucVerifyValue = 0x01;
// 		short iCount = 4;
// 		unsigned char szContent[512] = {0};
// 		
// 		CString t("我们");
// 		
// 		memcpy(szContent, t.GetBuffer(NULL),t.GetLength());
// 		
// 		unsigned char *pBuf = szSendBuf;
// 		*(unsigned char *)pBuf = ucHead; //1
// 		
// 		pBuf++;
// 		*(unsigned char *)pBuf = ucVerifyValue; //2
// 		
// 		pBuf++;
// 		*(short *)pBuf = iCount; //3 4
// 		
// 		pBuf+=2;
// 		memcpy(pBuf, szContent, 512); //4+1~4+512
// 		
// 		pBuf+=512;
// 		*(unsigned char *)pBuf = ucTail; //517
// 		
// 		//SEND
// 		CByteArray bytOutArr;
// 		bytOutArr.SetSize(517);
// 		for (int i=0; i<517; i++)
// 		{
// 			bytOutArr[i] = (BYTE)szSendBuf[i];
// 		}
// 		
// 		m_bytSendArr.Copy(bytOutArr);
// 		m_comm.SetOutput(COleVariant(m_bytSendArr));
// 	}
	
}

BEGIN_EVENTSINK_MAP(CSerialCommDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSerialCommDlg)
	ON_EVENT(CSerialCommDlg, IDC_MSCOMM1, 1 /* OnComm */, OnCommMscomm1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSerialCommDlg::OnCommMscomm1() 
{
	BYTE bytRecv[517] ={0};
	VARIANT variant_inp;
	long rec_data_len;
	switch(m_comm.GetCommEvent())
	{
	case 2:
		{
			Sleep(1000); 
			variant_inp = m_comm.GetInput();
			rec_data_len = variant_inp.parray->rgsabound->cElements;
			if (rec_data_len > 0)	
			{
				memset(bytRecv, 0, sizeof(bytRecv));
				memcpy(bytRecv, (char *)variant_inp.parray->pvData, rec_data_len);
			}
			
			//消息解析与验证
			
			//显示消息
			CHAR * pChar = (char*)bytRecv+5;
			memcpy(m_strRecv.GetBuffer(strlen(pChar)+1), pChar, strlen(pChar)+1);
			UpdateData(FALSE);
// 			int iSendLen,iRecvLen;			
// 			iSendLen = m_bytSendArr.GetSize();
// 			iRecvLen = m_bytRecvArr.GetSize();
// 			if (iSendLen != iRecvLen)
// 			{
// 				MessageBox("接收和发送字节数不同！");
// 				break;
// 			}
			
// 			BOOL blTest = TRUE;
// 			for (i=0; i<iSendLen; i++)
// 			{
// 				if (m_bytRecvArr.GetAt(i) != m_bytSendArr.GetAt(i))
// 				{
// 					blTest = FALSE;
// 					break;
// 				}	
// 			}
// 			
// 			if (blTest)
// 			{
// 				MessageBox("接收与发送内容相同！");
// 			}
// 			else
// 			{
// 				MessageBox("接收与发送内容不同！");
// 			}
			break;
		}
		
	default:
		break;
	}
}

void CSerialCommDlg::OnBtSend() 
{
	UpdateData(TRUE);
	if (m_strSend.IsEmpty())
	{
		MessageBox("发送消息不能为空！");
		return;
	}
	
	m_bytSendArr.RemoveAll();
	unsigned char szSendBuf[517] = {0};
	
	/*HEAD AND TAIL*/
	unsigned char ucHead = 0xaf;
	unsigned char ucTail = 0xfa;
	unsigned char ucVerifyValue = 0x01;
	short iCount = 4;
	unsigned char szContent[512] = {0};
	
	memcpy(szContent, m_strSend.GetBuffer(NULL),m_strSend.GetLength());
	
	unsigned char *pBuf = szSendBuf;
	*(unsigned char *)pBuf = ucHead; //1
	
	pBuf++;
	*(unsigned char *)pBuf = ucVerifyValue; //2
	
	pBuf++;
	*(short *)pBuf = iCount; //3 4
	
	pBuf+=2;
	memcpy(pBuf, szContent, 512); //4+1~4+512
	
	pBuf+=512;
	*(unsigned char *)pBuf = ucTail; //517
	
	//SEND
	CByteArray bytOutArr;
	bytOutArr.SetSize(517);
	for (int i=0; i<517; i++)
	{
		bytOutArr[i] = (BYTE)szSendBuf[i];
	}
	
	m_bytSendArr.Copy(bytOutArr);
	m_comm.SetOutput(COleVariant(m_bytSendArr));
}

void CSerialCommDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	CDialog::EndDialog(IDCANCEL);	
}
