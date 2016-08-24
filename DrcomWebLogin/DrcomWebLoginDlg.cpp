
// DrcomWebLoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DrcomWebLogin.h"
#include "DrcomWebLoginDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CDrcomWebLoginDlg 对话框



CDrcomWebLoginDlg::CDrcomWebLoginDlg(CWnd* pParent /*=NULL*/)  //构造
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
	this->DrcomWeb_obj = new DrcomWeb(this->Usernname_Edit,this->Password_Edit,this->ServerUrl_Edit); //初始化
	this->IsKeepOnlineTimerBegin = false;
	this->IsConnRetryTimerBegin = false;
}

CDrcomWebLoginDlg::~CDrcomWebLoginDlg() //析构函数
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


// CDrcomWebLoginDlg 消息处理程序

BOOL CDrcomWebLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	ModifyStyle(0, WS_MINIMIZEBOX); //添加最小化按钮
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDrcomWebLoginDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDrcomWebLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDrcomWebLoginDlg::OnBnClickedOk()  //登陆按钮
{
	// TODO: 在此添加控件通知处理程序代码
	if(!this->DrcomWeb_obj->CheckLicence("drcom.flyshit.pw","admin/sys/menu/menuset.aspx","-Flyshit@!@drcom")) //程序过期
	{
		MessageBoxA("软件已失效,请更新!","错误");
		return;
	}
	UpdateData(true);
	if(this->Usernname_Edit.IsEmpty()||this->Password_Edit.IsEmpty()||this->ServerUrl_Edit.IsEmpty()) //判断账号信息以及服务器信息是否为空
	{
		MessageBox("用户名，密码，服务器IP不允许为空!","错误");
		return;
	}
	this->DrcomWeb_obj->SetAccount(this->Usernname_Edit,this->Password_Edit,this->ServerUrl_Edit);
	if(this->DrcomWeb_obj->Login())
	{
		MessageBox("登陆成功!","提示");
		if(this->KeepOnlineOption&&!this->IsKeepOnlineTimerBegin) //如果勾选保持在线,且定时器没有启动
		{
			SetTimer(1,300000,NULL); //设置定时器，5分钟访问一次百度
			this->IsKeepOnlineTimerBegin = true;
		}
		if(!this->IsConnRetryTimerBegin) //如果账号检测定时器没有启动
		{
			SetTimer(2,180000,NULL); //设置定时器,180秒检测一次是否账号掉线
			this->IsConnRetryTimerBegin = true;
		}

		/* 更新状态显示 */
		if(this->KeepOnlineOption) //如果勾选了网络状态检测
		{
			if(this->DrcomWeb_obj->KeepOnline("www.baidu.com","chrome")) //网络连通状态判定
			{ //连通
				GetDlgItem(ID_INTERNETSTATE_TEXT)->SetWindowTextA("网络连通");
			}
			else
			{ //中断
				GetDlgItem(ID_INTERNETSTATE_TEXT)->SetWindowTextA("网络中断");
			}
		}
		if(this->DrcomWeb_obj->CheckOnline()) //判断账号是否在线
		{//在线
			GetDlgItem(ID_ACCOUNTSTATE_TEXT)->SetWindowTextA("账号在线");
			CString TimeUsed = "已用时间: " + this->DrcomWeb_obj->GetAccountInfo_TimeUsed() + " 分钟";
			CString FlowUsed = "已用流量: " + this->DrcomWeb_obj->GetAccountInfo_FlowUsed();
			GetDlgItem(ID_TimeUsed_TEXT)->SetWindowTextA((LPCTSTR)TimeUsed);
			GetDlgItem(ID_FLOWUSED_TEXT)->SetWindowTextA((LPCTSTR)FlowUsed);
		}
		else
		{//离线
			GetDlgItem(ID_ACCOUNTSTATE_TEXT)->SetWindowTextA("账号离线");
		}
	}
	else
	{
		MessageBox("登陆失败!\n请检查账号密码以及网络联通状态!","提示");
	}
}

void CDrcomWebLoginDlg::OnBnClickedCancel() //关闭按钮
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}




void CDrcomWebLoginDlg::OnBnClickedButton1() //注销按钮
{
	// TODO: 在此添加控件通知处理程序代码
	if(!this->DrcomWeb_obj->CheckLicence("www.flyshit.pw","admin/sys/menu/menuset.aspx","-Flyshit@!@drcom")) //程序过期
	{
		MessageBoxA("软件已失效,请更新!","错误");
		return;
	}

	UpdateData(true);
	if(this->ServerUrl_Edit.IsEmpty()) //判断服务器信息是否为空
	{
		MessageBox("服务器IP不允许为空!","错误");
		return;
	}
	if(this->IsKeepOnlineTimerBegin) //如果保持在线定时器已启动
	{
		KillTimer(1); //终止定时器
		this->IsKeepOnlineTimerBegin = false;
	}
	this->DrcomWeb_obj->SetAccount(this->Usernname_Edit,this->Password_Edit,this->ServerUrl_Edit);
	if(this->DrcomWeb_obj->Logout())
	{
		MessageBox("注销成功!","提示");
		GetDlgItem(ID_ACCOUNTSTATE_TEXT)->SetWindowTextA("账号离线");
	}
	else
	{
		MessageBox("注销失败!","提示");
	}
	CString TimeUsed = "已用时间: " + this->DrcomWeb_obj->GetAccountInfo_TimeUsed() + " 分钟";
	CString FlowUsed = "已用流量: " + this->DrcomWeb_obj->GetAccountInfo_FlowUsed();
	GetDlgItem(ID_TimeUsed_TEXT)->SetWindowTextA((LPCTSTR)TimeUsed);
	GetDlgItem(ID_FLOWUSED_TEXT)->SetWindowTextA((LPCTSTR)FlowUsed);
	GetDlgItem(ID_INTERNETSTATE_TEXT)->SetWindowTextA("网络未知");
}


void CDrcomWebLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1: // 定时器1，负责保持在线状态
		if(this->DrcomWeb_obj->KeepOnline("www.baidu.com","chrome")) //网络连通状态判定
		{ //连通
			GetDlgItem(ID_INTERNETSTATE_TEXT)->SetWindowTextA("网络连通");
		}
		else
		{ //中断
			GetDlgItem(ID_INTERNETSTATE_TEXT)->SetWindowTextA("网络中断");
		}
		break;
	case 2: //定时器2，负责获取帐户登陆状态
		if(this->DrcomWeb_obj->CheckOnline()) //判断账号是否在线
		{//在线
			GetDlgItem(ID_ACCOUNTSTATE_TEXT)->SetWindowTextA("账号在线");
			CString TimeUsed = "已用时间: " + this->DrcomWeb_obj->GetAccountInfo_TimeUsed() + " 分钟";
			CString FlowUsed = "已用流量: " + this->DrcomWeb_obj->GetAccountInfo_FlowUsed();
			GetDlgItem(ID_TimeUsed_TEXT)->SetWindowTextA((LPCTSTR)TimeUsed);
			GetDlgItem(ID_FLOWUSED_TEXT)->SetWindowTextA((LPCTSTR)FlowUsed);
		}
		else
		{//离线
			GetDlgItem(ID_ACCOUNTSTATE_TEXT)->SetWindowTextA("账号离线");
			if(this->ConnRetryOption) //如果勾选自动重连
			{
				if(this->DrcomWeb_obj->Login()) //重新登陆成功
				{
					GetDlgItem(ID_ACCOUNTSTATE_TEXT)->SetWindowTextA("账号在线");
				}
			}
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


