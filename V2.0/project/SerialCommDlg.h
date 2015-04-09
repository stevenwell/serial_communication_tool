// SerialCommDlg.h : header file
//
//{{AFX_INCLUDES()
#include "mscomm.h"
#include "HistoryDetailDialog.h"
//}}AFX_INCLUDES

#if !defined(AFX_SERIALCOMMDLG_H__37996DD8_CC5E_464D_ABF3_FD32D3B8D6B1__INCLUDED_)
#define AFX_SERIALCOMMDLG_H__37996DD8_CC5E_464D_ABF3_FD32D3B8D6B1__INCLUDED_

#include "mscomm.h"	// Added by ClassView
#include "MsgDefine.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//定义定时器ID类型
#define TIMER_ID_COM1 0x01
#define TIMER_ID_COM2 0x02
#define TIMER_ID_SCAN_COMM 0x03 //1s定时器自动扫描

//定义释放空间宏
#define MSG_FREE(pMsg) \
	do \
	{ \
	if (NULL == pMsg) \
		{ \
		free(pMsg);\
		pMsg = NULL; \
		} \
	} while (0);

//COM枚举类型，逻辑上仅使用2个COM2口
typedef enum{
	COM1_ENUM = 0x01,
	COM2_ENUM = 0x02,
	COM_BUTT_ENUM
}COM_NUMBER_ENUM; 

//COM口的状态 发送状态，空闲状态
typedef enum{
	STATUS_BUSY = 0x01,
	STATUS_FREE,
	STATUS_INVALID
}COM_STATUS_ENUM;

//COM口发送数据结果状态
typedef enum {
	SEND_RESULT_SUCCESS = 0x01,
	SEND_RESULT_FAIL,
	SEND_RESULT_BUTT
}SEND_RESULT_ENUM;

//定义通道数目
typedef enum {
	CHANNEL_NUM_ONE=0,
	CHANNEL_NUM_TWO,
	CHANNEL_NUM_THREE,
	CHANNEL_NUM_BUTT
}CHANNEL_NUM_ENUM;

//COM口状态标记结构体
typedef struct STU_SENDIING_CTRL_BLOCK
{
	CMD_TYPE enCmdType;
	COM_STATUS_ENUM enComSendingStatus;
	unsigned int ulCounter;
	SEND_RESULT_ENUM enSendResult;
	UINT8 aucComMsgBuffer[1024];
}SENDING_CTRL_BLOCK;

//通用发送消息接口使用参数结构
typedef struct STU_COMMAND_CONTENT
{
	unsigned int ulContentLength;
	UINT8* pContent;
}COMMAND_CONTENT;

//由于通常需要给2个串口发送数据，故意此处设计此结构体
typedef struct STU_TWO_COMMAND_CONTENT
{
	CMD_TYPE enCmdType;
	COMMAND_CONTENT stCom1CmdContent;
	COMMAND_CONTENT stCom2CmdContent;
}TWO_COMMAND_CONTENT;

/////////////////////////////////////////////////////////////////////////////
// CSerialCommDlg dialog

class CSerialCommDlg : public CDialog
{
// Construction
public:
	//历史记录
	CStringArray m_strArryMsgHistory[CHANNEL_NUM_BUTT];
	//发射机通道
	unsigned int m_aulSenderChannelCom1[CHANNEL_NUM_BUTT];
	unsigned int m_aulSenderChannelCom2[CHANNEL_NUM_BUTT];
	//当前发射通道数
	CHANNEL_NUM_ENUM m_enCurrentChannel;

	SENDING_CTRL_BLOCK m_stCom2SendingBlock;
	SENDING_CTRL_BLOCK m_stCom1SendingBlock;
	CByteArray m_bytRecvArr;
	CSerialCommDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSerialCommDlg)
	enum { IDD = IDD_SERIALCOMM_DIALOG };
	CListCtrl	m_listHistorySendedMsg;
	CString m_strSendLogFile;
	CString m_strRecvLogFile;
	CMSComm	m_Mscomm_com1;
	CMSComm	m_Mscomm_com2;
	CString	m_strSendingContent;
	CString	m_strRecvContent;
	UINT	m_ulStartRevcerId;
	UINT	m_ulEndRecverId;
	CString	m_SendBuffer;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialCommDlg)
public:
	virtual void WriteToLogFile(CString strTmp, CString fileName);
	virtual void AppendToSendLogFile(CString strTmp);
	virtual void AppendToRecvLogFile(CString strTmp);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void FreshHistoryMsgSpace();
	void SetCurrentChannel(CHANNEL_NUM_ENUM enCur);
	unsigned int GetNewMsgNumberFromHistory();
	BOOL SendCommandMassage(TWO_COMMAND_CONTENT*  pTwoCmdInfo);
	BOOL CheckPreSendingConditionIsOk();
	void ProcessRecvedMsg(CMSComm &objComm, SENDING_CTRL_BLOCK *pCtrlBlock);
	void ProcessScanCommTimer(void);
	void ProcessComTimer(UINT nIDEvent, SENDING_CTRL_BLOCK* pCtrlBlock, CMSComm& objComm);
	void CloseTimer(unsigned int ulTimerID);
	void SetComPortSendingMsgStatus(COM_NUMBER_ENUM enComEnum, COM_STATUS_ENUM enStatus);
	void InitSendingCtrlBlock(SENDING_CTRL_BLOCK * pCtrlBlock);
	COM_STATUS_ENUM GetComPortSendingMsgStatus(COM_NUMBER_ENUM enComEnum);
	void StartTimer(unsigned int ulTimerID);
	BOOL SendMsgToCom(CMSComm& objComm, COMMON_MSG* pMsg);
	BOOL CheckSingleComSendCmdConditonIsOk(CMSComm &objComm, COM_NUMBER_ENUM enComEnum);
	void InitCommonMsgHeader(COMMON_MSG* pMsg, unsigned int ulCmdType,  unsigned int  ulContentLength);
	BOOL CheckAllComSendCmdConditonIsOk();
	BOOL CloseComPort(CMSComm &objComm);
	BOOL OpenComPort(CMSComm &objComm, short uPort);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSerialCommDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnBtCom1Open();
	afx_msg void OnBtCom2Open();
	afx_msg void OnCommMscommCom1();
	afx_msg void OnCommMscommCom2();
	afx_msg void OnBtConnect();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtLock();
	afx_msg void OnBtClearData();
	afx_msg void OnBtResend1();
	afx_msg void OnBtSend1();
	afx_msg void OnBtClearHistory1();
	afx_msg void OnBtSelectChannel1();
	afx_msg void OnBtSelectChannel2();
	afx_msg void OnBtSelectChannel3();
	afx_msg void OnSelchangeSenderChannel1();
	afx_msg void OnSelchangeSenderChannel2();
	afx_msg void OnDblClkListHistory1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALCOMMDLG_H__37996DD8_CC5E_464D_ABF3_FD32D3B8D6B1__INCLUDED_)
 
