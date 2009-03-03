#if !defined(AFX_UTILS_H__A85A5564_DCDD_11D3_8625_D268D3FDEB93__INCLUDED_)
#define AFX_UTILS_H__A85A5564_DCDD_11D3_8625_D268D3FDEB93__INCLUDED_


CString InitStateMachineName(void);
CString InitStateConfigData(void);
CString InitSuperStateConfigData(void);
CString InitCondBitData(void);
CString InitDontCareBitData(void);

CString CheckFSMOutputFile(CString OutputFile);

CString GetModelFile(void);
CString GetOutputFilePath(void);

int CreateTextFile(FILE *fin, FILE *fout);
static char* getRtfToken(/*const char* fname,*/ FILE* fin, int* plineno, int* plen);

CString GetCStringField(int FieldNumber, CString PassedCString);
CString CStringFirstCharCheck(CString PassedCString);

#define CONFIG_FILE_IDLE 0
#define CONFIG_FILE_PROCESSING 1
#define CONFIG_FILE_DONE 2

#endif /* AFX_UTILS_H__A85A5564_DCDD_11D3_8625_D268D3FDEB93__INCLUDED_ */