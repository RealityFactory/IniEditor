// IniEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "IniEditor.h"
#include "IniEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIniEditorApp

BEGIN_MESSAGE_MAP(CIniEditorApp, CWinApp)
	//{{AFX_MSG_MAP(CIniEditorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIniEditorApp construction

CIniEditorApp::CIniEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIniEditorApp object

CIniEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CIniEditorApp initialization

BOOL CIniEditorApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CIniEditorDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
		FILE *fdOutput = NULL;
		char szOutputString[1024] = {""};
		TCHAR m_dir[512];

		strcpy(m_dir, dlg.m_currentdir);
		strcat(m_dir, "\\RealityFactory.ini");

		fdOutput = fopen(m_dir, "wt");
		fputs(";\n; RealityFactory.INI - RealityFactory Initialization file\n;\n", fdOutput);
		fputs("; Created by IniEditor\n;\n\n", fdOutput);

		sprintf(szOutputString, "GameName=%s\n\n", dlg.m_gamename);
		fputs(szOutputString, fdOutput);

		fputs(";\n; PlayerAvatar defines the player actor\n;\n\n", fdOutput);
		sprintf(szOutputString, "PlayerAvatar=%s\n\n", dlg.m_actorname);
		fputs(szOutputString, fdOutput);

		fputs(";\n; Starting Gamma level\n;\n\n", fdOutput);
		sprintf(szOutputString, "Gamma=%s\n\n", dlg.m_gammaamount);
		fputs(szOutputString, fdOutput);

		fputs(";\n; Define the menu initialization file\n;\n\n", fdOutput);
		sprintf(szOutputString, "Menu=%s\n\n", dlg.m_menuname);
		fputs(szOutputString, fdOutput);

		fputs(";\n; Define the virtual pack file\n;\n\n", fdOutput);
		sprintf(szOutputString, "PackFile=%s\n\n", dlg.m_packfile);
		fputs(szOutputString, fdOutput);

		fputs(";\n; Define the starting level\n;\n\n", fdOutput);
		sprintf(szOutputString, "StartLevel=%s\n\n", dlg.m_levelname);
		fputs(szOutputString, fdOutput);

		fputs(";\n; Define the directories\n;\n\n", fdOutput);
		fputs("LevelDirectory=media\\levels\n", fdOutput);
		fputs("BitmapDirectory=media\\bitmaps\n", fdOutput);
		fputs("ActorDirectory=media\\actors\n", fdOutput);
		fputs("AudioDirectory=media\\audio\n", fdOutput);
		fputs("AudioStreamDirectory=media\\audio\n", fdOutput);
		fputs("VideoDirectory=media\\video\n", fdOutput);
		fputs("MIDIDirectory=media\\midi\n\n", fdOutput);

		fputs(";\n; Set Weapon Positioning\n;\n\n", fdOutput);
		if(dlg.m_weapon)
			fputs("WeaponPosition=On\n\n", fdOutput);
		else
			fputs("WeaponPosition=Off\n\n", fdOutput);

		fputs(";\n; Set UseDialog\n;\n\n", fdOutput);
		if(dlg.m_usedialog)
			fputs("UseDialog=true\n\n", fdOutput);
		else
			fputs("UseDialog=false\n\n", fdOutput);

		fputs(";\n; Set ShowTrack\n;\n\n", fdOutput);
		if(dlg.m_showtrack)
			fputs("ShowTrack=true\n\n", fdOutput);
		else
			fputs("ShowTrack=false\n\n", fdOutput);

		long w, h;

		if(dlg.m_drivername.Find("Pick")==-1)
		{
			dlg.m_pDriver = geDriver_SystemGetNextDriver(dlg.m_pDriverSystem, NULL);
			for (int cnt = 0; cnt < dlg.m_videoindex; cnt++)
			{
				dlg.m_pDriver = geDriver_SystemGetNextDriver(dlg.m_pDriverSystem, dlg.m_pDriver);
			}
			dlg.m_pMode = geDriver_GetNextMode(dlg.m_pDriver, NULL);
			for (cnt = 0; cnt < dlg.m_resindex; cnt++)
			{
				dlg.m_pMode = geDriver_GetNextMode(dlg.m_pDriver, dlg.m_pMode);
			}

			geDriver_ModeGetWidthHeight(dlg.m_pMode, &w, &h);
		}
		else
		{
			w=640;
			h=480;
		}

		fputs(";\n; Fullscreen tells the engine to go full-screen or not\n;\n\n", fdOutput);

		if ((w == -1) && (h == -1))
		{
			fputs("FullScreen=false\n\n", fdOutput);
			w = 640;
			h = 480;
		}
		else
			fputs("FullScreen=true\n\n", fdOutput);

		fputs(";\n; Width is the width of the game display\n;\n\n", fdOutput);
		sprintf(szOutputString, "Width=%d\n\n", w);
		fputs(szOutputString, fdOutput);

		fputs(";\n; Height is the height of the game display\n;\n\n", fdOutput);
		sprintf(szOutputString, "Height=%d\n\n", h);
		fputs(szOutputString, fdOutput);

		fputs(";\n; Renderer is the renderer to use, HARDWARE or SOFTWARE\n;\n", fdOutput);
		
		if(dlg.m_drivername.Find("Software")!=-1)
			fputs(";\n\nRenderer=software\n\n", fdOutput);
		else
			fputs(";\n\nRenderer=hardware\n\n", fdOutput);

		fputs(";\n; Driver tells the system which driver to use\n;\n", fdOutput);

		if(dlg.m_drivername.Find("D3D")!=-1)
			fputs(";\n\nDriver=d3d\n\n", fdOutput);
		else if(dlg.m_drivername.Find("Glide")!=-1)
			fputs(";\n\nDriver=glide\n\n", fdOutput);
		else if(dlg.m_drivername.Find("Open")!=-1)
			fputs(";\n\nDriver=opengl\n\n", fdOutput);
		else if(dlg.m_drivername.Find("Software")!=-1)
			fputs(";\n\nDriver=windowed\n\n", fdOutput);
		else if(dlg.m_drivername.Find("Wire")!=-1)
			fputs(";\n\nDriver=wire\n\n", fdOutput);
		else if(dlg.m_drivername.Find("Pick")!=-1)
			fputs(";\n\nDriver=pick\n\n", fdOutput);

		fputs(";\n; Set UseCharSelect\n;\n\n", fdOutput);
		if(dlg.m_usecselect)
			fputs("UseCharSelect=true\n\n", fdOutput);
		else
			fputs("UseCharSelect=false\n\n", fdOutput);

		fputs(";\n; Set UseFirst\n;\n\n", fdOutput);
		if(dlg.m_usefirst)
			fputs("UseFirst=true\n\n", fdOutput);
		else
			fputs("UseFirst=false\n\n", fdOutput);

		fputs(";\n; Set UseCutScene\n;\n\n", fdOutput);
		if(dlg.m_usecutscene)
			fputs("UseCutScene=true\n\n", fdOutput);
		else
			fputs("UseCutScene=false\n\n", fdOutput);

		fputs(";\n; SplashScreen defines the splash screen to use\n;\n\n", fdOutput);
		sprintf(szOutputString, "SplashScreen=%s\n\n", dlg.m_splashscreen);
		fputs(szOutputString, fdOutput);

		fputs(";\n; SplashAudio defines the splash screen audio to use\n;\n\n", fdOutput);
		sprintf(szOutputString, "SplashAudio=%s\n\n", dlg.m_splashaudio);
		fputs(szOutputString, fdOutput);

		fputs(";\n; CutScene defines the cut scene to use\n;\n\n", fdOutput);
		sprintf(szOutputString, "CutScene=%s\n\n", dlg.m_cutscene);
		fputs(szOutputString, fdOutput);

		fputs(";\n; Set UseSecond\n;\n\n", fdOutput);
		if(dlg.m_usesecond)
			fputs("UseSecond=true\n\n", fdOutput);
		else
			fputs("UseSecond=false\n\n", fdOutput);

		fputs(";\n; Set UseCutScene1\n;\n\n", fdOutput);
		if(dlg.m_usecutscene2)
			fputs("UseCutScene1=true\n\n", fdOutput);
		else
			fputs("UseCutScene1=false\n\n", fdOutput);

		fputs(";\n; SplashScreen defines the splash screen to use\n;\n\n", fdOutput);
		sprintf(szOutputString, "SplashScreen1=%s\n\n", dlg.m_splashscreen2);
		fputs(szOutputString, fdOutput);

		fputs(";\n; SplashAudio defines the splash screen audio to use\n;\n\n", fdOutput);
		sprintf(szOutputString, "SplashAudio1=%s\n\n", dlg.m_splashaudio2);
		fputs(szOutputString, fdOutput);

		fputs(";\n; CutScene defines the cut scene to use\n;\n\n", fdOutput);
		sprintf(szOutputString, "CutScene1=%s\n\n", dlg.m_cutscene2);
		fputs(szOutputString, fdOutput);

		fclose(fdOutput);
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
