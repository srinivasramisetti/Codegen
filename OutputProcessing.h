#if !defined(AFX_OUTPUTPROCESSING_H__A85A5564_DCDD_11D3_8625_D268D3FDEB93__INCLUDED_)
#define AFX_OUTPUTPROCESSING_H__A85A5564_DCDD_11D3_8625_D268D3FDEB93__INCLUDED_

#define LOOKING_FOR_BEG_AUTO_GEN_AREA   0
#define ADDING_AUTO_GEN_DATA            1
#define LOOKING_FOR_END_AUTO_GEN_AREA   2
#define FINISHING_UP                    3

void UpdateFSMFiles(void);
void UpdateConfigSourceFile(void);
void OutputTimeStampAndInfo(File FileToUpdate);
void OutputTransBeg(CString StateName);
void OutputTrans(CString TargetState, CString TransCondMask, CString TransDontCareMask,\
                 CString TransActionFunctionName);
void OutputTransEnd(void);
CString AddLeadingZeroes(CString IntCString);
void OutputStateBeg(CString StateMachineName);
void OutputStateEnd(void);
void OutputState(CString StateName);
void OutputMainStateMachineConfig(void);
void UpdatePublicHeaderFile(void);
void UpdateFunctsSourceFile(void);
void UpdatePrivateHeaderFile(void);
void UpdateDebugSourceFile(void);

#endif /* AFX_OUTPUTPROCESSING_H__A85A5564_DCDD_11D3_8625_D268D3FDEB93__INCLUDED_ */