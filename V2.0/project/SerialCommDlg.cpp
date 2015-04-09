// SerialCommDlg.cpp : implementation file
//

#include "stdafx.h"
#include <fstream.h>
#include "SerialComm.h"
#include "SerialCommDlg.h"
#include "MsgDefine.h"

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
	m_strSendLogFile = _T("SendedMessage.log");
	m_strRecvLogFile = _T("RecvedMessage.log");
	m_strSendingContent = _T("");
	m_strRecvContent = _T("");
	m_ulStartRevcerId = 1;
	m_ulEndRecverId = 1;
	m_enCurrentChannel = CHANNEL_NUM_ONE;
	m_SendBuffer = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialCommDlg)
	DDX_Control(pDX, IDC_LIST_HISTORY_1, m_listHistorySendedMsg);
	DDX_Control(pDX, IDC_MSCOMM_COM1, m_Mscomm_com1);
	DDX_Control(pDX, IDC_MSCOMM_COM2, m_Mscomm_com2);
	DDX_Text(pDX, IDC_EDIT_RECV_1, m_strRecvContent);
	DDX_Text(pDX, IDC_EDIT_SEND_1, m_SendBuffer);
	DDV_MaxChars(pDX, m_SendBuffer, 512);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSerialCommDlg, CDialog)
	//{{AFX_MSG_MAP(CSerialCommDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_COM1_OPEN, OnBtCom1Open)
	ON_BN_CLICKED(IDC_BT_COM2_OPEN, OnBtCom2Open)
	ON_BN_CLICKED(IDC_BT_CONNECT, OnBtConnect)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BT_LOCK, OnBtLock)
	ON_BN_CLICKED(IDC_BT_CLEARDATA, OnBtClearData)
	ON_BN_CLICKED(IDC_BT_RESEND_1, OnBtResend1)
	ON_BN_CLICKED(IDC_BT_SEND_1, OnBtSend1)
	ON_BN_CLICKED(IDC_BT_CLEAR_HISTORY_1, OnBtClearHistory1)
	ON_BN_CLICKED(IDC_BT_CHANNEL_1, OnBtSelectChannel1)
	ON_BN_CLICKED(IDC_BT_CHANNEL_2, OnBtSelectChannel2)
	ON_BN_CLICKED(IDC_BT_CHANNEL_3, OnBtSelectChannel3)
	ON_CBN_SELCHANGE(IDC_COMBO_SENDMSG_SENDCHANNEL_1, OnSelchangeSenderChannel1)
	ON_CBN_SELCHANGE(IDC_COMBO_SENDMSG_SENDCHANNEL_2, OnSelchangeSenderChannel2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_HISTORY_1, OnDblClkListHistory1)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
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
	//设置窗口启动后，默认COM口选择
	((CComboBox *)GetDlgItem(IDC_COMBO_COM1))->SetCurSel(0);
	((CComboBox *)GetDlgItem(IDC_COMBO_COM2))->SetCurSel(0);

	((CComboBox *)GetDlgItem(IDC_COMBO_SET_RECVCHANNEL_1))->SetCurSel(0);
	((CComboBox *)GetDlgItem(IDC_COMBO_SET_RECVCHANNEL_2))->SetCurSel(0);

	((CComboBox *)GetDlgItem(IDC_COMBO_SET_SENDCHANNEL_1))->SetCurSel(0);
	((CComboBox *)GetDlgItem(IDC_COMBO_SET_SENDCHANNEL_2))->SetCurSel(0);

	((CComboBox *)GetDlgItem(IDC_COMBO_SENDMSG_SENDCHANNEL_1))->SetCurSel(0);
	((CComboBox *)GetDlgItem(IDC_COMBO_SENDMSG_SENDCHANNEL_2))->SetCurSel(0);

	//通道设置窗口
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE); //默认通道1选上
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
	
	m_listHistorySendedMsg.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_TWOCLICKACTIVATE);
    m_listHistorySendedMsg.InsertColumn(0,"序号",LVCFMT_LEFT,50);
    m_listHistorySendedMsg.InsertColumn(1,"已发消息",LVCFMT_LEFT,2000);
	
	//初始化发送数据区域频道
	for (int i=0; i<CHANNEL_NUM_BUTT; i++)
	{
		m_aulSenderChannelCom1[i] = 1;
		m_aulSenderChannelCom2[i] = 11;
	}

	//发送控制块消息初始化
	InitSendingCtrlBlock(&m_stCom1SendingBlock);
	InitSendingCtrlBlock(&m_stCom2SendingBlock);

	//启动1s定时器，自动扫描串口并打开
	StartTimer(TIMER_ID_SCAN_COMM);	
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

BEGIN_EVENTSINK_MAP(CSerialCommDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSerialCommDlg)
	ON_EVENT(CSerialCommDlg, IDC_MSCOMM_COM1, 1 /* OnComm */, OnCommMscommCom1, VTS_NONE)
	ON_EVENT(CSerialCommDlg, IDC_MSCOMM_COM2, 1 /* OnComm */, OnCommMscommCom2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSerialCommDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	CDialog::EndDialog(IDCANCEL);	
}

void CSerialCommDlg::WriteToLogFile(CString strTmp, CString strFileName)
{
	COleDateTime objTime;
	CString strTime;
	objTime=COleDateTime::GetCurrentTime();
	strTime = objTime.Format("\n%Y-%m-%d %H:%M:%S ");

	fstream outFile;
	
	outFile.open(strFileName, ios::ate | ios::app | ios::out);
	outFile.write(strTime, strTime.GetLength());
	outFile.write(strTmp, strTmp.GetLength());
	outFile.close();
}

void CSerialCommDlg::AppendToSendLogFile(CString strTmp)
{
	WriteToLogFile(strTmp, m_strSendLogFile);
}

void CSerialCommDlg::AppendToRecvLogFile(CString strTmp)
{
	WriteToLogFile(strTmp, m_strRecvLogFile);
}

void CSerialCommDlg::OnBtCom1Open() 
{
	//若已经打开，则关闭，并设置显示为打开
	if (m_Mscomm_com1.GetPortOpen())
	{
		CloseComPort(m_Mscomm_com1);
		GetDlgItem(IDC_BT_COM1_OPEN)->SetWindowText("打开");
		return;
	}

	//打开串口
	short uPort = ((CComboBox *)GetDlgItem(IDC_COMBO_COM1))->GetCurSel() + 1;
	BOOL bRet = OpenComPort(m_Mscomm_com1, uPort);
	if(!bRet)
	{
		MessageBox("打开端口1失败！");
		return;
	}

	//设置显示为关闭
	GetDlgItem(IDC_BT_COM1_OPEN)->SetWindowText("关闭");
}

void CSerialCommDlg::OnBtCom2Open() 
{

	if (m_Mscomm_com2.GetPortOpen())
	{
		CloseComPort(m_Mscomm_com2);
		GetDlgItem(IDC_BT_COM2_OPEN)->SetWindowText("打开");
		return;
	}

	short uPort = ((CComboBox *)GetDlgItem(IDC_COMBO_COM2))->GetCurSel() + 1;
	BOOL bRet = OpenComPort(m_Mscomm_com2, uPort);
	if(!bRet)
	{
		MessageBox("打开端口2失败！");
		return;
	}

	GetDlgItem(IDC_BT_COM2_OPEN)->SetWindowText("关闭");
}



void CSerialCommDlg::OnCommMscommCom1() 
{
	// TODO: Add your control notification handler code here
	short uEvtId = m_Mscomm_com1.GetCommEvent();
	switch(uEvtId)
	{
	case 2:
		{
			AppendToRecvLogFile("OnCommMscommCom1");
			//暂时先取消等待，尝试解决下位机2次发送数据，上层接收数据认为一次处理
			Sleep(600); 
			ProcessRecvedMsg(m_Mscomm_com1, &m_stCom1SendingBlock);
		}
		break;
	//1011	检索端口 设备控制块 (DCB) 时的意外错误
#if 0
	case 1011:
		{
			AppendToSendLogFile("To Close Com!");
			//若已经打开，则关闭串口
			if (m_Mscomm_com1.GetPortOpen())
			{
				CloseComPort(m_Mscomm_com1);
			}
		}
		break;
#endif
	default:
		break;
	}
}

void CSerialCommDlg::OnCommMscommCom2() 
{
	// TODO: Add your control notification handler code here
	switch(m_Mscomm_com2.GetCommEvent())
	{
	case 2:
		{
			AppendToRecvLogFile("OnCommMscommCom2");
			Sleep(500); 
			ProcessRecvedMsg(m_Mscomm_com2, &m_stCom2SendingBlock);
		}
		
	default:
		break;
	}
}

BOOL CSerialCommDlg::OpenComPort(CMSComm &objComm, short uPort)
{
	CString strSetting;
	strSetting.Format("9600, N, 8, 1");
	
	//串口已经打开,直接返回成功
	if (objComm.GetPortOpen())
	{
		return TRUE;
	}
	//记录日志
	CString strLog;
	strLog.Format("Port=%d,", uPort);
	strLog+= strSetting;
	AppendToSendLogFile(strLog);


	//打开串口参数
	objComm.SetCommPort(uPort);
	objComm.SetInBufferSize(1024); //接收缓冲区
	objComm.SetOutBufferSize(1024); //发送缓冲区
	objComm.SetInputMode(1);     //二进制数据传输数据
	objComm.SetRThreshold(1); //每接收517个字符就触发一次事件
	objComm.SetSThreshold(0);   //不触发发送事件
	objComm.SetInputLen(0);    //设置当前接收区数据为0，表示全部读取
	objComm.SetSettings(strSetting);

	//打开串口操作
	try
	{
		objComm.SetPortOpen(TRUE);
	}
	catch (...)
	{
		AppendToSendLogFile("SetPortOpen called failed.");
		return FALSE;
	}

	return TRUE;
}

BOOL CSerialCommDlg::CloseComPort(CMSComm &objComm)
{
	//串口已经关闭,直接返回成功
	if (!objComm.GetPortOpen())
	{
		return TRUE;
	}

	try
	{
		objComm.SetPortOpen(FALSE);
	}
	catch(...)
	{
		AppendToSendLogFile("SetPortOpen called failed.");
		return FALSE;
	}
	
	return TRUE;
}

void CSerialCommDlg::OnBtConnect() 
{
	unsigned short usCom1Channel = 0;
	unsigned short usCom2Channel = 0;
	TWO_COMMAND_CONTENT stTwoCmdContent;
	CONNECT_CMD_CONTENT stConnectCmdInfo1;
	CONNECT_CMD_CONTENT stConnectCmdInfo2;

	//获取数据
	usCom1Channel = ((CComboBox *)GetDlgItem(IDC_COMBO_SET_RECVCHANNEL_1))->GetCurSel() + 1;
	usCom2Channel = ((CComboBox *)GetDlgItem(IDC_COMBO_SET_RECVCHANNEL_2))->GetCurSel() + 1;
	
	stConnectCmdInfo1.usChannel = usCom1Channel;
	stConnectCmdInfo2.usChannel = usCom1Channel; //通道2界面显示取消，频道2使用频道1的频道 2013-6-15 22:38:43

	//发送命令前做通用检查
	if (!CheckPreSendingConditionIsOk())
	{
		return;
	}
	
	//分别构造需要发送给2个串口消息内容
	stTwoCmdContent.enCmdType = CMD_TYPE_CONNECT;
	stTwoCmdContent.stCom1CmdContent.ulContentLength = sizeof(CONNECT_CMD_CONTENT);
	stTwoCmdContent.stCom1CmdContent.pContent = (UINT8*)(&stConnectCmdInfo1);
	stTwoCmdContent.stCom2CmdContent.ulContentLength = sizeof(CONNECT_CMD_CONTENT);
	stTwoCmdContent.stCom2CmdContent.pContent = (UINT8*)(&stConnectCmdInfo2);

	//调用接口发送数据，该接口内自行判断该发给哪个串口，发送失败则返回FALSE
	if (!SendCommandMassage(&stTwoCmdContent))
	{
		MessageBox("发送连接命令失败！");
		return;
	}
}

BOOL CSerialCommDlg::CheckAllComSendCmdConditonIsOk()
{
	BOOL bCom1OpenStatus, bCom2OpenStatus;
	COM_STATUS_ENUM enCom1Status, enCom2Status;
	
	//获取串口是否已经打开
	bCom1OpenStatus = m_Mscomm_com1.GetPortOpen();
	bCom2OpenStatus = m_Mscomm_com2.GetPortOpen();
	
	//获取串口发送消息的状态
	enCom1Status = GetComPortSendingMsgStatus(COM1_ENUM);
	enCom2Status = GetComPortSendingMsgStatus(COM2_ENUM);
	
	if ( (bCom1OpenStatus && (STATUS_BUSY == enCom1Status)) //COM1打开&& 正在发送数据
		||(bCom2OpenStatus && (STATUS_BUSY == enCom2Status))//com2打开&& 正在发送数据
		||(!bCom1OpenStatus && !bCom2OpenStatus)	//COM1和COM2均关闭
		)
	{
		return FALSE;
	}

	return TRUE;
}

void CSerialCommDlg::InitCommonMsgHeader(COMMON_MSG *pMsg, unsigned int ulCmdType, unsigned int ulContentLength)
{
	pMsg->ucHead = 0xAF;
	pMsg->ucCmdType = (UINT8)ulCmdType;
	pMsg->usLength = (UINT16)ulContentLength;
	pMsg->usReserved = 0xFFFF;
}

BOOL CSerialCommDlg::CheckSingleComSendCmdConditonIsOk(CMSComm &objMscomm, COM_NUMBER_ENUM enComEnum)
{
	BOOL bComOpenStatus;
	COM_STATUS_ENUM enRetStatus;

	bComOpenStatus = objMscomm.GetPortOpen();
	enRetStatus = GetComPortSendingMsgStatus(enComEnum);
	
	//串口打开、且空闲状态
	if (bComOpenStatus && (STATUS_FREE == enRetStatus))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CSerialCommDlg::SendMsgToCom(CMSComm &objComm, COMMON_MSG *pMsg)
{
	//命令字无效，不发送，直接返回失败
	if (CMD_TYPE_BUTT <= pMsg->ucCmdType || NULL == pMsg )
	{
		CString strLog;
		strLog.Format("SendMsgToCom-> CMDTYPE %#x is Invalid", pMsg->ucCmdType);
		AppendToSendLogFile(strLog);
		return FALSE;
	}
	CString strHexSend,strTmp;		/*六进制字符串*/
	UINT16 usMsgLength = pMsg->usLength + COMMON_MSG_HEADER_LENGTH;
	CByteArray objSendArr; 
	unsigned char* pByteMsg = (unsigned char*)pMsg;

	//构造发送数据的byte对象
	objSendArr.SetSize(usMsgLength);
	for (UINT16 i=0; i<usMsgLength; i++)
	{
		objSendArr[i] = *(pByteMsg+i);
		strTmp.Format(" %02X", objSendArr[i]);
		strHexSend += strTmp;
	}

	try
	{
		//清空接收缓冲区
		objComm.SetInBufferCount(0);

		//发送数据前先清空缓冲区，确保缓冲区满
		objComm.SetOutBufferCount(0);
		//发送数据
		objComm.SetOutput(COleVariant(objSendArr));
	}
	catch (...)
	{
		//记录日志
		AppendToSendLogFile("SetOutput called Failed");
		return FALSE;
	}

	//记录日志
	AppendToSendLogFile(strHexSend);//16进制
	return TRUE;
}

void CSerialCommDlg::StartTimer(unsigned int ulTimerID)
{
	/*启动定时器*/
	SetTimer(ulTimerID,1000, NULL);
}
void CSerialCommDlg::CloseTimer(unsigned int ulTimerID)
{
	/*关闭定时器*/
	KillTimer(ulTimerID);
}

COM_STATUS_ENUM CSerialCommDlg::GetComPortSendingMsgStatus(COM_NUMBER_ENUM enComEnum)
{
	COM_STATUS_ENUM enRetStatus = STATUS_INVALID;

	switch (enComEnum)
	{
	case COM1_ENUM:
		enRetStatus = m_stCom1SendingBlock.enComSendingStatus;
		break;
	case COM2_ENUM:
		enRetStatus = m_stCom2SendingBlock.enComSendingStatus;
		break;
	default:
		;
	}

	return enRetStatus;
}

void CSerialCommDlg::InitSendingCtrlBlock(SENDING_CTRL_BLOCK * pCtrlBlock)
{
	//发送控制块消息初始化
	pCtrlBlock->enCmdType = CMD_TYPE_BUTT;
	pCtrlBlock->enComSendingStatus = STATUS_FREE;
	pCtrlBlock->ulCounter = 0;
	pCtrlBlock->enSendResult = SEND_RESULT_BUTT;

	//发送消息缓存区域
	memset(pCtrlBlock->aucComMsgBuffer, 0, 1024);
}

void CSerialCommDlg::SetComPortSendingMsgStatus(COM_NUMBER_ENUM enComEnum, COM_STATUS_ENUM enStatus)
{
	switch (enComEnum)
	{
	case COM1_ENUM:
		m_stCom1SendingBlock.enComSendingStatus = enStatus;
		break;
	case COM2_ENUM:
		m_stCom2SendingBlock.enComSendingStatus = enStatus;
		break;
	default:
		;
	}
}
//1s定时器扫描串口，并打开串口
void CSerialCommDlg::ProcessScanCommTimer(void)
{
	BOOL bRet = FALSE;
	
	//若串口已经打开，不必打开串口
	if (m_Mscomm_com1.GetPortOpen())
	{
		return;
	}

	//遍历所有串口默认遍历1~30个串口
	for(short uPort=1; uPort<=30; uPort++)
	{
		bRet = OpenComPort(m_Mscomm_com1, uPort);
		if(bRet)
		{
			break;
		}
	}

	CString strTmp;

	if(bRet)
	{
		//设置端口和选择
		((CComboBox *)GetDlgItem(IDC_COMBO_COM1))->SetCurSel(uPort-1);
		((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(TRUE);
		
		strTmp.Format("Auto open COM%u Success!", uPort);
		AppendToSendLogFile(strTmp);
	}
	else
	{
		//设置不选中
		((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(FALSE);
		AppendToSendLogFile("No available Com!");
	}
}

void CSerialCommDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	switch(nIDEvent)
	{
	case TIMER_ID_COM1:
		ProcessComTimer(nIDEvent, &m_stCom1SendingBlock, m_Mscomm_com1);
		break;
	case TIMER_ID_COM2:
		ProcessComTimer(nIDEvent, &m_stCom2SendingBlock, m_Mscomm_com2);
		break;
	case TIMER_ID_SCAN_COMM:
		ProcessScanCommTimer();
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CSerialCommDlg::ProcessComTimer(UINT nIDEvent, SENDING_CTRL_BLOCK* pCtrlBlock, CMSComm& objComm)
{
	if (pCtrlBlock->enSendResult == SEND_RESULT_SUCCESS)
	{
		KillTimer(nIDEvent);
		pCtrlBlock->enComSendingStatus = STATUS_FREE;
		pCtrlBlock->ulCounter = 0;

		//发送消息缓存区域
		memset(pCtrlBlock->aucComMsgBuffer, 0, 1024);
		return;
	}

	if (pCtrlBlock->ulCounter < 3)
	{
		pCtrlBlock->ulCounter++;
		SendMsgToCom(objComm, (COMMON_MSG *)(pCtrlBlock->aucComMsgBuffer));
	}
	else
	{
		KillTimer(nIDEvent);
		pCtrlBlock->enComSendingStatus = STATUS_FREE;
		pCtrlBlock->ulCounter = 0;
		memset(pCtrlBlock->aucComMsgBuffer, 0, 1024);
		MessageBox("消息发送失败");
	}
}

void CSerialCommDlg::ProcessRecvedMsg(CMSComm &objComm, SENDING_CTRL_BLOCK *pCtrlBlock)
{
	UINT8 aucRecvBuffer[1024] ={0};
	VARIANT variant_inp;
	long rec_data_len;
	COMMON_MSG * pRecvMsg = NULL;
	void * pMsgContent = NULL;
	
	//读取串口信息
	variant_inp = objComm.GetInput();
	rec_data_len = variant_inp.parray->rgsabound->cElements;
	if (rec_data_len > 0)	
	{
		memset(aucRecvBuffer, 0, sizeof(aucRecvBuffer));
		memcpy(aucRecvBuffer, (char *)variant_inp.parray->pvData, rec_data_len);
	}
	
	//记录日志
	CString strHexRecv,strTmp;
	for (int i=0; i<rec_data_len; i++)
	{
		strTmp.Format(" %02X",aucRecvBuffer[i]);
		strHexRecv+= strTmp;
	}
	
	AppendToRecvLogFile(strHexRecv); //接收16进制内容
	
	//消息解析与验证
	pRecvMsg = (COMMON_MSG *)(void*)aucRecvBuffer;
	pMsgContent = pRecvMsg->aucValue;
	if(pRecvMsg->ucCmdType == CMD_TYPE_ACK)
	{	
		if (((ACK_CMD_CONTENT*)pMsgContent)->usAckCmdType == pCtrlBlock->enCmdType)
		{
			pCtrlBlock->enSendResult = SEND_RESULT_SUCCESS;
			
			switch(pCtrlBlock->enCmdType)
			{
			case CMD_TYPE_CONNECT:
				MessageBox("连接成功！");
				break;
			case CMD_TYPE_SEND:
				{
					int  ulLastPos = m_strArryMsgHistory[m_enCurrentChannel].GetSize()-1;
					//上次历史记录与这次历史记录相同则不添加到历史记录区中
					BOOL bIsLastHistoryMsg = (ulLastPos >= 0) 
						&& (0 == m_strArryMsgHistory[m_enCurrentChannel].GetAt(ulLastPos).Compare(m_strSendingContent));
					if (!bIsLastHistoryMsg)
					{	
						//最近一条记录与当前发送消息相同，则不用添加到历史记录,否则添加历史记录
						AppendToSendLogFile(m_strSendingContent);
						//添加历史记录
						m_strArryMsgHistory[m_enCurrentChannel].Add(m_strSendingContent);
						//刷新历史记录区域
						FreshHistoryMsgSpace();
					}
				}
				break;
			case CMD_TYPE_RESEND:
				break;
			default:
				break;
			}
		}
	}
	else if (pRecvMsg->ucCmdType == CMD_TYPE_RECV)
	{
		CString strTmp;
		unsigned int ulLength = (pRecvMsg->usLength <= 512) ? (pRecvMsg->usLength) : 512;
		char * pTmpRecv = strTmp.GetBuffer(ulLength+1);
		
		memset(pTmpRecv, 0, ulLength+1);
		memcpy(pTmpRecv, pRecvMsg->aucValue, ulLength);
		m_strRecvContent = strTmp;
		
		//接收消息区域
		GetDlgItem(IDC_EDIT_RECV_1)->SetWindowText(strTmp);
		
		//设置发送消息区为空
		GetDlgItem(IDC_EDIT_SEND_1)->SetWindowText("");
	}
}

BOOL CSerialCommDlg::CheckPreSendingConditionIsOk()
{
	//判断是否可以发送命令
	BOOL bCom1OpenStatus = m_Mscomm_com1.GetPortOpen();
	BOOL bCom2OpenStatus = m_Mscomm_com2.GetPortOpen();
	BOOL bRet = FALSE;

	if (!bCom1OpenStatus && !bCom2OpenStatus)
	{
		MessageBox("请先打开端口");
		return FALSE;
	}
	
	bRet = CheckAllComSendCmdConditonIsOk();
	if (!bRet)
	{
		MessageBox("系统正忙，请稍后再发！");
		return FALSE;
	}

	return TRUE;
}

BOOL CSerialCommDlg::SendCommandMassage(TWO_COMMAND_CONTENT *pTwoCmdInfo)
{
	BOOL bRet = FALSE;
	BOOL bCom1ConditionIsok = FALSE;
	BOOL bCom2ConditionIsok = FALSE;
	CMD_TYPE enCmdType;
	COMMAND_CONTENT* pCom1Content;
	COMMAND_CONTENT* pCom2Content;
	COMMON_MSG *pCom1Msg = NULL;
	COMMON_MSG *pCom2Msg = NULL;

	//非空指针，发送命令长度为0
	if (NULL == pTwoCmdInfo 
		|| (pTwoCmdInfo->stCom1CmdContent.ulContentLength == 0 && pTwoCmdInfo->stCom2CmdContent.ulContentLength == 0)
		)
	{
		AppendToSendLogFile("SendCommandMassage->Paramaters invalid!");
		return FALSE;
	}

	enCmdType = pTwoCmdInfo->enCmdType;
	pCom1Content = &(pTwoCmdInfo->stCom1CmdContent);
	pCom2Content = &(pTwoCmdInfo->stCom2CmdContent);
	
	//检查COM口发送消息的可用状态
	bCom1ConditionIsok = CheckSingleComSendCmdConditonIsOk(m_Mscomm_com1, COM1_ENUM);
	bCom2ConditionIsok = CheckSingleComSendCmdConditonIsOk(m_Mscomm_com2, COM2_ENUM);
	
	//检查COM1口是否能发送命令的条件
	if(bCom1ConditionIsok)
	{
		//构造COM1消息
		pCom1Msg = (COMMON_MSG *) malloc(COMMON_MSG_HEADER_LENGTH + pCom1Content->ulContentLength);
		(void)InitCommonMsgHeader(pCom1Msg, enCmdType, pCom1Content->ulContentLength);
		(void)memcpy(pCom1Msg->aucValue, pCom1Content->pContent, pCom1Content->ulContentLength);
	}
	
	//检查COM2是否符合发送命令的条件
	if(bCom2ConditionIsok)
	{
		//构造COM2消息
		pCom2Msg = (COMMON_MSG *) malloc(COMMON_MSG_HEADER_LENGTH + pCom2Content->ulContentLength);
		(void)InitCommonMsgHeader(pCom2Msg, enCmdType, pCom2Content->ulContentLength);
		(void)memcpy(pCom2Msg->aucValue, pCom2Content->pContent, pCom2Content->ulContentLength);
	}
	
	//发送COM1消息
	if(bCom1ConditionIsok)
	{
		//COM1发送控制块设置
		m_stCom1SendingBlock.enCmdType = enCmdType;
		memcpy(m_stCom1SendingBlock.aucComMsgBuffer, pCom1Msg, COMMON_MSG_HEADER_LENGTH + pCom1Content->ulContentLength);
		m_stCom1SendingBlock.enSendResult = SEND_RESULT_FAIL;
		m_stCom1SendingBlock.enComSendingStatus = STATUS_BUSY;
		
		//调用通用接口发送消息
		bRet = SendMsgToCom(m_Mscomm_com1, pCom1Msg);
		if (bRet)
		{
			StartTimer(TIMER_ID_COM1);	
		}
		else
		{
			//记录日志，这里不做返回处理。
			AppendToSendLogFile("COM1 SendMsgToCom Failed");
		}
		
		//释放内存空间
		MSG_FREE(pCom1Msg);
	}

	//发送COM2消息
	if(bCom2ConditionIsok)
	{
		//COM2发送控制块设置
		m_stCom2SendingBlock.enCmdType = enCmdType;
		memcpy(m_stCom2SendingBlock.aucComMsgBuffer, pCom2Msg, COMMON_MSG_HEADER_LENGTH + pCom2Content->ulContentLength);
		m_stCom2SendingBlock.enSendResult = SEND_RESULT_FAIL;
		m_stCom2SendingBlock.enComSendingStatus = STATUS_BUSY;
	
		//调用通用接口发送消息
		bRet = SendMsgToCom(m_Mscomm_com2, pCom2Msg);
		if (bRet)
		{
			StartTimer(TIMER_ID_COM2);
		}
		else
		{
			//记录日志，这里不做返回处理。
			AppendToSendLogFile("COM1 SendMsgToCom Failed");
		}
		//释放空间
		MSG_FREE(pCom2Msg);
	}

	return TRUE;
}

void CSerialCommDlg::OnBtLock() 
{
	//获取数据
	unsigned int ulSendMsgLength;
	LOCK_CMD_CONTENT stSendCmdContentCom1;
	LOCK_CMD_CONTENT stSendCmdContentCom2;
	unsigned short usCom1SenderChannel, usCom2SenderChannel;
	TWO_COMMAND_CONTENT stTwoCmdContent;

	//获取发射机频道
	usCom1SenderChannel = ((CComboBox *)GetDlgItem(IDC_COMBO_SET_SENDCHANNEL_1))->GetCurSel() + 1;
	usCom2SenderChannel = ((CComboBox *)GetDlgItem(IDC_COMBO_SET_SENDCHANNEL_2))->GetCurSel() + 1;

	CString strTmp;
	int iStartId, iEndId;

	GetDlgItem(IDC_EDIT_START_RECVID)->GetWindowText(strTmp);
	iStartId = _ttoi(strTmp);

	GetDlgItem(IDC_EDIT_END_RECVID)->GetWindowText(strTmp);
	iEndId = _ttoi(strTmp);
	

	if(iStartId < 1 || iStartId > 65536 
		|| iEndId < 1 || iEndId > 65536)
	{
		MessageBox("起始接收机ID范围应在1-65536之间！");
		return ;
	}

	if (iStartId > iEndId)
	{
		MessageBox("请重新输入接收机ID范围，注意起始ID必须小于等于结束ID！");
		return ;
	}

	//赋值起始ID
	m_ulStartRevcerId = iStartId;
	m_ulEndRecverId = iEndId;

	//发送命令前做通用检查
	if (!CheckPreSendingConditionIsOk())
	{
		return;
	}
	
	//构造消息
	stSendCmdContentCom1.usChannel = usCom1SenderChannel;
	stSendCmdContentCom1.usStartReciverId = m_ulStartRevcerId;
	stSendCmdContentCom1.usEndRecverId = m_ulEndRecverId;
	
	stSendCmdContentCom2.usChannel = usCom2SenderChannel;
	stSendCmdContentCom2.usStartReciverId = m_ulStartRevcerId;
	stSendCmdContentCom2.usEndRecverId = m_ulEndRecverId;
	
	//发送数据的长度
	ulSendMsgLength = sizeof(LOCK_CMD_CONTENT);
	
	//分别构造需要发送给2个串口消息内容
	stTwoCmdContent.enCmdType = CMD_TYPE_LOCK;
	stTwoCmdContent.stCom1CmdContent.ulContentLength = ulSendMsgLength;
	stTwoCmdContent.stCom1CmdContent.pContent = (UINT8*)(&stSendCmdContentCom1);
	stTwoCmdContent.stCom2CmdContent.ulContentLength = ulSendMsgLength;
	stTwoCmdContent.stCom2CmdContent.pContent = (UINT8*)(&stSendCmdContentCom2);
	
	//调用接口发送数据，该接口内自行判断该发给哪个串口，发送失败则返回FALSE
	if (!SendCommandMassage(&stTwoCmdContent))
	{
		MessageBox("发送锁定命令失败！");
		return;
	}		
}

void CSerialCommDlg::OnBtClearData() 
{
	//获取数据
	unsigned int ulSendMsgLength;
	LOCK_CMD_CONTENT stSendCmdContentCom1;
	LOCK_CMD_CONTENT stSendCmdContentCom2;
	unsigned short usCom1SenderChannel, usCom2SenderChannel;
	TWO_COMMAND_CONTENT stTwoCmdContent;
	
	//获取发射机频道
	usCom1SenderChannel = ((CComboBox *)GetDlgItem(IDC_COMBO_SET_SENDCHANNEL_1))->GetCurSel() + 1;
	usCom2SenderChannel = ((CComboBox *)GetDlgItem(IDC_COMBO_SET_SENDCHANNEL_2))->GetCurSel() + 1;
	
	CString strTmp;
	int iStartId, iEndId;
	
	GetDlgItem(IDC_EDIT_START_RECVID)->GetWindowText(strTmp);
	iStartId = _ttoi(strTmp);
	
	GetDlgItem(IDC_EDIT_END_RECVID)->GetWindowText(strTmp);
	iEndId = _ttoi(strTmp);
	
	
	if(iStartId < 1 || iStartId > 65536 
		|| iEndId < 1 || iEndId > 65536)
	{
		MessageBox("起始接收机ID范围应在1-65536之间！");
		return ;
	}
	
	if (iStartId > iEndId)
	{
		MessageBox("请重新输入接收机ID范围，注意起始ID必须小于等于结束ID！");
		return ;
	}
	
	//赋值起始ID
	m_ulStartRevcerId = iStartId;
	m_ulEndRecverId = iEndId;

	//发送命令前做通用检查
	if (!CheckPreSendingConditionIsOk())
	{
		return;
	}
	
	//构造消息
	stSendCmdContentCom1.usChannel = usCom1SenderChannel;
	stSendCmdContentCom1.usStartReciverId = m_ulStartRevcerId;
	stSendCmdContentCom1.usEndRecverId = m_ulEndRecverId;
	
	stSendCmdContentCom2.usChannel = usCom2SenderChannel;
	stSendCmdContentCom2.usStartReciverId = m_ulStartRevcerId;
	stSendCmdContentCom2.usEndRecverId = m_ulEndRecverId;
	
	//发送数据的长度
	ulSendMsgLength = sizeof(LOCK_CMD_CONTENT);
	
	//分别构造需要发送给2个串口消息内容
	stTwoCmdContent.enCmdType = CMD_TYPE_CLEAR;
	stTwoCmdContent.stCom1CmdContent.ulContentLength = ulSendMsgLength;
	stTwoCmdContent.stCom1CmdContent.pContent = (UINT8*)(&stSendCmdContentCom1);
	stTwoCmdContent.stCom2CmdContent.ulContentLength = ulSendMsgLength;
	stTwoCmdContent.stCom2CmdContent.pContent = (UINT8*)(&stSendCmdContentCom2);
	
	//调用接口发送数据，该接口内自行判断该发给哪个串口，发送失败则返回FALSE
	if (!SendCommandMassage(&stTwoCmdContent))
	{
		MessageBox("发送数据清除命令失败");
		return;
	}	
}

void CSerialCommDlg::OnBtResend1() 
{
	//获取数据
	int iPos = m_listHistorySendedMsg.GetSelectionMark();
	if (iPos == -1)
	{
		MessageBox("请选择重发数据！");
		return;
	}

	CString strMsgNum, strMsgContent;
	unsigned short usMsgNum;
	unsigned int ulMsgLength, ulSendMsgLength;
	STU_RESEND_CMD_CONTENT stSendCmdContentCom1;
	STU_RESEND_CMD_CONTENT stSendCmdContentCom2;
	unsigned short usCom1SenderChannel, usCom2SenderChannel;
	TWO_COMMAND_CONTENT stTwoCmdContent;
	//获取选择数据
	strMsgNum = m_listHistorySendedMsg.GetItemText(iPos, 0);
	strMsgContent = m_listHistorySendedMsg.GetItemText(iPos, 1);
	//转化
	usMsgNum = _ttoi(strMsgNum);
	ulMsgLength = strMsgContent.GetLength();
	
	//获取发射机频道
	CString strTmp1, strTmp2;	
	((CComboBox *)GetDlgItem(IDC_COMBO_SENDMSG_SENDCHANNEL_1))->GetWindowText(strTmp1);
	((CComboBox *)GetDlgItem(IDC_COMBO_SENDMSG_SENDCHANNEL_2))->GetWindowText(strTmp2);
	
	usCom1SenderChannel = _ttoi(strTmp1);
	usCom2SenderChannel = _ttoi(strTmp2);

	m_aulSenderChannelCom1[m_enCurrentChannel] = usCom1SenderChannel;
	m_aulSenderChannelCom2[m_enCurrentChannel] = usCom2SenderChannel;

	//发送命令前做通用检查
	if (!CheckPreSendingConditionIsOk())
	{
		return;
	}
	
	//构造消息
	stSendCmdContentCom1.usChannel = usCom1SenderChannel;
	stSendCmdContentCom1.usMsgNum = usMsgNum;
	memcpy(stSendCmdContentCom1.aucMsg, m_strSendingContent.GetBuffer(NULL), ulMsgLength);
	
	stSendCmdContentCom2.usChannel = usCom2SenderChannel;
	stSendCmdContentCom2.usMsgNum = usMsgNum;
	memcpy(stSendCmdContentCom2.aucMsg, m_strSendingContent.GetBuffer(NULL), ulMsgLength);
	
	//发送数据的长度
	ulSendMsgLength = sizeof(stSendCmdContentCom1.usChannel) + sizeof(stSendCmdContentCom2.usMsgNum) + ulMsgLength;
	
	//分别构造需要发送给2个串口消息内容
	stTwoCmdContent.enCmdType = CMD_TYPE_RESEND;
	stTwoCmdContent.stCom1CmdContent.ulContentLength = ulSendMsgLength;
	stTwoCmdContent.stCom1CmdContent.pContent = (UINT8*)(&stSendCmdContentCom1);
	stTwoCmdContent.stCom2CmdContent.ulContentLength = ulSendMsgLength;
	stTwoCmdContent.stCom2CmdContent.pContent = (UINT8*)(&stSendCmdContentCom2);
	
	//调用接口发送数据，该接口内自行判断该发给哪个串口，发送失败则返回FALSE
	if (!SendCommandMassage(&stTwoCmdContent))
	{
		MessageBox("发送重传命令失败！");
		return;
	}	
}

void CSerialCommDlg::OnBtSend1() 
{
	unsigned short usCom1SenderChannel, usCom2SenderChannel;
	unsigned short usNewMsgNum = 0;
	unsigned int ulMsgLength, ulSendMsgLength;
	SEND_CMD_CONTENT stSendCmdContentCom1 = {0};
	SEND_CMD_CONTENT stSendCmdContentCom2 = {0};
	TWO_COMMAND_CONTENT stTwoCmdContent;
	
	//拷贝输入到变量
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_EDIT_SEND_1);
	pEdit->GetWindowText(m_strSendingContent);

	//过滤空格
	m_strSendingContent.TrimLeft();
	m_strSendingContent.TrimRight();
	
	if (m_strSendingContent.IsEmpty())
	{
		MessageBox("发送消息不能为空！");
		return;
	}
	
	ulMsgLength = m_strSendingContent.GetLength();
	if (ulMsgLength > 512 )
	{
		MessageBox("发送数据超长");
		return;
	}

	CString strTmp1, strTmp2;

	((CComboBox *)GetDlgItem(IDC_COMBO_SENDMSG_SENDCHANNEL_1))->GetWindowText(strTmp1);
	((CComboBox *)GetDlgItem(IDC_COMBO_SENDMSG_SENDCHANNEL_2))->GetWindowText(strTmp2);
	
	usCom1SenderChannel = _ttoi(strTmp1);
	usCom2SenderChannel = _ttoi(strTmp2);

	m_aulSenderChannelCom1[m_enCurrentChannel] = usCom1SenderChannel;
	m_aulSenderChannelCom2[m_enCurrentChannel] = usCom2SenderChannel;

	usNewMsgNum = GetNewMsgNumberFromHistory();

	//发送命令前做通用检查
	if (!CheckPreSendingConditionIsOk())
	{
		return;
	}

	//构造消息
	stSendCmdContentCom1.usChannel = usCom1SenderChannel;
	stSendCmdContentCom1.usMsgNum = usNewMsgNum;
	memcpy(stSendCmdContentCom1.aucMsg, m_strSendingContent.GetBuffer(NULL), ulMsgLength);

	stSendCmdContentCom2.usChannel = usCom2SenderChannel;
	stSendCmdContentCom2.usMsgNum = usNewMsgNum;
	memcpy(stSendCmdContentCom2.aucMsg, m_strSendingContent.GetBuffer(NULL), ulMsgLength);
	
	//发送数据的长度
	ulSendMsgLength = sizeof(stSendCmdContentCom1.usChannel) + sizeof(stSendCmdContentCom1.usMsgNum) + ulMsgLength;

	//分别构造需要发送给2个串口消息内容
	stTwoCmdContent.enCmdType = CMD_TYPE_SEND;
	stTwoCmdContent.stCom1CmdContent.ulContentLength = ulSendMsgLength;
	stTwoCmdContent.stCom1CmdContent.pContent = (UINT8*)(&stSendCmdContentCom1);
	stTwoCmdContent.stCom2CmdContent.ulContentLength = ulSendMsgLength;
	stTwoCmdContent.stCom2CmdContent.pContent = (UINT8*)(&stSendCmdContentCom2);
	
	//调用接口发送数据，该接口内自行判断该发给哪个串口，发送失败则返回FALSE
	if (!SendCommandMassage(&stTwoCmdContent))
	{
		MessageBox("发送发送命令失败！");
		return;
	}	
}

void CSerialCommDlg::OnBtClearHistory1() 
{
	//删除内存中的数据
	m_strArryMsgHistory[m_enCurrentChannel].RemoveAll();
	
	//刷新历史记录显示
	FreshHistoryMsgSpace();
}

unsigned int CSerialCommDlg::GetNewMsgNumberFromHistory()
{
	//从历史记录长度+1
	return m_strArryMsgHistory[m_enCurrentChannel].GetSize()+1;
}

void CSerialCommDlg::SetCurrentChannel(CHANNEL_NUM_ENUM enCur)
{
	CString strTemp;
	strTemp = "您的选择：";
	switch(enCur)
	{
	case CHANNEL_NUM_ONE:
		strTemp+="通道1";
		break;
	case CHANNEL_NUM_TWO:
		strTemp+="通道2";
		break;
	case CHANNEL_NUM_THREE:
		strTemp+="通道3";
		break;
	default:
		break;
	}
	
	//处理控件显示
	GetDlgItem(IDC_STATIC_CUR_SELECT)->SetWindowText(strTemp);
	
	//设置当前显示通道
	m_enCurrentChannel = enCur;

	//刷新频道区域
	int iCom1Pos = m_aulSenderChannelCom1[m_enCurrentChannel]-1;
	int iCom2Pos = m_aulSenderChannelCom2[m_enCurrentChannel]-11;

	((CComboBox *)GetDlgItem(IDC_COMBO_SENDMSG_SENDCHANNEL_1))->SetCurSel(iCom1Pos);
	((CComboBox *)GetDlgItem(IDC_COMBO_SENDMSG_SENDCHANNEL_2))->SetCurSel(iCom2Pos);
	
	//刷新历史记录区域
	FreshHistoryMsgSpace();
}

void CSerialCommDlg::OnBtSelectChannel1() 
{
	// TODO: Add your control notification handler code here
	SetCurrentChannel(CHANNEL_NUM_ONE);
}

void CSerialCommDlg::OnBtSelectChannel2() 
{
	// TODO: Add your control notification handler code here
	SetCurrentChannel(CHANNEL_NUM_TWO);
}

void CSerialCommDlg::OnBtSelectChannel3() 
{
	// TODO: Add your control notification handler code here
	SetCurrentChannel(CHANNEL_NUM_THREE);
}

void CSerialCommDlg::FreshHistoryMsgSpace()
{
	int iSenderMsgNum = 0;
	int i=0;
	CString strSenderMsgNo, strSenderMsgContent;

	//清空表格内容
    m_listHistorySendedMsg.DeleteAllItems();
	
	//获取消息总长度
	iSenderMsgNum = m_strArryMsgHistory[m_enCurrentChannel].GetSize();
	
	//遍历并刷新消息从大到小
	for (i=0; i<iSenderMsgNum; i++)
	{
		//获取序号和内容
		strSenderMsgNo.Format("%d",iSenderMsgNum-i);
		strSenderMsgContent = m_strArryMsgHistory[m_enCurrentChannel].GetAt(iSenderMsgNum-i-1);

		//显示到LIST控件上
		m_listHistorySendedMsg.InsertItem(i, "");
		m_listHistorySendedMsg.SetItemText(i, 0, strSenderMsgNo);
		m_listHistorySendedMsg.SetItemText(i, 1, strSenderMsgContent);
	}

}

void CSerialCommDlg::OnSelchangeSenderChannel1() 
{
	int iChannelPos1, iChannelPos2;
	CString strTmp1,strTmp2;

	iChannelPos1 = ((CComboBox *)GetDlgItem(IDC_COMBO_SENDMSG_SENDCHANNEL_1))->GetCurSel();
	iChannelPos2 = iChannelPos1;
	
	((CComboBox *)GetDlgItem(IDC_COMBO_SENDMSG_SENDCHANNEL_2))->SetCurSel(iChannelPos2);
	
	((CComboBox *)GetDlgItem(IDC_COMBO_SENDMSG_SENDCHANNEL_1))->GetWindowText(strTmp1);
	((CComboBox *)GetDlgItem(IDC_COMBO_SENDMSG_SENDCHANNEL_2))->GetWindowText(strTmp2);
	
	m_aulSenderChannelCom1[m_enCurrentChannel] = _ttoi(strTmp1);
	m_aulSenderChannelCom2[m_enCurrentChannel] = _ttoi(strTmp2);
}

void CSerialCommDlg::OnSelchangeSenderChannel2() 
{
//	unsigned short usCom2SenderChannel;
	
//	usCom2SenderChannel = ((CComboBox *)GetDlgItem(IDC_COMBO_SENDMSG_SENDCHANNEL_2))->GetCurSel() + 11;
//	m_aulSenderChannelCom2[m_enCurrentChannel] = usCom2SenderChannel;
}

void CSerialCommDlg::OnDblClkListHistory1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iPos = m_listHistorySendedMsg.GetSelectionMark();
	if(iPos == -1)
	{
		return ;
	}

	//获取选择数据
	CHistoryDetailDialog objDlg;
	objDlg.m_iHistoryNum = _ttoi(m_listHistorySendedMsg.GetItemText(iPos, 0));
	
	int iCurSel = objDlg.m_iHistoryNum;		//当前选择序号
	int iSenderMsgNum = m_strArryMsgHistory[m_enCurrentChannel].GetSize(); //历史记录表长度

	if(objDlg.m_iHistoryNum <= iSenderMsgNum)
	{
		//修改历史记录明细显示不全问题，更改从历史记录表中获取
		objDlg.m_strMsgContent = m_strArryMsgHistory[m_enCurrentChannel].GetAt(iCurSel-1);
		//AppendToSendLogFile(objDlg.m_strMsgContent);
		objDlg.DoModal();
	}

	*pResult = 0;
}

void CSerialCommDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	OnBtSelectChannel1();
}

void CSerialCommDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	OnBtSelectChannel2();
}

void CSerialCommDlg::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	OnBtSelectChannel3();
}

void CSerialCommDlg::OnCheck1() 
{
	BOOL bRet = FALSE;
	//若已经打开，则关闭串口
	if (m_Mscomm_com1.GetPortOpen())
	{
		CloseComPort(m_Mscomm_com1);
	}

	//遍历所有串口默认遍历1~30个串口
	for(short uPort=1; uPort<=30; uPort++)
	{
		bRet = OpenComPort(m_Mscomm_com1, uPort);
		if(bRet)
		{
			break;
		}
	}

	CString strTmp;

	if(bRet)
	{
		//设置端口和选择
		((CComboBox *)GetDlgItem(IDC_COMBO_COM1))->SetCurSel(uPort-1);
		((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(TRUE);
		
		strTmp.Format("Auto open COM%u Success!", uPort);
		AppendToSendLogFile(strTmp);
	}
	else
	{
		//设置不选中
		((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(FALSE);
		AppendToSendLogFile("No available Com!");
		MessageBox("打开端口1失败！");
	}

#if 0
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		//若串口已经打开，不必打开串口
		if (m_Mscomm_com1.GetPortOpen())
		{
			return;
		}

		//打开串口
		short uPort = ((CComboBox *)GetDlgItem(IDC_COMBO_COM1))->GetCurSel() + 1;
		BOOL bRet = OpenComPort(m_Mscomm_com1, uPort);
		if(!bRet)
		{
			//设置不选中
			((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(FALSE);
			MessageBox("打开端口1失败！");
			return;
		}
	}
	else
	{
		//若已经打开，则关闭串口
		if (m_Mscomm_com1.GetPortOpen())
		{
			CloseComPort(m_Mscomm_com1);
		}
	}
#endif

}

void CSerialCommDlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here
		if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
	{
		//若串口已经打开，不必打开串口
		if (m_Mscomm_com2.GetPortOpen())
		{
			return;
		}

		//打开串口
		short uPort = ((CComboBox *)GetDlgItem(IDC_COMBO_COM2))->GetCurSel() + 1;
		BOOL bRet = OpenComPort(m_Mscomm_com2, uPort);
		if(!bRet)
		{
			//设置不选中
			((CButton *)GetDlgItem(IDC_CHECK2))->SetCheck(FALSE);
			MessageBox("打开端口2失败！");
			return;
		}
	}
	else
	{
		//若已经打开，则关闭串口
		if (m_Mscomm_com2.GetPortOpen())
		{
			CloseComPort(m_Mscomm_com2);
		}
	}
}
