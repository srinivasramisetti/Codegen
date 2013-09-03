// File class implementation file

#include "stdafx.h"
#include "File.h"
#include "Utils.h"

//*********************************************************
// Name: OpenFileForReading
//
// Description:	This attempts to open the requested file
//				for reading and stores the filepointer in
//              FilePointer.
//				This checks to see if the requested file
//				exists.  If it does, this sets the
//				FileStatus to "open", sets TheFilename
//				to the name of the requested file, and
//				returns FILE_OK.  Else, this returns
//				FILE_ERROR.
//*********************************************************
int File::OpenFileForReading(CString Filename)
{
	int ii;
	char TempDrive[_MAX_DRIVE];
	char TempDir[_MAX_DIR];
	char TempFilename[_MAX_FNAME];
	char TempExtension[_MAX_EXT];
	char FullPath[_MAX_PATH];
	char *TempCharPtr;
	CString TempCString1, TempCString2;
	
	// Init the temporary arrays above
	for(ii = 0; ii < _MAX_DRIVE; ii++)
	{
		TempDrive[ii] = ' ';
	}
	for(ii = 0; ii < _MAX_DIR; ii++)
	{
		TempDir[ii] = ' ';
	}
	for(ii = 0; ii < _MAX_FNAME; ii++)
	{
		TempFilename[ii] = ' ';
	}
	for(ii = 0; ii < _MAX_EXT; ii++)
	{
		TempExtension[ii] = ' ';
	}
	for(ii = 0; ii < _MAX_PATH; ii++)
	{
		FullPath[ii] = ' ';
	}

	// IF1 "Null" was not passed
	if("" != Filename)
	{ // THEN1 Attempt to open the file (read only)
		FilePointer = fopen(Filename, "r");
		// IF2 The file exists
		if(NULL != FilePointer)
		{ // THEN2 Set the file status to open
			FileStatus = FILE_OPEN;
            // Store the filename
            FileName = Filename;
			
			// Store the full path to the file
			_fullpath(FullPath, FileName, _MAX_PATH);
			FileNameAndPath = FullPath;
			
			// Store the filename without the path included
			_splitpath( FullPath, TempDrive, TempDir, TempFilename, TempExtension );
			TempCString1 = "";
			TempCString2 = "";
			for(ii = 0; ii < _MAX_FNAME; ii++)
			{
				if(0 == TempFilename[ii])
				{
					break;
				}
				FileNameNoPath += TempFilename[ii];
			}

			TempCharPtr = &TempExtension[0];
			for(ii = 0; ii < _MAX_FNAME; ii++)
			{
				if(0 == TempExtension[ii])
				{
					break;
				}
				FileNameNoPath += TempExtension[ii];
			}

			// Return FILE_OK
			return(FILE_OK);
		}
		else
		{ // ELSE2 Return FILE_ERROR
			return(FILE_ERROR);
		}
	}
	else
	{ // ELSE1 Return FILE_ERROR
		return(FILE_ERROR);
	}
}

//*********************************************************
// Name: OpenFileForReadingAndWriting
//
// Description:	This attempts to open the requested file
//				for reading/writing and stores the
//              filepointer in FilePointer.
//				This checks to see if the requested file
//				exists.  If it does, this sets the
//				FileStatus to "open", sets TheFilename
//				to the name of the requested file, and
//				returns FILE_OK.  Else, this returns
//				FILE_ERROR.
//*********************************************************
int File::OpenFileForReadingAndWriting(CString Filename)
{
	char FullPath[_MAX_PATH];

	// IF1 "Null" was not passed
	if("" != Filename)
	{ // THEN1 Attempt to open the file (read/write)
		FilePointer = fopen(Filename, "r+");
		// IF2 The file exists and it is not a read only file
		if(NULL != FilePointer)
		{ // THEN2 Set the file status to open
			FileStatus = FILE_OPEN;
            // Store the filename
            FileName = Filename;

			// Store the full path to the file
			_fullpath(FullPath, FileName, _MAX_PATH);
			FileNameAndPath = FullPath;

            // Return FILE_OK
			return(FILE_OK);
		}
		else
		{ // ELSE2 Return FILE_ERROR
			return(FILE_ERROR);
		}
	}
	else
	{ // ELSE1 Return FILE_ERROR
		return(FILE_ERROR);
	}
}

//*********************************************************
// Name: OpenNewFile
//
// Description:	This creates the requested file and stores
//				the filepointer in FilePointer. This sets
//              the FileStatus to "open" and sets
//              TheFilename to the name of the requested
//              file.  This will return an error if the
//				file exists and is a read only file.
//*********************************************************
int File::OpenNewFile(CString Filename)
{
	char FullPath[_MAX_PATH];

	// IF1 "Null" was not passed
	if("" != Filename)
	{ // THEN1 Attempt to open a new file
		FilePointer = fopen(Filename, "w");

		// IF2 The file exists and it is not a read only file
		if(NULL != FilePointer)
		{ // THEN2 Set the file status to open
			FileStatus = FILE_OPEN;

            // Store the filename
            FileName = Filename;

			// Store the full path to the file
			_fullpath(FullPath, FileName, _MAX_PATH);
			FileNameAndPath = FullPath;

            // Return FILE_OK
			return(FILE_OK);
		}
		else
		{ // ELSE2 Return FILE_ERROR (file exists but is a read only file)
			return(FILE_ERROR);
		}
		
    }
    else
	{ // ELSE1 Return FILE_ERROR
		return(FILE_ERROR);
	}
}

//*********************************************************
// Name: CloseFile
//
// Description:	This closes the file pointed to by
//				FilePointer and sets the FileStatus
//				to FILE_CLOSED.
//*********************************************************
void File::CloseFile(void)
{
	FileStatus = FILE_CLOSED;
	fclose(FilePointer);
}

//*********************************************************
// Name: EOFCheck
//
// Description:	This returns TRUE if the FilePointer is
//				pointing at the end of file, else this
//				returns FALSE.
//*********************************************************
BOOL File::EOFCheck(void)
{
	// IF1 Not at the end of the file
	if(0 == feof(FilePointer))
	{ // THEN1 Return false
		return(FALSE);
	}
	else
	{ // ELSE1 Return true (end of file reached)
		return(TRUE);
	}
}

//*********************************************************
// Name: ResetFilePointer
//
// Description:	This resets the file pointer to the
//				beginning of the file.
//*********************************************************
void File::ResetFilePointer(void)
{
	fseek(FilePointer, 0, SEEK_SET);
}

//*********************************************************
// Name: StoreFileLine
//
// Description:	This stores the currently pointed to
// file line in FileLine.  The fgets call automatically
// increments the file pointer to the next line.
//*********************************************************
void File::StoreFileLine(void)
{
	CString TempFileLine;
	char FileLineArray[MAX_CHARS];

	TwoFileLinesBack_ptr = OneFileLineBack_ptr;
	TwoFileLinesBack_cnt = OneFileLineBack_cnt;
	OneFileLineBack_ptr = FilePointer->_ptr;
	OneFileLineBack_cnt = FilePointer->_cnt;

	FileLineArray[0] = '\0';
	char* result;
	if (NULL == fgets(&FileLineArray[0], MAX_CHARS, FilePointer))
	{
		FileLine = "";
	}
	else
	{
		FileLine = &FileLineArray[0];
	}
}

//*********************************************************
// Name: GoBackTwoLines
//
// Description:	This resets the file pointer back two
// lines.
//*********************************************************
void File::GoBackTwoLines(void)
{
	FilePointer->_cnt = TwoFileLinesBack_cnt;
	FilePointer->_ptr = TwoFileLinesBack_ptr;
}

//*********************************************************
// Name: StoreEntireFile
//
// Description:	This stores an entire file in the
// EntireFile CString array and stores the number of
// file lines in NumFileLines.
//*********************************************************
void File::StoreEntireFile(void)
{
    int ii = 0;
    char *Status;
	char FileLineArray[MAX_CHARS];

    // Reset the file pointer to the beginning
	fseek(FilePointer, 0, SEEK_SET);

    // WHILE1 Not at the end of the file
    while(0 == feof(FilePointer))
    { // Store the current file line
        Status = fgets(&FileLineArray[0], MAX_CHARS, FilePointer);
		// IF1 If the current line is not NULL (the last line
		// will be NULL and this makes sure not to process it)
        if(NULL != Status)
        { // THEN1 Store the line
            ii++;
        }
    }
	// Store the file line count
    NumFileLines = ii;

	// Dynamically create storage for the entire file and assign a pointer to it
	BegEntireFilePtr = new CString[NumFileLines];

	// Set the working pointer to the beginnig as well
	EntireFilePtr = BegEntireFilePtr;

	// Store the entire file in the newly created file storage area

    // Reset the file pointer to the beginning
	fseek(FilePointer, 0, SEEK_SET);

	for(ii = 0; ii < NumFileLines; ii++)
	{
        Status = fgets(&FileLineArray[0], MAX_CHARS, FilePointer);
		// IF1 If the current line is not NULL (the last line
		// will be NULL and this makes sure not to process it)
        if(NULL != Status)
        { // THEN1 Store the line
            *EntireFilePtr++ = &FileLineArray[0];
        }
	}
}

//*********************************************************
// Name: GetEntireFileLine
//
// Description:	This returns a file line from the
// EntireFile CString array
//*********************************************************
CString File::GetEntireFileLine(int LineNumber)
{
	// Reset the working pointer to the beginning
	EntireFilePtr = BegEntireFilePtr;

	// Advance to the selected line number
	EntireFilePtr += LineNumber;

    return(*EntireFilePtr);
}

//*********************************************************
// Name: GetField
//
// Description:	This returns the requested field from
// the current file line (determined by FileLine).
// A field is determined by groups of characters
// separated by white space.
//
// NOTE: A field can include white space characters if
// grouped with double quotes.
//*********************************************************
CString File::GetField(int FieldNumber)
{
	return(GetCStringField(FieldNumber, FileLine));
}

//*********************************************************
// Name: GetNumChars
//
// Description:	This returns the number of characters
// in the current file line (determined by FileLine).
//*********************************************************
int File::GetNumChars(void)
{
    return(FileLine.GetLength());
}

//*********************************************************
// Name: GetChar
//
// Description:	This returns the requested of character
// in the current file line (determined by FileLine).
//*********************************************************
char File::GetChar(int Index)
{
    return(FileLine[Index]);
}

//*********************************************************
// Name: PutString
//
// Description:	This stores the passed string in the
// location pointed to by FilePointer.
//*********************************************************
void File::PutString(CString String)
{
	fputs(String, FilePointer);
}

//*********************************************************
// Name: PutChar
//
// Description:	This stores the passed character in the
// location pointed to by FilePointer.
//*********************************************************
void File::PutChar(char character)
{
	char *TabPtr = &character;

	fputc(*TabPtr, FilePointer);
}

//*********************************************************
// Name: GetFileNameNoSuffix
//
// Description:	This returns the file excluding the suffix.
//*********************************************************
CString File::GetFileNameNoSuffix(void)
{
//	int ii, TempIndex;
	CString TempCString;

	TempCString = FileName.SpanExcluding(".");

	return(TempCString);
}

//*********************************************************
// Name: GetFilePointer
//
// Description:	This returns the file pointer.
//*********************************************************
FILE *File::GetFilePointer(void)
{
	return(FilePointer);
}
