// nbrwexampleDlg.h : 头文件
//

#pragma once


//ini 文件定义
#define NBINIFILENAME                   "nbdatatr.ini"     //ini文件名

#define SEC_TRANSCONTRL                 "TransContrl"
#define SEC_REDWOOD                     "RedWood"
#define SEC_UDPLISTENER                 "UdpListener"

#define KEY_TRANSCONTRL_TRANSINTERVAL   "TransInterval"
#define KEY_TRANSCONTRL_TRANSFEROPTION  "IsTransChangeData"

#define KEY_REDWOOD_SRCDATAIP           "srcDataIp"
#define KEY_REDWOOD_SRCDATAPORT         "srcDataPort"
#define KEY_REDWOOD_USER                "user"
#define KEY_REDWOOD_PASSWORD            "password"

#define KEY_UDPLISTENER_DESCIP          "DescIp"
#define KEY_UDPLISTENER_DESCPORT        "DescPort"


// CnbrwexampleDlg 对话框
class CnbrwexampleDlg : public CDialog
{
// 构造
public:
	CnbrwexampleDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NBRWEXAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
    void SetParameter();


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonStart();
    afx_msg void OnBnClickedButtonStop();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedCancel();
};
