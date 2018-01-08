// IniEditorDlg.h : header file
//

#if !defined(__INIEDITORDLG_H__)
#define __INIEDITORDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <string>
#include "IniEditor.h"

// CIniEditorDlg dialog

class CIniEditorDlg
{
// Construction
public:
	CIniEditorDlg(HWND hwndParent = NULL);	// standard constructor
	~CIniEditorDlg();
	BOOL Create();
	BOOL IsInitialized()	{ return m_bInit; }
	HWND GetHWnd()			{ return m_hWnd; }


	BOOL m_fullscreen;
	int  m_width;
	int  m_height;
	char m_driver[32];

protected:
// Dialog Data
	enum { IDD = IDD_INIEDITOR_DIALOG };

	std::string	m_actorname;
	std::string	m_menuname;
	std::string	m_levelname;
	std::string	m_gamename;
	std::string	m_packfile;
	std::string	m_LevelDir;
	std::string	m_BitmapDir;
	std::string	m_ActorDir;
	std::string	m_AudioDir;
	std::string	m_AudioStreamDir;
	std::string	m_VideoDir;
	std::string	m_MIDIDir;
	BOOL	m_usefirst;
	BOOL	m_usecutscene;
	std::string	m_splashscreen;
	std::string	m_splashaudio;
	std::string	m_cutscene;
	BOOL	m_usesecond;
	BOOL	m_usecutscene2;
	std::string	m_splashscreen2;
	std::string	m_splashaudio2;
	std::string	m_cutscene2;
	BOOL	m_weapon;
	BOOL	m_usedialog;
	BOOL	m_showtrack;
	BOOL	m_usecselect;
	BOOL	m_difficult;
	std::string	m_ddif;
	std::string	m_dlang;
	BOOL	m_usenselect;
	std::string	m_playername;
	BOOL	m_usedirectinput;
	BOOL	m_logging;

	TCHAR m_currentdir[512];

protected:
	HWND m_hWnd;
	BOOL m_bInit;


public:
	HICON m_hIcon;
	int count;

	BOOL UpdateData(BOOL bSaveAndValidate = TRUE);

	virtual BOOL OnInitDialog();
	void OnOk();
	void OnPaint();
	HCURSOR OnQueryDragIcon();
	void OnActbrowse();
	void OnMenuini();
	void OnStartlevel();
	void OnPack();
	void OnCbutton();
	void OnSabutton();
	void OnSsbutton();
	void OnSsbutton2();
	void OnSabutton2();
	void OnCbutton2();
	void OnDdif();
	void OnDlang();
};

#endif // !defined(__INIEDITORDLG_H__)
