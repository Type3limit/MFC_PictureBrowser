
// PicBrowser.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPicBrowserApp: 
// �йش����ʵ�֣������ PicBrowser.cpp
//

class CPicBrowserApp : public CWinApp
{
public:
	CPicBrowserApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPicBrowserApp theApp;