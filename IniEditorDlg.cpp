// IniEditorDlg.cpp : implementation file
//

#include <windows.h>
#include <tchar.h>
#include "IniEditor.h"
#include "IniEditorDlg.h"
#include "MFileDlg.h"
#include <direct.h>


static LRESULT wm_Command(
		HWND hwnd,
		WORD /* wNotifyCode */,
		WORD wID,
		HWND /* hwndCtl */)
{
	switch(wID)
	{
	case IDOK:
		theApp.GetDlg()->OnOk();
		return 0;
	case IDCANCEL:
		DestroyWindow(hwnd);
		return 0;
	case IDC_SSBUTTON2:
		theApp.GetDlg()->OnSsbutton2();
		return 0;
	case IDC_SABUTTON2:
		theApp.GetDlg()->OnSabutton2();
		return 0;
	case IDC_CBUTTON2:
		theApp.GetDlg()->OnCbutton2();
		return 0;
	case IDC_SSBUTTON:
		theApp.GetDlg()->OnSsbutton();
		return 0;
	case IDC_SABUTTON:
		theApp.GetDlg()->OnSabutton();
		return 0;
	case IDC_CBUTTON:
		theApp.GetDlg()->OnCbutton();
		return 0;
	case IDC_PACK:
		theApp.GetDlg()->OnPack();
		return 0;
	case IDC_STARTLEVEL:
		theApp.GetDlg()->OnStartlevel();
		return 0;
	case IDC_MENUINI:
		theApp.GetDlg()->OnMenuini();
		return 0;
	case IDC_ACTBROWSE:
		theApp.GetDlg()->OnActbrowse();
		return 0;
	case IDC_DDIFF:
		theApp.GetDlg()->OnDdif();
		return 0;
	case IDC_DLANG:
		theApp.GetDlg()->OnDlang();
		return 0;
	}

	return 0;
}

static BOOL CALLBACK IniEditor_DlgProc(
		HWND hwndDlg,	// handle to dialog box
		UINT uMsg,		// message
		WPARAM wParam,	// first message parameter
		LPARAM lParam)	// second message parameter
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		{
			WORD wNotifyCode = HIWORD(wParam);
			WORD wID = LOWORD(wParam);
			HWND hwndCtl = (HWND)lParam;

			return wm_Command(hwndDlg, wNotifyCode, wID, hwndCtl);
		}
	case WM_QUERYDRAGICON:
		{
			theApp.GetDlg()->OnQueryDragIcon();
			return 0;
		}
	case WM_PAINT:
		{
			theApp.GetDlg()->OnPaint();
			return 0;
		}
	}

	return FALSE;
}

// CIniEditorDlg dialog

CIniEditorDlg::CIniEditorDlg(HWND pParent /*= NULL*/)
{
	m_actorname = _T("");
	m_menuname = _T("");
	m_levelname = _T("");
	m_gamename = _T("");
	m_LevelDir			= _T("media\\levels");
	m_BitmapDir			= _T("media\\bitmaps");
	m_ActorDir			= _T("media\\actors");
	m_AudioDir			= _T("media\\audio");
	m_AudioStreamDir	= _T("media\\audio");
	m_VideoDir			= _T("media\\video");
	m_MIDIDir			= _T("media\\midi");
	m_weapon = FALSE;
	m_packfile = _T("");
	m_usedialog = FALSE;
	m_usecutscene = FALSE;
	m_splashscreen = _T("");
	m_splashaudio = _T("");
	m_cutscene = _T("");
	m_usecutscene2 = FALSE;
	m_cutscene2 = _T("");
	m_splashaudio2 = _T("");
	m_usesecond = FALSE;
	m_splashscreen2 = _T("");
	m_usefirst = FALSE;
	m_showtrack = FALSE;
	m_usecselect = FALSE;
	m_difficult = FALSE;
	m_ddif = _T("1");
	m_dlang = _T("1");
	m_usenselect = FALSE;
	m_playername = _T("");
	m_usedirectinput = TRUE;
	m_logging = FALSE;

	m_hWnd = NULL;
	m_bInit = FALSE;

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = LoadIcon(theApp.GetHInstance(), MAKEINTRESOURCE(IDR_MAINFRAME));
}

CIniEditorDlg::~CIniEditorDlg()
{
}

BOOL CIniEditorDlg::Create()
{
	m_hWnd = CreateDialog(
		theApp.GetHInstance(),
		MAKEINTRESOURCE(IDD_INIEDITOR_DIALOG),
		NULL,
		IniEditor_DlgProc);

	if(m_hWnd == NULL)
		return FALSE;

	m_bInit = OnInitDialog();

	return m_bInit;
}

/////////////////////////////////////////////////////////////////////////////
// CIniEditorDlg message handlers

BOOL CIniEditorDlg::OnInitDialog()
{
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SendMessage(m_hWnd, WM_SETICON, TRUE, (LPARAM)m_hIcon);		// Set big icon
	SendMessage(m_hWnd, WM_SETICON, FALSE, (LPARAM)m_hIcon);	// Set small icon

	SendDlgItemMessage(m_hWnd, IDC_PLAYERNAME, EM_SETLIMITTEXT, (WPARAM)63, (LPARAM)0);
	SendDlgItemMessage(m_hWnd, IDC_DDIFF, EM_SETLIMITTEXT, (WPARAM)1, (LPARAM)0);
	SendDlgItemMessage(m_hWnd, IDC_DLANG, EM_SETLIMITTEXT, (WPARAM)1, (LPARAM)0);

	// center dialog on desktop
	RECT WndRect, ScreenRect;
	int Width, Height;

	GetWindowRect(m_hWnd, &WndRect);
	Width = WndRect.right - WndRect.left;
	Height = WndRect.bottom - WndRect.top;

	GetWindowRect(GetDesktopWindow(), &ScreenRect);
	SetWindowPos(m_hWnd, 0, (((ScreenRect.right - ScreenRect.left) / 2) - (Width / 2)),
				 (((ScreenRect.bottom - ScreenRect.top) / 2) - (Height / 2)),
				  Width, Height, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);

	ShowWindow(m_hWnd, SW_SHOWNORMAL);

	_chdir("..");
	_getcwd(m_currentdir, 512);


	FILE *fdInput = NULL;
	char szInputString[1024] = {""};
	char szOutputString[1024] = {""};
	char *szAtom = NULL;

	fdInput = fopen("RealityFactory.ini", "rt");
	if(!fdInput)
	{
		MessageBox(m_hWnd, "ERROR - RealityFactory.ini VANISHED! SERIOUS FATAL ERROR - EXITING!",
			"RealityFactory INI Editor", MB_ICONSTOP | MB_OK);
		OnOk();
		return FALSE;
	}

	while(fgets(szInputString, 512, fdInput) != NULL)
	{
		if(szInputString[0] == ';' || strlen(szInputString) <= 5)
			continue;
		strcpy(szOutputString, szInputString);
		szAtom = strtok(szOutputString, "=");
		if(szAtom == NULL)
			continue;
		if(!stricmp(szAtom, "GameName"))
		{
			szAtom = strtok(NULL, "\n");
			m_gamename = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "PlayerName"))
		{
			szAtom = strtok(NULL, "\n");
			m_playername = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "PlayerAvatar"))
		{
			szAtom = strtok(NULL, " \n");
			m_actorname = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "menu"))
		{
			szAtom = strtok(NULL, " \n");
			m_menuname = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "StartLevel"))
		{
			szAtom = strtok(NULL, " \n");
			m_levelname = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "PackFile"))
		{
			szAtom = strtok(NULL, " \n");
			m_packfile = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "LevelDirectory"))
		{
			szAtom = strtok(NULL, " \n");
			m_LevelDir = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "BitmapDirectory"))
		{
			szAtom = strtok(NULL, " \n");
			m_BitmapDir = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "ActorDirectory"))
		{
			szAtom = strtok(NULL, " \n");
			m_ActorDir = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "AudioDirectory"))
		{
			szAtom = strtok(NULL, " \n");
			m_AudioDir = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "AudioStreamDirectory"))
		{
			szAtom = strtok(NULL, " \n");
			m_AudioStreamDir = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "VideoDirectory"))
		{
			szAtom = strtok(NULL, " \n");
			m_VideoDir = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "MIDIDirectory"))
		{
			szAtom = strtok(NULL, " \n");
			m_MIDIDir = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "UseCharSelect"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_usecselect = TRUE;
			continue;
		}
		if(!stricmp(szAtom, "UseNameSelect"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_usenselect = TRUE;
			continue;
		}
		if(!stricmp(szAtom, "UseDifficultLevel"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_difficult = TRUE;
			continue;
		}
		if(!stricmp(szAtom, "UseFirst"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_usefirst = TRUE;
			continue;
		}
		if(!stricmp(szAtom, "UseCutScene"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_usecutscene = TRUE;
			continue;
		}
		if(!stricmp(szAtom, "SplashScreen"))
		{
			szAtom = strtok(NULL, " \n");
			if(szAtom)
				m_splashscreen = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "SplashAudio"))
		{
			szAtom = strtok(NULL, " \n");
			if(szAtom)
				m_splashaudio = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "CutScene"))
		{
			szAtom = strtok(NULL, " \n");
			if(szAtom)
				m_cutscene = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "UseSecond"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_usesecond = TRUE;
			continue;
		}
		if(!stricmp(szAtom, "UseCutScene1"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_usecutscene2 = TRUE;
			continue;
		}
		if(!stricmp(szAtom, "SplashScreen1"))
		{
			szAtom = strtok(NULL, " \n");
			if(szAtom)
				m_splashscreen2 = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "SplashAudio1"))
		{
			szAtom = strtok(NULL, " \n");
			if(szAtom)
				m_splashaudio2 = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "CutScene1"))
		{
			szAtom = strtok(NULL, " \n");
			if(szAtom)
				m_cutscene2 = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "WeaponPosition"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "On"))
				m_weapon = TRUE;
			continue;
		}
		if(!stricmp(szAtom, "UseDialog"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_usedialog = TRUE;
			continue;
		}
		if(!stricmp(szAtom, "ShowTrack"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_showtrack = TRUE;
			continue;
		}
		if(!stricmp(szAtom, "Width"))
		{
			szAtom = strtok(NULL, " \n");
			m_width = atoi(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "Height"))
		{
			szAtom = strtok(NULL, " \n");
			m_height = atoi(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "FullScreen"))
		{
			m_fullscreen = TRUE;
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "false"))
				m_fullscreen = FALSE;
			continue;
		}
		if(!stricmp(szAtom, "Driver"))
		{
			szAtom = strtok(NULL, " \n");
			strcpy(m_driver, szAtom);
			continue;
		}
		if(!stricmp(szAtom, "UseDirectInput"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "false"))
				m_usedirectinput = FALSE;
			continue;
		}
		if(!stricmp(szAtom, "DefaultDifficulty"))
		{
			szAtom = strtok(NULL, " \n");
			m_ddif = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "DefaultLanguage"))
		{
			szAtom = strtok(NULL, " \n");
			m_dlang = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "Logging"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_logging = TRUE;
			continue;
		}

	}
	fclose(fdInput);

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


BOOL CIniEditorDlg::UpdateData(BOOL bSaveAndValidate /*= TRUE*/)
{
	if(bSaveAndValidate) // retrieve data
	{
		TCHAR Text[256];
		SendDlgItemMessage(m_hWnd, IDC_EDITPLAYER,	WM_GETTEXT, (WPARAM)256, (LPARAM)Text);m_actorname = Text;
		SendDlgItemMessage(m_hWnd, IDC_EDITMENU,	WM_GETTEXT, (WPARAM)256, (LPARAM)Text);m_menuname = Text;
		SendDlgItemMessage(m_hWnd, IDC_EDITSTART,	WM_GETTEXT, (WPARAM)256, (LPARAM)Text);m_levelname = Text;
		SendDlgItemMessage(m_hWnd, IDC_GAMENAME,	WM_GETTEXT, (WPARAM)256, (LPARAM)Text);m_gamename = Text;
		SendDlgItemMessage(m_hWnd, IDC_PACKFILE,	WM_GETTEXT, (WPARAM)256, (LPARAM)Text);m_packfile = Text;
		SendDlgItemMessage(m_hWnd, IDC_SSCREEN,		WM_GETTEXT, (WPARAM)256, (LPARAM)Text);m_splashscreen = Text;
		SendDlgItemMessage(m_hWnd, IDC_SAUDIO, 		WM_GETTEXT, (WPARAM)256, (LPARAM)Text);m_splashaudio = Text;
		SendDlgItemMessage(m_hWnd, IDC_CUTSCENE,	WM_GETTEXT, (WPARAM)256, (LPARAM)Text);m_cutscene = Text;
		SendDlgItemMessage(m_hWnd, IDC_SSCREEN2,	WM_GETTEXT, (WPARAM)256, (LPARAM)Text);m_splashscreen2 = Text;
		SendDlgItemMessage(m_hWnd, IDC_SAUDIO2,		WM_GETTEXT, (WPARAM)256, (LPARAM)Text);m_splashaudio2 = Text;
		SendDlgItemMessage(m_hWnd, IDC_CUTSCENE2,	WM_GETTEXT, (WPARAM)256, (LPARAM)Text);m_cutscene2 = Text;
		SendDlgItemMessage(m_hWnd, IDC_PLAYERNAME,	WM_GETTEXT, (WPARAM) 64, (LPARAM)Text);m_playername = Text;
		SendDlgItemMessage(m_hWnd, IDC_DDIFF,		WM_GETTEXT, (WPARAM)  2, (LPARAM)Text);m_ddif = Text;
		SendDlgItemMessage(m_hWnd, IDC_DLANG,		WM_GETTEXT, (WPARAM)  2, (LPARAM)Text);m_dlang = Text;

		m_usefirst		= (SendDlgItemMessage(m_hWnd, IDC_USEFIRST,		BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_usecutscene	= (SendDlgItemMessage(m_hWnd, IDC_CUTCHECK,		BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_usesecond		= (SendDlgItemMessage(m_hWnd, IDC_SECONDCHECK,	BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_usecutscene2	= (SendDlgItemMessage(m_hWnd, IDC_CUT2CHECK,	BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_weapon		= (SendDlgItemMessage(m_hWnd, IDC_CHECKWEAPON,	BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_usedialog		= (SendDlgItemMessage(m_hWnd, IDC_USEDIALOG,	BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_showtrack		= (SendDlgItemMessage(m_hWnd, IDC_SHOWTRACK,	BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_usecselect	= (SendDlgItemMessage(m_hWnd, IDC_USECSELECT,	BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_difficult		= (SendDlgItemMessage(m_hWnd, IDC_DIFFICULT,	BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_usenselect	= (SendDlgItemMessage(m_hWnd, IDC_USENSELECT,	BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_usedirectinput= (SendDlgItemMessage(m_hWnd, IDC_USEDIRECTINPUT,BM_GETCHECK,0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_logging		= (SendDlgItemMessage(m_hWnd, IDC_LOGGING,		BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;

		return TRUE;
	}
	else // initialize data
	{
		SendDlgItemMessage(m_hWnd, IDC_EDITPLAYER,	WM_SETTEXT, (WPARAM)0, (LPARAM)m_actorname.c_str());
		SendDlgItemMessage(m_hWnd, IDC_EDITMENU,	WM_SETTEXT, (WPARAM)0, (LPARAM)m_menuname.c_str());
		SendDlgItemMessage(m_hWnd, IDC_EDITSTART,	WM_SETTEXT, (WPARAM)0, (LPARAM)m_levelname.c_str());
		SendDlgItemMessage(m_hWnd, IDC_GAMENAME,	WM_SETTEXT, (WPARAM)0, (LPARAM)m_gamename.c_str());
		SendDlgItemMessage(m_hWnd, IDC_PACKFILE,	WM_SETTEXT, (WPARAM)0, (LPARAM)m_packfile.c_str());
		SendDlgItemMessage(m_hWnd, IDC_SSCREEN,		WM_SETTEXT, (WPARAM)0, (LPARAM)m_splashscreen.c_str());
		SendDlgItemMessage(m_hWnd, IDC_SAUDIO, 		WM_SETTEXT, (WPARAM)0, (LPARAM)m_splashaudio.c_str());
		SendDlgItemMessage(m_hWnd, IDC_CUTSCENE,	WM_SETTEXT, (WPARAM)0, (LPARAM)m_cutscene.c_str());
		SendDlgItemMessage(m_hWnd, IDC_SSCREEN2,	WM_SETTEXT, (WPARAM)0, (LPARAM)m_splashscreen2.c_str());
		SendDlgItemMessage(m_hWnd, IDC_SAUDIO2,		WM_SETTEXT, (WPARAM)0, (LPARAM)m_splashaudio2.c_str());
		SendDlgItemMessage(m_hWnd, IDC_CUTSCENE2,	WM_SETTEXT, (WPARAM)0, (LPARAM)m_cutscene2.c_str());
		SendDlgItemMessage(m_hWnd, IDC_PLAYERNAME,	WM_SETTEXT, (WPARAM)0, (LPARAM)m_playername.c_str());
		SendDlgItemMessage(m_hWnd, IDC_DDIFF,		WM_SETTEXT, (WPARAM)0, (LPARAM)m_ddif.c_str());
		SendDlgItemMessage(m_hWnd, IDC_DLANG,		WM_SETTEXT, (WPARAM)0, (LPARAM)m_dlang.c_str());

		SendDlgItemMessage(m_hWnd, IDC_USEFIRST,	BM_SETCHECK, (WPARAM)(m_usefirst ? BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_CUTCHECK,	BM_SETCHECK, (WPARAM)(m_usecutscene ? BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_SECONDCHECK,	BM_SETCHECK, (WPARAM)(m_usesecond ? BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_CUT2CHECK,	BM_SETCHECK, (WPARAM)(m_usecutscene2 ? BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_CHECKWEAPON,	BM_SETCHECK, (WPARAM)(m_weapon ? BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_USEDIALOG,	BM_SETCHECK, (WPARAM)(m_usedialog ? BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_SHOWTRACK,	BM_SETCHECK, (WPARAM)(m_showtrack ? BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_USECSELECT,	BM_SETCHECK, (WPARAM)(m_usecselect ? BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_DIFFICULT,	BM_SETCHECK, (WPARAM)(m_difficult ? BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_USENSELECT,	BM_SETCHECK, (WPARAM)(m_usenselect ? BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_USEDIRECTINPUT,BM_SETCHECK,(WPARAM)(m_usedirectinput ? BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_LOGGING,		BM_SETCHECK, (WPARAM)(m_logging ? BST_CHECKED : BST_UNCHECKED), 0);

		return TRUE;
	}
}


void CIniEditorDlg::OnDdif()
{
	SendDlgItemMessage(m_hWnd, IDC_DDIFF, WM_GETTEXT, (WPARAM)  2, (LPARAM)m_ddif.c_str());

	if(atoi(m_ddif.c_str())>3)
	{
		m_ddif = _T("3");
		SendDlgItemMessage(m_hWnd, IDC_DDIFF, WM_SETTEXT, (WPARAM)0, (LPARAM)m_ddif.c_str());
	}
	else if(atoi(m_ddif.c_str())<1)
	{
		m_ddif = _T("1");
		SendDlgItemMessage(m_hWnd, IDC_DDIFF, WM_SETTEXT, (WPARAM)0, (LPARAM)m_ddif.c_str());
	}
}

void CIniEditorDlg::OnDlang()
{
	SendDlgItemMessage(m_hWnd, IDC_DLANG, WM_GETTEXT, (WPARAM)  2, (LPARAM)m_dlang.c_str());

	if(atoi(m_dlang.c_str())>5)
	{
		m_dlang = _T("5");
		SendDlgItemMessage(m_hWnd, IDC_DLANG, WM_SETTEXT, (WPARAM)0, (LPARAM)m_dlang.c_str());
	}
	else if(atoi(m_dlang.c_str())<1)
	{
		m_dlang = _T("1");
		SendDlgItemMessage(m_hWnd, IDC_DLANG, WM_SETTEXT, (WPARAM)0, (LPARAM)m_dlang.c_str());
	}
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIniEditorDlg::OnPaint()
{
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIniEditorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIniEditorDlg::OnActbrowse()
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	lstrcpy(m_dir, m_currentdir);
	lstrcat(m_dir, "\\media\\actors");
	dlgFile.SetInitalDir(m_dir);

	std::string strFilter;

	// append the "*.act"  files filter
	strFilter = _T("Genesis Actor Files (*.act)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.act");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter.c_str();
	dlgFile.m_ofn.hwndOwner = m_hWnd;

	UpdateData();
	if(dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			std::string strPath = dlgFile.GetNextPathName(pos);
			m_actorname = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnMenuini()
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	lstrcpy(m_dir, m_currentdir);
	lstrcat(m_dir, "\\install");
	dlgFile.SetInitalDir(m_dir);

	std::string strFilter;

	// append the "*.ini"  files filter
	strFilter = _T("Menu INI Files (*.ini)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.ini");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter.c_str();
	dlgFile.m_ofn.hwndOwner = m_hWnd;

	UpdateData();
	if(dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			std::string strPath = dlgFile.GetNextPathName(pos);
			m_menuname = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnStartlevel()
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	lstrcpy(m_dir, m_currentdir);
	lstrcat(m_dir, "\\media\\levels");
	dlgFile.SetInitalDir(m_dir);

	std::string strFilter;

	// append the "*.bsp"  files filter
	strFilter = _T("Genesis Level Files (*.bsp)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.bsp");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter.c_str();
	dlgFile.m_ofn.hwndOwner = m_hWnd;

	UpdateData();
	if(dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			std::string strPath = dlgFile.GetNextPathName(pos);
			m_levelname = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnPack()
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	lstrcpy(m_dir, m_currentdir);
	lstrcat(m_dir, "\\");
	dlgFile.SetInitalDir(m_dir);

	std::string strFilter;

	// append the "*.vfs"  files filter
	strFilter = _T("Pack Files (*.vfs)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.vfs");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter.c_str();
	dlgFile.m_ofn.hwndOwner = m_hWnd;

	UpdateData();
	if(dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			std::string strPath = dlgFile.GetNextPathName(pos);
			m_packfile = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnCbutton()
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	lstrcpy(m_dir, m_currentdir);
	lstrcat(m_dir, "\\media\\video");
	dlgFile.SetInitalDir(m_dir);

	std::string strFilter;

	// append the "*.avi"  files filter
	strFilter = _T("Video Files (*.avi)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.avi");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	strFilter += _T("Gif Files (*.gif)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.gif");
	dlgFile.m_ofn.nMaxCustFilter++;
	strFilter += (TCHAR)'\0';   // last string
	strFilter += (TCHAR)'\0';

	dlgFile.m_ofn.lpstrFilter = strFilter.c_str();
	dlgFile.m_ofn.hwndOwner = m_hWnd;

	UpdateData();
	if(dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			std::string strPath = dlgFile.GetNextPathName(pos);
			m_cutscene = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}

}

void CIniEditorDlg::OnSabutton()
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	lstrcpy(m_dir, m_currentdir);
	lstrcat(m_dir, "\\media\\audio");
	dlgFile.SetInitalDir(m_dir);

	std::string strFilter;

	// append the "*.wav"  files filter
	strFilter = _T("Sound Files (*.wav)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.wav");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter.c_str();
	dlgFile.m_ofn.hwndOwner = m_hWnd;

	UpdateData();
	if(dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			std::string strPath = dlgFile.GetNextPathName(pos);
			m_splashaudio = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnSsbutton()
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	lstrcpy(m_dir, m_currentdir);
	lstrcat(m_dir, "\\media\\bitmaps");
	dlgFile.SetInitalDir(m_dir);

	std::string strFilter;

	// append the "*.bmp"  files filter
	strFilter = _T("Graphic Files (*.bmp)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.bmp");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter.c_str();
	dlgFile.m_ofn.hwndOwner = m_hWnd;

	UpdateData();
	if(dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			std::string strPath = dlgFile.GetNextPathName(pos);
			m_splashscreen = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnSsbutton2()
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	lstrcpy(m_dir, m_currentdir);
	lstrcat(m_dir, "\\media\\bitmaps");
	dlgFile.SetInitalDir(m_dir);

	std::string strFilter;

	// append the "*.bmp"  files filter
	strFilter = _T("Graphic Files (*.bmp)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.bmp");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter.c_str();
	dlgFile.m_ofn.hwndOwner = m_hWnd;

	UpdateData();
	if(dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			std::string strPath = dlgFile.GetNextPathName(pos);
			m_splashscreen2 = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnSabutton2()
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	lstrcpy(m_dir, m_currentdir);
	lstrcat(m_dir, "\\media\\audio");
	dlgFile.SetInitalDir(m_dir);

	std::string strFilter;

	// append the "*.wav"  files filter
	strFilter = _T("Sound Files (*.wav)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.wav");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter.c_str();
	dlgFile.m_ofn.hwndOwner = m_hWnd;

	UpdateData();
	if(dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			std::string strPath = dlgFile.GetNextPathName(pos);
			m_splashaudio2 = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnCbutton2()
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	lstrcpy(m_dir, m_currentdir);
	lstrcat(m_dir, "\\media\\video");
	dlgFile.SetInitalDir(m_dir);

	std::string strFilter;

	// append the "*.avi"  files filter
	strFilter = _T("Video Files (*.avi)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.avi");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	strFilter += _T("Gif Files (*.gif)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.gif");
	dlgFile.m_ofn.nMaxCustFilter++;
	strFilter += (TCHAR)'\0';   // last string
	strFilter += (TCHAR)'\0';

	dlgFile.m_ofn.lpstrFilter = strFilter.c_str();
	dlgFile.m_ofn.hwndOwner = m_hWnd;

	UpdateData();
	if(dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			std::string strPath = dlgFile.GetNextPathName(pos);
			m_cutscene2 = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}


void CIniEditorDlg::OnOk()
{
	FILE *fdOutput = NULL;
	char szOutputString[1024] = {""};
	TCHAR m_dir[512];

	strcpy(m_dir, m_currentdir);
	strcat(m_dir, "\\RealityFactory.ini");

	fdOutput = fopen(m_dir, "wt");

	if(fdOutput)
	{
		UpdateData();

		fputs(";\n; RealityFactory.INI - RealityFactory Initialization file\n;\n", fdOutput);
		fputs("; Created by IniEditor\n;\n\n", fdOutput);

		fputs(";\n; GameName defines the name of the game\n;\n\n", fdOutput);
		sprintf(szOutputString, "GameName=%s\n\n", m_gamename.c_str());
		fputs(szOutputString, fdOutput);

		fputs(";\n; PlayerName defines the name of the player\n;\n\n", fdOutput);
		sprintf(szOutputString, "PlayerName=%s\n\n", m_playername.c_str());
		fputs(szOutputString, fdOutput);

		fputs(";\n; PlayerAvatar defines the player actor\n;\n\n", fdOutput);
		sprintf(szOutputString, "PlayerAvatar=%s\n\n", m_actorname.c_str());
		fputs(szOutputString, fdOutput);

		fputs(";\n; Define the menu initialization file\n;\n\n", fdOutput);
		sprintf(szOutputString, "Menu=%s\n\n", m_menuname.c_str());
		fputs(szOutputString, fdOutput);

		fputs(";\n; Define the virtual pack file\n;\n\n", fdOutput);
		sprintf(szOutputString, "PackFile=%s\n\n", m_packfile.c_str());
		fputs(szOutputString, fdOutput);

		fputs(";\n; Define the starting level\n;\n\n", fdOutput);
		sprintf(szOutputString, "StartLevel=%s\n\n", m_levelname.c_str());
		fputs(szOutputString, fdOutput);

		fputs(";\n; Define the directories\n;\n\n", fdOutput);
		sprintf(szOutputString, "LevelDirectory=%s\n", m_LevelDir.c_str());
		fputs(szOutputString, fdOutput);
		sprintf(szOutputString, "BitmapDirectory=%s\n", m_BitmapDir.c_str());
		fputs(szOutputString, fdOutput);
		sprintf(szOutputString, "ActorDirectory=%s\n", m_ActorDir.c_str());
		fputs(szOutputString, fdOutput);
		sprintf(szOutputString, "AudioDirectory=%s\n", m_AudioDir.c_str());
		fputs(szOutputString, fdOutput);
		sprintf(szOutputString, "AudioStreamDirectory=%s\n", m_AudioStreamDir.c_str());
		fputs(szOutputString, fdOutput);
		sprintf(szOutputString, "VideoDirectory=%s\n", m_VideoDir.c_str());
		fputs(szOutputString, fdOutput);
		sprintf(szOutputString, "MIDIDirectory=%s\n", m_MIDIDir.c_str());
		fputs(szOutputString, fdOutput);

		fputs(";\n; Set Weapon Positioning\n;\n\n", fdOutput);
		if(m_weapon)
			fputs("WeaponPosition=On\n\n", fdOutput);
		else
			fputs("WeaponPosition=Off\n\n", fdOutput);

		fputs(";\n; Set UseDialog\n;\n\n", fdOutput);
		if(m_usedialog)
			fputs("UseDialog=true\n\n", fdOutput);
		else
			fputs("UseDialog=false\n\n", fdOutput);

		fputs(";\n; Set ShowTrack\n;\n\n", fdOutput);
		if(m_showtrack)
			fputs("ShowTrack=true\n\n", fdOutput);
		else
			fputs("ShowTrack=false\n\n", fdOutput);

		fputs(";\n; Fullscreen tells the engine to go full-screen or not\n;\n\n", fdOutput);
		if(!m_fullscreen)
			fputs("FullScreen=false\n\n", fdOutput);
		else
			fputs("FullScreen=true\n\n", fdOutput);

		fputs(";\n; Width is the width of the game display\n;\n\n", fdOutput);
		sprintf(szOutputString, "Width=%d\n\n", m_width);
		fputs(szOutputString, fdOutput);

		fputs(";\n; Height is the height of the game display\n;\n\n", fdOutput);
		sprintf(szOutputString, "Height=%d\n\n", m_height);
		fputs(szOutputString, fdOutput);

		fputs(";\n; Driver tells the system which driver to use\n;\n", fdOutput);
		sprintf(szOutputString, ";\n\nDriver=%s\n\n", m_driver);
		fputs(szOutputString, fdOutput);

		fputs(";\n; Use DirectInput (Joystick)\n;\n\n", fdOutput);
		if(m_usedirectinput)
			fputs("UseDirectInput=true\n\n", fdOutput);
		else
			fputs("UseDirectInput=false\n\n", fdOutput);

		fputs(";\n; Set UseCharSelect\n;\n\n", fdOutput);
		if(m_usecselect)
			fputs("UseCharSelect=true\n\n", fdOutput);
		else
			fputs("UseCharSelect=false\n\n", fdOutput);

		fputs(";\n; Set UseNameSelect\n;\n\n", fdOutput);
		if(m_usenselect)
			fputs("UseNameSelect=true\n\n", fdOutput);
		else
			fputs("UseNameSelect=false\n\n", fdOutput);

		fputs(";\n; Set UseDifficultLevel\n;\n\n", fdOutput);
		if(m_difficult)
			fputs("UseDifficultLevel=true\n\n", fdOutput);
		else
			fputs("UseDifficultLevel=false\n\n", fdOutput);

		fputs(";\n; Set UseFirst\n;\n\n", fdOutput);
		if(m_usefirst)
			fputs("UseFirst=true\n\n", fdOutput);
		else
			fputs("UseFirst=false\n\n", fdOutput);

		fputs(";\n; Set UseCutScene\n;\n\n", fdOutput);
		if(m_usecutscene)
			fputs("UseCutScene=true\n\n", fdOutput);
		else
			fputs("UseCutScene=false\n\n", fdOutput);

		fputs(";\n; SplashScreen defines the splash screen to use\n;\n\n", fdOutput);
		sprintf(szOutputString, "SplashScreen=%s\n\n", m_splashscreen.c_str());
		fputs(szOutputString, fdOutput);

		fputs(";\n; SplashAudio defines the splash screen audio to use\n;\n\n", fdOutput);
		sprintf(szOutputString, "SplashAudio=%s\n\n", m_splashaudio.c_str());
		fputs(szOutputString, fdOutput);

		fputs(";\n; CutScene defines the cut scene to use\n;\n\n", fdOutput);
		sprintf(szOutputString, "CutScene=%s\n\n", m_cutscene.c_str());
		fputs(szOutputString, fdOutput);

		fputs(";\n; Set UseSecond\n;\n\n", fdOutput);
		if(m_usesecond)
			fputs("UseSecond=true\n\n", fdOutput);
		else
			fputs("UseSecond=false\n\n", fdOutput);

		fputs(";\n; Set UseCutScene1\n;\n\n", fdOutput);
		if(m_usecutscene2)
			fputs("UseCutScene1=true\n\n", fdOutput);
		else
			fputs("UseCutScene1=false\n\n", fdOutput);

		fputs(";\n; SplashScreen defines the splash screen to use\n;\n\n", fdOutput);
		sprintf(szOutputString, "SplashScreen1=%s\n\n", m_splashscreen2.c_str());
		fputs(szOutputString, fdOutput);

		fputs(";\n; SplashAudio defines the splash screen audio to use\n;\n\n", fdOutput);
		sprintf(szOutputString, "SplashAudio1=%s\n\n", m_splashaudio2.c_str());
		fputs(szOutputString, fdOutput);

		fputs(";\n; CutScene defines the cut scene to use\n;\n\n", fdOutput);
		sprintf(szOutputString, "CutScene1=%s\n\n", m_cutscene2.c_str());
		fputs(szOutputString, fdOutput);

		if(atoi(m_ddif.c_str())>3)
			m_ddif = _T("3");
		if(atoi(m_ddif.c_str())<1)
			m_ddif = _T("1");
		fputs(";\n; Default Difficulty Level\n;\n\n", fdOutput);
		sprintf(szOutputString, "DefaultDifficulty=%s\n\n", m_ddif.c_str());
		fputs(szOutputString, fdOutput);

		if(atoi(m_dlang.c_str())>5)
			m_dlang = _T("5");
		if(atoi(m_dlang.c_str())<1)
			m_dlang = _T("1");
		fputs(";\n; Default Language\n;\n\n", fdOutput);
		sprintf(szOutputString, "DefaultLanguage=%s\n\n", m_dlang.c_str());
		fputs(szOutputString, fdOutput);

		fputs(";\n; Use detailed logging\n;\n\n", fdOutput);
		if(m_logging)
			fputs("Logging=true\n\n", fdOutput);
		else
			fputs("Logging=false\n\n", fdOutput);

		fclose(fdOutput);
	}

	DestroyWindow(m_hWnd);
}
