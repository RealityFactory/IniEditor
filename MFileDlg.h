
#if !defined(__MFILEDLG_H__)
#define __MFILEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// abstract iteration position
struct __POSITION { };
typedef __POSITION* POSITION;

//
// MFileDlg
//
class MFileDlg
{

public:
	MFileDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL , LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	LPCTSTR lpszFilter = NULL);

	virtual ~MFileDlg();

	int DoModal();
	void SetInitalDir(LPCTSTR dir);

	/**
	 * @brief retrieving file full name
	 **/
	LPCTSTR GetFileName() const;
	/**
	 * @brief retrieving file title
	 **/
	LPCTSTR GetFileTitle() const { return m_szFileTitle; }

	LPCTSTR GetNextPathName(POSITION& pos) const;

	POSITION GetStartPosition() const { return (POSITION)m_ofn.lpstrFile; }

	// members
public:
	OPENFILENAME  m_ofn;

private:
    TCHAR m_szFileTitle[_MAX_PATH];	// contains file title after return
	TCHAR m_szFileName[_MAX_PATH];	// contains full path name after return
	OPENFILENAME*  m_pofnTemp;
};

#endif // !defined(__MFILEDLG_H__)