
#if !defined(AFX_MFILEDLG_H__D47E1646_D06E_11D4_85EA_0060674A702D__INCLUDED_)
#define AFX_MFILEDLG_H__D47E1646_D06E_11D4_85EA_0060674A702D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
// MFileDlg
//
class MFileDlg : public CFileDialog
{
	DECLARE_DYNAMIC(MFileDlg)
		
public:
	MFileDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL , LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT/* | OFN_ALLOWMULTISELECT*/,
		LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL) ;
	
	int DoModal();
	void SetInitalDir(char *dir);
	virtual ~MFileDlg();
	
protected:
	//{{AFX_MSG(MFileDlg)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	
	TCHAR* m_pszFile;
	DECLARE_MESSAGE_MAP()
};

#endif