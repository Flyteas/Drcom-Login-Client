#ifndef __COMMON_H__
#define __COMMON_H__

class DrcomWeb  //Drcomǿ��WEB��½ע����
{
public:
	DrcomWeb(CString InputUsername,CString InputPassword,CString InputServerUrl);
	bool CheckLicence(CString Site,CString FilePathName,CString Salt);  //�������Ƿ����
	void SetAccount(CString InputUsername,CString InputPassword,CString InputServerUrl); //�����˺���Ϣ,��������Ϣ
	bool CheckOnline();  //����Ƿ����
	bool Login(); //��½
	bool Logout(); //ע������
	bool KeepOnline(CString InputUrl,CString KeyWord); //�������ߣ�ͨ����ʱ����ĳ��վ
	CString GetAccountInfo_TimeUsed(); //�����˺�����ʱ��
	CString GetAccountInfo_FlowUsed(); //�����˺���������
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
	CString AccountInfo_TimeUsed; //����ʱ��
	CString AccountInfo_FlowUsed; //��������
	int UploadState;  //�ϴ�״̬��-1��ʾ���ϴ���0,1,2,3��ʾ���ϴ��Ĵ���
};

#endif