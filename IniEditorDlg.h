// IniEditorDlg.h : header file
//

#if !defined(AFX_INIEDITORDLG_H__7B0FA1E6_D330_11D4_85EA_0060674A702D__INCLUDED_)
#define AFX_INIEDITORDLG_H__7B0FA1E6_D330_11D4_85EA_0060674A702D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SmartEdit.h"
#include "sliders.h"
#include "genesis.h"

/////////////////////////////////////////////////////////////////////////////
// CIniEditorDlg dialog

class CIniEditorDlg : public CDialog
{
// Construction
public:
	CIniEditorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIniEditorDlg)
	enum { IDD = IDD_INIEDITOR_DIALOG };
	CButton	m_ssbutton2;
	CButton	m_sabutton2;
	CButton	m_cutbutton2;
	CButton	m_ssbutton;
	CButton	m_sabutton;
	CButton	m_cutbutton;
	CButton	m_browsepack;
	CSmartEdit	m_editgamma;
	CComboBox	m_resolution;
	CComboBox	m_videolist;
	CLinkSlider	m_gamma;
	CButton	m_createini;
	CButton	m_levelbrowse;
	CButton	m_menubrowse;
	CButton	m_actbrowse;
	CEdit	m_GNEdit;
	CString	m_actorname;
	CString	m_menuname;
	CString	m_levelname;
	CString	m_gamename;
	int		m_gammavalue;
	CString	m_gammaamount;
	BOOL	m_weapon;
	int		m_videoindex;
	int		m_resindex;
	CString	m_packfile;
	BOOL	m_usedialog;
	BOOL	m_usecutscene;
	CString	m_splashscreen;
	CString	m_splashaudio;
	CString	m_cutscene;
	BOOL	m_usecutscene2;
	CString	m_cutscene2;
	CString	m_splashaudio2;
	BOOL	m_usesecond;
	CString	m_splashscreen2;
	BOOL	m_usefirst;
	BOOL	m_showtrack;
	BOOL	m_usecselect;
	//}}AFX_DATA
	TCHAR m_currentdir[512];
	geEngine			*m_pEngine;
	geDriver			*m_pDriver;
	geDriver_Mode		*m_pMode;
	geDriver_System		*m_pDriverSystem;
	CString	m_drivername;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIniEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	BOOL  m_fullscreen;
	int   m_width;
	int m_height;
	int count;

	// Generated message map functions
	//{{AFX_MSG(CIniEditorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnActbrowse();
	afx_msg void OnMenuini();
	afx_msg void OnStartlevel();
	afx_msg void OnSelchangeCombovideo();
	afx_msg void OnSelchangeCombores();
	afx_msg void OnPack();
	afx_msg void OnCbutton();
	afx_msg void OnSabutton();
	afx_msg void OnSsbutton();
	afx_msg void OnSsbutton2();
	afx_msg void OnSabutton2();
	afx_msg void OnCbutton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INIEDITORDLG_H__7B0FA1E6_D330_11D4_85EA_0060674A702D__INCLUDED_)
