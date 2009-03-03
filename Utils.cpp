#include "stdafx.h"
#include "Globals.h"
#include "Utils.h"
#include "ModelProcessing.h"

//****************************************************************************
// Name:  InitStateMachineName
//
// This inits the StateMachineName variable in the Diagram object with the
// state machine name from the config file.
//
// This returns "OK" if ok or an error if a problem is found in the config
// file.
//****************************************************************************
CString InitStateMachineName(void)
{
	CString TempMessage;

	// Start back at the beg of the config file
	ConfigFile.ResetFilePointer();
    while(FALSE == ConfigFile.EOFCheck())
    {
        ConfigFile.StoreFileLine();
		// IF1 The line with the state machine name is found
        if("__STATE_MACHINE_NAME" == ConfigFile.GetField(1))
        { // THEN1 Store it in a global and return "OK"
            StateMachineName = ConfigFile.GetField(2);
			return("OK");
        }
    }
	TempMessage = "__STATE_MACHINE_NAME marker not found in the \"";
	TempMessage += ConfigFile.GetFileNameNoPath();
	TempMessage += "\" configuration file";
	return(TempMessage);
}

//****************************************************************************
// Name:  InitStateConfigData
//
// This initializes the state number to text linked list in the Diagram
// object with the list of states from the config file.  The order in the
// config file determines the state "number" with the first in the list in the
// config file starting with 0, the next 1, etc.  The state number to text
// linked list is used to convert state text to the corresponding number,
// state number to the corresponding name, and compare the number of states in
// the config file to the number of states in the model.
//
// This returns "OK" if ok or an error if a problem is found in the config
// file.
//****************************************************************************
CString InitStateConfigData(void)
{
    int State = CONFIG_FILE_IDLE;
    CString TempCString1, TempCString2, TempMessage;
    
    // First determine how many states there are
    // Start back at the beg of the config file
    ConfigFile.ResetFilePointer();
    // WHILE1 Not at the end of the file
    while(FALSE == ConfigFile.EOFCheck())
    {
        ConfigFile.StoreFileLine();
        // IF1 The current processing state is CONFIG_FILE_IDLE
        if(CONFIG_FILE_IDLE == State)
        { // THEN1 IF2 The line signifying the beginning of the state config is found
            if("__BEG_STATE_CONFIG" == ConfigFile.GetField(1))
            { // THEN2 Change the processing state
                State = CONFIG_FILE_PROCESSING;
            }
        } // ELSE1 IF3 The current processing state is CONFIG_FILE_PROCESSING
        else if(CONFIG_FILE_PROCESSING == State)
        { // THEN3 Store the next line
            TempCString1 = ConfigFile.GetField(1);
            // IF4 The line signifying the end of the state config is found
            if("__END_STATE_CONFIG" == TempCString1)
            { // THEN4 Change the processing state to done
                State = CONFIG_FILE_DONE;
                break; // Break out of the while loop
            }
            else
            { // ELSE4 Continue processing
                // Add the state to the state text array
                TempCString1.MakeUpper();
                Diagram.AddStateNumberToTextElement(TempCString1);
            }
        }
    }
    // Return the status (will return error if no marker is found)
    if(CONFIG_FILE_IDLE == State)
    {
		TempMessage = "__BEG_STATE_CONFIG marker not found in the \"";
		TempMessage += ConfigFile.GetFileNameNoPath();
		TempMessage += "\" configuration file";
        return(TempMessage);
    }
    else if(CONFIG_FILE_PROCESSING == State)
    {
		TempMessage = "__END_STATE_CONFIG marker not found in the \"";
		TempMessage += ConfigFile.GetFileNameNoPath();
		TempMessage += "\" configuration file";
        return(TempMessage);
    }
    else
    {
        return("OK");
    }
}

//****************************************************************************
// Name:  InitSuperStateConfigData
//
// This initializes the super state number to text linked list in the Diagram
// object with the list of super states from the config file.  The order in
// the config file determines the super state "number" with the first starting
// with 0, the next 1, etc.  The super state number to text linked list is
// used to convert super state text to the corresponding number, super state
// number to the corresponding name, and compare the number of super states
// in the config file to the number of super states in the model.
//
// This returns "OK" if ok or an error if a problem is found in the config
// file.
//****************************************************************************
CString InitSuperStateConfigData(void)
{
    int State = CONFIG_FILE_IDLE;
    CString TempCString1, TempCString2, TempMessage;
	char *TempEnd;
    unsigned int TempUnsignedInt;
    
	// Start back at the beg of the config file
	ConfigFile.ResetFilePointer();
    while(FALSE == ConfigFile.EOFCheck())
    {
        ConfigFile.StoreFileLine();
        
        // IF1 The current processing state is IDLE
        if(CONFIG_FILE_IDLE == State)
        { // THEN1 IF2 The line signifying the beginning of the super state config is found
            if("__BEG_SUPER_STATE_CONFIG" == ConfigFile.GetField(1))
            { // THEN2 Change the processing state
                State = CONFIG_FILE_PROCESSING;
            }
        } // ELSE1 IF3 The current processing state is CONFIG_FILE_PROCESSING
        else if(CONFIG_FILE_PROCESSING == State)
        { // THEN3 Store the next line
            TempCString1 = ConfigFile.GetField(1);
            // IF4 The line signifying the end of the super state config is found            
            if("__END_SUPER_STATE_CONFIG" == TempCString1)
            { // THEN4 Change the processing state to done
                State = CONFIG_FILE_DONE;
                break; // Break out of the while loop
            }
            else
            { // ELSE4 Continue processing
                // Add the super state to the state text array
                TempCString1.MakeUpper();
                Diagram.AddSuperStateNumberToTextElement(TempCString1);
                
                // Add additional cond mask data to the super state additional cond mask array
                TempCString2 = ConfigFile.GetField(2);
                TempUnsignedInt = static_cast<unsigned int>(strtoul(TempCString2, &TempEnd, 16));
                Diagram.AddSuperStateAdditionalCondMask(TempUnsignedInt);
                
                // Add additional don't care mask data to the super state additional don't care mask array
                TempCString2 = ConfigFile.GetField(3);
                TempUnsignedInt = static_cast<unsigned int>(strtoul(TempCString2, &TempEnd, 16));
                Diagram.AddSuperStateAdditionalDontCareMask(TempUnsignedInt);
            }
        }
    }
	// Return the status (will return error if no marker is found)
    if(CONFIG_FILE_IDLE == State)
    {
		TempMessage = "__BEG_SUPER_STATE_CONFIG marker not found in the \"";
		TempMessage += ConfigFile.GetFileNameNoPath();
		TempMessage += "\" configuration file";
        return(TempMessage);
    }
    else if(CONFIG_FILE_PROCESSING == State)
    {
		TempMessage = "__END_SUPER_STATE_CONFIG marker not found in the \"";
		TempMessage += ConfigFile.GetFileNameNoPath();
		TempMessage += "\" configuration file";
        return(TempMessage);
    }
    else
    {
        return("OK");
    }
}

//****************************************************************************
// Name:  InitCondBitData
//
// This initializes the cond bit linked list in the Diagram object with
// condition bit information from the config file.
//
// This returns "OK" if ok or an error if a problem is found in the config
// file.
//****************************************************************************
CString InitCondBitData(void)
{
    int State = CONFIG_FILE_IDLE;
    CString TempCString, TempMessage;
    CondBit_t TempCondBit;

	// Start back at the beg of the config file
	ConfigFile.ResetFilePointer();
    while(FALSE == ConfigFile.EOFCheck())
    {
        ConfigFile.StoreFileLine();
        
        // IF1 The current processing state is IDLE
        if(CONFIG_FILE_IDLE == State)
        { // THEN1 IF2 The line signifying the beginning of the condition bit config is found
            if("__BEG_COND_CONFIG" == ConfigFile.GetField(1))
            { // THEN2 Change the processing state
                State = CONFIG_FILE_PROCESSING;
            }
        } // ELSE1 IF3 The current processing state is CONFIG_FILE_PROCESSING
        else if(CONFIG_FILE_PROCESSING == State)
        { // THEN3 Store the next line
            TempCString = ConfigFile.GetField(1);
            
            // IF4 The line signifying the end of the condition bit config is found            
            if("__END_COND_CONFIG" == TempCString)
            { // THEN4 Change the processing state to done
                State = CONFIG_FILE_DONE;
                break; // Break out of the while loop
            }
            else
            { // ELSE4 Continue processing
                TempCondBit.CondBitText = TempCString;
                
                if("0" != ConfigFile.GetField(2))
                {
                    TempCondBit.CondBit = 1;
                }
                else
                {
                    TempCondBit.CondBit = 0;
                }
                // Add the new condition bit
                Diagram.AddCondBit(&TempCondBit);
            }
        }
    }
	// Return the status (will return error if no marker is found)
    if(CONFIG_FILE_IDLE == State)
    {
		TempMessage = "__BEG_COND_CONFIG marker not found in the \"";
		TempMessage += ConfigFile.GetFileNameNoPath();
		TempMessage += "\" configuration file";
        return(TempMessage);
    }
    else if(CONFIG_FILE_PROCESSING == State)
    {
		TempMessage = "__END_COND_CONFIG marker not found in the \"";
		TempMessage += ConfigFile.GetFileNameNoPath();
		TempMessage += "\" configuration file";
        return(TempMessage);
    }
	else
    {
        return("OK");
    }
}

//****************************************************************************
// Name:  InitDontCareBitData
//
// This initializes the don't care bit linked list in the Diagram object with
// condition bit information from the config file.
//
// This returns "OK" if ok or an error if a problem is found in the config
// file.
//****************************************************************************
CString InitDontCareBitData(void)
{
    int State = CONFIG_FILE_IDLE;
    CString TempCString, BitMaskCString, TempMessage;
	char *TempEnd;
    unsigned int BitMask = 0;
    unsigned int temp;
    DontCareBit_t TempDontCareBit;

	// Start back at the beg of the config file
	ConfigFile.ResetFilePointer();
    while(FALSE == ConfigFile.EOFCheck())
    {
        ConfigFile.StoreFileLine();
        
        // IF1 The current processing state is CONFIG_FILE_IDLE
        if(CONFIG_FILE_IDLE == State)
        { // THEN1 IF2 The line signifying the beginning of the don't care bit config is found
            if("__BEG_DONT_CARE_CONFIG" == ConfigFile.GetField(1))
            { // THEN2 Change the processing state
                State = CONFIG_FILE_PROCESSING;
            }
        } // ELSE1 IF3 The current processing state is CONFIG_FILE_PROCESSING
        else if(CONFIG_FILE_PROCESSING == State)
        { // THEN3 Store the next line
            TempCString = ConfigFile.GetField(1);

            // IF4 The line signifying the end of the don't care bit config is found            
            if("__END_DONT_CARE_CONFIG" == TempCString)
            { // THEN4 Change the processing state to done
                State = CONFIG_FILE_DONE;
                break; // Break out of the while loop
            }
            else
            { // ELSE4 Continue processing
                BitMaskCString = ConfigFile.GetField(2);
                
                temp = static_cast<WORD>(strtoul(BitMaskCString, &TempEnd, 10));
                BitMask = 0x00000001 << temp;
                
                TempDontCareBit.DontCareBit = BitMask;
                TempCString.MakeUpper();
                TempDontCareBit.DontCareBitText = TempCString;
                // Add the new don't care bit mask
                Diagram.AddDontCareBit(&TempDontCareBit);
            }
        }
    }
	// Return the status (will return error if no marker is found)
    if(CONFIG_FILE_IDLE == State)
    {
		TempMessage = "__BEG_DONT_CARE_CONFIG marker not found in the \"";
		TempMessage += ConfigFile.GetFileNameNoPath();
		TempMessage += "\" configuration file";
        return(TempMessage);
    }
    else if(CONFIG_FILE_PROCESSING == State)
    {
		TempMessage = "__END_DONT_CARE_CONFIG marker not found in the \"";
		TempMessage += ConfigFile.GetFileNameNoPath();
		TempMessage += "\" configuration file";
        return(TempMessage);
    }
	else
    {
        return("OK");
    }
}

//****************************************************************************
// Name:  CheckFSMOutputFile
//
// This checks an FSM output file for existence, that it is not a read only
// file, and that it contains both the __BEG_AUTO_GEN_AREA and
// __END_AUTO_GEN_AREA markers.
//****************************************************************************
CString CheckFSMOutputFile(CString OutputFile)
{
    CString ReturnStatus = "OK";
    int BegMarkerStatus = FALSE, EndMarkerStatus = FALSE;
    File TempFile;
    
    // IF1 The file exists
    if(FILE_OK == TempFile.OpenFileForReading(OutputFile))
    { // THEN1 Close the file and check for read only
		TempFile.CloseFile();
		
		// IF2 The file is not a read only file
		if(FILE_OK == TempFile.OpenFileForReadingAndWriting(OutputFile))
		{ // THEN2 Make sure it has the and __END_AUTO_GEN_AREA markers
			
			while(FALSE == TempFile.EOFCheck())
			{
				TempFile.StoreFileLine();
				if("/*__BEG_AUTO_GEN_AREA*/" == TempFile.GetField(1))
				{
					BegMarkerStatus = TRUE;
				}
				else if("/*__END_AUTO_GEN_AREA*/" == TempFile.GetField(1))
				{
					EndMarkerStatus = TRUE;
				}
			}
			// IF3 The file doesn't contain either marker
			if((FALSE == BegMarkerStatus) && (FALSE == EndMarkerStatus))
			{ // THEN3 Set the return status to error
				ReturnStatus = "missing __BEG_AUTO_GEN_AREA and __END_AUTO_GEN_AREA markers";
			}
			// ELSE3 IF4 The file doesn't contain the markers
			else if(FALSE == BegMarkerStatus)
			{ // THEN4 Set the return status to error
				ReturnStatus = "missing __BEG_AUTO_GEN_AREA marker";
			}
			// ELSE4 IF5 The file doesn't contain the markers
			else if(FALSE == EndMarkerStatus)
			{ // THEN5 Set the return status to error
				ReturnStatus = "missing __END_AUTO_GEN_AREA marker";
			}
			// Close the file
			TempFile.CloseFile();
		}
		else
		{ // ELSE2 Set the return status to error
			ReturnStatus = "is a read only file";
		}
    }
    else
    { // ELSE1 Set the return status to error
        ReturnStatus = "does not exist";
    }
    
    return(ReturnStatus);
}

//****************************************************************************
// Name:  GetModelFile
//
// This reads the model file out of the config file and returns it.
//
// This returns the model file if ok or an error if a problem is found in the
// config file.
//****************************************************************************
CString GetModelFile(void)
{
	// Start back at the beg of the config file
	ConfigFile.ResetFilePointer();
    while(FALSE == ConfigFile.EOFCheck())
    {
        ConfigFile.StoreFileLine();
		// IF1 The line with the model file path is found            
        if("__MODEL_FILE" == ConfigFile.GetField(1))
        { // THEN1 Return the model file path
			return(ConfigFile.GetField(2));
        }
    }
	// If this point is reached, then the marker was not found
	return("No __MODEL_FILE marker found in the configuration file");;
}

//****************************************************************************
// Name:  GetOutputFilePath
//
// This reads the FSM output file path out of the config file and returns it.
//
// This returns the output file path if ok or an error if a problem is found
// in the config file.
//****************************************************************************
CString GetOutputFilePath(void)
{
	// Start back at the beg of the config file
	ConfigFile.ResetFilePointer();
    while(FALSE == ConfigFile.EOFCheck())
    {
        ConfigFile.StoreFileLine();
		// IF1 The line with the output file path is found            
        if("__OUTPUT_FILE_PATH" == ConfigFile.GetField(1))
        {
			return(ConfigFile.GetField(2));
        }
    }
	// If this point is reached, then the marker was not found
	return("No __OUTPUT_FILE_PATH marker found in the configuration file");;
}

//****************************************************************************
// Name:  CreateTextFile
//
// This converts the passed rich text file to a text file.  The new text file
// will have the same name with a txt suffix.
//
// This returns the FILE_OK if the conversion went ok otherwise this returns
// an error.
//
// The majority of this function was copied from a website that had some
// example code for converting a rich text file to a text file.  Some changes/
// additions were made including cleaning up some of the formatting.  The
// code additions are noted by the text "BEG/END LEE ADDITION".
//****************************************************************************
int CreateTextFile(FILE *fin, FILE *fout)
{
  /* This loop does an RTF to ASCII conversion.
     
     RTF encapsulates all text and extensions within curly brace segments,
     {...}, and escapes certain characters, including: '{', '}', '\', and '\n'.
     
     Running changes in font are inlines escape sequences, such as
     "\fi-5820\li5820", "\fi0\li0", or "\b" .... "\b0"; the latter indicates
     a run of text in bold face.
     
     To process the RTF source, we first track the curly brace level.  There
     should be no content at level zero and un-escaped parentheses should
     balance over the entire file.
  **/
    int level = 0, lineno = 1;
	int c, len;
	static char *tok;

	// BEG LEE ADDITION
	int ii, ReturnStatus = FILE_OK;
	CString TempCString;
	char *TempTok;
	// END LEE ADDITION
	
    while( (c=getc(fin)) != EOF)
	{
		// BEG LEE ADDITION
		// IF A carriage return is encountered
		if(c == 0x0D)
		{ // THEN Output a newline
			putc( '\n', fout);
			continue;
		}
		// END LEE ADDITION
		if(c=='\n')
		{
			lineno++;
			continue;
		} /* bare newlines are swallowed. */
		if(c == '{')
		{
			level++;
			continue;
		}
		if(c == '}')
		{
			// At this point "level" should become less than 0, if it does then the RTF file may not be formatted properly
			// IF level is < 0
			if(level--<0)
			{ // Set an error and stop
				// BEG LEE ADDITION
				ReturnStatus = FILE_ERROR;
				break;
				// END LEE ADDITION
			}
			c = getc(fin);
			if(c != '\n')
			{
				ungetc( c, fin);
			}
			else
			{
				lineno++; /* newline at this point is swallowed. */
			}
			continue;
		}
		if(level>1)
		{
			continue;
		}
		if(c=='\\')
		{
			tok = getRtfToken( /*fname,*/ fin, &lineno, &len);
			// Return an error if we could not get the next token
			if( ! tok )
			{
				ReturnStatus = FILE_ERROR;
				break;
			}
			// BEG LEE ADDITION
			TempTok = tok;
			TempCString = "";
			// Save the information after the appropriate keywords
			for(ii = 0; ii < len; ii++)
			{
				TempCString += *TempTok++;
			}
			// IF An appropriate keyword is found
			if(("par" == TempCString) || ("line" == TempCString) || ("\\\n\\line" == TempCString))
			{ // THEN Output a newline
				putc( '\n', fout);
			}
			// END LEE ADDITION
			switch( *tok )
			{
			case '\n':
				{
					putc( *tok, fout);
					continue; /* lineno is incremented when the newline is read. */
				}
			case '\\':
				{
					putc( *tok, fout);
					continue;
				}
			case '{':
				{
					putc( *tok, fout);
					continue;
				}
			case '}':
				{
					putc( *tok, fout);
					continue;
				}
			default:
				{
					continue;
				}
			}
		}
		putc( c, fout); /* plain text in the RTF stream. */
		continue;
    } /* while(...) */
	
	// At this point "level" should be 0, if not then the RTF file may not be formatted properly
	// IF level is non-zero
    if( level )
	{ // THEN Set an error
		// BEG LEE ADDITION
		ReturnStatus = FILE_ERROR;
		// END LEE ADDITION
    }

	return(ReturnStatus);
}

//****************************************************************************
// Name:  getRtfToken
//
// This returns rich text file tokens.
//
// The majority of this function was copied from a website that had some
// example code for converting a rich text file to a text file.  Some changes/
// additions were made including cleaning up some of the formatting.  The
// code additions are noted by the text "BEG/END LEE ADDITION".
//****************************************************************************
static char* getRtfToken(/*const char* fname,*/ FILE* fin, int* plineno, int* plen)
{
	static char buf[1024];
	char* p = buf;
	int c;
	// BEG LEE ADDITION
	int ii;
	
	// Clear the buffer before processing
	for(ii = 0; ii < 1024; ii++)
	{
		buf[ii] = 0;
	}
	// END LEE ADDITION

	for( c = getc(fin); c != EOF; c = getc(fin))
	{
		if(c=='\n')
		{
			(*plineno)++; /* we will be swallowing that newline. */
		}
		if((c == '\\') || (c == '{') || (c == '}') || (c=='\n'))
		{
		/* As the first character in a token, it forms that token. 
		Otherwise, it is put back and serves to terminate the current
		token.  Newlines are not put back in this context.  Some RTF
		sequences are newline terminated, and those newlines need to be
		quietly swallowed.
			*/
			if(p==buf)
			{
				*p++ = c; /* the character forms the entire token? */
			}
			else if(c!='\n')
			{
				ungetc(c,fin); /* the character terminates the token. */
			}
			break;
		}
		if(isspace(c))
		{
			break;	/* whitespace terminates a token and is swallowed. */
		}
		*p++ = c;		/* the character is part of the token. */
	}
	// IF An unexpected end of file is encountered here
	if( c == EOF )
	{ // THEN Return an error
		return (char*)NULL;
	}
	*plen = p - buf;
	return buf;
}

//*********************************************************
// Name: GetCStringField
//
// Description:	This returns the requested field from
// the passed CString.
// A field is determined by groups of characters
// separated by white space.
//
// NOTE: A field can include white space characters if
// grouped with double quotes.
//*********************************************************
CString GetCStringField(int FieldNumber, CString PassedCString)
{
    int FieldCounter, CharCounter, FieldCharCounter;
    CString TempCString;
    char StringChar = 0;
    int Done, QuoteStatus;
    
	// Search for beginning of desired field
	// Init some variables
	FieldCounter = 0;
	CharCounter = 0;
	QuoteStatus = LOOKING_FOR_FIRST_QUOTE;
	// WHILE1 (Not at the requested field number)
	while(FieldCounter < FieldNumber)
	{
		TempCString = "";
		FieldCharCounter = 0;
		Done = FALSE;
		// Find out where current field ends
		// WHILE2 (Not at the end of the current field)
		while((FALSE == Done) && (CharCounter < PassedCString.GetLength()))
		{ // Store the current character
			StringChar = PassedCString.GetAt(CharCounter);
			// IF1 The the current character is a double quote
			if('"' == StringChar)
			{ // THEN1 IF2 This is the first double quote for this field
				if(LOOKING_FOR_FIRST_QUOTE == QuoteStatus)
				{ // THEN2 Advance past the quote and set the quote status
					// variable to look for the second double quote for this
					// double quote pair
					CharCounter++;
					QuoteStatus = LOOKING_FOR_SECOND_QUOTE;
				}
				// ELSE2 IF3 This is the second double quote for this field
				else if(LOOKING_FOR_SECOND_QUOTE == QuoteStatus)
				{ // THEN3 Advance past the quote, reset the quote status
					// variable for the next possible field enclosed in
					// double quotes, and stop the while loop
					CharCounter++;
					QuoteStatus = LOOKING_FOR_FIRST_QUOTE;
					Done = TRUE;
				}
			}
			// ELSE1 IF4 At the end of the current field
			else if((LOOKING_FOR_FIRST_QUOTE == QuoteStatus) && (('\t' == StringChar) || (' ' == StringChar)))
			{ // THEN4 Stop the while loop
				Done = TRUE;
			}
			// ELSE4 IF5 At the end of the line
			else if('\n' == StringChar)
			{ // THEN5 Stop the while loop
				Done = TRUE;
			}
			else
			{ // ELSE5 Store next character, increment the character
				// and field counters
				TempCString.Insert(FieldCharCounter, StringChar);
				CharCounter++;
				FieldCharCounter++;
			}
		} // END WHILE2
		
		// Find out where next field begins
		// IF6 Not at the end of the line
		if('\n' != StringChar)
		{ // THEN6 Search for the beginning of the next field (this allows
			// for multiple white space characters to separate fields
			Done = FALSE;
			// WHILE3 (Not at the beginning of the next field)
			while((FALSE == Done) && (CharCounter < PassedCString.GetLength()))
			{ // Store the current character
				StringChar = PassedCString.GetAt(CharCounter);
				// IF7 A double quote is encountered
				if('"' == StringChar)
				{ // THEN7 Advance past the quote, set the quote status
					// variable to look for the second double quote for this
					// double quote pair, and stop the while loop
					CharCounter++;
					QuoteStatus = LOOKING_FOR_SECOND_QUOTE;
					Done = TRUE;
				}
				// ELSE7 IF8 At the beginning of the next field
				else if(('\t' != StringChar) && (' ' != StringChar) && ('\n' != StringChar))
				{ // THEN8 Stop the while loop
					Done = TRUE;
				}
				else
				{ // ELSE8 Increment the character counter
					CharCounter++;
				}
			} // END WHILE3
		}
		// Increment the field counter
		FieldCounter++;
	} // END WHILE1
	// Remove the unwanted newline character (if it exists)
	TempCString.Remove('\n');
	// Return the requested field
    return(TempCString);
}

//*********************************************************
// Name: CStringFirstCharCheck
//
// Description:	This checks to see if the first character
// in the passed CString is an underscore or a number.  If
// it is then this replaces the first character with a 'z'.
//*********************************************************
CString CStringFirstCharCheck(CString PassedCString)
{
	char TempChar;

	// Store the first char in a temporary
	TempChar = PassedCString.GetAt(0);
	// IF1 The first char is an underscore or a number
	if(('_' == TempChar) || ('0' == TempChar) || ('1' == TempChar) ||\
		('2' == TempChar) || ('3' == TempChar) || ('4' == TempChar) ||\
		('5' == TempChar) || ('6' == TempChar) || ('7' == TempChar) ||\
		('8' == TempChar) || ('9' == TempChar))
	{ // THEN1 Change it to a 'z'
		PassedCString.SetAt(0, 'z');
	}
	return(PassedCString);
}