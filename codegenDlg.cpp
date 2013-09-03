// codegenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "codegen.h"
#include "Globals.h"
#include "codegenDlg.h"
#include "Utils.h"
#include "ModelProcessing.h"
#include "OutputProcessing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodegenDlg dialog

CCodegenDlg::CCodegenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCodegenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCodegenDlg)
	m_CheckMutExcOnly = FALSE;
	m_EditVersion = _T(CODEGEN_VERSION_STRING);
	m_EditVersion += " (";
	m_EditVersion += __DATE__;
	m_EditVersion += ")";
	m_ComboConfigFileData = _T("");
	m_FunctNameSizeLimit = _T("");
	m_BackwardCompModeSelection = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCodegenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCodegenDlg)
	DDX_Control(pDX, IDC_COMBO1_CONFIG_FILE, m_ComboConfigFile);
	DDX_Check(pDX, IDC_CHECK_MUT_EXC_ONLY, m_CheckMutExcOnly);
	DDX_Text(pDX, IDC_EDIT_VERSION, m_EditVersion);
	DDX_CBString(pDX, IDC_COMBO1_CONFIG_FILE, m_ComboConfigFileData);
	DDX_Text(pDX, IDC_EDIT1_FUNCT_NAME_SIZE_LIMIT, m_FunctNameSizeLimit);
	DDX_Check(pDX, IDC_CHECK1_BACKWARDS_COMP_MODE, m_BackwardCompModeSelection);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCodegenDlg, CDialog)
	//{{AFX_MSG_MAP(CCodegenDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FILE_SELECT_BUTTON, OnFileSelectButton)
	ON_CBN_SELCHANGE(IDC_COMBO1_CONFIG_FILE, OnSelchangeCombo1ConfigFile)
	ON_BN_CLICKED(IDC_CHECK1_BACKWARDS_COMP_MODE, OnCheck1BackwardsCompMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Storage for a copy of the .ini file data
CStringArray IniFileCopy;

/////////////////////////////////////////////////////////////////////////////
// CCodegenDlg message handlers

BOOL CCodegenDlg::OnInitDialog()
{
	int ii;
	CString TempFileLine;
	CDialog::OnInitDialog();
    CString TempCString, TempCString1, TempCString2, TempCString3, TempCString4;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
    // IF1 A .ini file exists
    if(FILE_OK == IniFile.OpenFileForReading(".\\codegen.ini"))
	{ // THEN1 Close it again
		IniFile.CloseFile();
		if(FILE_OK == IniFile.OpenFileForReadingAndWriting(".\\codegen.ini"))
		{ // IF2 The file is not a read only file
			// THEN2 Store the ini file for processing below
			IniFile.StoreEntireFile();

			// Prepare for processing below
			IniFile.ResetFilePointer();
			
			// Process the .ini file.  This supports both the old and new formats.
			// The old format only had 1 field, the path to the most
			// recently used config text file.
			// The new format has 4 fields:
			// Field 1 - __FUNCT_NAME_SIZE_LIMIT tag
			// Field 2 - Auto-generated function name size limit value enclosed in double quotes
			// Field 3 - __CONFIG_TEXT_FILE_NAME tag
			// Field 4 - The config text file name enclosed in double quotes

			// Put previous config files used in pulldown
			// Also put in the previously used auto-generated function name size limit in the dialog
			for(ii = 0; ii < IniFile.GetNumberFileLines(); ii++)
			{
				IniFile.StoreFileLine();
				IniFileCopy.Add(IniFile.GetEntireFileLine(ii));
				if(("__FUNCT_NAME_SIZE_LIMIT" == IniFile.GetField(1)) && ("__CONFIG_TEXT_FILE_NAME" == IniFile.GetField(3)))
				{ // THEN3 Process the new format
					// IF4 This is the first time
					if(0 == ii)
					{ // THEN4
						// Store the previous auto-generated function name size limit in the dialog
						m_FunctNameSizeLimit = IniFile.GetField(2);

						// IF6 codegen version 34 backwards compatibility mode was selected
						// last time for this entry
						if("XXX" == m_FunctNameSizeLimit)
						{ // THEN6 Default the backwards compatibility mode checkbox to selected
							m_BackwardCompModeSelection = TRUE;
						}
						
						// Store the previous config text file in the dialog
						m_ComboConfigFileData = IniFile.GetField(4);

						// Store the previous config text file in the first entry in the pulldown
						m_ComboConfigFile.InsertString((0), m_ComboConfigFileData);
					}
					else
					{ // ELSE4 Store previous config text file entries in the pulldown
						TempCString = IniFile.GetField(4);
						TempCString.Remove('\n');
						m_ComboConfigFile.InsertString((ii), TempCString);
					}
				}
				else
				{ // ELSE3 Process the old format
					// IF5 This is the first time
					if(0 == ii)
					{ // THEN5
						// Store XXX for the auto-generated function name size limit in the dialog
						m_FunctNameSizeLimit = "XXX";
						
						// Default the backwards compatibility mode checkbox to selected
						m_BackwardCompModeSelection = TRUE;
						
						// Store the previous config text file in the dialog
						TempFileLine = IniFile.GetEntireFileLine(0);
						TempFileLine.Remove('\n');
						m_ComboConfigFileData = TempFileLine;

						// Store the previous config text file in the first entry in the pulldown
						m_ComboConfigFile.InsertString((0), m_ComboConfigFileData);
					}
					else
					{ // ELSE5 Store previous config text file entries in the pulldown
						TempFileLine = IniFile.GetEntireFileLine(ii);
						TempFileLine.Remove('\n');
						m_ComboConfigFile.InsertString((ii), TempFileLine);
					}
				}
			}

			// Close the .ini file
			IniFile.CloseFile();
		}
		else
		{ // ELSE2 Output an error message and stop
			::MessageBox(0, "The codegen.ini file is read only", "Error", MB_ICONERROR);
			CDialog::OnOK();
		}
	}
	else
	{ // ELSE1 Create a new .ini file
		IniFile.OpenNewFile(".\\codegen.ini");
        // and the close it
        IniFile.CloseFile();
	}
	
	// Update the window display
    UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CCodegenDlg::OnPaint() 
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
HCURSOR CCodegenDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//*********************************************************
// Name: OnOK
//
// Description:	This performs the main processing of
// codegen.
//
// NOTE: This actually is the Process button (I renamed
// the original OK function).
//*********************************************************
void CCodegenDlg::OnOK() 
{
    int FileOpenStatus, ModelFileStatus, Index, ii;
    CString ConfigFileStatus, ProcessModelFileStatus, FSMOutputFileStatus, MutualExclusiveStatus;
	CString ProcessStateSuperStatesTargetsStatus;
    CString TempCString, TempCString2 = "The following files were updated:\n\n", TempCString3, TempTextModelFile;
	CString StateConfigFileModelMismatchStatus = "OK";
	CString LocalOutputFilePath, CheckForTransitionErrorsStatus, CheckPowerupTransitionTargetStatus;
	CString TempMessage;
	char TempArray[50];
	
	// Read current system time and date
	GetLocalTime(&CurrentLocalTime);
    
    // Get the entered information
    UpdateData(TRUE);
	
    // IF1 The config file window is blank
    if("" == m_ComboConfigFileData)
    { // THEN1 Display error
        ::MessageBox(0, "Please enter a configuration file", "Stop", MB_ICONSTOP);
    }
	// ELSE1 IF111 The auto-generated symbol name size limit is blank
    else if("" == m_FunctNameSizeLimit)
    { // ELSE1 THEN111 Display error
        ::MessageBox(0, "Please enter an auto-generated symbol name size limit", "Stop", MB_ICONSTOP);
    }
    else
    { // ELSE111 Attempt to process the config file and the model file
        // Attempt to open the config file
        FileOpenStatus = ConfigFile.OpenFileForReading(m_ComboConfigFileData);
        
        // IF1 The requested config file exists
        if(FILE_OK == FileOpenStatus)
        { // THEN1 Check for the model file marker in the config file
			ConfigFileStatus = GetModelFile();
			// IF1_1 The correct marker is found in the config file
			if("No __" != ConfigFileStatus.Left(5))
			{ // THEN1_1 IF1_2 The model file is a .rtf file
				if(".rtf" == ConfigFileStatus.Right(4))
				{
					// THEN1_2 Attempt to open the model file
					FileOpenStatus = RichTextModelFile.OpenFileForReading(ConfigFileStatus);                        
					// IF2 The model files exists
					if(FILE_OK == FileOpenStatus)
					{ // THEN2 Prepare to convert the RTF file to a text file
						// Open a new text model file with a unique name
						TempTextModelFile = RichTextModelFile.GetFileNameNoSuffix() + "_";
						TempTextModelFile += _itoa(CurrentLocalTime.wHour, &TempArray[0], 10);
						TempTextModelFile += _itoa(CurrentLocalTime.wMinute, &TempArray[0], 10);
						TempTextModelFile += _itoa(CurrentLocalTime.wMonth, &TempArray[0], 10);
						TempTextModelFile += _itoa(CurrentLocalTime.wDay, &TempArray[0], 10);
						TempTextModelFile += _itoa(CurrentLocalTime.wYear, &TempArray[0], 10);
						TempTextModelFile += ".txt";
						FileOpenStatus = ModelFile.OpenNewFile(TempTextModelFile);
						// IF2_3 The text file is not read only
						if(FILE_OK == FileOpenStatus)
						{
							// THEN2_3 Convert the RTF to a text file
							// Seem to need to close the file and then open again to make work
							// properly under all circumstances (e.g. if the file exists, if
							// it doesn't, etc.)
							ModelFile.CloseFile();
							FileOpenStatus = ModelFile.OpenFileForReadingAndWriting(TempTextModelFile);
							ModelFileStatus = CreateTextFile(RichTextModelFile.GetFilePointer(), ModelFile.GetFilePointer());
							
							// IF2_1 The RTF to text conversion went ok
							if(FILE_OK == ModelFileStatus)
							{ // THEN2_1 Reset the model file file pointer
								ModelFile.ResetFilePointer();
								// Check for the output file path marker in the config file
								ConfigFileStatus = GetOutputFilePath();
								// IF2_2 The correct marker is found in the config file
								if("No __" != ConfigFileStatus.Left(5))
								{ // THEN1_1 Init the state config data
									ConfigFileStatus = InitStateConfigData();
									// IF3 It initialized ok
									if("OK" == ConfigFileStatus)
									{ // THEN3 Init the super state config data
										ConfigFileStatus = InitSuperStateConfigData();
										// IF4 It initialized ok
										if("OK" == ConfigFileStatus)
										{ // THEN4 Init the cond bit data
											ConfigFileStatus = InitCondBitData();
											// IF5 It initialized ok
											if("OK" == ConfigFileStatus)
											{ // THEN5 Init the don't care bit data
												ConfigFileStatus = InitDontCareBitData();
												// IF6 It initialized ok
												if("OK" == ConfigFileStatus)
												{ // THEN6 Init the state machine name global
													ConfigFileStatus = InitStateMachineName();
												}
											}
										}
									}
									// IF5 All went well with the config file above
									if("OK" == ConfigFileStatus)
									{ // THEN5 Process the text model file generated from Rhapsody
										// and generate a C source state machine configuration file                    
										
										// Check the FSM output files for existence and proper formatting
										// Skip the check if mutual exclusion check only is selected
										LocalOutputFilePath = GetOutputFilePath();
										LocalOutputFilePath.Insert(MAX_CHARS, '\\');
										FSMOutputFilesPath = LocalOutputFilePath;
										TempCString = LocalOutputFilePath + StateMachineName + "_" + "fsm_config.c";
										TempCString2 += TempCString + '\n';
										FSMOutputFileStatus = CheckFSMOutputFile(TempCString);
										// IF6 The <FSM name>_fsm_config.c file is ok or a mutual
										// exclusion only check was selected
										if(("OK" == FSMOutputFileStatus) || (TRUE == m_CheckMutExcOnly))
										{ // THEN6 Update the processing complete file list
											TempCString = LocalOutputFilePath + StateMachineName + "_" + "fsm_functs.c";
											FSMOutputFileStatus = CheckFSMOutputFile(TempCString);
											// IF7 The <FSM name>_fsm_functs.c file is ok or a mutual
											// exclusion only check was selected
											if(("OK" == FSMOutputFileStatus) || (TRUE == m_CheckMutExcOnly))
											{ // THEN7 Update the processing complete file list
												TempCString2 += TempCString + '\n';
												TempCString = LocalOutputFilePath + StateMachineName + "_" + "fsm_public.h";
												FSMOutputFileStatus = CheckFSMOutputFile(TempCString);
												// IF8 The <FSM name>_fsm_public.h file is ok or a mutual
												// exclusion only check was selected
												if(("OK" == FSMOutputFileStatus) || (TRUE == m_CheckMutExcOnly))
												{ // THEN8 Update the processing complete file list
													TempCString2 += TempCString + '\n';
													TempCString = LocalOutputFilePath + StateMachineName + "_" + "fsm_private.h";
													FSMOutputFileStatus = CheckFSMOutputFile(TempCString);
													// IF9 The <FSM name>_fsm_private.h file is ok or a mutual
													// exclusion only check was selected
													if(("OK" == FSMOutputFileStatus) || (TRUE == m_CheckMutExcOnly))
													{ // THEN9 Update the processing complete file list
														// Init the initial state name global
														TempCString2 += TempCString + '\n';
														
														// Start back at the beg of the text model file
														ConfigFile.ResetFilePointer();
														
														// Store the selected auto-generated function name size limit in a global
														// for use in during model file processing
														// IF99 XXX is in the symbol name size limit box
														if("XXX" == m_FunctNameSizeLimit)
														{ // THEN99 Set the global name size limit to the default
															AutoGenFunctNameLimit = MAX_AUTO_GEN_FUNCT_NAME_SIZE;
														}
														else
														{ // ELSE99 Set the global name size limit to the selected limit
															AutoGenFunctNameLimit = atoi(m_FunctNameSizeLimit);
														}
														// Store the selected auto-generated function name size backwards
														// compatibility mode in a global for use in during model file processing
														BackwardCompModeSelected = m_BackwardCompModeSelection;
														
														// Store information from the text model file in the Diagram object
														ProcessModelFileStatus = ProcessModelFile();
														// IF10 There were no model file processing errors
														if("OK" == ProcessModelFileStatus)
														{ // THEN10 IF11 The number of states in the config file
															// doesn't match the number in the model
															if(Diagram.GetNumStates(NON_SUPER_STATE) != Diagram.GetNumStatesInConfigFile())
															{ // THEN11 Prepare to display an error
																StateConfigFileModelMismatchStatus = "State mismatch.  This is caused by either:";
																StateConfigFileModelMismatchStatus += '\n';
																StateConfigFileModelMismatchStatus += "   1) The number of states in the \"";
																StateConfigFileModelMismatchStatus += ConfigFile.GetFileNameNoPath();
																StateConfigFileModelMismatchStatus += "\" file doesn't match the number of states in the model";
																StateConfigFileModelMismatchStatus += '\n';
																StateConfigFileModelMismatchStatus += "   2) There is a state in the \"";
																StateConfigFileModelMismatchStatus += ConfigFile.GetFileNameNoPath();
																StateConfigFileModelMismatchStatus += "\" file that is not in the model";
															}
															
															// IF12 The number of super states in the config
															// file doesn't match the number in the model file
															if(Diagram.GetNumStates(SUPER_STATE) != Diagram.GetNumSuperStatesInConfigFile())
															{ // THEN12 Prepare to display an error
																//IF13 There wasn't a state number mismatch
																if("OK" == StateConfigFileModelMismatchStatus)
																{ // THEN13 Prepare to display a super state error
																	StateConfigFileModelMismatchStatus = "Super state mismatch.  This is caused by either:";
																	StateConfigFileModelMismatchStatus += '\n';
																	StateConfigFileModelMismatchStatus += "   1) The number of super states in the \"";
																	StateConfigFileModelMismatchStatus += ConfigFile.GetFileNameNoPath();
																	StateConfigFileModelMismatchStatus += "\" file doesn't match the number of super states in the model";
																	StateConfigFileModelMismatchStatus += '\n';
																	StateConfigFileModelMismatchStatus += "   2) There is a super state in the \"";
																	StateConfigFileModelMismatchStatus += ConfigFile.GetFileNameNoPath();
																	StateConfigFileModelMismatchStatus += "\" file that is not in the model";
																}
																else
																{ // ELSE13 Add the super state error
																	StateConfigFileModelMismatchStatus += "\n\nSuper state mismatch.  This is caused by either:";
																	StateConfigFileModelMismatchStatus += '\n';
																	StateConfigFileModelMismatchStatus += "   1) The number of super states in the \"";
																	StateConfigFileModelMismatchStatus += ConfigFile.GetFileNameNoPath();
																	StateConfigFileModelMismatchStatus += "\" file doesn't match the number of super states in the model";
																	StateConfigFileModelMismatchStatus += '\n';
																	StateConfigFileModelMismatchStatus += "   2) There is a super state in the \"";
																	StateConfigFileModelMismatchStatus += ConfigFile.GetFileNameNoPath();
																	StateConfigFileModelMismatchStatus += "\" file that is not in the model";
																}
															}
															// IF14 There was a state number mismatch
															if("OK" != StateConfigFileModelMismatchStatus)
															{ // THEN14 Display the error
																::MessageBox(0, StateConfigFileModelMismatchStatus, "Error", MB_ICONERROR);
																// Close the dialog because of the error
																CDialog::OnOK();
															}
															else
															{ // ELSE14 Store additional super/substate information in the Diagram object
																ProcessSuperStateTrans();
																// Replace target super states with default trans targets in the Diagram object
																ProcessStateSuperStatesTargetsStatus = ProcessStateSuperStatesTargets();
																// Replace the powerup trans target with non-super state if it is a super state
																// This needs to be called after the ProcessStateSuperStatesTargets call
																CheckPowerupTransitionTargetStatus = CheckPowerupTransitionTarget(ProcessStateSuperStatesTargetsStatus);
																
																// Update state trans with addl cond and don't care masks from config file super state data
																UpdateTransWithAddlCondDontCareMasks();
																// Add trans to sub-states in the Diagram object
																AddTransFromSuperStates();
																// IF15 The super state name replacement went ok
																if("OK" == CheckPowerupTransitionTargetStatus)
																{ // THEN15 Check for transition errors
																	CheckForTransitionErrorsStatus = CheckForTransitionErrors();
																	// IF16 There were no transition errors
																	if("" == CheckForTransitionErrorsStatus)
																	{ // Check all trans for mutual exclusion
																		MutualExclusiveStatus = TransMutualExclusionCheck();
																		// IF17 A trans mutual exclusion only is selected
																		if(TRUE == m_CheckMutExcOnly)
																		{ // THEN17 Display the results of the check
																			::MessageBox(0, MutualExclusiveStatus, "Check complete", MB_ICONWARNING);
																		}
																		else
																		{ // ELSE17 IF18 Non-mutually exclusive transitions were found
																			if("The following" == MutualExclusiveStatus.Left(13))
																			{ // THEN18 Display the non-mutually exclusive transitions
																				::MessageBox(0, MutualExclusiveStatus, "Check complete", MB_ICONWARNING);
																			}
																			// Generate update FSM output files
																			UpdateFSMFiles();
																			// Report what files were updated
																			::MessageBox(0, TempCString2, "Processing complete", MB_OK);
																		}
																	}
																	else
																	{ // ELSE16 Report the transition errors
																		TempCString = "The following transition errors were found (limited to 50):\n\n";
																		TempCString += CheckForTransitionErrorsStatus;
																		::MessageBox(0, TempCString, "Transition errors", MB_ICONERROR);
																		// Prepare to close the dialog because of the error
																		// at the end of processing below
																		CDialog::OnOK();
																	}
																}
																else
																{ // ELSE15 Report the error
																	::MessageBox(0, CheckPowerupTransitionTargetStatus, "Error", MB_ICONERROR);
																	// Prepare to close the dialog because of the error
																	// at the end of processing below
																	CDialog::OnOK();
																}
															}
															// Processing complete so prepare to close the dialog box
															// at the end of processing below
															CDialog::OnOK();
														}
														else
														{ // ELSE10 Prepare to close the dialog because of the error
															// at the end of processing below
															CDialog::OnOK();
														}
													}
													else
													{ // ELSE9 Report the error and prepare to close the dialog
														// at the end of processing below (this close dialog and
														// the close dialogs in ELSE8-ELSE5 below are needed
														// because of the use of dynamic memory use in the
														// Diagram object)
														TempCString = TempCString + " " + FSMOutputFileStatus;
														::MessageBox(0, TempCString, "Error", MB_ICONERROR);
														CDialog::OnOK();
													}
												}
												else
												{ // ELSE8 Report the error and prepare to close the dialog
													// at the end of processing below
													TempCString = TempCString + " " + FSMOutputFileStatus;
													::MessageBox(0, TempCString, "Error", MB_ICONERROR);
													CDialog::OnOK();
												}
											}
											else
											{ // ELSE7 Report the error and prepare to close the dialog
												// at the end of processing below
												TempCString = TempCString + " " + FSMOutputFileStatus;
												::MessageBox(0, TempCString, "Error", MB_ICONERROR);
												CDialog::OnOK();
											}
										}
			                            else
			                            { // ELSE6 Report the error and prepare to close the dialog
			                                // at the end of processing below
			                                TempCString = TempCString + " " + FSMOutputFileStatus;
			                                ::MessageBox(0, TempCString, "Error", MB_ICONERROR);
			                                CDialog::OnOK();
			                            }
			                        }
			                        else
			                        { // ELSE5 Output error message and prepare to close the dialog
			                            // at the end of processing below
			                            ::MessageBox(0, ConfigFileStatus, "Error", MB_ICONERROR);
			                            CDialog::OnOK();
									}
								}
								else
								{ // ELSE2_2 Output a __OUTPUT_FILE_PATH marker not found in configuration file" message
									TempMessage = "__OUTPUT_FILE_PATH marker not found in the \"";
									TempMessage += ConfigFile.GetFileNameNoPath();
									TempMessage += "\" configuration file";
									::MessageBox(0, TempMessage, "Error", MB_ICONERROR);
								}
							}
							else
							{ // ELSE2_1 Output a "Model file does not exist" message
								TempMessage = "The  \"";
								TempMessage += RichTextModelFile.GetFileNameNoPath();
								TempMessage += "\" model file is not a properly formatted rich text file";
								TempMessage += "\nDouble check the __MODEL_FILE entry in the \"";
								TempMessage += ConfigFile.GetFileNameNoPath();
								TempMessage += "\" configuration file";
								::MessageBox(0, TempMessage, "Error", MB_ICONERROR);
							}
						}
						else
						{ // ELSE2_3 Output a "The temporary model text file is read only (please delete and try again)" message
								TempMessage = "codegen tried to create a temporary model text file with the name \"";
								TempMessage += TempTextModelFile;
								TempMessage += "\"";
								TempMessage += "\n\nFor some reason this file was already in existence and is a read only file";
								TempMessage += "\nPlease delete this file and try again.";
								::MessageBox(0, TempMessage, "Error", MB_ICONERROR);
						}
						// Delete the text model file (close it first)
						if(FILE_OPEN == ModelFile.GetFileStatus())
						{
							ModelFile.CloseFile();
						}
						ModelFileStatus = remove(ModelFile.GetFilePath());
					}
					else
					{ // ELSE2 Output a "Model file does not exist" message
						TempMessage = "The model file does not exist";
						TempMessage += "\nDouble check the __MODEL_FILE entry in the \"";
						TempMessage += ConfigFile.GetFileNameNoPath();
						TempMessage += "\" configuration file";
						::MessageBox(0, TempMessage, "Error", MB_ICONERROR);
					}
				}
				else
				{ // ELSE1_2 Output a "Model file does not have an rtf extension" message
				TempMessage = "The model file does not have an rtf extension";
				TempMessage += "\nDouble check the __MODEL_FILE entry in the \"";
				TempMessage += ConfigFile.GetFileNameNoPath();
				TempMessage += "\" configuration file";
					::MessageBox(0, TempMessage, "Error", MB_ICONERROR);
				}
			}
			else
			{ // ELSE1_1 Output a "__MODEL_FILE marker not found in configuration file" message
				TempMessage = "__MODEL_FILE marker not found in the \"";
				TempMessage += ConfigFile.GetFileNameNoPath();
				TempMessage += "\" configuration file";
				::MessageBox(0, TempMessage, "Error", MB_ICONERROR);
			}
			
			// Update the ini file
			// Look for the selected config file in the ini file (really in the combo
			// box initialized from the data in the ini file)
			Index = m_ComboConfigFile.FindStringExact(0, m_ComboConfigFileData);
			// IF12 The selected config file is already in the ini file
			if(CB_ERR != Index)
			{ // THEN12 Remove the item and refresh the combo box
				m_ComboConfigFile.DeleteString(Index);
				IniFileCopy.RemoveAt(Index, 1);
			}
			// Insert the selected config file at the beginning of the ini file list
			m_ComboConfigFile.InsertString(0, m_ComboConfigFileData);
			
			TempCString3 = "__FUNCT_NAME_SIZE_LIMIT ";
			TempCString3 += m_FunctNameSizeLimit;
			TempCString3 += " __CONFIG_TEXT_FILE_NAME \"";
			TempCString3 += m_ComboConfigFileData;
			TempCString3 += '\"';
			TempCString3 += '\n';
			IniFileCopy.InsertAt(0, TempCString3);
			
			// Open a new .ini file
			IniFile.OpenNewFile(IniFile.GetFilePath());

			// Store the model file list in the new .ini file
			for(ii = 0; ii < m_ComboConfigFile.GetCount(); ii++)
			{
				IniFile.PutString(IniFileCopy.GetAt(ii));
			}
			// Close the new .ini file
			IniFile.CloseFile();
        }
        else
        { // ELSE1 Output a "Configuration file does not exist" message
            ::MessageBox(0, "The entered configuration file does not exist", "Error", MB_ICONERROR);
        }
		
		// Refresh the window display.  After an error, the data in the config file window would
		// clear out for some reason.  This statement prevents this from happening.
		UpdateData(FALSE);
		
        // Make sure files are closed before stopping
        if(FILE_OPEN == ConfigFile.GetFileStatus())
        {
            ConfigFile.CloseFile();
        }
        if(FILE_OPEN == RichTextModelFile.GetFileStatus())
        {
            RichTextModelFile.CloseFile();
        }
        if(FILE_OPEN == OutputFSMConfigFile.GetFileStatus())
        {
            OutputFSMConfigFile.CloseFile();
        }
        if(FILE_OPEN == OutputFSMPrivateFile.GetFileStatus())
        {
            OutputFSMPrivateFile.CloseFile();
        }
        if(FILE_OPEN == OutputFSMPublicFile.GetFileStatus())
        {
            OutputFSMPublicFile.CloseFile();
        }
        if(FILE_OPEN == OutputFSMFunctsFile.GetFileStatus())
        {
            OutputFSMFunctsFile.CloseFile();
        }
    }
}

//*********************************************************
// Name: OnCancel
//
// Description:	This closes all open files and cancels the
// main dialog box.
//*********************************************************
void CCodegenDlg::OnCancel() 
{
    // Close all open files
    if(FILE_OPEN == ConfigFile.GetFileStatus())
    {
        ConfigFile.CloseFile();
    }
    if(FILE_OPEN == RichTextModelFile.GetFileStatus())
    {
        RichTextModelFile.CloseFile();
    }
    if(FILE_OPEN == OutputFSMConfigFile.GetFileStatus())
    {
        OutputFSMConfigFile.CloseFile();
    }
    if(FILE_OPEN == OutputFSMPrivateFile.GetFileStatus())
    {
        OutputFSMPrivateFile.CloseFile();
    }
    if(FILE_OPEN == OutputFSMPublicFile.GetFileStatus())
    {
        OutputFSMPublicFile.CloseFile();
    }
    if(FILE_OPEN == OutputFSMFunctsFile.GetFileStatus())
    {
        OutputFSMFunctsFile.CloseFile();
    }
	
	CDialog::OnCancel();
}


//*********************************************************
// Name: OnFileSelectButton
//
// Description:	This opens a file dialog box and stores
// the filename and full path of the selected file in
// m_ComboConfigFileData.
//*********************************************************
void CCodegenDlg::OnFileSelectButton() 
{
	int Index;
	CString TempModelFilePath;

	CFileDialog FileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL);

	// Open a file dialog window
	FileDialog.DoModal();
	// Store the selected file and path in m_ComboConfigFileData
	TempModelFilePath = FileDialog.GetPathName();

	// IF1 A file selection was made (i.e. cancel was not selected)
	if("" != TempModelFilePath)
	{ // THEN1 Store the file selection
		m_ComboConfigFileData = TempModelFilePath;

		// Look for the selected config file in the ini file (really in the combo
		// box initialized from the data in the ini file)
		Index = m_ComboConfigFile.FindStringExact(0, m_ComboConfigFileData);
		// IF2 The selected config file is already in the ini file
		if(CB_ERR != Index)
		{ // THEN2 IF3 The selected .ini file entry in the pulldown is the new format
			if(("__FUNCT_NAME_SIZE_LIMIT" == GetCStringField(1, IniFileCopy.GetAt(Index))) &&\
				("__CONFIG_TEXT_FILE_NAME" == GetCStringField(3, IniFileCopy.GetAt(Index))))
			{ // THEN3 Update the auto-generated function name size limit box in the dialog with the entry in the .ini file
				m_FunctNameSizeLimit = GetCStringField(2, IniFileCopy.GetAt(Index));
				// IF4 codegen version 34 backwards compatibility mode was selected
				// last time for this entry
				if("XXX" == m_FunctNameSizeLimit)
				{ // THEN4 Set the backwards compatibility mode checkbox to selected
					m_BackwardCompModeSelection = TRUE;
				}
				else
				{ // ELSE4 Set the backwards compatibility mode checkbox to not selected
					m_BackwardCompModeSelection = FALSE;
				}
			}
			else
			{ // ELSE3 Store XXX for the auto-generated function name size limit in the dialog
				m_FunctNameSizeLimit = "XXX";				
				// Default the backwards compatibility mode checkbox to selected
				m_BackwardCompModeSelection = TRUE;
			}
		}
		else
		{ // ELSE1 Store blank for the auto-generated function name size limit in the dialog
			m_FunctNameSizeLimit = "";				
			// Default the backwards compatibility mode checkbox to selected
			m_BackwardCompModeSelection = FALSE;
		}
		// Update the window display
		UpdateData(FALSE);
	}
}

//*********************************************************
// Name: OnSelchangeCombo1ConfigFile
//
// Description:	This updates the auto-generated function
// name limit display and backwards compatibility mode
// check box in the dialog based on the selected
// config text file in the dialog pulldown.
//*********************************************************
void CCodegenDlg::OnSelchangeCombo1ConfigFile() 
{
	int SelectedPullDownIndex;
	CString TempCString;

    // Get the entered information
    UpdateData(TRUE);

	// Store the selected pull down index
	SelectedPullDownIndex = m_ComboConfigFile.GetCurSel();
	
	// IF1 The selected .ini file entry in the pulldown is the new format
	if(("__FUNCT_NAME_SIZE_LIMIT" == GetCStringField(1, IniFileCopy.GetAt(SelectedPullDownIndex))) &&\
		("__CONFIG_TEXT_FILE_NAME" == GetCStringField(3, IniFileCopy.GetAt(SelectedPullDownIndex))))
	{ // THEN1 Update the auto-generated function name size limit box in the dialog with the entry in the .ini file
		m_FunctNameSizeLimit = GetCStringField(2, IniFileCopy.GetAt(SelectedPullDownIndex));
		
		// IF2 codegen version 34 backwards compatibility mode was selected
		// last time for this entry
		if("XXX" == m_FunctNameSizeLimit)
		{ // THEN2 Set the backwards compatibility mode checkbox to selected
			m_BackwardCompModeSelection = TRUE;
		}
		else
		{ // ELSE2 Set the backwards compatibility mode checkbox to not selected
			m_BackwardCompModeSelection = FALSE;
		}
	}
	else
	{ // ELSE1 Update the auto-generated function name size limit box in the dialog with the default size
		m_FunctNameSizeLimit = "XXX";
		
		// Default the backwards compatibility mode checkbox to selected
		m_BackwardCompModeSelection = TRUE;
	}
	
	// Update the config text file display in the dialog
	m_ComboConfigFile.GetLBText(SelectedPullDownIndex, TempCString);
	m_ComboConfigFileData = TempCString;
	
	// Update the window display
    UpdateData(FALSE);
}

//*********************************************************
// Name: OnCheck1BackwardsCompMode
//
// Description:	This updates the auto-generated function
// name limit display in the dialog based on the backwards
// compatibility mode check box selection.
//*********************************************************
void CCodegenDlg::OnCheck1BackwardsCompMode() 
{
	int Index;

    // Get the entered information
    UpdateData(TRUE);

	// IF1 codegen version 34 backwards compatibility mode was selected
	if(TRUE == m_BackwardCompModeSelection)
	{ // THEN1 Display dashes in the function name size edit window
		m_FunctNameSizeLimit = "XXX";
	}
	else
	{ // ELSE1 Look for the selected config file in the ini file (really in the combo
		// box initialized from the data in the ini file)
		Index = m_ComboConfigFile.FindStringExact(0, m_ComboConfigFileData);
		// IF2 The selected config file is already in the ini file
		if(CB_ERR != Index)
		{ // THEN2 IF3 The selected .ini file entry in the pulldown is the new format
			// and codegen version 34 backwards compatibility mode was
			// not selected last time
			if(("__FUNCT_NAME_SIZE_LIMIT" == GetCStringField(1, IniFileCopy.GetAt(Index))) &&\
				("__CONFIG_TEXT_FILE_NAME" == GetCStringField(3, IniFileCopy.GetAt(Index))) &&\
				("XXX" != GetCStringField(2, IniFileCopy.GetAt(Index))))
			{ // THEN3 Update the auto-generated function name size limit box in the dialog with the entry in the .ini file
				m_FunctNameSizeLimit = GetCStringField(2, IniFileCopy.GetAt(Index));
			}
			else
			{ // ELSE3 Update the auto-generated function name size limit box in the dialog with the default size
				m_FunctNameSizeLimit = "";
			}
		}
	}

	// Update the window display
    UpdateData(FALSE);
}
