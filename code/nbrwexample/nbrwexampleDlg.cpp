// nbrwexampleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "nbrwexample.h"
#include "nbrwexampleDlg.h"

#include "nbRwTrans.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CnbrwexampleDlg 对话框

const char szTransferNotStart[]   = "转发器状态： 转发服务未启动";
const char szTransferStart[]      = "转发器状态： 转发服务正常启动";
const char szTransferErrorRead[]  = "转发器状态： 读取 redwood 设备数据不成功";
const char szTransferErrorParse[] = "转发器状态： 解析 redwood 设备数据不成功";
const char szTransferErrorSend[]  = "转发器状态： 发送 redwood 设备数据不成功";

CnbrwexampleDlg::CnbrwexampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CnbrwexampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CnbrwexampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CnbrwexampleDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_START, &CnbrwexampleDlg::OnBnClickedButtonStart)
    ON_BN_CLICKED(IDC_BUTTON_STOP, &CnbrwexampleDlg::OnBnClickedButtonStop)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDCANCEL, &CnbrwexampleDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CnbrwexampleDlg 消息处理程序

BOOL CnbrwexampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    SetDlgItemText(IDC_STATIC_NOTE, "使用说明：\r\n\r\n"
           "  1.本工具为转发 redwood 数据 dll的使用例子；\r\n\r\n"
           "  2.使用前请正确配置参数；\r\n\r\n"
           "  3.配置参数在配置文件 nbdatatr.ini 中进行；\r\n\r\n"
           "  4.点击 start 按钮后，将每隔一段时间向设置的 upd 服务器\r\n"
           "    发送每次读到的变化的数据；\r\n");

    SetDlgItemText(IDC_STATIC_STATE, szTransferNotStart);    

    SetTimer(1, 1000, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CnbrwexampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CnbrwexampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CnbrwexampleDlg::OnBnClickedButtonStart()
{
    // TODO: 在此添加控件通知处理程序代码
    SetParameter();
    StartTransferFlow();
}

void CnbrwexampleDlg::OnBnClickedButtonStop()
{
    // TODO: 在此添加控件通知处理程序代码
    StopTransferFlow();
}

void CnbrwexampleDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    int iTransferStates = GetTransferStates();

    switch (iTransferStates) {
        case TRANSFER_NOT_START:
            SetDlgItemText(IDC_STATIC_STATE, szTransferNotStart);  
            break;

        case TRANSFER_START:
            SetDlgItemText(IDC_STATIC_STATE, szTransferStart); 
            break;
            
        case TRANSFER_ERROR_READRWDATA:
            SetDlgItemText(IDC_STATIC_STATE, szTransferErrorRead);
            break;
            
        case TRANSFER_ERROR_PARSEDATA:
            SetDlgItemText(IDC_STATIC_STATE, szTransferErrorParse);
            break;
            
        case TRANSFER_ERROR_SENDDATA:
            SetDlgItemText(IDC_STATIC_STATE, szTransferErrorSend);
            break;

        default:
            break;
    }

    CDialog::OnTimer(nIDEvent);
}

void CnbrwexampleDlg::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    StopTransferFlow();
    OnCancel();

    /*
    char szIp[64] = {0};
    int iPort = 0;    
    char szUser[64] = {0};
    char szPw[64] = {0};
    int iTransInterval = 0;

    //数据源参数
    GetPrivateProfileStringA(SEC_REDWOOD, KEY_REDWOOD_SRCDATAIP, 
        "192.168.0.245", szIp, sizeof(szIp), NBINIFILENAME);
    iPort = GetPrivateProfileIntA(SEC_REDWOOD, KEY_REDWOOD_SRCDATAPORT, 
        80, NBINIFILENAME);

    rwhandler.setNetAddr(szIp, iPort);  

    GetPrivateProfileStringA(SEC_REDWOOD, KEY_REDWOOD_USER, 
        "admin", szUser, sizeof(szUser), NBINIFILENAME);
    GetPrivateProfileStringA(SEC_REDWOOD, KEY_REDWOOD_PASSWORD, 
        "redwood", szPw, sizeof(szPw), NBINIFILENAME);

    rwhandler.setUserAndPw(szUser, szPw);

    //udp 监听参数
    GetPrivateProfileStringA(SEC_UDPLISTENER, KEY_UDPLISTENER_DESCIP, 
        "127.0.0.1", szIp, sizeof(szIp), NBINIFILENAME);
    iPort = GetPrivateProfileIntA(SEC_UDPLISTENER, KEY_UDPLISTENER_DESCPORT, 
        2222, NBINIFILENAME);

    nbDataSender.setPeerAddr(szIp, iPort); 

    //数据转发参数
    iTransInterval = GetPrivateProfileIntA(SEC_TRANSCONTRL, KEY_TRANSCONTRL_TRANSINTERVAL, 
        2, NBINIFILENAME);

    if (iTransInterval <= 0)
        iTransInterval = 2;

    iTimeDelay = iTransInterval * 1000;

    */
}

void CnbrwexampleDlg::SetParameter()
{
    char szPath[256] = {0};
    char szIniFilePath[256] = {0};
    
    GetCurrentDirectory(256, szPath);
    sprintf(szIniFilePath, "%s/%s", szPath, NBINIFILENAME);

    char szSrcIp[64] = {0};
    int iSrcPort = 0;    
    char szUser[64] = {0};
    char szPw[64] = {0};
    char szUdpIp[64]= {0};
    int iUdpPort = 0;
    int iTransInterval = 0;
    int iTransOption = 0;

    //数据源参数    
    GetPrivateProfileStringA(SEC_REDWOOD, KEY_REDWOOD_SRCDATAIP, 
        "192.168.1.245", szSrcIp, sizeof(szSrcIp), szIniFilePath);
    iSrcPort = GetPrivateProfileIntA(SEC_REDWOOD, KEY_REDWOOD_SRCDATAPORT, 
        80, szIniFilePath);
    GetPrivateProfileStringA(SEC_REDWOOD, KEY_REDWOOD_USER, 
        "admin", szUser, sizeof(szUser), szIniFilePath);
    GetPrivateProfileStringA(SEC_REDWOOD, KEY_REDWOOD_PASSWORD, 
        "redwood", szPw, sizeof(szPw), szIniFilePath);

    //udp 监听参数
    GetPrivateProfileStringA(SEC_UDPLISTENER, KEY_UDPLISTENER_DESCIP, 
        "127.0.0.1", szUdpIp, sizeof(szUdpIp), szIniFilePath);
    iUdpPort = GetPrivateProfileIntA(SEC_UDPLISTENER, KEY_UDPLISTENER_DESCPORT, 
        2222, szIniFilePath);

    //数据转发参数
    iTransInterval = GetPrivateProfileIntA(SEC_TRANSCONTRL, KEY_TRANSCONTRL_TRANSINTERVAL, 
        2, szIniFilePath);
    if (iTransInterval <= 0)
        iTransInterval = 2;

    iTransOption = GetPrivateProfileIntA(SEC_TRANSCONTRL, KEY_TRANSCONTRL_TRANSFEROPTION, 
        0, szIniFilePath);

    SetTransferParameter(szSrcIp, iSrcPort,
        szUser, szPw, szUdpIp, iUdpPort, 
        iTransInterval, iTransOption);
}