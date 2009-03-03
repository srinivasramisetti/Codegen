// codegenDlg.h : header file
//

#if !defined(AFX_CODEGENDLG_H__556A5471_8CA4_47A6_AD67_97A1496D455C__INCLUDED_)
#define AFX_CODEGENDLG_H__556A5471_8CA4_47A6_AD67_97A1496D455C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCodegenDlg dialog

class CCodegenDlg : public CDialog
{
// Construction
public:
	CCodegenDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCodegenDlg)
	enum { IDD = IDD_CODEGEN_DIALOG };
	CComboBox	m_ComboConfigFile;
	BOOL	m_CheckMutExcOnly;
	CString	m_EditVersion;
	CString	m_ComboConfigFileData;
	CString	m_FunctNameSizeLimit;
	BOOL	m_BackwardCompModeSelection;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodegenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCodegenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnFileSelectButton();
	afx_msg void OnSelchangeCombo1ConfigFile();
	afx_msg void OnCheck1BackwardsCompMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODEGENDLG_H__556A5471_8CA4_47A6_AD67_97A1496D455C__INCLUDED_)
