// IniEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IniEditor.h"
#include "IniEditorDlg.h"
#include "MFileDlg.h"
#include <direct.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIniEditorDlg dialog

CIniEditorDlg::CIniEditorDlg(CWnd* pParent /*=NULL*/)
: CDialog(CIniEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIniEditorDlg)
	m_actorname = _T("");
	m_menuname = _T("");
	m_levelname = _T("");
	m_gamename = _T("");
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
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIniEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIniEditorDlg)
	DDX_Control(pDX, IDC_SSBUTTON2, m_ssbutton2);
	DDX_Control(pDX, IDC_SABUTTON2, m_sabutton2);
	DDX_Control(pDX, IDC_CBUTTON2, m_cutbutton2);
	DDX_Control(pDX, IDC_SSBUTTON, m_ssbutton);
	DDX_Control(pDX, IDC_SABUTTON, m_sabutton);
	DDX_Control(pDX, IDC_CBUTTON, m_cutbutton);
	DDX_Control(pDX, IDC_PACK, m_browsepack);
	DDX_Control(pDX, IDOK, m_createini);
	DDX_Control(pDX, IDC_STARTLEVEL, m_levelbrowse);
	DDX_Control(pDX, IDC_MENUINI, m_menubrowse);
	DDX_Control(pDX, IDC_ACTBROWSE, m_actbrowse);
	DDX_Control(pDX, IDC_GAMENAME, m_GNEdit);
	DDX_Text(pDX, IDC_EDITPLAYER, m_actorname);
	DDX_Text(pDX, IDC_EDITMENU, m_menuname);
	DDX_Text(pDX, IDC_EDITSTART, m_levelname);
	DDX_Text(pDX, IDC_GAMENAME, m_gamename);
	DDX_Check(pDX, IDC_CHECKWEAPON, m_weapon);
	DDX_Text(pDX, IDC_PACKFILE, m_packfile);
	DDX_Check(pDX, IDC_USEDIALOG, m_usedialog);
	DDX_Check(pDX, IDC_CUTCHECK, m_usecutscene);
	DDX_Text(pDX, IDC_SSCREEN, m_splashscreen);
	DDX_Text(pDX, IDC_SAUDIO, m_splashaudio);
	DDX_Text(pDX, IDC_CUTSCENE, m_cutscene);
	DDX_Check(pDX, IDC_CUT2CHECK, m_usecutscene2);
	DDX_Text(pDX, IDC_CUTSCENE2, m_cutscene2);
	DDX_Text(pDX, IDC_SAUDIO2, m_splashaudio2);
	DDX_Check(pDX, IDC_SECONDCHECK, m_usesecond);
	DDX_Text(pDX, IDC_SSCREEN2, m_splashscreen2);
	DDX_Check(pDX, IDC_USEFIRST, m_usefirst);
	DDX_Check(pDX, IDC_SHOWTRACK, m_showtrack);
	DDX_Check(pDX, IDC_USECSELECT, m_usecselect);
	DDX_Check(pDX, IDC_DIFFICULT, m_difficult);
	DDX_Text(pDX, IDC_DDIFF, m_ddif);
	DDV_MaxChars(pDX, m_ddif, 1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIniEditorDlg, CDialog)
//{{AFX_MSG_MAP(CIniEditorDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_ACTBROWSE, OnActbrowse)
ON_BN_CLICKED(IDC_MENUINI, OnMenuini)
ON_BN_CLICKED(IDC_STARTLEVEL, OnStartlevel)
	ON_BN_CLICKED(IDC_PACK, OnPack)
	ON_BN_CLICKED(IDC_CBUTTON, OnCbutton)
	ON_BN_CLICKED(IDC_SABUTTON, OnSabutton)
	ON_BN_CLICKED(IDC_SSBUTTON, OnSsbutton)
	ON_BN_CLICKED(IDC_SSBUTTON2, OnSsbutton2)
	ON_BN_CLICKED(IDC_SABUTTON2, OnSabutton2)
	ON_BN_CLICKED(IDC_CBUTTON2, OnCbutton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIniEditorDlg message handlers

BOOL CIniEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	_chdir("..");
	_getcwd(m_currentdir, 512);
	m_weapon = false;
	m_usedialog = false;
	m_usecutscene = false;
	m_usecutscene2 = false;
	m_usefirst = false;
	m_usesecond = false;
	m_showtrack = false;
	m_usecselect = false;
	m_difficult = false;

	FILE *fdInput = NULL;
	char szInputString[1024] = {""};
	char szOutputString[1024] = {""};
	char *szAtom = NULL;
	
	if((fdInput = fopen("RealityFactory.ini", "rt")) == NULL)
	{
		MessageBox("ERROR - RealityFactory.ini VANISHED! SERIOUS FATAL ERROR - EXITING!",
			"RealityFactory INI Editor", MB_ICONSTOP | MB_OK);
		CDialog::OnOK();
		return TRUE;
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
		if(!stricmp(szAtom, "UseCharSelect"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_usecselect = true;
			continue;
		}
		if(!stricmp(szAtom, "UseDifficultLevel"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_difficult = true;
			continue;
		}
		if(!stricmp(szAtom, "UseFirst"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_usefirst = true;
			continue;
		}
		if(!stricmp(szAtom, "UseCutScene"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_usecutscene = true;
			continue;
		}
		if(!stricmp(szAtom, "SplashScreen"))
		{
			szAtom = strtok(NULL, " \n");
			m_splashscreen = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "SplashAudio"))
		{
			szAtom = strtok(NULL, " \n");
			m_splashaudio = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "CutScene"))
		{
			szAtom = strtok(NULL, " \n");
			m_cutscene = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "UseSecond"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_usesecond = true;
			continue;
		}
		if(!stricmp(szAtom, "UseCutScene1"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_usecutscene2 = true;
			continue;
		}
		if(!stricmp(szAtom, "SplashScreen1"))
		{
			szAtom = strtok(NULL, " \n");
			m_splashscreen2 = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "SplashAudio1"))
		{
			szAtom = strtok(NULL, " \n");
			m_splashaudio2 = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "CutScene1"))
		{
			szAtom = strtok(NULL, " \n");
			m_cutscene2 = _T(szAtom);
			continue;
		}
		if(!stricmp(szAtom, "WeaponPosition"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "On"))
				m_weapon = true;
			continue;
		}
		if(!stricmp(szAtom, "UseDialog"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_usedialog = true;
			continue;
		}
		if(!stricmp(szAtom, "ShowTrack"))
		{
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "true"))
				m_showtrack = true;
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
			m_fullscreen = true;
			szAtom = strtok(NULL, " \n");
			if(!stricmp(szAtom, "false"))
				m_fullscreen = false;
			continue;
		}
		if(!stricmp(szAtom, "Driver"))
		{
			szAtom = strtok(NULL, " \n");
			strcpy(m_driver, szAtom);
		}
		if(!stricmp(szAtom, "DefaultDifficulty"))
		{
			szAtom = strtok(NULL, " \n");
			m_ddif = _T(szAtom);
			continue;
		}
	}
	fclose(fdInput);

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIniEditorDlg::OnPaint() 
{
	
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
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
	strcpy(m_dir, m_currentdir);
	strcat(m_dir, "\\media\\actors");
	dlgFile.SetInitalDir(m_dir);
	
	CString title, strFilter, strDefault;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));
	
	// append the "*.act"  files filter
	strFilter = _T("Genesis Actor Files (*.act)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.act");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	
	if (dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			CString strPath = dlgFile.GetNextPathName(pos);
			m_actorname = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnMenuini() 
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	strcpy(m_dir, m_currentdir);
	strcat(m_dir, "\\install");
	dlgFile.SetInitalDir(m_dir);
	
	CString title, strFilter, strDefault;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));
	
	// append the "*.ini"  files filter
	strFilter = _T("Menu INI Files (*.ini)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.ini");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	
	if (dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			CString strPath = dlgFile.GetNextPathName(pos);
			m_menuname = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnStartlevel() 
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	strcpy(m_dir, m_currentdir);
	strcat(m_dir, "\\media\\levels");
	dlgFile.SetInitalDir(m_dir);
	
	CString title, strFilter, strDefault;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));
	
	// append the "*.bsp"  files filter
	strFilter = _T("Genesis Level Files (*.bsp)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.bsp");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	
	if (dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			CString strPath = dlgFile.GetNextPathName(pos);
			m_levelname = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnPack() 
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	strcpy(m_dir, m_currentdir);
	strcat(m_dir, "\\");
	dlgFile.SetInitalDir(m_dir);
	
	CString title, strFilter, strDefault;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));
	
	// append the "*.vfs"  files filter
	strFilter = _T("Pack Files (*.vfs)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.vfs");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	
	if (dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			CString strPath = dlgFile.GetNextPathName(pos);
			m_packfile = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnCbutton() 
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	strcpy(m_dir, m_currentdir);
	strcat(m_dir, "\\media\\video");
	dlgFile.SetInitalDir(m_dir);
	
	CString title, strFilter, strDefault;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));
	
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
	
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	
	if (dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			CString strPath = dlgFile.GetNextPathName(pos);
			m_cutscene = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}

}

void CIniEditorDlg::OnSabutton() 
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	strcpy(m_dir, m_currentdir);
	strcat(m_dir, "\\media\\audio");
	dlgFile.SetInitalDir(m_dir);
	
	CString title, strFilter, strDefault;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));
	
	// append the "*.wav"  files filter
	strFilter = _T("Sound Files (*.wav)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.wav");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	
	if (dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			CString strPath = dlgFile.GetNextPathName(pos);
			m_splashaudio = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnSsbutton() 
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	strcpy(m_dir, m_currentdir);
	strcat(m_dir, "\\media\\bitmaps");
	dlgFile.SetInitalDir(m_dir);
	
	CString title, strFilter, strDefault;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));
	
	// append the "*.bmp"  files filter
	strFilter = _T("Graphic Files (*.bmp)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.bmp");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	
	if (dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			CString strPath = dlgFile.GetNextPathName(pos);
			m_splashscreen = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnSsbutton2() 
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	strcpy(m_dir, m_currentdir);
	strcat(m_dir, "\\media\\bitmaps");
	dlgFile.SetInitalDir(m_dir);
	
	CString title, strFilter, strDefault;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));
	
	// append the "*.bmp"  files filter
	strFilter = _T("Graphic Files (*.bmp)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.bmp");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	
	if (dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			CString strPath = dlgFile.GetNextPathName(pos);
			m_splashscreen2 = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnSabutton2() 
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	strcpy(m_dir, m_currentdir);
	strcat(m_dir, "\\media\\audio");
	dlgFile.SetInitalDir(m_dir);
	
	CString title, strFilter, strDefault;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));
	
	// append the "*.wav"  files filter
	strFilter = _T("Sound Files (*.wav)");
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.wav");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	
	if (dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			CString strPath = dlgFile.GetNextPathName(pos);
			m_splashaudio2 = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}

void CIniEditorDlg::OnCbutton2() 
{
	MFileDlg dlgFile(TRUE);
	TCHAR m_dir[512];
	strcpy(m_dir, m_currentdir);
	strcat(m_dir, "\\media\\video");
	dlgFile.SetInitalDir(m_dir);
	
	CString title, strFilter, strDefault;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));
	
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
	
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	
	if (dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			CString strPath = dlgFile.GetNextPathName(pos);
			m_cutscene2 = dlgFile.GetFileName();
			UpdateData(FALSE);
		}
	}
}
