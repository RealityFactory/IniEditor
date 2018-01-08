/************************************************************************************//**
 * @file MFileDlg.cpp
 * @brief MFileDlg class encapsulates OPENFILENAME and GetOpenFileName
 ****************************************************************************************/

#include "windows.h"
#include <commdlg.h>
#include <tchar.h>
#include "MFileDlg.h"

// class MFileDlg implementation

MFileDlg::MFileDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter)
{
	memset(&m_ofn, 0, sizeof(m_ofn)); // initialize structure to 0/NULL
	m_szFileName[0] = '\0';
	m_szFileTitle[0] = '\0';
	m_pofnTemp = NULL;

	m_ofn.lStructSize       = sizeof(OPENFILENAME);
	m_ofn.lpstrFile         = m_szFileName;
	m_ofn.nMaxFile          = sizeof(m_szFileName)/sizeof(m_szFileName[0]);
	m_ofn.lpstrDefExt       = lpszDefExt;
	m_ofn.lpstrFileTitle    = m_szFileTitle;
    m_ofn.nMaxFileTitle     = sizeof(m_szFileTitle)/sizeof(m_szFileTitle[0]);
	m_ofn.Flags             = dwFlags;
	m_ofn.lpstrFilter       = lpszFilter;

	// setup initial file name
	if(lpszFileName != NULL)
		lstrcpyn(m_szFileName, lpszFileName, sizeof(m_szFileName)/sizeof(m_szFileName[0]));
}

MFileDlg::~MFileDlg()
{
}

void MFileDlg::SetInitalDir(LPCTSTR dir)
{
	m_ofn.lpstrInitialDir = dir;
}

int MFileDlg::DoModal()
{
	DWORD nOffset = lstrlen(m_ofn.lpstrFile)+1;
	if(nOffset > m_ofn.nMaxFile)
		return IDABORT;

	memset(m_ofn.lpstrFile+nOffset, 0, (m_ofn.nMaxFile-nOffset)*sizeof(TCHAR));

	if(GetOpenFileName(&m_ofn))
		return IDOK;

	return IDCANCEL;
}


LPCTSTR MFileDlg::GetFileName() const
{
	return m_ofn.lpstrFileTitle;
}


LPCTSTR MFileDlg::GetNextPathName(POSITION& pos) const
{
	TCHAR chDelimiter = ' ';

	LPTSTR lpsz = (LPTSTR)pos;
	if(lpsz == m_ofn.lpstrFile) // first time
	{
		pos = NULL;
		return m_ofn.lpstrFile;
	}

	static TCHAR strPathName[_MAX_PATH];

	lstrcpyn(strPathName, m_ofn.lpstrFile, sizeof(strPathName)/sizeof(strPathName[0]));
	{
		LPTSTR lpszPath = m_ofn.lpstrFile;
		while(*lpszPath != chDelimiter)
			lpszPath = _tcsinc(lpszPath);
		if(lpszPath - m_ofn.lpstrFile < sizeof(strPathName)/sizeof(strPathName[0]))
			strPathName[lpszPath - m_ofn.lpstrFile] = '\0';
	}

	LPTSTR lpszFileName = lpsz;
	TCHAR strFileName[_MAX_PATH];
	lstrcpyn(strFileName, lpsz, sizeof(strFileName)/sizeof(strFileName[0]));

	// find char pos at next Delimiter
	while(*lpsz != chDelimiter && *lpsz != '\0')
		lpsz = _tcsinc(lpsz);

	if(*lpsz == '\0')
		pos = NULL;
	else
	{
		strFileName[lpsz - lpszFileName] = '\0';

		lpsz = _tcsinc(lpsz);
		if(*lpsz == '\0') // if double terminated then done
			pos = NULL;
		else
			pos = (POSITION)lpsz;
	}

	// only add '\\' if it is needed
	if(strPathName[0])
	{
		// check for last back-slash or forward slash (handles DBCS)
		LPCTSTR lpsz = _tcsrchr(strPathName, '\\');
		if(lpsz == NULL)
			lpsz = _tcsrchr(strPathName, '/');
		// if it is also the last character, then we don't need an extra
		if(lpsz != NULL &&
			(lpsz - (LPCTSTR)strPathName) == lstrlen(strPathName)-1)
		{
			lstrcat(strPathName, strFileName);
			return  strPathName;
		}
	}

	lstrcat(strPathName, (LPCTSTR)("\\"));
	lstrcat(strPathName, strFileName);

	return strPathName;
}
