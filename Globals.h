#include "File.h"
#include "DiagramClass.h"

#define NON_SUPER_STATE 0
#define SUPER_STATE     1

extern DiagramClass Diagram;

extern File RichTextModelFile;
extern File ModelFile;
extern File ConfigFile;

extern File OutputFSMConfigFile;
extern File OutputFSMPrivateFile;
extern File OutputFSMPublicFile;
extern File OutputFSMFunctsFile;

extern File IniFile;

extern CString FSMOutputFilesPath;
extern CString StateMachineName;

extern int AutoGenFunctNameLimit;
extern int BackwardCompModeSelected;

extern SYSTEMTIME CurrentLocalTime;
