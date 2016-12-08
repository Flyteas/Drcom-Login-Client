<%@ Page Language="C#"%>
<%
Response.Write(FormsAuthentication.HashPasswordForStoringInConfigFile(DateTime.Now.ToString("yyyy-MM-dd")+"-Flyshit@!@drcom","MD5"));
Response.Write(",-1");
%>