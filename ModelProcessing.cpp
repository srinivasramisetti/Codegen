#include "stdafx.h"
#include "Globals.h"
#include "ModelProcessing.h"
#include "DiagramClass.h"
#include "Utils.h"

//****************************************************************************
// Name:  ProcessModelFile
//
// This processes a model file generated from Rhapsody.  It adds states and
// super states to the Diagram object, calls ProcessTransitions to add
// transitions (corresponding to the states and super states) to the Diagram
// object, adds super state lists and super state list sub states to the
// Diagram object.
//
// This returns a string of "OK" if all states were found
// This returns a string of "Not OK" and displays error messages for the
// following conditions:
// a) No powerup transition was found
// b) If there a super state is missing a default transition
// c) If a state or super state was not found
//****************************************************************************
CString ProcessModelFile(void)
{
    int Processing1, TempStateNumber, ii, TempSuperStateNumber;
    CString TempCString1, TempCString2, TempFunctionBlock;
    CString TempEntryAction, TempActivityFunction, TempExitAction;
    CString Status = "OK";
    char TempChar;
    SuperState_t TempSuperStateData;
    State_t TempStateData;
    ListSubState_t TempListSubStateData;
    CString TempInitialStateName;
    
    Processing1 = TRUE;
    // WHILE1 Processing the model file generated from Rhapsody
    while(TRUE == Processing1)
    {
        // Store the next line from the model file
        ModelFile.StoreFileLine();
        // Clear the temporary function block CString
        TempFunctionBlock.Empty();
        // IF1 At the end of the model file
        if(TRUE == ModelFile.EOFCheck())
        { // THEN1 Indicate done
            Processing1 = FALSE;
        }
        else
        { // ELSE1 Continue processing
            // Ignore blank lines
            while("" == ModelFile.GetField(1))
            {
                ModelFile.StoreFileLine();
                if (TRUE == ModelFile.EOFCheck())
                {
                    Processing1 = FALSE;
                    break;
                }
            }
            // IF2 This is a state or super state
            if(("ActivityFunction" == ModelFile.GetField(1)) || 
                ("Or-state" == ModelFile.GetField(1)))
            { // THEN2 IF3 This is the root "state"
                if("ROOT" == TempCString1)
                { // THEN3 Search for the powerup transition
					// It will occur after the text "Default"
                    while("Default" != ModelFile.GetField(1))
                    {
                        ModelFile.StoreFileLine();
						// IF4 There is no powerup transition (if "Or-state" is encountered
						// here, then there is no powerup transition)
						if(("Or-state" == ModelFile.GetField(1)) || (TRUE == ModelFile.EOFCheck()))
						{ // THEN4 Display the error, stop processing, and
							// break out of while loop
							::MessageBox(0, "No powerup transition found", "Error", MB_ICONERROR);
							Status = "Not OK";
							Processing1 = FALSE;
							break;
						}
                    }
					// IF5 A powerup transition was found above
					if(TRUE == Processing1)
					{ // THEN5 Get the next line
						ModelFile.StoreFileLine();
						// IF6 There is a powerup transition action (powerup transition
						// actions begin with a '/' (i.e. no transition conditions)
						if('/' == ModelFile.GetChar(0))
						{ // THEN6 Start with three spaces
							TempFunctionBlock = "   ";
							// Get the first part after the '/' (the part on the same line as the '/')
							for(ii = 1; ii < (ModelFile.GetNumChars()); ii++)
							{
								TempChar = ModelFile.GetChar(ii);
								TempFunctionBlock.Insert(500, TempChar); // Use 500 to make sure it appends
							}
							// Get the rest of the powerup transition action (the ProcessFunctionBlock
							// will get any powerup transition action lines after the line
							// with the '/' until "Target:" text is encountered)
							TempFunctionBlock += ProcessFunctionBlock("Target:", "DontCare", "DontCare", "DontCare");
							// Add it to the Diagram object
							Diagram.AddPowerupTransAction(TempFunctionBlock);
						}
						// Store the line with the powerup transition target
						ModelFile.StoreFileLine();
                        TempInitialStateName = ModelFile.GetField(1);
                        TempInitialStateName.MakeUpper();
						Diagram.AddInitialStateName(TempInitialStateName);
					}
                }
                else
                { // ELSE3 IF7 This state has an activity function
                    if("ActivityFunction" == ModelFile.GetField(1))
                    { // THEN7 Store all of the activity function lines in TempFunctionBlock
                        TempFunctionBlock = ProcessFunctionBlock("Or-state", "DontCare", "DontCare", "DontCare");
                    }
                    // Store the next line from the model file which is a state name
                    ModelFile.StoreFileLine();
                    // IF8 The state is a super state (at this point TempCString
                    // contains the name of the super state stored the last pass)
                    if("Substates:" == ModelFile.GetField(1))
                    { // THEN8 Convert the super state name to its corresponding number
                        TempCString1.MakeUpper();

						// IF88 The super state has an activity function
						if("" != TempFunctionBlock)
						{ // THEN88 Output an error
							TempCString2 = "Super state " + TempCString1 + " contains an activity function";
							::MessageBox(0, TempCString2, "Error", MB_ICONERROR);
							Status = "Not OK";
							Processing1 = FALSE;
							break;
						}
						
                        TempSuperStateNumber = ConvertStateTextToNumber(TempCString1);
                        // IF9 It converted ok (it is a valid super state)
                        if((CONVERT_ERROR != TempSuperStateNumber) && (TempSuperStateNumber >= SUPER_STATE_OFFSET))
                        { // THEN9 Add the super state to the diagram object
                            TempSuperStateData.SuperStateName = TempCString1;
                            TempSuperStateData.SuperStateNumber = TempSuperStateNumber - SUPER_STATE_OFFSET;
                            Diagram.AddSuperState(&TempSuperStateData);

                            // WHILE2 Processing this super state's sub states
                            while(("Out" != ModelFile.GetField(1)) && ("Default" != ModelFile.GetField(1)))
                            { // Store the next line from the model file
                                ModelFile.StoreFileLine();
                                // Get possible sub state name
                                TempCString2 = ModelFile.GetField(1);
                                // IF10 A sub state is encountered
                                if(("Out" != TempCString2) && ("Default" != ModelFile.GetField(1)))
                                { // THEN10 IF11 This super state has no transitions out and is missing a default state
									// ("Or-state" or "ActivityFunction" will be encountered here if this is the case)
									if(("Or-state" == TempCString2) || ("ActivityFunction" == TempCString2))
									{ // THEN11 Remove the unwanted last item in the linked list of substates
										Diagram.RemoveLastSuperStateListSubStateName(TempSuperStateData.SuperStateNumber);
										// Go back two lines in the model file to start again
										ModelFile.GoBackTwoLines();
										break;										
									}
									else
									{ // ELSE11 Convert the sub state name to its corresponding number
										TempCString2.MakeUpper();
                                        TempStateNumber = ConvertStateTextToNumber(TempCString2);
										// IF12 It converted ok (it is a valid state)
										if(CONVERT_ERROR != TempStateNumber)
										{ // THEN12 Add the sub state
                                            TempListSubStateData.SubStateName = TempCString2;
											Diagram.AddSuperStateListSubState((TempSuperStateNumber - SUPER_STATE_OFFSET), &TempListSubStateData);
										}
										else
										{ // ELSE12 Report this unknown sub state, set the return status
											// to indicate a state error, stop processing, and break
											// out of the while loop
											// IF121 The current super state (stored in TempCString1) contains
											// an entry action
											if("ENTRYACTION" == TempCString2)
											{ // THEN121 Prepare to output a super state entry action error
												TempCString2 = "Super state " + TempCString1 + " contains an entry action";
											}
											else if("EXITACTION" == TempCString2)
											{ // ELSE 121 IF122 The current super state (stored in TempCString1) contains
												// an exit action
												// THEN122 Prepare to output a super state exit action error
												TempCString2 = "Super state " + TempCString1 + " contains an exit action";
											}
											else
											{ // ELSE122 Prepare to output the unknown state error
												TempCString2 = "State " + TempCString2 + " not found in the configuration file";
											}
											::MessageBox(0, TempCString2, "Error", MB_ICONERROR);
											Status = "Not OK";
											Processing1 = FALSE;
											break;
										}
									}
                                }
                            } // END WHILE2
							
							// TempSuperStateNumber now contains the super state number
							
							// IF13 The state or super state has any transitions (out) or it is a super state
							// with no transitions out
							if(("Out" == ModelFile.GetField(1)) || ("Default" == ModelFile.GetField(1)))
							{ // THEN13 IF14 The state has any transitions out
								if("Out" == ModelFile.GetField(1))
								{ // THEN14 Process the super state's transitions
									TempCString1 = ProcessTransitions(TempSuperStateNumber, SUPER_STATE);
									// IF15 A transition was detected with no conditions
									if("No transition conditions" == TempCString1)
									{ // THEN15 Fill in the not OK status and stop processing
										TempCString1 = ConvertStateNumberToText(TempSuperStateNumber)\
											+ " has a transition out with no transition conditions";
										::MessageBox(0, TempCString1, "Error", MB_ICONERROR);
										Status = "Not OK";
										Processing1 = FALSE;
									}
									// ELSE15 IF151 There is an unknown target state
									else if("State " == TempCString1.Left(6))
									{ // THEN151 Fill in the not OK status and stop processing
										::MessageBox(0, TempCString1, "Error", MB_ICONERROR);
										Status = "Not OK";
										Processing1 = FALSE;
									}
								}
								// IF16 Still processing
								if(TRUE == Processing1)
								{ // THEN16 IF17 This super state has a default transition
									if("Default" == ModelFile.GetField(1))
									{ // THEN17 Store (skip) the line with the word "Target:"
										ModelFile.StoreFileLine();
										// IF18 There is a trans action for this default trans
										if("Target:" != ModelFile.GetField(1))
										{ // THEN18 Indicate an error
											TempCString1 = ConvertStateNumberToText(TempSuperStateNumber)\
												+ "'s default transition has a transition action.";
											::MessageBox(0, TempCString1, "Error", MB_ICONERROR);
											Status = "Not OK";
											Processing1 = FALSE;
										}
										else
										{ // ELSE18 Store the line with the default transition
											ModelFile.StoreFileLine();
											// Add the default transition to the current super state
											Diagram.AddDefaultTransTargetState((TempSuperStateNumber - SUPER_STATE_OFFSET), ModelFile.GetField(1));
										}
									}
									else
									{ // ELSE17 Store possible next state
										TempCString1 = ModelFile.GetField(1);
									}
								}
							}
							else
							{ // ELSE13 Store possible state text
								TempCString1 = ModelFile.GetField(1);
							}
						}
                        else
						{ // ELSE9 Report this unknown super state, set the return status
							// to indicate a state error, stop processing, and break
							// out of the while loop
							TempCString1 = "Super state " + TempCString1 + " not found in the configuration file";
							::MessageBox(0, TempCString1, "Error", MB_ICONERROR);
							Status = "Not OK";
                            Processing1 = FALSE;
							break;
                        }
                    }
                    else
                    { // ELSE8 Process the non-super state
						// TempCString1 now contains the state text
						// Convert state name to its corresponding number
						TempCString1.MakeUpper();
						TempStateNumber = ConvertStateTextToNumber(TempCString1);
						// IF18 It converted ok (this is a valid state)
						if(CONVERT_ERROR != TempStateNumber)
						{ // THEN18 IF19 There is an activity function
							if("" != TempFunctionBlock)
							{ // THEN19 Prepare to store it in the Diagram object
                                TempActivityFunction = TempFunctionBlock;
							}
                            else
                            { // ELSE19 Clear the string
                                TempActivityFunction = "";
                            }
							// IF20 The state has an entry action
							if("EntryAction" == ModelFile.GetField(1))
							{ // THEN20 Prepare to store it in the Diagram object
								TempEntryAction = ProcessFunctionBlock("Out", "ExitAction", "Target:", "DontCare");
							}
                            else
                            { // ELSE20 Clear the string
                                TempEntryAction = "";
                            }
							// IF21 The state has an exit action
							if("ExitAction" == ModelFile.GetField(1))
							{ // THEN21 Prepare to store it in the Diagram object
								TempExitAction = ProcessFunctionBlock("Out", "Target:", "DontCare", "DontCare");
							}
                            else
                            { // ELSE21 Clear the string
                                TempExitAction = "";
                            }
							
                            // Fill in the state data from above
                            TempStateData.StateName = TempCString1;
                            TempStateData.StateNumber = TempStateNumber;
                            TempStateData.EntryAction = TempEntryAction;
                            TempStateData.ActivityFunction = TempActivityFunction;
                            TempStateData.ExitAction = TempExitAction;
                            // Add the state
                            Diagram.AddState(&TempStateData);

                            // IF22 The state has any transitions (out)
							if("Out" == ModelFile.GetField(1))
							{ // THEN22 Process the state's transitions
								TempCString1 = ProcessTransitions(TempStateNumber, NON_SUPER_STATE);
								if("No transition conditions" == TempCString1)
								{
									TempCString1 = ConvertStateNumberToText(TempStateNumber)\
										+ " has a transition out with no transition conditions";
									::MessageBox(0, TempCString1, "Error", MB_ICONERROR);
									Status = "Not OK";
									Processing1 = FALSE;
								}
								// ELSE22 IF221 There is an unknown target state
								else if("State " == TempCString1.Left(6))
								{ // THEN151 Fill in the not OK status and stop processing
									::MessageBox(0, TempCString1, "Error", MB_ICONERROR);
									Status = "Not OK";
									Processing1 = FALSE;
								}
							}
							else
							{ // ELSE22 Store possible state text
								TempCString1 = ModelFile.GetField(1);
							}
						}
						else
						{ // ELSE18 Report this unknown state, set the return status
							// to indicate a state error, stop processing, and break
							// out of the while loop
							TempCString1 = "State " + TempCString1 + " not found in the configuration file";
							::MessageBox(0, TempCString1, "Error", MB_ICONERROR);
							Status = "Not OK";
							Processing1 = FALSE;
							break;
						}
					}
				}
			}
            else
            { // ELSE2 Get possible state text
                TempCString1 = ModelFile.GetField(1);
                // IF23 A new diagram is encountered
                if("Multiplicity" == ModelFile.GetField(1))
                { // THEN23 Add the diagram
                    // Diagram.AddDiagram();
                }
            }
        }
    }
    return(Status);
}

//****************************************************************************
// Name:  ConvertStateTextToNumber
//
// This converts the passed state name to its corresponding number based on
// its position in either the state or super state text arrays.  Super state
// numbers use an offset of SUPER_STATE_OFFSET to distinguish them from non
// super states.
//
// This returns CONVERT_ERROR if the state text is not valid including if
// an empty string is passed to this function.
//****************************************************************************
int ConvertStateTextToNumber(CString StateTextToConvert)
{
    int ii, ReturnValue = CONVERT_ERROR;
    CString LocalStateTextToConvert = StateTextToConvert;
    LocalStateTextToConvert.MakeUpper();
	
	// IF1 The passed state text is not empty
	if("" != StateTextToConvert)
	{ // THEN1 First look for it in the non super state text array
        // Check to see if the state is a non-super state
		for(ii = 0; ii < Diagram.GetNumStatesInConfigFile(); ii++)
		{
			// IF2 It was found
			if(LocalStateTextToConvert == Diagram.GetStateNumberToTextElement(ii))
			{ // THEN2 Store the number and break out of the for loop
				ReturnValue = ii;
				break;
			}
		}
		// IF3 It wasn't found look in the super states
		if(Diagram.GetNumStatesInConfigFile() == ii)
		{ // THEN3 Look for it in the super state text array
			for(ii = 0; ii < Diagram.GetNumSuperStatesInConfigFile(); ii++)
			{
				// IF4 It was found
				if(LocalStateTextToConvert == Diagram.GetSuperStateNumberToTextElement(ii))
				{ // THEN4 Store the number and break out of the for loop
					ReturnValue = ii + SUPER_STATE_OFFSET;
					break;
				}
			}
		}
	}
	// END1
	return(ReturnValue);
}

//****************************************************************************
// Name:  ConvertStateNumberToText
//
// This converts the passed state number to its corresponding name.  Super
// states use an offset of SUPER_STATE_OFFSET to distinguish them from non
// super states.
//
// This returns "CONVERT_ERROR" if the passed state number is invalid.
//****************************************************************************
CString ConvertStateNumberToText(int StateNumberToConvert)
{
    CString TempStateText;

	// IF1 The passed number is a valid non-super state number
	if((StateNumberToConvert >= 0 ) && (StateNumberToConvert < Diagram.GetNumStatesInConfigFile()))
    { // THEN1 Return the corresponding non super state name
        return(Diagram.GetStateNumberToTextElement(StateNumberToConvert));
    }
	// ELSE1 IF2 The passed number is a valid super state number
	else if((StateNumberToConvert >= SUPER_STATE_OFFSET) && (StateNumberToConvert <\
        (SUPER_STATE_OFFSET + Diagram.GetNumSuperStatesInConfigFile())))
    { // THEN2 Return the corresponding super state name
        return(Diagram.GetSuperStateNumberToTextElement(StateNumberToConvert - SUPER_STATE_OFFSET));
    }
    else
	{
		return("CONVERT_ERROR");
	}
}

//****************************************************************************
// Name:  ConvertCondCStringToBit
//
// This converts the passed transition condition bit name to its corresponding
// bit mask value.
//****************************************************************************
int ConvertCondCStringToBit(CString CondCString)
{
    int ii;
    CString TempCString;

    // Perform an upper case compare
    CondCString.MakeUpper();
    // Search the condition bit array
    for(ii = 0; ii < Diagram.GetNumCondBits(); ii++)
    {
        // Store current condition text from the array
        TempCString = Diagram.GetCondBitText(ii);
        // Perform an upper case compare
        TempCString.MakeUpper(); 
        // IF1 A match is found
        if(CondCString == TempCString)
        { // THEN1 Break out of the for loop (the value of ii is the index)
            break;
        }
    }

    // IF2 A match was not found
    if(Diagram.GetNumCondBits() == ii)
    { // THEN2 Return an error
        return(CONVERT_ERROR);
    }
    else
    { // ELSE2 Return the corresponding condition bit mask value
        return(Diagram.GetCondBit(ii));
    }
}

//****************************************************************************
// Name:  ConvertDontCareCStringToBit
//
// This converts the passed transition don't care bit name to its
// corresponding bit mask value.
//****************************************************************************
int ConvertDontCareCStringToBit(CString DontCareCString)
{
    int ii;
    CString TempCString;

    // Perform an upper case compare
    DontCareCString.MakeUpper();
    // Search the don't care bit array
    for(ii = 0; ii < Diagram.GetNumDontCareBits(); ii++)
    {
        // Store current condition text from the array
        TempCString = Diagram.GetDontCareBitText(ii);
        // Perform an upper case compare
        TempCString.MakeUpper(); // Perform an upper case compare
        // IF1 A match is found
        if(DontCareCString == TempCString)
        { // THEN1 Break out of the for loop (the value of ii is the index)
            break;
        }
    }

    // IF2 A match was not found
    if(Diagram.GetNumDontCareBits() == ii)
    { // THEN2 Return an error
        return(CONVERT_ERROR);
    }
    else
    { // ELSE2 Return the corresponding don't care bit mask value
        return(Diagram.GetDontCareBit(ii));
    }
}

//****************************************************************************
// Name:  ProcessTransitions
//
// This adds transitions to the Diagram object for the current state or super
// state.  When this function is called, this expects the current model file
// line to be the "Out Transition" line before the first line of a transition
// condition expression (e.g. "[fsm_timeout1 = TRUE &&").  This returns a
// string that is either the string "Default" if the passed state was a
// a super state or a possible "next" state due to the nature of the
// processing.  This also returns "No transition conditions" if a transtion
// has no transition conditions.
//
// This uses a state machine to process the transition condition expression.
//****************************************************************************
CString ProcessTransitions(int CurrentState, int SuperStateFlag)
{
    CString TempCString, DontCareCString, CondCString, TransActionCString, TempCString2;
    CString TempTransActionCString1, TempTransActionCString2, TempTransLineContCString;
    char TempChar;
    int ProcessingAllTrans, ProcessingCurrentTrans, IllegalTransExpressionFlag;
    int TransProcessingState;
    int TransStatus;
    int DontCareBitMask, CondBitMask;
    int TempDontCareBitVal, TempCondBitVal, TempStateNumber;
    int ii;
    Trans_t TempTransConfig;
	char TempArray[50];
    
    ProcessingAllTrans = TRUE;
    while(TRUE == ProcessingAllTrans)
    {
        ProcessingCurrentTrans = TRUE;
        TransProcessingState = INIT;
        DontCareBitMask = 0x00000000;
        CondBitMask = 0x00000000;
        TransStatus = 0x00000000;
        DontCareCString.Empty();
        CondCString.Empty();
        TransActionCString.Empty();
        TempTransConfig.TransStatus = "";
		IllegalTransExpressionFlag = FALSE;

        // WHILE1 Processing the current transition condition expression
        while(TRUE == ProcessingCurrentTrans)
        {
            // Get transition condition expression line
            ModelFile.StoreFileLine();
            // FOR1 All characters in the current line
            // Subtract - from GetNumChars below to ignore the carriage return at the end
            for(ii = 0; ii < (ModelFile.GetNumChars() - 1); ii++)
            {
                // Store current character in a temporary
                TempChar = ModelFile.GetChar(ii);
                
                // IF1 The current character is not a whitespace character
                if((' ' != TempChar) && ('\t' != TempChar))
                { // THEN1 SWITCH1 On the current state
                    switch(TransProcessingState)
                    {
                    case(INIT):
                        { // IF2 This is the start of the trans cond exp
                            if('[' == TempChar)
                            { // THEN2 Set to the getting the don't care bit
                                TransProcessingState = GETTING_DONT_CARE_BIT;
                            }
							else
							{ // ELSE2 Prepare to indicate an error and stop processing
								TempCString = "No transition conditions";
								ProcessingCurrentTrans = FALSE;
								ProcessingAllTrans = FALSE;
							}
                            break;
                        }
                    case(GETTING_DONT_CARE_BIT):
                        { // Store the don't care string (e.g. fsm_timeout1,
                          // heat_mode, etc.)
							// IF3 At the beginning of the don't care string
							// (Allow for double && for "AND")
                            if('&' != TempChar)
                            { // THEN3 IF4 Done storing the don't care string
                                if('=' == TempChar)
                                { // THEN4 Go to get the condition bit string
                                    TransProcessingState = GETTING_COND_BIT;
                                }
								// ELSE4 IF5 This is the end of the current trans cond
                                else if(']' == TempChar)
                                { // THEN5 Indicate done
                                    ProcessingCurrentTrans = FALSE;
                                }
								// ELSE5 IF6 This is an illegal trans exp (i.e. one with illegal characters)
                                else if(('(' == TempChar) || (')' == TempChar) || ('|' == TempChar) ||\
                                        ('<' == TempChar) || ('>' == TempChar))
                                { // THEN6 Flag this as an illegal trans cond exp and abort and wait for the end bracket
                                    TransStatus |= ILLEGAL_LOGIC_MASK;
									IllegalTransExpressionFlag = TRUE;
									TransProcessingState = WAITING_FOR_END_BRACKET;
									TempTransConfig.TransStatus += ", illegal transition action expression";
									TempTransConfig.TransStatus += '\n';
									TempTransConfig.TransStatus += "Only \"=\" and \"&&\" symbols are allaowed in transition action expressions";
                                }
                                else
                                { // ELSE6 Store the don't care string (use a long
									// index here to make sure it is appended
                                    DontCareCString.Insert(500, TempChar);
                                }
                            }
                            break;
                        }
                    case(GETTING_COND_BIT):
                        { // Read the condition bit string (e.g. ON, OFF, TRUE, ON, etc.)
                          // and create a don't care bit mask and a condition bit mask
                            // IF7 Done getting the condition bit string
                            if(('&' == TempChar) || (']' == TempChar))
                            { // THEN7 Convert the don't care string to a bit value
                                TempDontCareBitVal = ConvertDontCareCStringToBit(DontCareCString);
                                // IF8 It is a valid string
                                if(CONVERT_ERROR != TempDontCareBitVal)
                                { // THEN8 Update the don't care mask
                                    DontCareBitMask |= TempDontCareBitVal;
                                }
                                else
                                { // ELSE8 flag an unknown condition bit error
                                    // for the current transition
                                    TransStatus |= UNKNOWN_DONT_CARE_MASK;
                                    TempTransConfig.TransStatus += ", don't care value\"";
                                    TempTransConfig.TransStatus += DontCareCString;
                                    TempTransConfig.TransStatus += "\"";
                                }
                                // Clear the don't care string local variable for next time
                                DontCareCString.Empty();
                                
                                // Convert the condition string to a bit value
                                TempCondBitVal = ConvertCondCStringToBit(CondCString);
                                // IF9 It is a valid string
                                if(CONVERT_ERROR != TempCondBitVal)
                                { // THEN9 IF10 The cond bit should be a one
                                    if(0 != TempCondBitVal)
                                    { // THEN10 Update the condition mask (set the appropriate bit)
                                        CondBitMask |= TempDontCareBitVal;
                                    }
                                }
                                else
                                { // ELSE10 IF101 This is the first time (this prevents multiple
									// "condition value xxx" statements in the "condition value xxx
									// not found in the configuration file" error message)
									if(0 == (TransStatus & UNKNOWN_COND_MASK))
									{ // THEN101 flag an unknown condition bit error
                                    // for the current transition
										TransStatus |= UNKNOWN_COND_MASK;
										TempTransConfig.TransStatus += ", condition value \"";
										TempTransConfig.TransStatus += CondCString;
										TempTransConfig.TransStatus += "\"";
									}
                                }
                                // Clear the condition string local variable for next time
                                CondCString.Empty();
                                
                                // IF11 This trans cond exp contains more than one cond
                                // ANDed together
                                if('&' == TempChar)
                                { // THEN11 Go get the next trans cond
                                    TransProcessingState = GETTING_DONT_CARE_BIT;
                                }
                                else
                                { // ELSE11 IF12 This trans contains a trans action
                                    if('/' == ModelFile.GetChar(ii + 1))
                                    { // THEN12 Indicate this in the trans status
                                        TransStatus |= TRANS_ACTION_PRESENT_MASK;
                                        TempTransLineContCString = "";
                                        // IF13 The trans action continues on this line
                                        if('\n' != ModelFile.GetChar(ii + 2))
                                        { // THEN13 Set the trans line continuation string to
                                            // the rest of the line including the '\n'
                                            //  Skip the '[' and '/' first
                                            ii += 2;
                                            for(; ii < ModelFile.GetNumChars(); ii++)
                                            {
                                                TempTransLineContCString.Insert(500, ModelFile.GetChar(ii));
                                            }
                                        }
                                    }
                                    // Indicate done with current trans
                                    ProcessingCurrentTrans = FALSE;
                                }
                            }
                            // ELSE7 IF14 This is an illegal trans exp (i.e. one with illegal characters)
							else if(('(' == TempChar) || (')' == TempChar) || ('|' == TempChar) ||\
								('<' == TempChar) || ('>' == TempChar))
                            { // THEN14 Flag this as an illegal trans cond exp and abort and wait for the end bracket
                                TransStatus |= ILLEGAL_LOGIC_MASK;
								IllegalTransExpressionFlag = TRUE;
								TransProcessingState = WAITING_FOR_END_BRACKET;
								TempTransConfig.TransStatus += ", illegal transition action expression";
								TempTransConfig.TransStatus += '\n';
								TempTransConfig.TransStatus += "Only \"=\" and \"&&\" symbols are allaowed in transition action expressions";
                            }
                            else
                            { // ELSE14 Update the cond string
                                // Use a long index here to make sure it is concatenated
                                CondCString.Insert(500, TempChar);
                            }
                            break;
                        }
                    case(WAITING_FOR_END_BRACKET):
                        {
							// IF141 The end bracket is encountered
                            if(']' == TempChar)
							{ // THEN141 IF142 This trans contains a trans action
								if('/' == ModelFile.GetChar(ii + 1))
								{ // THEN142 Indicate this in the trans status
									TransStatus |= TRANS_ACTION_PRESENT_MASK;
									TempTransLineContCString = "";
									// IF143 The trans action continues on this line
									if('\n' != ModelFile.GetChar(ii + 2))
									{ // THEN143 Set the trans line continuation string to
										// the rest of the line including the '\n'
										//  Skip the '[' and '/' first
										ii += 2;
										for(; ii < ModelFile.GetNumChars(); ii++)
										{
											TempTransLineContCString.Insert(500, ModelFile.GetChar(ii));
										}
									}
								}
								// Indicate done with current trans
								ProcessingCurrentTrans = FALSE;
							}
						}
						break;
                    }
                }
                // IF15 Done processing the current trans
                if(FALSE == ProcessingCurrentTrans)
                { // THEN15 Break out of the for loop if done
                    break;
                }
            }
        }
		// IF16 The transition has transition conditions
		if(TRUE == ProcessingAllTrans)
		{ // THEN16 IF16 There is a transition action
			if((TransStatus & TRANS_ACTION_PRESENT_MASK) != 0)
			{ // THEN16 Store the trans action in TransActionCString
				// IF17 The first part of the trans action was a line continuation
				// after the '/'
				if("" != TempTransLineContCString)
				{ // THEN17 Store the first part (TempTransLineContCString
					// was stored above)
					TransActionCString += "   " + TempTransLineContCString;
				}
				// Store the (rest of the) transition action
				TransActionCString += ProcessFunctionBlock("Target:", "DontCare", "DontCare", "DontCare");
				// Clear the trans action present bit for processing below
				TransStatus &= ~TRANS_ACTION_PRESENT_MASK;
			}
			else
			{ // ELSE16 Store the "Target:" text line
				ModelFile.StoreFileLine();
			}
			
			// Store the target state in TempCString
			ModelFile.StoreFileLine(); // Get line
			TempCString = ModelFile.GetField(1); // Get Target state
			TempCString.MakeUpper();
			
			// Convert the trans target state to a state number
			TempStateNumber = ConvertStateTextToNumber(TempCString);
            // I DON'T THINK THAT THE FOLLOWING VALID STATE CHECK IS NEEDED
			// BECAUSE IT WILL NEVER GET PAST THE ProcessModelFile VALID
			// STATE CHECK BUT THIS NEEDS TO BE VERIFIED BEFORE THIS CHECK
			// IS REMOVED.
            // ON 1/14/04 PAUL ENCOUNTERED AN ERROR WHERE THE TEXT "Target:"
            // WAS MISSING FROM THE MODEL TEXT FILE.  THIS IS SOME SORT OF
            // DEFECT IN RHAPSODY.  THIS SITUATION IS CAUGHT WITH THE CHECK
            // BELOW.  I SHOULD ADD AN ERROR OUTPUT MENTIONING THAT THIS IS
            // A KNOWN RHAPSODY DEFECT ETC.
			// I FOUND A DIFFERENT SCENARIO WHERE THE FOLLOWING CHECK WAS
			// NEEDED.  IT HAD TO DO WITH A SUPER THAT HAD A TARGET STATE
			// THAT WAS A SUPER STATE AND THAT TARGET STATE WAS NOT FOUND
			// IN THE CONFIG TEXT FILE.
			// IF17 The next state is a valid state
			if(CONVERT_ERROR != TempStateNumber)
			{ // THEN17 Store the trans target state in a local trans structure
				TempTransConfig.TargetState = TempStateNumber;
			}
			else
			{ // ELSE17 Indicate an unknown target state error
				TempCString2 = "State ";
				TempCString2 += TempCString;
				TempCString2 += " not found in the configuration file";
				TempCString = TempCString2;
				ProcessingCurrentTrans = FALSE;
				ProcessingAllTrans = FALSE;
				TransStatus |= UNKNOWN_TARGET_STATE_MASK;
				break;
			}
			
			// IF18 An "active error" exists
			if(0 != TransStatus)
			{ // THEN18 IF177 An illegal transition condition expression was not encountered
				if(FALSE == IllegalTransExpressionFlag)
				{
					// THEN177 Finish the trans status string for this trans
					TempTransConfig.TransStatus += " not found in the configuration file";
				}
			}
			else
			{ // ELSE18 Store "OK" in the trans status
				TempTransConfig.TransStatus = "OK";
				// Set the appropriate bits local cond and don't care variables
				if(FALSE == SuperStateFlag)
				{
					TempTransConfig.TransCondMask = CondBitMask;
					TempTransConfig.TransDontCareMask = DontCareBitMask;
				}
				else
				{
					TempTransConfig.TransCondMask = CondBitMask;
					TempTransConfig.TransDontCareMask = DontCareBitMask;
				}
			}
			
			// IF19 This transition has an action
			if("" != TransActionCString)
			{ // THEN19 Add the trans action function to the trans action function name array
				TempTransActionCString1 = "";
				TempTransActionCString2 = "";
				TempTransActionCString1 = ConvertStateNumberToText(CurrentState);
				TempTransActionCString1.MakeLower();
				TempTransActionCString2 = ConvertStateNumberToText(TempTransConfig.TargetState);
				TempTransActionCString2.MakeLower();

				// IF20 codegen version 34 compatibility mode is selected
				if(TRUE == BackwardCompModeSelected)
				{ // THEN20 Use the 13 right characters of the source state for the first "half"
					// of the transition action function name (this makes it compatible with codegen version 34)
					TempCString = TempTransActionCString1.Right(13);
				}
				else
				{ // ELSE20 Use the selected number of right characters of the source state minus 4
					// for the "_to_" divided by 2 subtract 2 to account for 2 of the 4 number digits
					// (up to 9999) that are appended to the end to keep the transition action function
					// names unique - to get the size of the first "half" - for the first "half"
					// of the transition action function name
					TempCString = TempTransActionCString1.Right(((AutoGenFunctNameLimit-4)/2)-2);
				}

				// IF201 codegen version 34 backwards compatibility mode is selected
				if(TRUE == BackwardCompModeSelected)
				{ // THEN201 IF21 The first char is an underscore
					if('_' == TempCString.GetAt(0))
					{ // THEN21 Delete it so no trans action function names begin with an underscore
						TempCString.Delete(0, 1);
					}
					// Store the first char in a temporary
					TempChar = TempCString.GetAt(0);
					// IF22 The first char is a number
					if(('0' == TempChar) || ('1' == TempChar) || ('2' == TempChar) ||\
						('3' == TempChar) || ('4' == TempChar) || ('5' == TempChar) ||\
						('6' == TempChar) || ('7' == TempChar) || ('8' == TempChar) ||\
						('9' == TempChar))
					{ // THEN22 Change it to a 'Z'
						TempCString.SetAt(0, 'Z');
					}

					// Use the 13 right characters of the target state for the second "half" of the
					// transition action function name (this makes it compatible with codegen version 34)
					TempTransConfig.TransActionFunctionName = TempCString +\
						"_to_" + TempTransActionCString2.Right(13) +\
						_itoa(Diagram.GetNumTransActions(), &TempArray[0], 10);
				}
				else
				{ // ELSE201 Use the selected number of right characters of the target state minus 4
					// for the "_to_" divided by 2 subtract 2 to account for 2 of the 4 number digits
					// (up to 9999) that are appended to the end to keep the transition action function
					// names unique - to get the size of the second "half" - for the second "half"
					// of the transition action function name.  The CStringFirstCharCheck call makes
					// sure the first character is not an underscore or a number.
					TempTransConfig.TransActionFunctionName = CStringFirstCharCheck(TempCString) +\
						"_to_" + TempTransActionCString2.Right(((AutoGenFunctNameLimit-4)/2)-2) +\
						_itoa(Diagram.GetNumTransActions(), &TempArray[0], 10);
				}
				
				// Add the trans action function to the trans action function body array
				TempTransConfig.TransActionFunctionBody = TransActionCString;
			}
            else
            { // ELSE19 Clear the transition action name and body
                TempTransConfig.TransActionFunctionName = "";
                TempTransConfig.TransActionFunctionBody = "";
            }
			
			// Add the transition to the diagram
			if(FALSE == SuperStateFlag)
			{
				TempTransConfig.AddedBySuperState = ""; // This flag is only used by AddTransFromSuperStates
				Diagram.AddTrans(NON_SUPER_STATE, CurrentState, &TempTransConfig);
			}
			else
			{
				TempTransConfig.AddedBySuperState = ""; // This flag is only used by AddTransFromSuperStates
				Diagram.AddTrans(SUPER_STATE, (CurrentState - SUPER_STATE_OFFSET), &TempTransConfig);
			}
			
			ModelFile.StoreFileLine(); // Get line
			TempCString = ModelFile.GetField(1); // Get either "Out", "Default" (if
			// this is a super state), or the next state
			// IF23 Done with all of the outgoing transitions
			if("Out" != TempCString)
			{
				// THEN23 Stop
				ProcessingAllTrans = FALSE;
			}
		}
    }
    return(TempCString);
}

//****************************************************************************
// Name:  ProcessSuperStateTrans
//
// This creates super state extended lists for each super state
// that only contain non super states in the lists
//
// WHY DO I USE THE SUPER STATE NAME IN THE DIAGRAM STRUCTURE INSTEAD
// OF CALLING ConvertStateTextToNumber?  Because I don't know which
// substates will be included in a super state so I can't use an
// index to indicate which substates are included.
//****************************************************************************
void ProcessSuperStateTrans(void)
{
    int jj;
    
    // FOR2 All super state list states
    for(jj = 0; jj < Diagram.GetNumStates(SUPER_STATE); jj++)
    {
        // Store a flat list of substates for the super state
        StoreSuperStateTrans(jj, Diagram.GetStateName(SUPER_STATE, jj),\
			Diagram.GetSuperStateAdditionalCondMask(jj),\
			Diagram.GetSuperStateAdditionalDontCareMask(jj));
    }
}

//****************************************************************************
// Name:  StoreSuperStateTrans
//
// This stores all substates in a passed super state.  This is a recursive
// function (i.e. it calls itself) so it handles multiple levels of super
// state/sub state nesting.  This returns true if the passed state is a
// super state and false if it is not.
//****************************************************************************
int StoreSuperStateTrans(int SuperStateExtendedListStateNumber, CString StateName, int AdditionalCondMask, int AdditionalDontCareMask)
{
    int ii, jj, kk, SuperStateFlag = FALSE, CurrentSuperStateNumber;
    CString CurrentStateName;
    ListSubState_t TempListSubStateData;
    
    // FOR1 All super state list states
    for(ii = 0; ii < Diagram.GetNumStates(SUPER_STATE); ii++)
    {
        // IF1 The passed state is found
        if(StateName == Diagram.GetStateName(SUPER_STATE, ii))
        { // THEN1 FOR2 All super state list sub states
            for(jj = 0; jj < Diagram.GetSuperStateListNumSubStates(ii); jj++)
            {
                // Store all states that are substates of the super state
                // IF2 The current state is not a super state
                CurrentStateName = Diagram.GetSuperStateListSubStateName(ii, jj);
                if(FALSE == StoreSuperStateTrans(SuperStateExtendedListStateNumber, CurrentStateName, AdditionalCondMask, AdditionalDontCareMask))
                { // THEN2 Add the sub state to the super state extended list
                    TempListSubStateData.SubStateName = CurrentStateName;
                    Diagram.AddSuperStateExtendedListSubState(SuperStateExtendedListStateNumber, &TempListSubStateData);
                }
				else
				{
					CurrentSuperStateNumber = ConvertStateTextToNumber(CurrentStateName);
					for(kk = 0; kk < Diagram.GetNumTrans(SUPER_STATE, (CurrentSuperStateNumber - SUPER_STATE_OFFSET)); kk++)
					{
						// Update all the condition and don't care masks for all
						// transitions for this state
						Diagram.UpdateTransCondDontCareMasks(SUPER_STATE, (CurrentSuperStateNumber - SUPER_STATE_OFFSET), kk,\
							AdditionalCondMask, AdditionalDontCareMask);
					} // END FOR6
				}
            }
            // Indicate that the passed state is a super state
            SuperStateFlag = TRUE;
            // Break out of the for loop
            break;
        }
    }
    return(SuperStateFlag);
}

//****************************************************************************
// Name:  ProcessStateSuperStatesTargets
//
// This replaces all transition target super states with the corresponding
// default state.  It handles nested super states.  It returns a CString with
// either "OK" or with a message listing super states with transitions in
// with no default transition.
//
// This handles states and super states.
//****************************************************************************
CString ProcessStateSuperStatesTargets(void)
{
    int jj, kk, ll;
    CString NewNextState, ReturnStatus = "OK", TempReturnStatus;
    int NewNextStateNumber;
	CString TempCString, TempCString2, TempCString3;
	GetNestedDefaultStateReturnStatus_t GetNestedDefaultStateReturnStatus;
    
    // FOR1 All non super states in this diagram
    for(jj = 0; jj < Diagram.GetNumStates(NON_SUPER_STATE); jj++)
    {
        // FOR2 All transitions for this non super state
        for(kk = 0; kk < Diagram.GetNumTrans(NON_SUPER_STATE, jj); kk++)
        {
            // Check the target state of each non super state transition to
            // see if it is a super state
            // FOR3 All super states
            for(ll = 0; ll < Diagram.GetNumStates(SUPER_STATE); ll++)
            { // IF1 The transition is valid
                if("OK" == Diagram.GetTransStatus(NON_SUPER_STATE, jj, kk))
                { // THEN1 IF2 The target state is a super state
                    if(ConvertStateNumberToText(Diagram.GetTransTargetState(NON_SUPER_STATE, jj, kk)) == 
                        ConvertStateNumberToText(ll + SUPER_STATE_OFFSET))
                    { // THEN2 
						// Update the super state target state with a non super
                        // state based on the super state default transition target
                        // state
                        NewNextStateNumber = ConvertStateTextToNumber(\
                            Diagram.GetDefaultTransTargetState(ll));
						// IF3 The super state has a default transition state
						if(CONVERT_ERROR != NewNextStateNumber)
						{ // THEN3 Update the next state to the default transition state
							// (i.e. instead of the super state name)

							// Drill down to the "last" super state in the nested set of super states
							GetNestedDefaultStateReturnStatus = GetNestedDefaultState(ll);
							
							// IF33 The "last" super state in the nested set of super states is missing a default transition
							if(CONVERT_ERROR == GetNestedDefaultStateReturnStatus.NextStateNumber)
							{ // THEN33 IF44 If this is the first time error
								if("OK" == ReturnStatus)
								{ // THEN44 Store the error message with the first error state
									ReturnStatus = "The following super states have transitions in\n";
									ReturnStatus += "but don't have default transitions:\n";
									ReturnStatus += GetNestedDefaultStateReturnStatus.Status + "\n";
								}
								else
								{ // ELSE44 Check to see if it is already in the list
									TempReturnStatus = GetNestedDefaultStateReturnStatus.Status + "\n";
									// IF55 The super state is not already listed
									if(-1 == ReturnStatus.Find(TempReturnStatus))
									{ // THEN55 Add it to the list
										ReturnStatus += TempReturnStatus;
									}
								}
							}
							else
							{ // ELSE33 Change the target super state to the target non-super state
								Diagram.UpdateTargetState(NON_SUPER_STATE, jj, kk, GetNestedDefaultStateReturnStatus.NextStateNumber);
							}
						}
						else
						{ // ELSE3 IF4 If this is the first time error
							if("OK" == ReturnStatus)
							{ // THEN4 Store the error message with the first error state
								ReturnStatus = "The following super states have transitions in\n";
								ReturnStatus += "but don't have default transitions:\n";
								ReturnStatus += ConvertStateNumberToText(ll + SUPER_STATE_OFFSET) + "\n";
							}
							else
							{ // ELSE4 Check to see if it is already in the list
                                TempReturnStatus = ConvertStateNumberToText(ll + SUPER_STATE_OFFSET) + "\n";
                                // IF5 The super state is not already listed
                                if(-1 == ReturnStatus.Find(TempReturnStatus))
                                { // THEN5 Add it to the list
                                    ReturnStatus += TempReturnStatus;
                                }
							}
						}
                    }
                }
            } // END FOR3
        } // END FOR2
    } // END FOR1

    // FOR4 All super states in this diagram
    for(jj = 0; jj < Diagram.GetNumStates(SUPER_STATE); jj++)
    {
        // FOR5 All transitions for this super state
        for(kk = 0; kk < Diagram.GetNumTrans(SUPER_STATE, jj); kk++)
        {
            // Check the target state of each super state transition to
            // see if it is a super state
            // FOR6 All super states
            for(ll = 0; ll < Diagram.GetNumStates(SUPER_STATE); ll++)
            { // IF6 The transition is valid
                if("OK" == Diagram.GetTransStatus(SUPER_STATE, jj, kk))
                { // THEN6 IF7 The target state is a super state
                    if(ConvertStateNumberToText(Diagram.GetTransTargetState(SUPER_STATE, jj, kk)) == 
                        ConvertStateNumberToText(ll + SUPER_STATE_OFFSET))
                    { // THEN7 
						// Update the super state target state with a non super
                        // state based on the super state default transition target
                        // state
                        NewNextStateNumber = ConvertStateTextToNumber(\
                            Diagram.GetDefaultTransTargetState(ll));
						// IF8 The super state has a default transition state
						if(CONVERT_ERROR != NewNextStateNumber)
						{ // THEN8 Update the next state to the default transition state
							// (i.e. instead of the super state name)

							// Drill down to the "last" super state in the nested set of super states
							GetNestedDefaultStateReturnStatus = GetNestedDefaultState(ll);
							
							// IF88 The "last" super state in the nested set of super states is missing a default transition
							if(CONVERT_ERROR == GetNestedDefaultStateReturnStatus.NextStateNumber)
							{ // THEN88 IF99 If this is the first time error
								if("OK" == ReturnStatus)
								{ // THEN99 Store the error message with the first error state
									ReturnStatus = "The following super states have transitions in\n";
									ReturnStatus += "but don't have default transitions:\n";
									ReturnStatus += GetNestedDefaultStateReturnStatus.Status + "\n";
								}
								else
								{ // ELSE99 Check to see if it is already in the list
									TempReturnStatus = GetNestedDefaultStateReturnStatus.Status + "\n";
									// IF11 The super state is not already listed
									if(-1 == ReturnStatus.Find(TempReturnStatus))
									{ // THEN11 Add it to the list
										ReturnStatus += TempReturnStatus;
									}
								}
							}
							else
							{ // ELSE88 Change the target super state to the target non-super state
								Diagram.UpdateTargetState(SUPER_STATE, jj, kk, GetNestedDefaultStateReturnStatus.NextStateNumber);
							}
						}
						else
						{ // ELSE8 IF9 If this is the first time error
							if("OK" == ReturnStatus)
							{ // THEN9 Store the error message with the first error state
								ReturnStatus = "The following super states have transitions in\n";
								ReturnStatus += "but don't have default transitions:\n";
								ReturnStatus += ConvertStateNumberToText(ll + SUPER_STATE_OFFSET) + "\n";
							}
							else
							{ // ELSE9 Check to see if it is already in the list
                                TempReturnStatus = ConvertStateNumberToText(ll + SUPER_STATE_OFFSET) + "\n";
                                // IF10 The super state is not already listed
                                if(-1 == ReturnStatus.Find(TempReturnStatus))
                                { // THEN10 Add it to the list
                                    ReturnStatus += TempReturnStatus;
                                }
							}
						}
                    }
                }
            } // END FOR6
        } // END FOR5
    } // END FOR4
	return(ReturnStatus);
}

//****************************************************************************
// Name:  TransMutualExclusionCheck
//
// This checks state and super state transitions (stored in the Diagram
// object) for mutual exclusion.  This returns a "No non-mutually exclusive
// states detected" message if non are found.  This returns the a message
// including the states, sub-states, and super states if any are found.
//****************************************************************************
CString TransMutualExclusionCheck(void)
{
    int TempNumTrans, TempCondMask1, TempCondMask2, TempDontCareMask1, TempDontCareMask2;
    int ii, jj, kk, TempPossibleNonMutExcFlag;
    CString TempNextState1, TempNextState2, TempTransActionCString1, TempTransActionCString2;
    CString TempCStringReturn = "No non-mutually exclusive states detected";
    int NonMutuallyExclusiveStateFlag = FALSE;
    int NonMutuallyExclusiveSuperStateFlag = FALSE;
    int NonMutuallyExclusiveStateAddedBysuperStateFlag = FALSE;
    int TempNumStates, TempNumSuperStates, TempMaxStates;

    int *BegTempStateArrayPtr, *TempStateArrayPtr;
    CString *BegTempAddedBySuperStateArrayPtr, *TempAddedBySuperStateArrayPtr;
    int *BegTempSuperStateArrayPtr, *TempSuperStateArrayPtr;
    int *BegTempStateAddedBySuperStateArrayPtr, *TempStateAddedBySuperStateArrayPtr;

    // Dynamically allocate and initialize local arrays for use below
    // Make sure the size is big enough by checking to see which is greater,
    // the number of states or the number of super states
    TempNumStates = Diagram.GetNumStates(NON_SUPER_STATE);
    TempNumSuperStates = Diagram.GetNumStates(SUPER_STATE);
    if(TempNumStates >= TempNumSuperStates)
    {
        TempMaxStates = TempNumStates;
        BegTempStateArrayPtr = new int[TempNumStates];
        TempStateArrayPtr = BegTempStateArrayPtr;
        
        BegTempAddedBySuperStateArrayPtr = new CString[TempNumStates];
        TempAddedBySuperStateArrayPtr = BegTempAddedBySuperStateArrayPtr;
        
        BegTempSuperStateArrayPtr = new int[TempNumStates];
        TempSuperStateArrayPtr = BegTempSuperStateArrayPtr;
        
        BegTempStateAddedBySuperStateArrayPtr = new int[TempNumStates];
        TempStateAddedBySuperStateArrayPtr = BegTempStateAddedBySuperStateArrayPtr;
        for(ii = 0; ii < TempNumStates; ii++)
        {
            *TempStateArrayPtr++ = FALSE;
            *TempAddedBySuperStateArrayPtr++ = "";
            *TempSuperStateArrayPtr++ = FALSE;
            *TempStateAddedBySuperStateArrayPtr++ = FALSE;
        }
    }
    else
    {
        TempMaxStates = TempNumSuperStates;
        BegTempStateArrayPtr = new int[TempNumSuperStates];
        TempStateArrayPtr = BegTempStateArrayPtr;
        
        BegTempAddedBySuperStateArrayPtr = new CString[TempNumSuperStates];
        TempAddedBySuperStateArrayPtr = BegTempAddedBySuperStateArrayPtr;
        
        BegTempSuperStateArrayPtr = new int[TempNumSuperStates];
        TempSuperStateArrayPtr = BegTempSuperStateArrayPtr;
        
        BegTempStateAddedBySuperStateArrayPtr = new int[TempNumSuperStates];
        TempStateAddedBySuperStateArrayPtr = BegTempStateAddedBySuperStateArrayPtr;
        for(ii = 0; ii < TempNumSuperStates; ii++)
        {
            *TempStateArrayPtr++ = FALSE;
            *TempAddedBySuperStateArrayPtr++ = "";
            *TempSuperStateArrayPtr++ = FALSE;
            *TempStateAddedBySuperStateArrayPtr++ = FALSE;
        }
    }
    
    // FOR1 All non super states in this diagram
    for(ii = 0; ii < TempNumStates; ii++)
    {
        // Store the number of trans for this state in a temporary
        TempNumTrans = Diagram.GetNumTrans(NON_SUPER_STATE, ii);
        // FOR2 All transitions for this non super state
        for(jj = 0; jj < TempNumTrans; jj++)
        {
            // Store the cond and don't care masks for this trans in temporaries
            TempCondMask1 = Diagram.GetTransCondMask(NON_SUPER_STATE, ii, jj);
            TempDontCareMask1 = Diagram.GetTransDontCareMask(NON_SUPER_STATE, ii, jj);
            TempNextState1 = ConvertStateNumberToText(Diagram.GetTransTargetState(
                NON_SUPER_STATE, ii, jj));

            // FOR3 All trans for this non super state starting without duplicating
            // previous searches (e.g. for a state with 4 trans 1, 2, 3, 4 check
            // 1-2, 1-3, 1-4, then 2-3, 2-4, then 3-4)
            for(kk = (jj + 1); kk < TempNumTrans; kk++)
            {
                // Store the cond and don't care masks for this trans in temporaries
                TempCondMask2 = Diagram.GetTransCondMask(NON_SUPER_STATE, ii, kk);
                TempDontCareMask2 = Diagram.GetTransDontCareMask(NON_SUPER_STATE, ii, kk);
                TempNextState2 = ConvertStateNumberToText(Diagram.GetTransTargetState(
                    NON_SUPER_STATE, ii, kk));

                TempPossibleNonMutExcFlag = FALSE;
                // IF1 Cond masks match
                if(TempCondMask1 == TempCondMask2)
                { // THEN1 Indicate possible non-mutually exclusive transitions
                    TempPossibleNonMutExcFlag = TRUE;
                }
                // ELSE1 IF2 The trans don't care masks don't at least contain
                // a common bit (a common bit is necessary for mutual exclusion)
                else if((TempDontCareMask1 & TempDontCareMask2) == 0)
                { // THEN2 Indicate possible non-mutually exclusive trans
                    TempPossibleNonMutExcFlag = TRUE;
                }
                else
                { // ELSE2 IF3 Non-mutually exclusive trans
                    if(0 == ((TempCondMask1 ^ TempCondMask2) & (TempDontCareMask1 & TempDontCareMask2)))
                    { // THEN3 Indicate possible non-mutually exclusive transitions
                        TempPossibleNonMutExcFlag = TRUE;
                    }
                }
                // IF4 There is a possible non-mutually exclusive situation
                if(TRUE == TempPossibleNonMutExcFlag)
                { // THEN4 IF5 Both trans going to the same state
                    if(TempNextState1 == TempNextState2)
                    { // THEN5 Prepare to compare the trans action functions
                        TempTransActionCString1 =\
                            Diagram.GetTransActionFunctionBody(NON_SUPER_STATE, ii, jj);
                        TempTransActionCString2 =\
                            Diagram.GetTransActionFunctionBody(NON_SUPER_STATE, ii, kk);

                        // Strip off spaces and carriage returns before comparing
                        TempTransActionCString1.Remove(' ');
                        TempTransActionCString1.Remove('\n');
                        TempTransActionCString2.Remove(' ');
                        TempTransActionCString2.Remove('\n');

						// IF6 The trans action functions are different
                        if(TempTransActionCString1 != TempTransActionCString2)
                        { // THEN6 IF7 This trans is not due to this state being a child of a super state
							if("" == Diagram.GetTransAddedBySuperState(ii, kk))
							{ // THEN7 Indicate non-mutually exclusive trans
                                BegTempStateArrayPtr[ii] = TRUE;
								NonMutuallyExclusiveStateFlag = TRUE;
							}
							else
							{ // ELSE7 Indicate non-mutually exclusive trans due to this state
								// being a child of a super state
								BegTempStateAddedBySuperStateArrayPtr[ii] = TRUE;
								NonMutuallyExclusiveStateAddedBysuperStateFlag = TRUE;
								// IF8 This super state name has not already been added to the string list for this child state
								if(-1 == BegTempAddedBySuperStateArrayPtr[ii].Find(Diagram.GetTransAddedBySuperState(ii, kk)))
								{ // THEN8 Add this super state name to the string list for this child state
									BegTempAddedBySuperStateArrayPtr[ii] += Diagram.GetTransAddedBySuperState(ii, kk) + "  ";
								}
							}
                        }
                    }
                    else
                    { // ELSE5 IF9 This trans is not due to this state being a child of a super state
						if("" == Diagram.GetTransAddedBySuperState(ii, kk))
						{ // THEN9 Indicate non-mutually exclusive trans
                            BegTempStateArrayPtr[ii] = TRUE;
							NonMutuallyExclusiveStateFlag = TRUE;
						}
						else
						{ // ELSE9 Indicate non-mutually exclusive trans due to this state
							// being a child of a super state
							BegTempStateAddedBySuperStateArrayPtr[ii] = TRUE;
							NonMutuallyExclusiveStateAddedBysuperStateFlag = TRUE;
							// IF10 This super state name has not already been added to the string list for this child state
							if(-1 == BegTempAddedBySuperStateArrayPtr[ii].Find(Diagram.GetTransAddedBySuperState(ii, kk)))
							{ // THEN10 Add this super state name to the string list for this child state
								BegTempAddedBySuperStateArrayPtr[ii] += Diagram.GetTransAddedBySuperState(ii, kk) + "   ";
							}
						}
                    }
                }
            } // END FOR3
        } // END FOR2
    } // END FOR1

    // FOR1 All super states in this diagram
    for(ii = 0; ii < TempNumSuperStates; ii++)
    {
        // Store the number of trans for this state in a temporary
        TempNumTrans = Diagram.GetNumTrans(SUPER_STATE, ii);
        // FOR2 All transitions for this super state
        for(jj = 0; jj < TempNumTrans; jj++)
        {
            // Store the cond and don't care masks for this trans in temporaries
            TempCondMask1 = Diagram.GetTransCondMask(SUPER_STATE, ii, jj);
            TempDontCareMask1 = Diagram.GetTransDontCareMask(SUPER_STATE, ii, jj);
            TempNextState1 = ConvertStateNumberToText(Diagram.GetTransTargetState(
                SUPER_STATE, ii, jj));

            // FOR3 All trans for this super state starting without duplicating
            // previous searches (e.g. for a state with 4 trans 1, 2, 3, 4 check
            // 1-2, 1-3, 1-4, then 2-3, 2-4, then 3-4)
            for(kk = (jj + 1); kk < TempNumTrans; kk++)
            {
                // Store the cond and don't care masks for this trans in temporaries
                TempCondMask2 = Diagram.GetTransCondMask(SUPER_STATE, ii, kk);
                TempDontCareMask2 = Diagram.GetTransDontCareMask(SUPER_STATE, ii, kk);
                TempNextState2 = ConvertStateNumberToText(Diagram.GetTransTargetState(
                    SUPER_STATE, ii, kk));

                TempPossibleNonMutExcFlag = FALSE;
                // IF1 Cond masks match
                if(TempCondMask1 == TempCondMask2)
                { // THEN1 Indicate possible non-mutually exclusive transitions
                    TempPossibleNonMutExcFlag = TRUE;
                }
                // ELSE1 IF2 The trans don't care masks don't at least contain
                // a common bit (a common bit is necessary for mutual exclusion)
                else if((TempDontCareMask1 & TempDontCareMask2) == 0)
                { // THEN2 Indicate possible non-mutually exclusive trans
                    TempPossibleNonMutExcFlag = TRUE;
                }
                else
                { // ELSE2 IF3 Non-mutually exclusive trans
                    if(0 == ((TempCondMask1 ^ TempCondMask2) & (TempDontCareMask1 & TempDontCareMask2)))
                    { // THEN3 Indicate possible non-mutually exclusive transitions
                        TempPossibleNonMutExcFlag = TRUE;
                    }
                }
                // IF4 There is a possible non-mutually exclusive situation
                if(TRUE == TempPossibleNonMutExcFlag)
                { // THEN4 IF5 Both trans going to the same state
                    if(TempNextState1 == TempNextState2)
                    { // THEN5 IF6 The trans action functions are different
                        TempTransActionCString1 =\
                            Diagram.GetTransActionFunctionBody(SUPER_STATE, ii, jj);
                        TempTransActionCString2 =\
                            Diagram.GetTransActionFunctionBody(SUPER_STATE, ii, kk);

                        // Strip off spaces and carriage returns before comparing
                        TempTransActionCString1.Remove(' ');
                        TempTransActionCString1.Remove('\n');
                        TempTransActionCString2.Remove(' ');
                        TempTransActionCString2.Remove('\n');

                        if(TempTransActionCString1 != TempTransActionCString2)
                        { // THEN6 Indicate non-mutually exclusive trans
                            BegTempSuperStateArrayPtr[ii] = TRUE;
                            NonMutuallyExclusiveSuperStateFlag = TRUE;
                        }
                    }
                    else
                    { // ELSE5 Indicate non-mutually exclusive trans
                        BegTempSuperStateArrayPtr[ii] = TRUE;
                        NonMutuallyExclusiveSuperStateFlag = TRUE;
                    }
                }
            } // END FOR3
        } // END FOR2
    } // END FOR1

    // IF3 There were non-mutually exclusive transitions
    if((TRUE == NonMutuallyExclusiveStateFlag) || (TRUE == NonMutuallyExclusiveSuperStateFlag) ||\
		(TRUE == NonMutuallyExclusiveStateAddedBysuperStateFlag))
    { // THEN3 Prepare to output "non-mutually exclusive" message
		TempCStringReturn = "The following states don't have mutually exclusive transitions.\n";
		TempCStringReturn += "Note:  Transitions to the same state with the same transition\n";
		TempCStringReturn += "action are ignored.\n";
		// IF4 Some were non-super states
        if(TRUE == NonMutuallyExclusiveStateFlag)
        { // THEN4 List the states
            TempCStringReturn += "States:\n";
            for(ii = 0; ii < TempMaxStates; ii++)
            {
                if(TRUE == BegTempStateArrayPtr[ii])
                {
                    TempCStringReturn += ConvertStateNumberToText(ii);
                    TempCStringReturn += "\n";
                }
            }
        }
        // IF5 Some were super states
        if(TRUE == NonMutuallyExclusiveSuperStateFlag)
        { // THEN5 List the super states
            TempCStringReturn += "\n";
            TempCStringReturn += "Super states:\n";
            for(ii = 0; ii < TempMaxStates; ii++)
            {
                if(TRUE == BegTempSuperStateArrayPtr[ii])
                {
                    TempCStringReturn += ConvertStateNumberToText(ii + SUPER_STATE_OFFSET);
                    TempCStringReturn += "\n";
                }
            }
        }
		// IF6 Some were non-super states due to super state additional trans
		// cond and don't care mask data
		if(TRUE == NonMutuallyExclusiveStateAddedBysuperStateFlag)
		{ // THEN6 List the sub states states
            TempCStringReturn += "\n";
            TempCStringReturn += "Sub states due to super state transitions:\n";
            for(ii = 0; ii < TempMaxStates; ii++)
            {
                if(TRUE == BegTempStateAddedBySuperStateArrayPtr[ii])
                {
                    TempCStringReturn += ConvertStateNumberToText(ii);
					TempCStringReturn += "  due to super state(s):  " + BegTempAddedBySuperStateArrayPtr[ii];
                    TempCStringReturn += "\n";
                }
            }
		}
    }
	return(TempCStringReturn);
}

//****************************************************************************
// Name: ProcessFunctionBlock
//
// This stores all lines in a CString until one of the passed the
// EndcompareCStringXs strings is reached, the end of the file is reached,
// or a state or super state name is reached.  A state or super state name
// would be reached for a state that has an entry or exit action but has
// no transistions out.  This returns a CString containing the function
// block.
//****************************************************************************
CString ProcessFunctionBlock(CString EndCompareCString1, CString EndCompareCString2,
                             CString EndCompareCString3, CString EndCompareCString4)
{
    int ii, ProcessingFunction = TRUE, TempStateNumber;
    CString TempCString, FunctionCString = "", TempCString2;
    CString TempCString3, TempCString4;

    // WHILE1 Processing the (rest of the) function
    while(TRUE == ProcessingFunction)
    {
        ModelFile.StoreFileLine();
        TempCString = ModelFile.GetField(1);
		// Prepare to be able to check if a state name is encountered
		TempStateNumber = ConvertStateTextToNumber(TempCString);
        // IF1 Done with the current function (the first field compares with any
		// of the passed "end" strings is reached), the end of file is reached,
		// or a state name is encountered
        if((EndCompareCString1 == TempCString) || (EndCompareCString2 == TempCString) ||
           (EndCompareCString3 == TempCString) || (EndCompareCString3 == TempCString) ||\
		   (TRUE == ModelFile.EOFCheck()) || (CONVERT_ERROR != TempStateNumber))
        { // THEN1 Indicate done
            ProcessingFunction = FALSE;
        }
        else
        { // ELSE1 Make the possible state name upper case first
			TempCString.MakeUpper();

			// FOR1 All states (not super states)
			for(ii = 0; ii < Diagram.GetNumStates(NON_SUPER_STATE); ii++)
			{
				// IF2 A state name is encountered (indicating done with the current
				// function).  This is for states with entry or exit actions but have
				// no transitions out.
				if(ConvertStateNumberToText(ii) == TempCString)
				{ // THEN2 Indicate done and break out of the for loop
					ProcessingFunction = FALSE;
					break;
				}
			}
			
			// IF3 Still processing
			if(TRUE == ProcessingFunction)
			{ // THEN3 FOR2 All super states
				for(ii = 0; ii < Diagram.GetNumStates(SUPER_STATE); ii++)
				{
					// IF4 A state name is encountered (indicating done with the current
					// function).  This is for states with entry or exit actions but have
					// no transitions out.
					if(ConvertStateNumberToText(ii + SUPER_STATE_OFFSET) == TempCString)
					{ // THEN4 Indicate done and break out of the for loop
						ProcessingFunction = FALSE;
						break;
					}
				}
			}

			// IF5 Still processing
			if(TRUE == ProcessingFunction)
			{ // THEN5 Create/append to the FunctionCString
                TempCString3 = ModelFile.GetLine();
                // Store a copy
                TempCString4 = TempCString3;
                // Remove all spaces and new lines
                TempCString3.Remove(' ');
                TempCString3.Remove('\n');
                // IF6 The string has more than spaces and/or new lines
                if("" != TempCString3)
                { // THEN6 Store the string
                    FunctionCString += "   " + TempCString4;
                }
			}
        }
    }
    return(FunctionCString);
}

//****************************************************************************
// Name: CheckForTransitionErrors
//
// This checks for transition errors for states and super states (e.g.
// don't care value not found in configuration file, condition value not
// found in the configuration file).  This returns a NULL string if no
// errors are found.  Otherwise, this returns up to 50 errors (limited to
// 50 so it won't scroll off the screen).
//****************************************************************************
CString CheckForTransitionErrors(void)
{
	int jj, kk, NumErrors = 0;
	CString CurrentState, CurrentTargetState, TransErrorCString = "";

	// Check non-super state transitions
	// FOR1 All states
	for(jj = 0; jj < Diagram.GetNumStates(NON_SUPER_STATE); jj++)
	{
		CurrentState = ConvertStateNumberToText(jj);
		// FOR2 All transitions
		for(kk = 0; kk < Diagram.GetNumTrans(NON_SUPER_STATE, jj); kk++)
		{
			// Convert the target state number to text
			CurrentTargetState = ConvertStateNumberToText(Diagram.GetTransTargetState(NON_SUPER_STATE, jj, kk));
			// Limit the trans error string to 50 errors so it won't
			// extend past the bottom of the screen and can't be closed
			if(NumErrors < 50)
			{ // IF4 The current trans has a problem
				if("OK" != Diagram.GetTransStatus(NON_SUPER_STATE, jj, kk))
				{ // THEN4 Store the error (will be displayed at the end)
					TransErrorCString += ConvertStateNumberToText(jj);
					TransErrorCString += " to ";
					TransErrorCString += CurrentTargetState;
					TransErrorCString += Diagram.GetTransStatus(NON_SUPER_STATE, jj, kk);
					TransErrorCString += "\n";
					NumErrors++;
				}
			}
		} // END FOR2
	} // END FOR1
		
		// Check super state transitions
	// FOR3 All super states
	for(jj = 0; jj < Diagram.GetNumStates(SUPER_STATE); jj++)
	{
		CurrentState = ConvertStateNumberToText(jj);
		// FOR4 All transitions
		for(kk = 0; kk < Diagram.GetNumTrans(SUPER_STATE, jj); kk++)
		{
			// Convert the target state number to text
			CurrentTargetState = ConvertStateNumberToText(Diagram.GetTransTargetState(SUPER_STATE, jj, kk));
			// Limit the trans error string to 50 errors so it won't
			// extend past the bottom of the screen and can't be closed
			if(NumErrors < 50)
			{ // IF8 The current trans has a problem
				if("OK" != Diagram.GetTransStatus(SUPER_STATE, jj, kk))
				{ // THEN8 Store the error (will be displayed at the end)
					TransErrorCString += ConvertStateNumberToText(jj + SUPER_STATE_OFFSET);
					TransErrorCString += " to ";
					TransErrorCString += CurrentTargetState;
					TransErrorCString += Diagram.GetTransStatus(SUPER_STATE, jj, kk);
					TransErrorCString += "\n";
					NumErrors++;
				}
			}
		} // END FOR4
	} // END FOR3
	return(TransErrorCString);
}

//****************************************************************************
// Name: UpdateTransWithAddlCondDontCare
//
// This updates state transitions with the additional transition condition
// mask and don't care mask values with the information in the configuration
// file.
//****************************************************************************
void UpdateTransWithAddlCondDontCareMasks(void)
{
	int ii, jj, kk, CurrentStateNumber;
	CString CurrentStateName;
	
	// Process all (non-super state) sub states in the super state extended lists
	// FOR1 All super states
	for(ii = 0; ii < Diagram.GetNumStates(SUPER_STATE); ii++)
	{
		// FOR2 All non-super state sub states
		for(jj = 0; jj < Diagram.GetSuperStateExtendedListNumSubStates(ii); jj++)
		{
			// Get the current sub state number
			CurrentStateName = Diagram.GetSuperStateExtendedListSubStateName(ii, jj);
			CurrentStateNumber = ConvertStateTextToNumber(CurrentStateName);
			
			// FOR3 All transitions for this state
			for(kk = 0; kk < Diagram.GetNumTrans(NON_SUPER_STATE, CurrentStateNumber); kk++)
			{
				// Update all the condition and don't care masks for all
				// transitions for this state
				Diagram.UpdateTransCondDontCareMasks(NON_SUPER_STATE, CurrentStateNumber, kk,\
					Diagram.GetSuperStateAdditionalCondMask(ii),\
					Diagram.GetSuperStateAdditionalDontCareMask(ii));
			} // END FOR3
		} // END FOR2
	} // END FOR1

	// Process all super states
	// FOR4 All super states
	for(ii = 0; ii < Diagram.GetNumStates(SUPER_STATE); ii++)
	{
		// FOR5 All sub states
		for(jj = 0; jj < Diagram.GetSuperStateListNumSubStates(ii); jj++)
		{
			// Get the current sub state number
			CurrentStateName = Diagram.GetSuperStateListSubStateName(ii, jj);
			CurrentStateNumber = ConvertStateTextToNumber(CurrentStateName);
			// IF1 The current sub state is a current super state
			if(CurrentStateNumber >= SUPER_STATE_OFFSET)
			{ // THEN1 Update all transitions for this super state
				// FOR6 All transitions for this state
				for(kk = 0; kk < Diagram.GetNumTrans(SUPER_STATE, (CurrentStateNumber - SUPER_STATE_OFFSET)); kk++)
				{
					// Update all the condition and don't care masks for all
					// transitions for this state
					Diagram.UpdateTransCondDontCareMasks(SUPER_STATE, (CurrentStateNumber - SUPER_STATE_OFFSET), kk,\
						Diagram.GetSuperStateAdditionalCondMask(ii),\
						Diagram.GetSuperStateAdditionalDontCareMask(ii));
				} // END FOR6
			}
		} // END FOR5
	} // END FOR4
}

//****************************************************************************
// Name: AddTransFromSuperStates
//
// This adds super state transitions to each sub state in the sub state
// transition list in the Diagram object.
//****************************************************************************
void AddTransFromSuperStates(void)
{
	int jj, ll, mm, nn;
	CString CurrentState;
    Trans_t TempTransConfig;

	// FOR1 All states
	for(jj = 0; jj < Diagram.GetNumStates(NON_SUPER_STATE); jj++)
	{                    
		CurrentState = ConvertStateNumberToText(jj);

		// FOR2 All super state list states
		for(ll = 0; ll < Diagram.GetNumStates(SUPER_STATE); ll++)
		{
			// FOR3 All super state extended list sub states
			for(mm = 0; mm < Diagram.GetSuperStateExtendedListNumSubStates(ll); mm++)
			{
				// IF1 This state is a sub state of the current super state
				if(CurrentState == Diagram.GetSuperStateExtendedListSubStateName(ll, mm))
				{ // THEN1 Output all of the transitions (in C file format)
					// At this point, ll is the super state number
					// FOR4 All transitions for the current super state
					for(nn = 0; nn < Diagram.GetNumTrans(SUPER_STATE, ll); nn++)
					{
						// IF2 The current trans is ok
						if("OK" == Diagram.GetTransStatus(SUPER_STATE, ll, nn))
						{ // THEN2 Prepare to add the current trans to the current state
							TempTransConfig.TransStatus = "OK";
							TempTransConfig.TargetState = Diagram.GetTransTargetState(SUPER_STATE, ll, nn);
							TempTransConfig.TransCondMask = Diagram.GetTransCondMask(SUPER_STATE, ll, nn);
							TempTransConfig.TransDontCareMask = Diagram.GetTransDontCareMask(SUPER_STATE, ll, nn);
							TempTransConfig.AddedBySuperState = ConvertStateNumberToText(SUPER_STATE_OFFSET + ll);
                            TempTransConfig.TransActionFunctionName = Diagram.GetTransActionFunctionName(SUPER_STATE, ll, nn);
                            TempTransConfig.TransActionFunctionBody = Diagram.GetTransActionFunctionBody(SUPER_STATE, ll, nn);
							// Add the current trans to the current state
							Diagram.AddTrans(NON_SUPER_STATE, jj, &TempTransConfig);
                        }
					} // END FOR4
				}
			} // END FOR3
		} // END FOR2
	} // END FOR1
}

//****************************************************************************
// Name:  CheckPowerupTransitionTarget
//
// This checks to see if the power-up transition target is a super state.
// If it is, then this replaces the super state name with the appropriate
// sub state.
//
// This is designed to be called after the ProcessStateSuperStatesTargets call
// and expects to have the status from the ProcessStateSuperStatesTargets call
// passed to this routine.  That way, if the power up transition target can't
// be found because a super state or nested super state that is pointed to by
// the powerup transition is missing a default transition, then this routine
// can add the super state name that is missing a default transition to the
// return status (if it is not already in the list due to another transition
// into that super state).
//****************************************************************************
CString CheckPowerupTransitionTarget(CString PassedStateSuperStatesTargetsStatus)
{
	int TempInitialStateNumber;
	CString ReturnStatus, TempReturnStatus;
	GetNestedDefaultStateReturnStatus_t GetNestedDefaultStateReturnStatus;

	// Init the initial state number to what is stored in the Diagram object
	TempInitialStateNumber = ConvertStateTextToNumber(Diagram.GetInitialStateName());
	// Init the return status to what was passed
	ReturnStatus = PassedStateSuperStatesTargetsStatus;

	//IF1 The initial state is a super state
	if(TempInitialStateNumber >= SUPER_STATE_OFFSET)
	{ // THEN1 Drill down and find the actual initial state inside the super state or inside nested super states
		GetNestedDefaultStateReturnStatus = GetNestedDefaultState(TempInitialStateNumber - SUPER_STATE_OFFSET);
		
		// IF2 The "last" super state in the nested set of super states is not missing a default transition
		if(CONVERT_ERROR != GetNestedDefaultStateReturnStatus.NextStateNumber)
		{ // THEN2 Change the initial state name from a super state to a state
			Diagram.AddInitialStateName(ConvertStateNumberToText(GetNestedDefaultStateReturnStatus.NextStateNumber));
		}
		else
		{ // ELSE2 IF3 The passed return status from the state super state target status check is ok
			if("OK" == ReturnStatus)
			{ // THEN3 Store the super state that is missing a default state in the return status
				ReturnStatus = "The following super states have transitions in\n";
				ReturnStatus += "but don't have default transitions:\n";
				ReturnStatus += GetNestedDefaultStateReturnStatus.Status + "\n";
			}
			else
			{ // ELSE3 Check to see if it is already in the list
				TempReturnStatus = GetNestedDefaultStateReturnStatus.Status + "\n";
				// IF4 The super state is not already listed
				if(-1 == ReturnStatus.Find(TempReturnStatus))
				{ // THEN4 Add it to the list
					ReturnStatus += TempReturnStatus;
				}
			}
		}
	}
	return(ReturnStatus);
}

//****************************************************************************
// Name:  GetNestedDefaultState
//
// This returns the default state number for the passed super state number.
// This is a recursive function to take care of nested super states.
//
// This returns a structure that contains a CString status and non-super state
// number.  If the passed super state or nested super states are missing a
// default transition then this returns CONVERT_ERROR for the non-super state
// number of the return structure and returns the name of the offending super
// state in the status of the return structure.
//****************************************************************************
GetNestedDefaultStateReturnStatus_t GetNestedDefaultState(int SuperStateNumber)
{
	CString TempNextStateText;
	int TempNextStateNumber;
	GetNestedDefaultStateReturnStatus_t ReturnStatus;

	ReturnStatus.Status = ConvertStateNumberToText(SuperStateNumber + SUPER_STATE_OFFSET);
	ReturnStatus.NextStateNumber = CONVERT_ERROR;

	// Get the default target state for the passed super state
	TempNextStateText = Diagram.GetDefaultTransTargetState(SuperStateNumber);
	
	// IF1 The passed super state has a default transition target state
	if("" != TempNextStateText)
	{ // THEN1 Convert the default target state name to text
		TempNextStateNumber = ConvertStateTextToNumber(TempNextStateText);
		ReturnStatus.NextStateNumber = TempNextStateNumber;
		ReturnStatus.Status = "OK";
		
		// IF2 The default target state is a super state
		if((CONVERT_ERROR != TempNextStateNumber) && (TempNextStateNumber >= SUPER_STATE_OFFSET))
		{ // THEN2 Call itself to drill down until there is no more nesting
			ReturnStatus = GetNestedDefaultState(TempNextStateNumber - SUPER_STATE_OFFSET);
		}
	}
	// Return the substate number
	return(ReturnStatus);
}
