#include "stdafx.h"
#include "Globals.h"

//************************************************************
// These are items global to the various functions.
//************************************************************

// The main FSM diagram object
DiagramClass Diagram;

// Files
File RichTextModelFile;
File ModelFile;
File ConfigFile;

File OutputFSMConfigFile;
File OutputFSMPrivateFile;
File OutputFSMPublicFile;
File OutputFSMFunctsFile;

File IniFile;

// Global data based on the information in the config file
CString FSMOutputFilesPath;
CString StateMachineName;

// Auto-generated function name length limit
int AutoGenFunctNameLimit;
// codegen version 34 backwards compatibility mode
int BackwardCompModeSelected;
// flag: generate debug code
BOOL GenerateDebugCode;

// Supports time stamping
SYSTEMTIME CurrentLocalTime;
