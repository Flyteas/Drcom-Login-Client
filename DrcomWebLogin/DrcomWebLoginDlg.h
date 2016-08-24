
// DrcomWebLoginDlg.h : ͷ�ļ�
//

#pragma once
#include "common.h"

// CDrcomWebLoginDlg �Ի���
class CDrcomWebLoginDlg : public CDialogEx
{
// ����
public:
	CDrcomWebLoginDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CDrcomWebLoginDlg(); //��������

// �Ի�������
	enum { IDD = IDD_DRCOMWEBLOGIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
