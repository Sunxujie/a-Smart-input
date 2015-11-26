/*
** 自由拼音输入法安装程序，参考了lost@SMTH的五笔拼音输入法安装程序，
** 与原有的register.c/setup.c结合而成。
**
** 版权所无 (c) 2003, 梁昌泰 <beos@turbolinux.com.cn>
** 版权所有 (c) 1999-2003, 李振春
** 版权所有 (c) 1999, lost@SMTh
**
** 本软件遵循葛驽通用公共许可证2.0版本(GNU GPL v2.0)各项条款发布。
**
*/  

#include <windows.h>
#include <tchar.h>
#include <imm.h>

#pragma comment (lib, "imm32")

#define CONVERSION_SET	0x6101
#define LAYOUTTEXT		_T("自由拼音输入法")
#define IMEFILE			_T("freepy.ime")
#define INST_TITLE		_T("安装输入法")

//int CheckOldVersion();
//int CopyIMEComponent();
//int RemoveOldVersion();
//int UninstallIME();

//ExtractPath(sModuleFile, sSrcDir);
int GetFilePath(TCHAR *FileName, TCHAR *Path) 
{
	int i;
	i = lstrlen(FileName) - 1;
	while (FileName[i] != '\\' && i > 1)
		i--;
	lstrcpy(Path, FileName);
	Path[i] = 0;
	return 0;
}

BOOL IsFileExist(const TCHAR *file)
{
	WIN32_FIND_DATA data;
	HANDLE handle;
	
	if(!file)
		return FALSE;
	
	handle = FindFirstFile(file, &data);
	if(handle == INVALID_HANDLE_VALUE)
		return FALSE;
	
	FindClose(handle);
	return TRUE;
}

BOOL CopyIMEFile(HINSTANCE hInstance, TCHAR* sImeFile)
{
	TCHAR	sModuleFile[MAX_PATH];
	TCHAR	sSrcDir[MAX_PATH], sDestDir[MAX_PATH];
	TCHAR	sSrc[MAX_PATH], sDest[MAX_PATH];


	//取得本文件执行路径。若用GetCurrentDirectory(400, sSrcDir)则可能是在别的目录执行。
	GetModuleFileName(hInstance, sModuleFile, sizeof(sModuleFile) - 1);
	GetFilePath(sModuleFile, sSrcDir);

	//取得windows系统目录
	GetSystemDirectory(sDestDir, sizeof(sDestDir) - 1);

	//复制文件
	lstrcpy(sSrc, sSrcDir);
	lstrcat(sSrc, "\\binary\\");
	lstrcat(sSrc, "freepy.hlp");
	lstrcpy(sDest, sDestDir);
	lstrcat(sDest, "\\");
	lstrcat(sDest, "freepy.hlp");
	if (CopyFile(sSrc, sDest, FALSE) == 0) {
		MessageBox(NULL,
			_T("输入法安装失败！\nfreepy.hlp复制失败。"),
			INST_TITLE,
			MB_OK | MB_ICONSTOP);
		return(FALSE);
	}

	//复制文件
	lstrcpy(sSrc, sSrcDir);
	lstrcat(sSrc, "\\binary\\");
	lstrcat(sSrc, "freepy.tab");
	lstrcpy(sDest, sDestDir);
	lstrcat(sDest, "\\");
	lstrcat(sDest, "freepy.tab");
	if(IsFileExist(sDest)) {
		if (MessageBox(NULL, _T("freepy.tab 已经存在, 您要覆盖它吗?\n"), INST_TITLE, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == IDYES) {
			if (CopyFile(sSrc, sDest, FALSE) == 0) {
				MessageBox(NULL,
					_T("输入法安装失败！\nfreepy.tab复制失败。"),
					INST_TITLE,
					MB_OK | MB_ICONSTOP);
				return(FALSE);
			}
		}
	}
	
	//复制文件
	lstrcpy(sSrc, sSrcDir);
	lstrcat(sSrc, "\\binary\\");
	lstrcat(sSrc, "freepysys.dic");
	lstrcpy(sDest, sDestDir);
	lstrcat(sDest, "\\");
	lstrcat(sDest, "freepysys.dic");
	if(IsFileExist(sDest)) {
		if (MessageBox(NULL, _T("freepysys.dic 已经存在, 您要覆盖它吗?\n"), INST_TITLE, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == IDYES) {
			if (CopyFile(sSrc, sDest, FALSE) == 0) {
				MessageBox(NULL,
					_T("输入法安装失败！\nfreepysys.dic复制失败。"),
					INST_TITLE,
					MB_OK | MB_ICONSTOP);
				return(FALSE);
			}
		}
	}
		
	//复制文件
	lstrcpy(sSrc, sSrcDir);
	lstrcat(sSrc, "\\binary\\");
	lstrcat(sSrc, "freepy.ime");
	lstrcpy(sDest, sDestDir);
	lstrcat(sDest, "\\");
	lstrcat(sDest, "freepy.ime");
	if(IsFileExist(sDest)) {
		if (MessageBox(NULL, _T("freepy.ime 已经存在, 您要覆盖它吗?\n"), INST_TITLE, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == IDYES) {
			if (CopyFile(sSrc, sDest, FALSE) == 0) {
				MessageBox(NULL,
					_T("输入法安装失败！\nfreepy.ime复制失败。"),
					INST_TITLE,
					MB_OK | MB_ICONSTOP);
				return(FALSE);
			}
		}
	}

	lstrcpy(sImeFile, sDest);
	return TRUE;
}

int APIENTRY 
WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	HKL		hklCurrent;
	HKEY	hKey,hChildKey;
	DWORD	dwDisposition;
	DWORD	dwConversionSet;
	TCHAR   sImeFile[MAX_PATH];

	if (MessageBox(NULL, _T("您要安装自由拼音输入法吗？\n"), INST_TITLE, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) != IDYES) {
		MessageBox(NULL,
			    _T("退出安装"),
			    INST_TITLE,
			    MB_OK | MB_ICONWARNING);
		return(FALSE);
	}

	if(!CopyIMEFile(hInstance, sImeFile))
		return FALSE;
	
	//输入法注册
	hklCurrent = ImmInstallIME(sImeFile, LAYOUTTEXT);
	
	// register freepy in HKEY_LOCAL_MACHINE\Software\freepy 
	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("Software"),
		0,
		KEY_READ,
		&hKey) != ERROR_SUCCESS ){
		MessageBox(NULL, _T("在注册表中打开Software注册表项时出错！\n"), INST_TITLE, MB_OK | MB_ICONSTOP);
		return(FALSE);
	}
	
	if( RegCreateKeyEx( hKey,
		_T("freepy"),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE ,
		KEY_ALL_ACCESS ,
		NULL,
		&hChildKey,
		&dwDisposition ) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		MessageBox(NULL, _T("在注册表中创建freepy注册表项时出错！"), INST_TITLE, MB_OK | MB_ICONSTOP);
		return(FALSE);
	}
	
	dwConversionSet = CONVERSION_SET;
	RegSetValueEx(hChildKey,
		_T("ConversionSet"),
		0,
		REG_DWORD,
		(LPBYTE)&dwConversionSet,
		sizeof(DWORD));
	
	RegCloseKey(hChildKey);
	RegCloseKey(hKey);
	
	//确认安装状况，若不成功应当删除文件。
	MessageBox(NULL, 
		_T("自由拼音输入法安装成功！"),
		INST_TITLE,
		MB_OK | MB_ICONINFORMATION);

	return(TRUE);
}