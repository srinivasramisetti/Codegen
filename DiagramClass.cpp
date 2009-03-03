#include "stdafx.h"
#include "DiagramClass.h"
#include "StateClass.h"
#include "SuperStateClass.h"
#include "ModelProcessing.h"
#include "Globals.h"

//****************************************************************************
// Class name: DiagramClass
//
// Description:	 This is the main class for FSM diagrams.  This is a data
// container class for FSM diagram information.  This maintains various
// pieces of FSM diagram information including linked lists of various pieces
// of FSM diagram information including linked lists of StateClass and
// SuperStateClass objects.
//
// This also serves as an interface class to StateClass and SuperStateClass
// objects from an interface function point of view.
//****************************************************************************
DiagramClass::DiagramClass(void)
{ // Init the powerup transistion action to NULL
    NumStatesInConfigFile = 0;
    HeadStateNumberToTextArrayPtr = 0;
    TailStateNumberToTextArrayPtr = 0;

    NumSuperStatesInConfigFile = 0;
    HeadSuperStateNumberToTextArrayPtr = 0;
    TailSuperStateNumberToTextArrayPtr = 0;
    
    NumSuperStateAdditionalCondMasks = 0;
    HeadSuperStateAdditionalCondMaskArrayPtr = 0;
    TailSuperStateAdditionalCondMaskArrayPtr = 0;

    NumSuperStateAdditionalDontCareMasks = 0;
    HeadSuperStateAdditionalDontCareMaskArrayPtr = 0;
    TailSuperStateAdditionalDontCareMaskArrayPtr = 0;

    PowerupTransAction = "";
    InitialStateName = "";
    NumTransActions = 1;

    NumCondBits = 0;
    HeadCondBitPtr = 0;
    TailCondBitPtr = 0;

    NumDontCareBits = 0;
    HeadDontCareBitPtr = 0;
    TailDontCareBitPtr = 0;

    NumStates = 0;
    HeadStatePtr = 0;
    TailStatePtr = 0;

    NumSuperStates = 0;
    HeadSuperStatePtr = 0;
    TailSuperStatePtr = 0;
}

void DiagramClass::AddPowerupTransAction(CString NewPowerupTransAction)
{
    PowerupTransAction = NewPowerupTransAction;
}

CString DiagramClass::GetPowerupTransAction(void)
{
    return(PowerupTransAction);
}

void DiagramClass::AddInitialStateName(CString NewInitialStateName)
{
    InitialStateName = NewInitialStateName;
}

CString DiagramClass::GetInitialStateName(void)
{
    return(InitialStateName);
}

void DiagramClass::AddCondBit(CondBit_t *CondBitPtr)
{
	// Dynamically create a transition linked list element
	CondBit_t *TempLinkedListPtr = new CondBit_t;

	// IF1 This is the first one
	if(0 == NumCondBits)
	{ // THEN1 Set the tail pointer, clear the next element pointer, and
		// set the heat pointer
		TailCondBitPtr = TempLinkedListPtr;
		TailCondBitPtr->NextElementPtr = 0;
		HeadCondBitPtr = TailCondBitPtr;
	}
	else
	{ // ELSE1 Link the new linked list element to the end and set the tail
		// pointer to the end of the linked list
		TailCondBitPtr->NextElementPtr = TempLinkedListPtr;
		TailCondBitPtr = TempLinkedListPtr;
	}

    TailCondBitPtr->CondBit = CondBitPtr->CondBit;
    TailCondBitPtr->CondBitText = CondBitPtr->CondBitText;

    NumCondBits++;
}

CondBit_t *DiagramClass::FindCondBitElementPtr(int CondBitNumber)
{
	CondBit_t *TempLinkedListPtr = HeadCondBitPtr;
	int ii;
	
    for(ii = 0; ii < NumCondBits; ii++,\
        TempLinkedListPtr = TempLinkedListPtr->NextElementPtr)
	{
        if(ii == CondBitNumber)
        {
            break;
        }
	}
	return(TempLinkedListPtr);
}

int DiagramClass::GetNumCondBits(void)
{
    return(NumCondBits);
}

int DiagramClass::GetCondBit(int CondBitNumber)
{
    return((FindCondBitElementPtr(CondBitNumber))->CondBit);
}

CString DiagramClass::GetCondBitText(int CondBitNumber)
{
    return((FindCondBitElementPtr(CondBitNumber))->CondBitText);
}

void DiagramClass::AddDontCareBit(DontCareBit_t *DontCareBitPtr)
{
	// Dynamically create a transition linked list element
	DontCareBit_t *TempLinkedListPtr = new DontCareBit_t;

	// IF1 This is the first one
	if(0 == NumDontCareBits)
	{ // THEN1 Set the tail pointer, clear the next element pointer, and
		// set the heat pointer
		TailDontCareBitPtr = TempLinkedListPtr;
		TailDontCareBitPtr->NextElementPtr = 0;
		HeadDontCareBitPtr = TailDontCareBitPtr;
	}
	else
	{ // ELSE1 Link the new linked list element to the end and set the tail
		// pointer to the end of the linked list
		TailDontCareBitPtr->NextElementPtr = TempLinkedListPtr;
		TailDontCareBitPtr = TempLinkedListPtr;
	}

    TailDontCareBitPtr->DontCareBit = DontCareBitPtr->DontCareBit;
    TailDontCareBitPtr->DontCareBitText = DontCareBitPtr->DontCareBitText;

    NumDontCareBits++;
}

DontCareBit_t *DiagramClass::FindDontCareBitElementPtr(int DontCareBitNumber)
{
	DontCareBit_t *TempLinkedListPtr = HeadDontCareBitPtr;
	int ii;
	
    for(ii = 0; ii < NumDontCareBits; ii++,\
        TempLinkedListPtr = TempLinkedListPtr->NextElementPtr)
	{
        if(ii == DontCareBitNumber)
        {
            break;
        }
	}
	return(TempLinkedListPtr);
}

int DiagramClass::GetNumDontCareBits(void)
{
    return(NumDontCareBits);
}

int DiagramClass::GetDontCareBit(int DontCareBitNumber)
{
    return((FindDontCareBitElementPtr(DontCareBitNumber))->DontCareBit);
}

CString DiagramClass::GetDontCareBitText(int DontCareBitNumber)
{
    return((FindDontCareBitElementPtr(DontCareBitNumber))->DontCareBitText);
}

void DiagramClass::AddState(State_t *StatePtr)
{
	// Dynamically create a transition linked list element
	StateClass *TempLinkedListPtr = new StateClass(StatePtr);

	// IF1 This is the first one
	if(0 == NumStates)
	{ // THEN1 Set the tail pointer, clear the next element pointer, and
		// set the heat pointer
		TailStatePtr = TempLinkedListPtr;
		TailStatePtr->SetNextElementPtr(0);
		HeadStatePtr = TailStatePtr;
	}
	else
	{ // ELSE1 Link the new linked list element to the end and set the tail
		// pointer to the end of the linked list
		TailStatePtr->SetNextElementPtr(TempLinkedListPtr);
		TailStatePtr = TempLinkedListPtr;
	}

    NumStates++;
}

StateClass *DiagramClass::FindStateElementPtr(int StateNumber)
{
	StateClass *TempLinkedListPtr = HeadStatePtr;
	int ii;
	
    for(ii = 0; ii < NumStates; ii++,\
        TempLinkedListPtr = TempLinkedListPtr->GetNextElementPtr())
	{
        if(StateNumber == TempLinkedListPtr->GetStateNumber())
        {
            break;
        }
	}
    // IF1 The passed state number was not found
    if(ii == NumStates)
    { // THEN1 Return an error
        return(reinterpret_cast<StateClass *>(CONVERT_ERROR));
    }
    else
    { // ELSE1 Return the linked list state pointer
        return(TempLinkedListPtr);
    }
}

CString DiagramClass::GetEntryAction(int StateNumber)
{
    return((FindStateElementPtr(StateNumber))->GetEntryAction());
}

CString DiagramClass::GetActivityFunction(int StateNumber)
{
    return((FindStateElementPtr(StateNumber))->GetActivityFunction());
}

CString DiagramClass::GetExitAction(int StateNumber)
{
    return((FindStateElementPtr(StateNumber))->GetExitAction());
}

void DiagramClass::AddSuperState(SuperState_t *SuperStatePtr)
{
	// Dynamically create a transition linked list element
	SuperStateClass *TempLinkedListPtr = new SuperStateClass(SuperStatePtr);

	// IF1 This is the first one
	if(0 == NumSuperStates)
	{ // THEN1 Set the tail pointer, clear the next element pointer, and
		// set the heat pointer
		TailSuperStatePtr = TempLinkedListPtr;
		TailSuperStatePtr->SetNextElementPtr(0);
		HeadSuperStatePtr = TailSuperStatePtr;
	}
	else
	{ // ELSE1 Link the new linked list element to the end and set the tail
		// pointer to the end of the linked list
		TailSuperStatePtr->SetNextElementPtr(TempLinkedListPtr);
		TailSuperStatePtr = TempLinkedListPtr;
	}

    NumSuperStates++;
}

SuperStateClass *DiagramClass::FindSuperStateElementPtr(int SuperStateNumber)
{
	SuperStateClass *TempLinkedListPtr = HeadSuperStatePtr;
	int ii;
	
    for(ii = 0; ii < NumSuperStates; ii++,\
        TempLinkedListPtr = TempLinkedListPtr->GetNextElementPtr())
	{
        if(SuperStateNumber == TempLinkedListPtr->GetSuperStateNumber())
        {
            break;
        }
	}
    // IF1 The passed super state number was not found
    if(ii == NumSuperStates)
    { // THEN1 Return an error
        return(reinterpret_cast<SuperStateClass *>(CONVERT_ERROR));
    }
    else
    { // ELSE1 Return the linked list state pointer
        return(TempLinkedListPtr);
    }
}

void DiagramClass::AddDefaultTransTargetState(int SuperStateNumber, CString NewDefaultTransTargetState)
{
    (FindSuperStateElementPtr(SuperStateNumber))->AddDefaultTransTargetState(NewDefaultTransTargetState);
}

CString DiagramClass::GetDefaultTransTargetState(int SuperStateNumber)
{
    return((FindSuperStateElementPtr(SuperStateNumber))->GetDefaultTransTargetState());
}

int DiagramClass::GetNumStates(int SuperStateFlag)
{
    if(SUPER_STATE == SuperStateFlag)
    {
        return(NumSuperStates);
    }
    else
    {
        return(NumStates);
    }
}

CString DiagramClass::GetStateName(int SuperStateFlag, int StateNumber)
{
	SuperStateClass *TempSuperStateLinkedListPtr;
	StateClass *TempStateLinkedListPtr;

    if(SUPER_STATE == SuperStateFlag)
    {
        TempSuperStateLinkedListPtr = FindSuperStateElementPtr(StateNumber);
        // IF1 The passed super state number was not found
        if(reinterpret_cast<SuperStateClass *>(CONVERT_ERROR) == TempSuperStateLinkedListPtr)
        { // THEN1 Return an error
            return("CONVERT_ERROR");
        }
        else
        { // ELSE1 Return the data
            return(TempSuperStateLinkedListPtr->GetSuperStateName());
        }
    }
    else
    {
        TempStateLinkedListPtr = FindStateElementPtr(StateNumber);
        // IF2 The passed state number was not found
        if(reinterpret_cast<StateClass *>(CONVERT_ERROR) == TempStateLinkedListPtr)
        { // THEN2 Return an error
            return("CONVERT_ERROR");
        }
        else
        { // ELSE2 Return the data
            return(TempStateLinkedListPtr->GetStateName());
        }
    }
}

void DiagramClass::UpdateTargetState(int SuperStateFlag, int StateNumber, int TransNumber, int UpdatedNextState)
{
    if(SUPER_STATE == SuperStateFlag)
    {
        (FindSuperStateElementPtr(StateNumber))->UpdateTargetState(TransNumber, UpdatedNextState);
    }
    else
    {
        (FindStateElementPtr(StateNumber))->UpdateTargetState(TransNumber, UpdatedNextState);
    }
}

void DiagramClass::UpdateTransCondDontCareMasks(int SuperStateFlag, int StateNumber, int TransNumber, int NewCondMask, int NewDontCareMask)
{
    if(SUPER_STATE == SuperStateFlag)
    {
        (FindSuperStateElementPtr(StateNumber))->UpdateTransCondDontCareMasks(TransNumber, NewCondMask, NewDontCareMask);
    }
    else
    {
        (FindStateElementPtr(StateNumber))->UpdateTransCondDontCareMasks(TransNumber, NewCondMask, NewDontCareMask);
    }
}

void DiagramClass::AddTrans(int SuperStateFlag, int StateNumber, Trans_t *TransPtr)
{
    if(SUPER_STATE == SuperStateFlag)
    {
        (FindSuperStateElementPtr(StateNumber))->AddTrans(TransPtr);
    }
    else
    {
        (FindStateElementPtr(StateNumber))->AddTrans(TransPtr);
    }
}

int DiagramClass::GetNumTrans(int SuperStateFlag, int StateNumber)
{
    if(SUPER_STATE == SuperStateFlag)
    {
        return((FindSuperStateElementPtr(StateNumber))->GetNumTrans());
    }
    else
    {
        return((FindStateElementPtr(StateNumber))->GetNumTrans());
    }
}

CString DiagramClass::GetTransActionFunctionName(int SuperStateFlag, int StateNumber, int TransNumber)
{
    if(SUPER_STATE == SuperStateFlag)
    {
        return((FindSuperStateElementPtr(StateNumber))->GetTransActionFunctionName(TransNumber));
    }
    else
    {
        return((FindStateElementPtr(StateNumber))->GetTransActionFunctionName(TransNumber));
    }
}

CString DiagramClass::GetTransActionFunctionBody(int SuperStateFlag, int StateNumber, int TransNumber)
{
    if(SUPER_STATE == SuperStateFlag)
    {
        return((FindSuperStateElementPtr(StateNumber))->GetTransActionFunctionBody(TransNumber));
    }
    else
    {
        return((FindStateElementPtr(StateNumber))->GetTransActionFunctionBody(TransNumber));
    }
}

int DiagramClass::GetTransTargetState(int SuperStateFlag, int StateNumber, int TransNumber)
{
    if(SUPER_STATE == SuperStateFlag)
    {
        return((FindSuperStateElementPtr(StateNumber))->GetTransTargetState(TransNumber));
    }
    else
    {
        return((FindStateElementPtr(StateNumber))->GetTransTargetState(TransNumber));
    }
}

int DiagramClass::GetTransCondMask(int SuperStateFlag, int StateNumber, int TransNumber)
{
    if(SUPER_STATE == SuperStateFlag)
    {
        return((FindSuperStateElementPtr(StateNumber))->GetTransCondMask(TransNumber));
    }
    else
    {
        return((FindStateElementPtr(StateNumber))->GetTransCondMask(TransNumber));
    }
}

int DiagramClass::GetTransDontCareMask(int SuperStateFlag, int StateNumber, int TransNumber)
{
    if(SUPER_STATE == SuperStateFlag)
    {
        return((FindSuperStateElementPtr(StateNumber))->GetTransDontCareMask(TransNumber));
    }
    else
    {
        return((FindStateElementPtr(StateNumber))->GetTransDontCareMask(TransNumber));
    }
}

CString DiagramClass::GetTransAddedBySuperState(int StateNumber, int TransNumber)
{
    // Only meant for non-super states
    return((FindStateElementPtr(StateNumber))->GetTransAddedBySuperState(TransNumber));
}

CString DiagramClass::GetTransStatus(int SuperStateFlag, int StateNumber, int TransNumber)
{
    if(SUPER_STATE == SuperStateFlag)
    {
        return((FindSuperStateElementPtr(StateNumber))->GetTransStatus(TransNumber));
    }
    else
    {
        return((FindStateElementPtr(StateNumber))->GetTransStatus(TransNumber));
    }
}

void DiagramClass::AddSuperStateListSubState(int SuperStateNumber, ListSubState_t *NewListSubStateData)
{
    (FindSuperStateElementPtr(SuperStateNumber))->AddSuperStateListSubState(NewListSubStateData);
}

int DiagramClass::GetSuperStateListNumSubStates(int SuperStateNumber)
{
    return((FindSuperStateElementPtr(SuperStateNumber))->GetNumSuperStateListSubStates());
}

CString DiagramClass::GetSuperStateListSubStateName(int SuperStateNumber, int SubStateNumber)
{
    return((FindSuperStateElementPtr(SuperStateNumber))->GetSuperStateListSubStateName(SubStateNumber));
}

void DiagramClass::RemoveLastSuperStateListSubStateName(int SuperStateNumber)
{
    (FindSuperStateElementPtr(SuperStateNumber))->RemoveLastSuperStateListSubStateName();
}

void DiagramClass::AddSuperStateExtendedListSubState(int SuperStateNumber, ListSubState_t *NewListSubStateData)
{
    (FindSuperStateElementPtr(SuperStateNumber))->AddSuperStateExtendedListSubState(NewListSubStateData);
}

int DiagramClass::GetSuperStateExtendedListNumSubStates(int SuperStateNumber)
{
    return((FindSuperStateElementPtr(SuperStateNumber))->GetSuperStateExtendedListNumSubStates());
}

CString DiagramClass::GetSuperStateExtendedListSubStateName(int SuperStateNumber, int SubStateNumber)
{
    return((FindSuperStateElementPtr(SuperStateNumber))->GetSuperStateExtendedListSubStateName(SubStateNumber));
}

void DiagramClass::AddStateNumberToTextElement(CString ElementText)
{
	// Dynamically create a linked list element
	StateNumberToText_t *TempLinkedListPtr = new StateNumberToText_t;

	// IF1 This is the first one
	if(0 == NumStatesInConfigFile)
	{ // THEN1 Set the tail pointer, clear the next element pointer, and
		// set the heat pointer
		TailStateNumberToTextArrayPtr = TempLinkedListPtr;
		TailStateNumberToTextArrayPtr->NextElementPtr = 0;
		HeadStateNumberToTextArrayPtr = TailStateNumberToTextArrayPtr;
	}
	else
	{ // ELSE1 Link the new linked list element to the end and set the tail
		// pointer to the end of the linked list
		TailStateNumberToTextArrayPtr->NextElementPtr = TempLinkedListPtr;
		TailStateNumberToTextArrayPtr = TempLinkedListPtr;
	}

    TailStateNumberToTextArrayPtr->Text = ElementText;

    NumStatesInConfigFile++;
}

int DiagramClass::GetNumStatesInConfigFile(void)
{
    return(NumStatesInConfigFile);
}

CString DiagramClass::GetStateNumberToTextElement(int ElementNumber)
{
    CString ReturnText = "CONVERT_ERROR";
    StateNumberToText_t *TempLinkedListPtr = HeadStateNumberToTextArrayPtr;
    int ii;
    
    if((ElementNumber >= 0) && (ElementNumber < NumStatesInConfigFile))
    {
        for(ii = 0; (ii < NumStatesInConfigFile); ii++,\
            TempLinkedListPtr = TempLinkedListPtr->NextElementPtr)
        {
            if(ii == ElementNumber)
            {
                ReturnText = TempLinkedListPtr->Text;
                break;
            }
        }
    }
    return(ReturnText);
}

void DiagramClass::AddSuperStateNumberToTextElement(CString ElementText)
{
	// Dynamically create a linked list element
	StateNumberToText_t *TempLinkedListPtr = new StateNumberToText_t;

	// IF1 This is the first one
	if(0 == NumSuperStatesInConfigFile)
	{ // THEN1 Set the tail pointer, clear the next element pointer, and
		// set the heat pointer
		TailSuperStateNumberToTextArrayPtr = TempLinkedListPtr;
		TailSuperStateNumberToTextArrayPtr->NextElementPtr = 0;
		HeadSuperStateNumberToTextArrayPtr = TailSuperStateNumberToTextArrayPtr;
	}
	else
	{ // ELSE1 Link the new linked list element to the end and set the tail
		// pointer to the end of the linked list
		TailSuperStateNumberToTextArrayPtr->NextElementPtr = TempLinkedListPtr;
		TailSuperStateNumberToTextArrayPtr = TempLinkedListPtr;
	}

    TailSuperStateNumberToTextArrayPtr->Text = ElementText;

    NumSuperStatesInConfigFile++;
}

int DiagramClass::GetNumSuperStatesInConfigFile(void)
{
    return(NumSuperStatesInConfigFile);
}

CString DiagramClass::GetSuperStateNumberToTextElement(int ElementNumber)
{
    CString ReturnText = "CONVERT_ERROR";
    StateNumberToText_t *TempLinkedListPtr = HeadSuperStateNumberToTextArrayPtr;
    int ii;
    
    if((ElementNumber >= 0) && (ElementNumber < NumSuperStatesInConfigFile))
    {
        for(ii = 0; (ii < NumSuperStatesInConfigFile); ii++,\
            TempLinkedListPtr = TempLinkedListPtr->NextElementPtr)
        {
            if(ii == ElementNumber)
            {
                ReturnText = TempLinkedListPtr->Text;
                break;
            }
        }
    }
    return(ReturnText);
}

void DiagramClass::AddSuperStateAdditionalCondMask(int Mask)
{
	// Dynamically create a linked list element
	SuperStateAdditionalMask_t *TempLinkedListPtr = new SuperStateAdditionalMask_t;

	// IF1 This is the first one
	if(0 == NumSuperStateAdditionalCondMasks)
	{ // THEN1 Set the tail pointer, clear the next element pointer, and
		// set the heat pointer
		TailSuperStateAdditionalCondMaskArrayPtr = TempLinkedListPtr;
		TailSuperStateAdditionalCondMaskArrayPtr->NextElementPtr = 0;
		HeadSuperStateAdditionalCondMaskArrayPtr = TailSuperStateAdditionalCondMaskArrayPtr;
	}
	else
	{ // ELSE1 Link the new linked list element to the end and set the tail
		// pointer to the end of the linked list
		TailSuperStateAdditionalCondMaskArrayPtr->NextElementPtr = TempLinkedListPtr;
		TailSuperStateAdditionalCondMaskArrayPtr = TempLinkedListPtr;
	}

    TailSuperStateAdditionalCondMaskArrayPtr->Mask = Mask;

    NumSuperStateAdditionalCondMasks++;
}

int DiagramClass::GetNumSuperStateAdditionalCondMasks(void)
{
    return(NumSuperStateAdditionalCondMasks);
}

int DiagramClass::GetSuperStateAdditionalCondMask(int ElementNumber)
{
    SuperStateAdditionalMask_t *TempLinkedListPtr = HeadSuperStateAdditionalCondMaskArrayPtr;
    int ii, ReturnValue = CONVERT_ERROR;
    
    if((ElementNumber >= 0) && (ElementNumber < NumSuperStateAdditionalCondMasks))
    {
        for(ii = 0; (ii < NumSuperStateAdditionalCondMasks); ii++,\
            TempLinkedListPtr = TempLinkedListPtr->NextElementPtr)
        {
            if(ii == ElementNumber)
            {
                ReturnValue = TempLinkedListPtr->Mask;
                break;
            }
        }
    }
    return(ReturnValue);
}

void DiagramClass::AddSuperStateAdditionalDontCareMask(int Mask)
{
	// Dynamically create a linked list element
	SuperStateAdditionalMask_t *TempLinkedListPtr = new SuperStateAdditionalMask_t;

	// IF1 This is the first one
	if(0 == NumSuperStateAdditionalDontCareMasks)
	{ // THEN1 Set the tail pointer, clear the next element pointer, and
		// set the heat pointer
		TailSuperStateAdditionalDontCareMaskArrayPtr = TempLinkedListPtr;
		TailSuperStateAdditionalDontCareMaskArrayPtr->NextElementPtr = 0;
		HeadSuperStateAdditionalDontCareMaskArrayPtr = TailSuperStateAdditionalDontCareMaskArrayPtr;
	}
	else
	{ // ELSE1 Link the new linked list element to the end and set the tail
		// pointer to the end of the linked list
		TailSuperStateAdditionalDontCareMaskArrayPtr->NextElementPtr = TempLinkedListPtr;
		TailSuperStateAdditionalDontCareMaskArrayPtr = TempLinkedListPtr;
	}

    TailSuperStateAdditionalDontCareMaskArrayPtr->Mask = Mask;

    NumSuperStateAdditionalDontCareMasks++;
}

int DiagramClass::GetNumSuperStateAdditionalDontCareMasks(void)
{
    return(NumSuperStateAdditionalDontCareMasks);
}

int DiagramClass::GetSuperStateAdditionalDontCareMask(int ElementNumber)
{
	SuperStateAdditionalMask_t *TempLinkedListPtr = HeadSuperStateAdditionalDontCareMaskArrayPtr;
    int ii, ReturnValue = CONVERT_ERROR;
    
    if((ElementNumber >= 0) && (ElementNumber < NumSuperStateAdditionalDontCareMasks))
    {
        for(ii = 0; (ii < NumSuperStateAdditionalDontCareMasks); ii++,\
            TempLinkedListPtr = TempLinkedListPtr->NextElementPtr)
        {
            if(ii == ElementNumber)
            {
                ReturnValue = TempLinkedListPtr->Mask;
                break;
            }
        }
    }
    return(ReturnValue);
}

int DiagramClass::GetNumTransActions(void)
{
    return(NumTransActions);
}

void DiagramClass::IncNumTransActions(void)
{
    NumTransActions++;
}
