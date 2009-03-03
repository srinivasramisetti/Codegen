#include "stdafx.h"
#include "CommonStateClass.h"
#include "StateClass.h"

//****************************************************************************
// Class name: StateClass
//
// Description:	 This is a data container class for state information
// from an FSM diagram.  Its base class is the CommonStateClass.
//****************************************************************************
StateClass::StateClass(State_t *StateData)
{
    State.StateNumber = StateData->StateNumber;
    State.StateName = StateData->StateName;
    State.EntryAction = StateData->EntryAction;
    State.ActivityFunction = StateData->ActivityFunction;
    State.ExitAction = StateData->ExitAction;

    NextElementPtr = 0;
}

int StateClass::GetStateNumber(void)
{
    return(State.StateNumber);
}

CString StateClass::GetStateName(void)
{
    return(State.StateName);
}

CString StateClass::GetEntryAction(void)
{
    return(State.EntryAction);
}

CString StateClass::GetActivityFunction(void)
{
    return(State.ActivityFunction);
}

CString StateClass::GetExitAction(void)
{
    return(State.ExitAction);
}

void StateClass::SetNextElementPtr(StateClass *NewNextElementPtr)
{
    NextElementPtr = NewNextElementPtr;
}

StateClass *StateClass::GetNextElementPtr(void)
{
    return(NextElementPtr);
}
