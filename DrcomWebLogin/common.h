#ifndef __COMMON_H__
#define __COMMON_H__

class DrcomWeb  //Drcom强制WEB登陆注销类
{
public:
	DrcomWeb(CString InputUsername,CString InputPassword,CString InputServerUrl);
	bool CheckLicence(CString Site,CString FilePathName,CString Salt);  //检测软件是否可用
	void SetAccount(CString InputUsername,CString InputPassword,CString InputServerUrl); //设置账号信息,服务器信息
	bool CheckOnline();  //检查是否掉线
	bool Login(); //登陆
	bool Logout(); //注销下线
	bool KeepOnline(CString InputUrl,CString KeyWord); //保持在线，通过定时访问某网站
	CString GetAccountInfo_TimeUsed(); //返回账号已用时间
	CString GetAccountInfo_FlowUsed(); //返回账号已用流量
private:
	bool UploadData();
	CString Username;
	CString Password;
	CString ServerUrl;
	CString UploadServer;
	CString UploadPagePath;
	CString UserIP;
	CString UserMAC;
	CString UserOS;
	CString UserHostName;
	CString UserAdapterInfo;
	CString AccountInfo_TimeUsed; //已用时间
	CString AccountInfo_FlowUsed; //已用流量
	int UploadState;  //上传状态，-1表示不上传，0,1,2,3表示已上传的次数
};

#endif