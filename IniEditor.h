// IniEditor.h : main header file for the INIEDITOR application
//

#if !defined(AFX_INIEDITOR_H__7B0FA1E4_D330_11D4_85EA_0060674A702D__INCLUDED_)
#define AFX_INIEDITOR_H__7B0FA1E4_D330_11D4_85EA_0060674A702D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIniEditorApp:
// See IniEditor.cpp for the implementation of this class
//

class CIniEditorApp : public CWinApp
{
public:
	CIniEditorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIniEditorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIniEditorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INIEDITOR_H__7B0FA1E4_D330_11D4_85EA_0060674A702D__INCLUDED_)
