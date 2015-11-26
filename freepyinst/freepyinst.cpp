/*
** ����ƴ�����뷨��װ���򣬲ο���lost@SMTH�����ƴ�����뷨��װ����
** ��ԭ�е�register.c/setup.c��϶��ɡ�
**
** ��Ȩ���� (c) 2003, ����̩ <beos@turbolinux.com.cn>
** ��Ȩ���� (c) 1999-2003, ����
** ��Ȩ���� (c) 1999, lost@SMTh
**
** �������ѭ����ͨ�ù������֤2.0�汾(GNU GPL v2.0)�����������
**
*/  

#include <windows.h>
#include <tchar.h>
#include <imm.h>

#pragma comment (lib, "imm32")

#define CONVERSION_SET	0x6101
#define LAYOUTTEXT		_T("����ƴ�����뷨")
#define IMEFILE			_T("freepy.ime")
#define INST_TITLE		_T("��װ���뷨")

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


	//ȡ�ñ��ļ�ִ��·��������GetCurrentDirectory(400, sSrcDir)��������ڱ��Ŀ¼ִ�С�
	GetModuleFileName(hInstance, sModuleFile, sizeof(sModuleFile) - 1);
	GetFilePath(sModuleFile, sSrcDir);

	//ȡ��windowsϵͳĿ¼
	GetSystemDirectory(sDestDir, sizeof(sDestDir) - 1);

	//�����ļ�
	lstrcpy(sSrc, sSrcDir);
	lstrcat(sSrc, "\\binary\\");
	lstrcat(sSrc, "freepy.hlp");
	lstrcpy(sDest, sDestDir);
	lstrcat(sDest, "\\");
	lstrcat(sDest, "freepy.hlp");
	if (CopyFile(sSrc, sDest, FALSE) == 0) {
		MessageBox(NULL,
			_T("���뷨��װʧ�ܣ�\nfreepy.hlp����ʧ�ܡ�"),
			INST_TITLE,
			MB_OK | MB_ICONSTOP);
		return(FALSE);
	}

	//�����ļ�
	lstrcpy(sSrc, sSrcDir);
	lstrcat(sSrc, "\\binary\\");
	lstrcat(sSrc, "freepy.tab");
	lstrcpy(sDest, sDestDir);
	lstrcat(sDest, "\\");
	lstrcat(sDest, "freepy.tab");
	if(IsFileExist(sDest)) {
		if (MessageBox(NULL, _T("freepy.tab �Ѿ�����, ��Ҫ��������?\n"), INST_TITLE, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == IDYES) {
			if (CopyFile(sSrc, sDest, FALSE) == 0) {
				MessageBox(NULL,
					_T("���뷨��װʧ�ܣ�\nfreepy.tab����ʧ�ܡ�"),
					INST_TITLE,
					MB_OK | MB_ICONSTOP);
				return(FALSE);
			}
		}
	}
	
	//�����ļ�
	lstrcpy(sSrc, sSrcDir);
	lstrcat(sSrc, "\\binary\\");
	lstrcat(sSrc, "freepysys.dic");
	lstrcpy(sDest, sDestDir);
	lstrcat(sDest, "\\");
	lstrcat(sDest, "freepysys.dic");
	if(IsFileExist(sDest)) {
		if (MessageBox(NULL, _T("freepysys.dic �Ѿ�����, ��Ҫ��������?\n"), INST_TITLE, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == IDYES) {
			if (CopyFile(sSrc, sDest, FALSE) == 0) {
				MessageBox(NULL,
					_T("���뷨��װʧ�ܣ�\nfreepysys.dic����ʧ�ܡ�"),
					INST_TITLE,
					MB_OK | MB_ICONSTOP);
				return(FALSE);
			}
		}
	}
		
	//�����ļ�
	lstrcpy(sSrc, sSrcDir);
	lstrcat(sSrc, "\\binary\\");
	lstrcat(sSrc, "freepy.ime");
	lstrcpy(sDest, sDestDir);
	lstrcat(sDest, "\\");
	lstrcat(sDest, "freepy.ime");
	if(IsFileExist(sDest)) {
		if (MessageBox(NULL, _T("freepy.ime �Ѿ�����, ��Ҫ��������?\n"), INST_TITLE, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == IDYES) {
			if (CopyFile(sSrc, sDest, FALSE) == 0) {
				MessageBox(NULL,
					_T("���뷨��װʧ�ܣ�\nfreepy.ime����ʧ�ܡ�"),
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

	if (MessageBox(NULL, _T("��Ҫ��װ����ƴ�����뷨��\n"), INST_TITLE, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) != IDYES) {
		MessageBox(NULL,
			    _T("�˳���װ"),
			    INST_TITLE,
			    MB_OK | MB_ICONWARNING);
		return(FALSE);
	}

	if(!CopyIMEFile(hInstance, sImeFile))
		return FALSE;
	
	//���뷨ע��
	hklCurrent = ImmInstallIME(sImeFile, LAYOUTTEXT);
	
	// register freepy in HKEY_LOCAL_MACHINE\Software\freepy 
	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("Software"),
		0,
		KEY_READ,
		&hKey) != ERROR_SUCCESS ){
		MessageBox(NULL, _T("��ע����д�Softwareע�����ʱ����\n"), INST_TITLE, MB_OK | MB_ICONSTOP);
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
		MessageBox(NULL, _T("��ע����д���freepyע�����ʱ����"), INST_TITLE, MB_OK | MB_ICONSTOP);
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
	
	//ȷ�ϰ�װ״���������ɹ�Ӧ��ɾ���ļ���
	MessageBox(NULL, 
		_T("����ƴ�����뷨��װ�ɹ���"),
		INST_TITLE,
		MB_OK | MB_ICONINFORMATION);

	return(TRUE);
}