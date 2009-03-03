#include "stdafx.h"
#include "CommonStateClass.h"
#include "Globals.h"

//****************************************************************************
// Class name: CommonStateClass
//
// Description:	 This is the base class for StateClass and SuperState objects.
// This also maintains a linked lists of transition objects of type Trans_t.
//****************************************************************************
CommonStateClass::CommonStateClass(void)
{
    NumTrans = 0;
    HeadTransPtr = 0;
    TailTransPtr = 0;
}

void CommonStateClass::UpdateTargetState(int TransNumber, int UpdatedNextState)
{
    FindTransElementPtr(TransNumber)->TargetState = UpdatedNextState;
}

void CommonStateClass::UpdateTransCondDontCareMasks(int TransNumber, int NewCondMask, int NewDontCareMask)
{
	FindTransElementPtr(TransNumber)->TransCondMask |= NewCondMask;
	FindTransElementPtr(TransNumber)->TransDontCareMask |= NewDontCareMask;
}

int CommonStateClass::GetNumTrans(void)
{
    return(NumTrans);
}

void CommonStateClass::AddTrans(Trans_t *TransPtr)
{
	// Dynamically create a transition linked list element
	Trans_t *TempLinkedListPtr = new Trans_t;

	// IF1 This is the first one
	if(0 == NumTrans)
	{ // THEN1 Set the tail pointer, clear the next element pointer, and
		// set the heat pointer
		TailTransPtr = TempLinkedListPtr;
		TailTransPtr->NextElementPtr = 0;
		HeadTransPtr = TailTransPtr;
	}
	else
	{ // ELSE1 Link the new linked list element to the end and set the tail
		// pointer to the end of the linked list
		TailTransPtr->NextElementPtr = TempLinkedListPtr;
		TailTransPtr = TempLinkedListPtr;
	}

    TailTransPtr->TransStatus = TransPtr->TransStatus;
    TailTransPtr->TargetState = TransPtr->TargetState;
    TailTransPtr->TransCondMask = TransPtr->TransCondMask;
    TailTransPtr->TransDontCareMask = TransPtr->TransDontCareMask;
    TailTransPtr->AddedBySuperState = TransPtr->AddedBySuperState;
    TailTransPtr->TransActionFunctionName = TransPtr->TransActionFunctionName;
    TailTransPtr->TransActionFunctionBody = TransPtr->TransActionFunctionBody;

    NumTrans++;

    if("" != TransPtr->TransActionFunctionName)
    {
        Diagram.IncNumTransActions();
    }
}

Trans_t *CommonStateClass::FindTransElementPtr(int TransNumber)
{
	Trans_t *TempLinkedListPtr = HeadTransPtr;
	int ii;
	
    for(ii = 0; ii < NumTrans; ii++,\
        TempLinkedListPtr = TempLinkedListPtr->NextElementPtr)
	{
        if(ii == TransNumber)
        {
            break;
        }
	}
	return(TempLinkedListPtr);
}

CString CommonStateClass::GetTransActionFunctionName(int TransNumber)
{
    return(FindTransElementPtr(TransNumber)->TransActionFunctionName);
}

CString CommonStateClass::GetTransActionFunctionBody(int TransNumber)
{
    return(FindTransElementPtr(TransNumber)->TransActionFunctionBody);
}

int CommonStateClass::GetTransTargetState(int TransNumber)
{
    return(FindTransElementPtr(TransNumber)->TargetState);
}

int CommonStateClass::GetTransCondMask(int TransNumber)
{
    return(FindTransElementPtr(TransNumber)->TransCondMask);
}

int CommonStateClass::GetTransDontCareMask(int TransNumber)
{
    return(FindTransElementPtr(TransNumber)->TransDontCareMask);
}

CString CommonStateClass::GetTransAddedBySuperState(int TransNumber)
{
    return(FindTransElementPtr(TransNumber)->AddedBySuperState);
}

CString CommonStateClass::GetTransStatus(int TransNumber)
{
    return(FindTransElementPtr(TransNumber)->TransStatus);
}
