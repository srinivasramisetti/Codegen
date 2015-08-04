// codegen.h : main header file for the CODEGEN application
//

#if !defined(AFX_CODEGEN_H__DBD904C0_206F_4A28_976A_E65B533A1FFB__INCLUDED_)
#define AFX_CODEGEN_H__DBD904C0_206F_4A28_976A_E65B533A1FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define CODEGEN_VERSION_STRING "codegen version v35_20"
#define MAX_AUTO_GEN_FUNCT_NAME_SIZE 32

/////////////////////////////////////////////////////////////////////////////
// CCodegenApp:
// See codegen.cpp for the implementation of this class
//

class CCodegenApp : public CWinApp
{
public:
	CCodegenApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodegenApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCodegenApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODEGEN_H__DBD904C0_206F_4A28_976A_E65B533A1FFB__INCLUDED_)
