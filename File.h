// File class header file

#if !defined(AFX_FILE_H__A85A5564_DCDD_11D3_8625_D268D3FDEB93__INCLUDED_)
#define AFX_FILE_H__A85A5564_DCDD_11D3_8625_D268D3FDEB93__INCLUDED_

#define MAX_CHARS		    1000		// Number of characters in a single line

#define	FILE_OK				0
#define	FILE_ERROR     		1
#define FILE_CLOSED			0
#define FILE_OPEN			1

#define LOOKING_FOR_FIRST_QUOTE		0
#define LOOKING_FOR_SECOND_QUOTE	1

//*********************************************************
// Name: File class
//
// Description:	This is the class for file opening,
//				reading, etc.
//*********************************************************
class File
{
public:
	// Constructor
	File(void) { FileStatus = FILE_CLOSED; }

	// The following functions operate directly on the file stored on the hard drive
	int OpenFileForReading(CString Filename);
	int OpenFileForReadingAndWriting(CString Filename);
    int OpenNewFile(CString Filename);
    CString GetFileName(void) { return(FileName); }
	CString GetFilePath(void) { return(FileNameAndPath); }
	CString GetFileNameNoPath(void) { return(FileNameNoPath); }
	void CloseFile(void);
	BOOL EOFCheck(void);
	int GetFileStatus(void) { return(FileStatus); }
	void ResetFilePointer(void);
    void PutString(CString String);
    void PutChar(char character);
	CString GetFileNameNoSuffix(void);
	FILE *GetFilePointer(void);

	// The following functions operate on a single file line stored in FileLine
	void StoreFileLine(void);
	void GoBackTwoLines(void);
    CString GetField(int FieldNumber);
    int GetNumChars(void);
    char GetChar(int Index);
    CString GetLine(void) { return(FileLine); }

	// The following functions operate on the entire file stored in the EntireFile array
	void StoreEntireFile(void);
    int GetNumberFileLines(void) { return(NumFileLines); }
    CString GetEntireFileLine(int LineNumber);
//protected:
private:
    CString FileName; // To store the opened filename
	CString FileNameAndPath; // To store the opened filename and full path
	CString FileNameNoPath; // To store the filename without the path
	FILE *FilePointer; // To store the opened file pointer
	int FileStatus; // Status to indicate opened or closed

	CString FileLine; // To store a file line

	char *OneFileLineBack_ptr;
	int OneFileLineBack_cnt;
	char *TwoFileLinesBack_ptr;
	int TwoFileLinesBack_cnt;

	// The following two items use the entire file stored in the EntireFile array
    CString *BegEntireFilePtr; // Beginning pointer to the whole file storage array
    CString *EntireFilePtr; // Pointer to the whole file storage array
    int NumFileLines; // To store the number of file lines stored in EntireFile
};

#endif // !defined(AFX_FILE_H__A85A5564_DCDD_11D3_8625_D268D3FDEB93__INCLUDED_)
