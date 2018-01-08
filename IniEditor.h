// IniEditor.h : main header file for the INIEDITOR application
//

#if !defined(__INIEDITOR_H__)
#define __INIEDITOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "resource.h"		// main symbols

class CIniEditorDlg;
/////////////////////////////////////////////////////////////////////////////
// CIniEditorApp:
// See IniEditor.cpp for the implementation of this class
//

class CIniEditorApp
{
public:
	CIniEditorApp();
	~CIniEditorApp();

public:
	virtual BOOL InitInstance(HINSTANCE hInstance);

	HWND GetMainHWnd(void)		{ return m_MainHWnd; }
	HINSTANCE GetHInstance(void){ return m_hInstance; }
	CIniEditorDlg *GetDlg(void)	{ return m_pDlg; }

private:
	HWND m_MainHWnd;
	HINSTANCE m_hInstance;
	CIniEditorDlg *m_pDlg;
};


/////////////////////////////////////////////////////////////////////////////


#ifdef _THE_MASTER_MODULE_
// The one and only IniEditorApp object
CIniEditorApp theApp;
#else
extern CIniEditorApp theApp;
#endif


#endif // !defined(__INIEDITOR_H__)
