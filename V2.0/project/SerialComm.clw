; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CHistoryDetailDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "serialcomm.h"
LastPage=0
CDK=Y

ClassCount=4
Class1=CSerialCommApp
Class2=CAboutDlg
Class3=CSerialCommDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDD_SERIALCOMM_DIALOG
Class4=CHistoryDetailDialog
Resource3=IDD_HISTORY_DETAIL

[CLS:CSerialCommApp]
Type=0
BaseClass=CWinApp
HeaderFile=SerialComm.h
ImplementationFile=SerialComm.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=SerialCommDlg.cpp
ImplementationFile=SerialCommDlg.cpp

[CLS:CSerialCommDlg]
Type=0
BaseClass=CDialog
HeaderFile=SerialCommDlg.h
ImplementationFile=SerialCommDlg.cpp
LastObject=IDC_EDIT_SEND_1
Filter=D
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SERIALCOMM_DIALOG]
Type=1
Class=CSerialCommDlg
ControlCount=51
Control1=IDC_STATIC,static,1342308353
Control2=IDC_STATIC,button,1342177287
Control3=IDC_COMBO_COM1,combobox,1344471043
Control4=IDC_BT_COM1_OPEN,button,1208025088
Control5=IDC_EDIT_RECV_1,edit,1352734724
Control6=IDC_EDIT_SEND_1,edit,1352732676
Control7=IDC_BT_SEND_1,button,1342242816
Control8=IDC_MSCOMM_COM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control9=IDC_COMBO_SET_RECVCHANNEL_1,combobox,1344339971
Control10=IDC_STATIC,static,1342308353
Control11=IDC_STATIC,static,1208090625
Control12=IDC_COMBO_COM2,combobox,1210253315
Control13=IDC_BT_COM2_OPEN,button,1208025088
Control14=IDC_COMBO_SET_RECVCHANNEL_2,combobox,1210122243
Control15=IDC_STATIC,static,1208090625
Control16=IDC_MSCOMM_COM2,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control17=IDC_BT_CONNECT,button,1342242816
Control18=IDC_STATIC,button,1342177287
Control19=IDC_STATIC,static,1342308353
Control20=IDC_EDIT_START_RECVID,edit,1350639744
Control21=IDC_BT_LOCK,button,1342242816
Control22=IDC_STATIC,button,1342177287
Control23=IDC_COMBO_SET_SENDCHANNEL_1,combobox,1344339971
Control24=IDC_STATIC,static,1342308353
Control25=IDC_COMBO_SET_SENDCHANNEL_2,combobox,1344339971
Control26=IDC_STATIC,static,1342308353
Control27=IDC_BT_CLEARDATA,button,1342242816
Control28=IDC_STATIC,static,1342308353
Control29=IDC_EDIT_END_RECVID,edit,1350639744
Control30=IDC_STATIC,button,1342177287
Control31=IDC_COMBO_SENDMSG_SENDCHANNEL_1,combobox,1344339971
Control32=IDC_STATIC,static,1342308353
Control33=IDC_COMBO_SENDMSG_SENDCHANNEL_2,combobox,1478557699
Control34=IDC_STATIC,static,1342308353
Control35=IDC_BT_CLEAR_HISTORY_1,button,1342242816
Control36=IDC_STATIC,button,1342177287
Control37=IDC_STATIC,button,1342177287
Control38=IDC_STATIC,button,1342177287
Control39=IDC_BT_RESEND_1,button,1342242816
Control40=IDC_STATIC,button,1342177287
Control41=IDC_STATIC,button,1342177287
Control42=IDC_BT_CHANNEL_1,button,1208025088
Control43=IDC_BT_CHANNEL_2,button,1208025088
Control44=IDC_BT_CHANNEL_3,button,1208025088
Control45=IDC_LIST_HISTORY_1,SysListView32,1350631429
Control46=IDC_STATIC_CUR_SELECT,static,1342308352
Control47=IDC_CHECK1,button,1342242819
Control48=IDC_CHECK2,button,1208025091
Control49=IDC_RADIO1,button,1342177289
Control50=IDC_RADIO2,button,1342177289
Control51=IDC_RADIO3,button,1342177289

[DLG:IDD_HISTORY_DETAIL]
Type=1
Class=CHistoryDetailDialog
ControlCount=5
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_HISTORY_DETAIL,edit,1352734724
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_NUM,edit,1484849280

[CLS:CHistoryDetailDialog]
Type=0
HeaderFile=HistoryDetailDialog.h
ImplementationFile=HistoryDetailDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

