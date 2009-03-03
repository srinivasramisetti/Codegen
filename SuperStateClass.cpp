#include "stdafx.h"
#include "CommonStateClass.h"
#include "SuperStateClass.h"

//****************************************************************************
// Class name: SuperStateClass
//
// Description:	 This is a data container class for super state information
// from an FSM diagram.  Its base class is the CommonStateClass.
//****************************************************************************
SuperStateClass::SuperStateClass(SuperState_t *SuperStateData)
{
    SuperState.SuperStateNumber = SuperStateData->SuperStateNumber;
    SuperState.SuperStateName = SuperStateData->SuperStateName;
    SuperState.DefaultTransTargetState = SuperStateData->DefaultTransTargetState;

    NumListSubStates = 0;
    HeadSubStateListPtr = 0;
    TailSubStateListPtr = 0;

    NumExtendedListSubStates = 0;
    HeadSubStateExtendedListPtr = 0;
    TailSubStateExtendedListPtr = 0;

    NextElementPtr = 0;
}

void SuperStateClass::AddDefaultTransTargetState(CString NewDefaultTransTargetState)
{
    SuperState.DefaultTransTargetState = NewDefaultTransTargetState;
}

CString SuperStateClass::GetDefaultTransTargetState(void)
{
    return(SuperState.DefaultTransTargetState);
}

int SuperStateClass::GetSuperStateNumber(void)
{
    return(SuperState.SuperStateNumber);
}

CString SuperStateClass::GetSuperStateName(void)
{
    return(SuperState.SuperStateName);
}

void SuperStateClass::SetNextElementPtr(SuperStateClass *NewNextElementPtr)
{
    NextElementPtr = NewNextElementPtr;
}

SuperStateClass *SuperStateClass::GetNextElementPtr(void)
{
    return(NextElementPtr);
}

void SuperStateClass::AddSuperStateListSubState(ListSubState_t *NewListSubStateData)
{
	// Dynamically create a new linked list element
	ListSubState_t *TempLinkedListPtr = new ListSubState_t;

	// IF1 This is the first one
	if(0 == NumListSubStates)
	{ // THEN1 Set the tail pointer, clear the next element pointer, and
		// set the head pointer
		TailSubStateListPtr = TempLinkedListPtr;
		TailSubStateListPtr->LastElementPtr = 0;
		TailSubStateListPtr->NextElementPtr = 0;
		HeadSubStateListPtr = TailSubStateListPtr;
	}
	else
	{ // ELSE1 Set the last element pointer, link the new linked list element to the end,
        //and set the tail pointer to the end of the linked list
		TempLinkedListPtr->LastElementPtr = TailSubStateListPtr;
		TailSubStateListPtr->NextElementPtr = TempLinkedListPtr;
		TailSubStateListPtr = TempLinkedListPtr;
	}

    TailSubStateListPtr->SubStateName = NewListSubStateData->SubStateName;

    NumListSubStates++;
}

void SuperStateClass::RemoveLastSuperStateListSubStateName(void)
{
	ListSubState_t *TempTailSubStateListPtr;

	// Keep a temporary copy of the unwanted tail pointer
	TempTailSubStateListPtr = TailSubStateListPtr;

	// Make sure there is more than one substate first
	// IF1 There is more than one substate
    if(NumListSubStates > 1)
    { // THEN1 Reset the tail pointer to the next to last item in the linked list
        TailSubStateListPtr = TailSubStateListPtr->LastElementPtr;
        TailSubStateListPtr->NextElementPtr = 0;
    }

	// Delete the unwanted tail pointer
    delete TempTailSubStateListPtr;

    NumListSubStates--;
}

ListSubState_t *SuperStateClass::FindSubStateListElementPtr(int SubStateListNumber)
{
	ListSubState_t *TempLinkedListPtr = HeadSubStateListPtr;
	int ii;
	
    for(ii = 0; ii < NumListSubStates; ii++,\
        TempLinkedListPtr = TempLinkedListPtr->NextElementPtr)
	{
        if(ii == SubStateListNumber)
        {
            break;
        }
	}
	return(TempLinkedListPtr);
}

int SuperStateClass::GetNumSuperStateListSubStates(void)
{
    return(NumListSubStates);
}

CString SuperStateClass::GetSuperStateListSubStateName(int SubStateListNumber)
{
    return((FindSubStateListElementPtr(SubStateListNumber))->SubStateName);
}

void SuperStateClass::SetHeadSubStateListPtr(ListSubState_t *NewHeadSubStateListPtr)
{
    HeadSubStateListPtr = NewHeadSubStateListPtr;
}

ListSubState_t *SuperStateClass::GetHeadSubStateListPtr(void)
{
    return(HeadSubStateListPtr);
}

void SuperStateClass::SetTailSubStateListPtr(ListSubState_t *NewTailSubStateListPtr)
{
    TailSubStateListPtr = NewTailSubStateListPtr;
}

ListSubState_t *SuperStateClass::GetTailSubStateListPtr(void)
{
    return(TailSubStateListPtr);
}

void SuperStateClass::AddSuperStateExtendedListSubState(ListSubState_t *NewListSubStateData)
{
	// Dynamically create a new linked list element
	ListSubState_t *TempLinkedListPtr = new ListSubState_t;

	// IF1 This is the first one
	if(0 == NumExtendedListSubStates)
	{ // THEN1 Set the tail pointer, clear the next element pointer, and
		// set the head pointer
		TailSubStateExtendedListPtr = TempLinkedListPtr;
		TailSubStateExtendedListPtr->NextElementPtr = 0;
		HeadSubStateExtendedListPtr = TailSubStateExtendedListPtr;
	}
	else
	{ // ELSE1 Link the new linked list element to the end and set the tail
		// pointer to the end of the linked list
		TailSubStateExtendedListPtr->NextElementPtr = TempLinkedListPtr;
		TailSubStateExtendedListPtr = TempLinkedListPtr;
	}

    TailSubStateExtendedListPtr->SubStateName = NewListSubStateData->SubStateName;

    NumExtendedListSubStates++;
}

ListSubState_t *SuperStateClass::FindSubStateExtendedListElementPtr(int SubStateExtendedListNumber)
{
	ListSubState_t *TempLinkedListPtr = HeadSubStateExtendedListPtr;
	int ii;
	
    for(ii = 0; ii < NumExtendedListSubStates; ii++,\
        TempLinkedListPtr = TempLinkedListPtr->NextElementPtr)
	{
        if(ii == SubStateExtendedListNumber)
        {
            break;
        }
	}
	return(TempLinkedListPtr);
}

int SuperStateClass::GetSuperStateExtendedListNumSubStates(void)
{
    return(NumExtendedListSubStates);
}

CString SuperStateClass::GetSuperStateExtendedListSubStateName(int SubStateExtendedListNumber)
{
    return((FindSubStateExtendedListElementPtr(SubStateExtendedListNumber))->SubStateName);
}

void SuperStateClass::SetHeadSubStateExtendedListPtr(ListSubState_t *NewHeadSubStateExtendedListPtr)
{
    HeadSubStateExtendedListPtr = NewHeadSubStateExtendedListPtr;
}

ListSubState_t *SuperStateClass::GetHeadSubStateExtendedListPtr(void)
{
    return(HeadSubStateExtendedListPtr);
}

void SuperStateClass::SetTailSubStateExtendedListPtr(ListSubState_t *NewTailSubStateExtendedListPtr)
{
    TailSubStateExtendedListPtr = NewTailSubStateExtendedListPtr;
}

ListSubState_t *SuperStateClass::GetTailSubStateExtendedListPtr(void)
{
    return(TailSubStateExtendedListPtr);
}
