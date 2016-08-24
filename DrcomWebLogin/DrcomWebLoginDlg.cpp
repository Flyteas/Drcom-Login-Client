
// DrcomWebLoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DrcomWebLogin.h"
#include "DrcomWebLoginDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDrcomWebLoginDlg �Ի���



CDrcomWebLoginDlg::CDrcomWebLoginDlg(CWnd* pParent /*=NULL*/)  //����
	: CDialogEx(CDrcomWebLoginDlg::IDD, pParent)
	, Usernname_Edit(_T(""))
	, Password_Edit(_T(""))
	, ServerUrl_Edit(_T(""))
	, ConnRetryOption(FALSE)
	, KeepOnlineOption(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->Usernname_Edit = "";
	this->Password_Edit = "";
	this->ServerUrl_Edit = "202.202.0.163";
	this->DrcomWeb_obj = new DrcomWeb(this->Usernname_Edit,this->Password_Edit,this->ServerUrl_Edit); //��ʼ��
	this->IsKeepOnlineTimerBegin = false;
	this->IsConnRetryTimerBegin = false;
}

CDrcomWebLoginDlg::~CDrcomWebLoginDlg() //��������
{
	delete this->DrcomWeb_obj;
	this->DrcomWeb_obj = NULL;
}

void CDrcomWebLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Usernname_Edit);
	DDX_Text(pDX, IDC_EDIT2, Password_Edit);
	DDX_Text(pDX, IDC_EDIT3, ServerUrl_Edit);
	DDX_Check(pDX, IDC_CHECK2, ConnRetryOption);
	DDX_Check(pDX, IDC_CHECK1, KeepOnlineOption);
}

BEGIN_MESSAGE_MAP(CDrcomWebLoginDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CDrcomWebLoginDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDrcomWebLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDrcomWebLoginDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CDrcomWebLoginDlg ��Ϣ�������

BOOL CDrcomWebLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	ModifyStyle(0, WS_MINIMIZEBOX); //�����С����ť
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDrcomWebLoginDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDrcomWebLoginDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDrcomWebLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDrcomWebLoginDlg::OnBnClickedOk()  //��½��ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!this->DrcomWeb_obj->CheckLicence("drcom.flyshit.pw","admin/sys/menu/menuset.aspx","-Flyshit@!@drcom")) //�������
	{
		MessageBoxA("�����ʧЧ,�����!","����");
		return;
	}
	UpdateData(true);
	if(this->Usernname_Edit.IsEmpty()||this->Password_Edit.IsEmpty()||this->ServerUrl_Edit.IsEmpty()) //�ж��˺���Ϣ�Լ���������Ϣ�Ƿ�Ϊ��
	{
		MessageBox("�û��������룬������IP������Ϊ��!","����");
		return;
	}
	this->DrcomWeb_obj->SetAccount(this->Usernname_Edit,this->Password_Edit,this->ServerUrl_Edit);
	if(this->DrcomWeb_obj->Login())
	{
		MessageBox("��½�ɹ�!","��ʾ");
		if(this->KeepOnlineOption&&!this->IsKeepOnlineTimerBegin) //�����ѡ��������,�Ҷ�ʱ��û������
		{
			SetTimer(1,300000,NULL); //���ö�ʱ����5���ӷ���һ�ΰٶ�
			this->IsKeepOnlineTimerBegin = true;
		}
		if(!this->IsConnRetryTimerBegin) //����˺ż�ⶨʱ��û������
		{
			SetTimer(2,180000,NULL); //���ö�ʱ��,180����һ���Ƿ��˺ŵ���
			this->IsConnRetryTimerBegin = true;
		}

		/* ����״̬��ʾ */
		if(this->KeepOnlineOption) //�����ѡ������״̬���
		{
			if(this->DrcomWeb_obj->KeepOnline("www.baidu.com","chrome")) //������ͨ״̬�ж�
			{ //��ͨ
				GetDlgItem(ID_INTERNETSTATE_TEXT)->SetWindowTextA("������ͨ");
			}
			else
			{ //�ж�
				GetDlgItem(ID_INTERNETSTATE_TEXT)->SetWindowTextA("�����ж�");
			}
		}
		if(this->DrcomWeb_obj->CheckOnline()) //�ж��˺��Ƿ�����
		{//����
			GetDlgItem(ID_ACCOUNTSTATE_TEXT)->SetWindowTextA("�˺�����");
			CString TimeUsed = "����ʱ��: " + this->DrcomWeb_obj->GetAccountInfo_TimeUsed() + " ����";
			CString FlowUsed = "��������: " + this->DrcomWeb_obj->GetAccountInfo_FlowUsed();
			GetDlgItem(ID_TimeUsed_TEXT)->SetWindowTextA((LPCTSTR)TimeUsed);
			GetDlgItem(ID_FLOWUSED_TEXT)->SetWindowTextA((LPCTSTR)FlowUsed);
		}
		else
		{//����
			GetDlgItem(ID_ACCOUNTSTATE_TEXT)->SetWindowTextA("�˺�����");
		}
	}
	else
	{
		MessageBox("��½ʧ��!\n�����˺������Լ�������ͨ״̬!","��ʾ");
	}
}

void CDrcomWebLoginDlg::OnBnClickedCancel() //�رհ�ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}




void CDrcomWebLoginDlg::OnBnClickedButton1() //ע����ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!this->DrcomWeb_obj->CheckLicence("www.flyshit.pw","admin/sys/menu/menuset.aspx","-Flyshit@!@drcom")) //�������
	{
		MessageBoxA("�����ʧЧ,�����!","����");
		return;
	}

	UpdateData(true);
	if(this->ServerUrl_Edit.IsEmpty()) //�жϷ�������Ϣ�Ƿ�Ϊ��
	{
		MessageBox("������IP������Ϊ��!","����");
		return;
	}
	if(this->IsKeepOnlineTimerBegin) //����������߶�ʱ��������
	{
		KillTimer(1); //��ֹ��ʱ��
		this->IsKeepOnlineTimerBegin = false;
	}
	this->DrcomWeb_obj->SetAccount(this->Usernname_Edit,this->Password_Edit,this->ServerUrl_Edit);
	if(this->DrcomWeb_obj->Logout())
	{
		MessageBox("ע���ɹ�!","��ʾ");
		GetDlgItem(ID_ACCOUNTSTATE_TEXT)->SetWindowTextA("�˺�����");
	}
	else
	{
		MessageBox("ע��ʧ��!","��ʾ");
	}
	CString TimeUsed = "����ʱ��: " + this->DrcomWeb_obj->GetAccountInfo_TimeUsed() + " ����";
	CString FlowUsed = "��������: " + this->DrcomWeb_obj->GetAccountInfo_FlowUsed();
	GetDlgItem(ID_TimeUsed_TEXT)->SetWindowTextA((LPCTSTR)TimeUsed);
	GetDlgItem(ID_FLOWUSED_TEXT)->SetWindowTextA((LPCTSTR)FlowUsed);
	GetDlgItem(ID_INTERNETSTATE_TEXT)->SetWindowTextA("����δ֪");
}


void CDrcomWebLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 1: // ��ʱ��1�����𱣳�����״̬
		if(this->DrcomWeb_obj->KeepOnline("www.baidu.com","chrome")) //������ͨ״̬�ж�
		{ //��ͨ
			GetDlgItem(ID_INTERNETSTATE_TEXT)->SetWindowTextA("������ͨ");
		}
		else
		{ //�ж�
			GetDlgItem(ID_INTERNETSTATE_TEXT)->SetWindowTextA("�����ж�");
		}
		break;
	case 2: //��ʱ��2�������ȡ�ʻ���½״̬
		if(this->DrcomWeb_obj->CheckOnline()) //�ж��˺��Ƿ�����
		{//����
			GetDlgItem(ID_ACCOUNTSTATE_TEXT)->SetWindowTextA("�˺�����");
			CString TimeUsed = "����ʱ��: " + this->DrcomWeb_obj->GetAccountInfo_TimeUsed() + " ����";
			CString FlowUsed = "��������: " + this->DrcomWeb_obj->GetAccountInfo_FlowUsed();
			GetDlgItem(ID_TimeUsed_TEXT)->SetWindowTextA((LPCTSTR)TimeUsed);
			GetDlgItem(ID_FLOWUSED_TEXT)->SetWindowTextA((LPCTSTR)FlowUsed);
		}
		else
		{//����
			GetDlgItem(ID_ACCOUNTSTATE_TEXT)->SetWindowTextA("�˺�����");
			if(this->ConnRetryOption) //�����ѡ�Զ�����
			{
				if(this->DrcomWeb_obj->Login()) //���µ�½�ɹ�
				{
					GetDlgItem(ID_ACCOUNTSTATE_TEXT)->SetWindowTextA("�˺�����");
				}
			}
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


