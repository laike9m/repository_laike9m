
// Bad Apple Console.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBadAppleConsoleApp:
// �йش����ʵ�֣������ Bad Apple Console.cpp
//

class CBadAppleConsoleApp : public CWinApp
{
public:
	CBadAppleConsoleApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBadAppleConsoleApp theApp;