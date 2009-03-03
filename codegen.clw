; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCodegenDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "codegen.h"

ClassCount=3
Class1=CCodegenApp
Class2=CCodegenDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CErrorDlg
Resource2=IDD_CODEGEN_DIALOG

[CLS:CCodegenApp]
Type=0
HeaderFile=codegen.h
ImplementationFile=codegen.cpp
Filter=N

[CLS:CCodegenDlg]
Type=0
HeaderFile=codegenDlg.h
ImplementationFile=codegenDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_CHECK1_BACKWARDS_COMP_MODE



[DLG:IDD_CODEGEN_DIALOG]
Type=1
Class=CCodegenDlg
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_CONFIG_FILE,static,1342308352
Control4=IDC_CHECK_MUT_EXC_ONLY,button,1342242819
Control5=IDC_EDIT_VERSION,edit,1342179456
Control6=IDC_COMBO1_CONFIG_FILE,combobox,1344340226
Control7=IDC_FILE_SELECT_BUTTON,button,1342242816
Control8=IDC_STATIC_FUNCT_NAME_SIZE_LIMIT,static,1342308352
Control9=IDC_EDIT1_FUNCT_NAME_SIZE_LIMIT,edit,1350631424
Control10=IDC_STATIC_AUTO_GEN_GROUP,button,1342177287
Control11=IDC_CHECK1_BACKWARDS_COMP_MODE,button,1342242819

[CLS:CErrorDlg]
Type=0
HeaderFile=ErrorDlg.h
ImplementationFile=ErrorDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_ERROR_EDIT
VirtualFilter=dWC

