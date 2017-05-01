// MFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFileDlg.h"


//
// Implementation of CFileDialog clone
//

IMPLEMENT_DYNAMIC(MFileDlg, CFileDialog)

MFileDlg::MFileDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
				   DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_pszFile = new TCHAR[2048]; //set a 2K buffer to hold selected files
	m_pszFile[0] = '\0'; //initialize pointer;
	m_ofn.lpstrInitialDir = NULL;
}

BEGIN_MESSAGE_MAP(MFileDlg, CFileDialog)
//{{AFX_MSG_MAP(MFileDlg)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

MFileDlg::~MFileDlg()
{
	if (m_pszFile != NULL)
		delete [] m_pszFile; //cleanup
}

void MFileDlg::SetInitalDir(char *dir)
{
	m_ofn.lpstrInitialDir = dir;
}

int MFileDlg::DoModal()
{
	ASSERT_VALID(this);
	//ASSERT(m_ofn.Flags & OFN_ALLOWMULTISELECT); //make sure multiple file selection is on
	
	m_ofn.lpstrFile = m_pszFile; //initialize the OPENFILENAME structure
	m_ofn.nMaxFile = 2048;
	
	return CFileDialog::DoModal();
}
