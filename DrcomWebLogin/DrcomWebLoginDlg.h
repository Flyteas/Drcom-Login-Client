
// DrcomWebLoginDlg.h : 头文件
//

#pragma once
#include "common.h"

// CDrcomWebLoginDlg 对话框
class CDrcomWebLoginDlg : public CDialogEx
{
// 构造
public:
	CDrcomWebLoginDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CDrcomWebLoginDlg(); //析构函数

// 对话框数据
	enum { IDD = IDD_DRCOMWEBLOGIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CString Usernname_Edit;
	CString Password_Edit;
	CString ServerUrl_Edit;
	DrcomWeb* DrcomWeb_obj;
	bool IsConnRetryTimerBegin;
	bool IsKeepOnlineTimerBegin;
	afx_msg void OnBnClickedButton1();
	BOOL ConnRetryOption;
	BOOL KeepOnlineOption;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
