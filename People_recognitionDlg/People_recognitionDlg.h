
// People_recognitionDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPeople_recognitionDlgApp: 
// �йش����ʵ�֣������ People_recognitionDlg.cpp
//

class CPeople_recognitionDlgApp : public CWinApp
{
public:
	CPeople_recognitionDlgApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPeople_recognitionDlgApp theApp;